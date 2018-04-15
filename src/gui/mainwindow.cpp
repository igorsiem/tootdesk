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

#include <sstream>
#include <QException>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "statuswidget.h"
#include "timelinewidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timelineWidget(nullptr)
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
    std::tie(tf1,addressPushButton, m_timelineWidget) =
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

std::tuple<QFrame*,QPushButton*,TimelineWidget*>
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
    // auto listWidget = new 
    auto timelineWidget = new TimelineWidget(frame);
    frame->layout()->addWidget(timelineWidget);

    return std::make_tuple(frame, addressPushButton, timelineWidget);

}   // end createTimelineFrame

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
                m_timelineWidget->add(status);
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
