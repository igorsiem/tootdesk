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
 * Implements the `mainwindow` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <sstream>
#include <QException>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup();
}   // end constructor

MainWindow::~MainWindow()
{
    delete ui;
}   // end destructor

void MainWindow::setup(void)
{

    ui->centralWidget->setLayout(new QHBoxLayout(ui->centralWidget));

    // Create two timeline frames
    //
    // TODO This is really only demo code, not for production
    QFrame* tf1 = nullptr;
    QPushButton* addressPushButton = nullptr;
    // QScrollArea* statusScrollArea = nullptr;
    std::tie(tf1,addressPushButton, m_statusListWidget) =
    createTimelineFrame(
                "Federated Timeline",
                ui->centralWidget);
    ui->centralWidget->layout()->addWidget(tf1);

    connect(
        addressPushButton,
        SIGNAL(released()),
        this,
        SLOT(handleButton()));

    auto tf2 = createColumnFrame(ui->centralWidget);
    ui->centralWidget->layout()->addWidget(tf2);

}   // end createLayout method

std::tuple<QFrame*,QPushButton*,QListWidget*>
MainWindow::createTimelineFrame(
        const QString& title,
        QWidget* parent)
{

    // Create the frame and a layout for it.
    QFrame* frame = createColumnFrame(parent);

    auto label = new QLabel(frame);
    label->setText(title);
    frame->layout()->addWidget(label);

    QPushButton* addressPushButton = nullptr;
    QWidget* addressWidget = nullptr;
    std::tie(addressWidget, addressPushButton) =
        createAddressWidget("Go", frame);
    frame->layout()->addWidget(addressWidget);

    // auto scrollArea = new QScrollArea(frame);
    auto listWidget = new QListWidget(frame);
    frame->layout()->addWidget(listWidget);

    // TODO demo code - not for production
    auto item = createTimelineItemWidget(listWidget, "Hello", "There");

    return std::make_tuple(frame, addressPushButton, listWidget);

}   // end createTimelineFrame

QListWidgetItem* MainWindow::createTimelineItemWidget(
        QListWidget* parent,
        const QString& a,
        const QString& c)
{

    auto contentWidget = new QWidget();

    auto mainLayout = new QHBoxLayout(contentWidget);
    contentWidget->setLayout(mainLayout);

    // TODO make a proper avatar
    auto avatar = new QLabel(contentWidget);
    avatar->setText("<avatar>");
    avatar->setBaseSize(100, 100);
    avatar->setMaximumSize(100, 100);
    mainLayout->addWidget(avatar);

    auto textLayout = new QVBoxLayout();
    // mainLayout->addLayout(textLayout);
    mainLayout->addItem(textLayout);
    
    // TOOD add proper author info
    auto author = new QLabel(contentWidget);
    author->setText(a);
    textLayout->addWidget(author);

    // TODO add proper content info
    auto content = new QLabel(contentWidget);
    content->setText(c);
    textLayout->addWidget(content);

    auto listWidgetItem = new QListWidgetItem;
    listWidgetItem->setSizeHint(
        QSize(
            contentWidget->size().width(),
           100));
    parent->insertItem(0, listWidgetItem);
    parent->setItemWidget(listWidgetItem, contentWidget);

    return listWidgetItem;
    
}

QListWidgetItem* MainWindow::createTimelineItemWidget(
        QListWidget* parent,
        const Mastodon::Easy::Easy::Status& status)
{
    return createTimelineItemWidget(
        parent,
        QString::fromStdString(status.account().acct()),
        QString::fromStdString(status.content()));
}   // end createTimelineItemWidget

std::tuple<QWidget*, QPushButton*> MainWindow::createAddressWidget(
        const QString& buttonText,
        QWidget* parent)
{
    auto widget = new QWidget(parent);
    widget->setLayout(new QHBoxLayout());

    auto addressLineEdit = new QLineEdit(widget);
    widget->layout()->addWidget(addressLineEdit);    

    auto pushButton = new QPushButton(widget);
    pushButton->setText(buttonText);
    widget->layout()->addWidget(pushButton);

    return std::make_tuple(widget, pushButton);
}   // end createServerAddressWidget

QFrame* MainWindow::createColumnFrame(QWidget* parent)
{
    auto frame = new QFrame(parent);
    frame->setMinimumSize(500, 500);
    frame->setLayout(new QVBoxLayout());

    return frame;
}   // end createColumnFrame

void MainWindow::handleButton(void)
{

    try
    {

        int counter = 0;
        getPublicTimeline("freeradical.zone",
            [&counter,this](const Mastodon::Easy::Easy::Status& status)
            {
                createTimelineItemWidget(m_statusListWidget, status);
                // createTimelineItemWidget(m_statusScrollArea, "this", "that");
                

                counter++;
            });

        std::stringstream strm;
        strm << counter << " toot(s)";

        QMessageBox::information(
            this,
            "Loaded Toots",
            QString::fromStdString(strm.str())
        );
    }
    catch (const QException& error)
    {
        QMessageBox::critical(
            this,
            "Refresh",
            error.what());
    } 
    catch (const std::exception& error)
    {
        QMessageBox::critical(
            this,
            "Refresh",
            error.what());
    }
    catch (...)
    {
        QMessageBox::critical(
            this,
            "Refresh",
            "An unknown error occurred. The operation could not be "
            "completed");

    }
}   // end handleButton

void MainWindow::getPublicTimeline(
        const std::string& address,
        statusProcessorFn processorFn)
{
    using Mastodon::Easy;
    Easy masto(address, "");

    std::string response;
    masto.get(Mastodon::API::v1::timelines_public, response);

    for (const std::string& str :
            Mastodon::Easy::Easy::json_array_to_vector(response))

    {
        Easy::Status status(str);
        processorFn(status);
    }
}   // end getPublicTimeline
