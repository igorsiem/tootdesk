// TootDesk - Desktop Mastodon Client
// Copyright (C) 2018 Igor Siemienowicz
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

/**
 * \file mainwindow.cpp
 * Implements the `MainWindow` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

///#include <sstream>
#include <QDebug>
///#include <QException>
#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QTableView>
///#include <QThreadPool>
///#include <QVBoxLayout>

#include <deskgui/deskgui.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

///#include "lambdarunnable.h"
///#include "statuswidget.h"
///#include "timelinewidget.h"

namespace TdGui = Td::Gui;

MainWindow::MainWindow(QSettings& settings, QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_settings(settings)
    , m_servers()
{

    // Retrieve servers info from persistent settings
    auto serverVar = m_settings.value("servers");
    if (serverVar.isValid())
        TdApi::convertFromSerialisation(serverVar.toMap(), m_servers);

    // Connect up the error signal of our servers to a slot that displays
    // the error in a message box
    for (auto server : m_servers)
    {
        connect(
            server.get(),
            &TdApi::Server::errorOccurred,
            this,
            &MainWindow::serverErrorOccurred);
    }

    setupUi();

}   // end constructor

MainWindow::~MainWindow()
{
    delete ui;
}   // end destructor

void MainWindow::closeEvent(QCloseEvent *event)
{

    // Record window geometry in persistent settings
    m_settings.beginGroup("MainWindow");
    m_settings.setValue("geometry", saveGeometry());
    m_settings.setValue("windowState", saveState());
    m_settings.endGroup();    

    QMainWindow::closeEvent(event);
    
}   // end closeEvent

void MainWindow::setupUi(void)
{
    
    qDebug() << "UI setup begin here";

    // Internal Qt framework setup
    ui->setupUi(this);

    // Restore window geometry from persistent settings
    m_settings.beginGroup("MainWindow");
    restoreGeometry(m_settings.value("geometry").toByteArray());
    restoreState(m_settings.value("state").toByteArray());
    m_settings.endGroup();

    setupCentralWidget();

    qDebug() << "UI setup finishes here";

}   // end setupUi method

void MainWindow::setupCentralWidget(void)
{

    ui->centralWidget->setLayout(new QHBoxLayout(ui->centralWidget));

    // Set up the main splitter area
    auto mainSplitter = new QSplitter(this);
    ui->centralWidget->layout()->addWidget(mainSplitter);

    // Set up the left and right halves of the splitter
    mainSplitter->addWidget(setupLeftSplit(mainSplitter));

    // TODO set up a proper right side of the window
    auto rightFrame = new QFrame(this);
    rightFrame->setFrameShape(QFrame::StyledPanel);
    mainSplitter->addWidget(rightFrame);

    // Set up the geometry of the main splitter - getting it from persistent
    // settings if they are present.
    m_settings.beginGroup("MainWindow");
    int leftSplit = m_settings.value("mainSplitterLeft", 50).toInt(),
        rightSplit = m_settings.value("mainSplitterRight", 1000).toInt();
    m_settings.endGroup();
    mainSplitter->setSizes(QList<int>({ leftSplit, rightSplit }));

    // Event handler - when main splitter sizes change, record them
    connect(
        mainSplitter,
        &QSplitter::splitterMoved,
        [this, mainSplitter](int, int)
        {
            // Splitter has moved - write its sizes to persistent storage
            auto sizes = mainSplitter->sizes();
            m_settings.beginGroup("MainWindow");
            m_settings.setValue("mainSplitterLeft", sizes[0]);
            m_settings.setValue("mainSplitterRight", sizes[1]);
            m_settings.endGroup();
        });

///    // Create two timeline frames
///    //
///    auto tf1 = createTimelineFrame("Federated Timeline", ui->centralWidget);
///    ui->centralWidget->layout()->addWidget(tf1);
///
///    auto tf2 = createColumnFrame(ui->centralWidget);
///    ui->centralWidget->layout()->addWidget(tf2);

}   // end setupCentralWidget method

QWidget* MainWindow::setupLeftSplit(QWidget* parent)
{

    // Create the frame for the view
    auto frame = new QFrame(parent);
    frame->setLayout(new QVBoxLayout(frame));
    frame->setFrameShape(QFrame::StyledPanel);

    // Add a widget for the Server instances table
    //
    // TODO Consider whether we want to display the server instances all the
    // the time
    auto serversWidget = new TdGui::ServersWidget(m_servers, frame);

    // When a new server is created, make sure its error signal is hooked up
    // to our error display method.
    connect(
        serversWidget,
        &TdGui::ServersWidget::newServerCreated,
        [this](TdApi::ServerPtr newServer)
        {
            connect(
                newServer.get(),
                &TdApi::Server::errorOccurred,
                this,
                &MainWindow::serverErrorOccurred);
        });

    // When the servers Widget updates the list of Servers, make sure the
    // info is written to persistent settings.
    connect(
        serversWidget,
        &TdGui::ServersWidget::serversCollectionChanged,
        [this](void)
        {
            auto serialisableServerInfo =
                TdApi::convertForSerialisation(m_servers);
            m_settings.setValue("servers", serialisableServerInfo);
        });

    frame->layout()->addWidget(serversWidget);

    return frame;

}   // end setupLeftSplit

///QFrame* MainWindow::createTimelineFrame(
///        const QString& title,
///        QWidget* parent)
///{
///
///    // Create the frame and a layout for it.
///    QFrame* frame = createColumnFrame(parent);
///
///    auto label = new QLabel(frame);
///    label->setText(title);
///    frame->layout()->addWidget(label);
///
///    auto addressWidget = new SimpleAddressWidget(
///       "Go",
///       [this](const QString& address)
///       {
///            m_timelineWidget->clear();
///
///            QThreadPool::globalInstance()->start(
///                new LambdaRunnable(
///                    [this,&address](void)
///                    {
///                        getPublicTimeline(address.toStdString(),
///                            [this](ConstStatusPtr status)
///                            {
///                                emit(statusDecoded(status));
///                            });
///                    }));           
///       },
///       frame);
///
///    frame->layout()->addWidget(addressWidget);
///
///    // auto scrollArea = new QScrollArea(frame);
///    // auto listWidget = new 
///    m_timelineWidget = new TimelineWidget(frame);
///
///    connect(
///        this,
///        SIGNAL(statusDecoded(ConstStatusPtr)),
///        m_timelineWidget,
///        SLOT(add(ConstStatusPtr)));
///
///    frame->layout()->addWidget(m_timelineWidget);
///
///    return frame;
///
///}   // end createTimelineFrame

///QFrame* MainWindow::createColumnFrame(QWidget* parent)
///{
///    auto frame = new QFrame(parent);
///    frame->setMinimumSize(500, 500);
///    frame->setLayout(new QVBoxLayout());
///
///    return frame;
///}   // end createColumnFrame

///void MainWindow::getPublicTimeline(
///        const std::string& address,
///        StatusProcessorFn processorFn)
///{
///    using Mastodon::Easy;
///    Easy masto(address, "");
///
///    std::string response;
///    masto.get(Mastodon::API::v1::timelines_public, response);
///
///    for (const std::string& str :
///            Mastodon::Easy::Easy::json_array_to_vector(response))
///
///    {
///        // Easy::Status status(str);
///        auto status = std::make_shared<status_t>(str);
///        processorFn(status);
///    }
///}   // end getPublicTimeline
