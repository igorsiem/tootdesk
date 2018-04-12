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

#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
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
    auto tf1 = createTimelineFrame(
                "Federated Timeline",
                ui->centralWidget);
    ui->centralWidget->layout()->addWidget(tf1);

    auto tf2 = createColumnFrame(ui->centralWidget);
    ui->centralWidget->layout()->addWidget(tf2);

}   // end createLayout method

QFrame* MainWindow::createTimelineFrame(
        const QString& title,
        QWidget* parent)
{

    // Create the frame and a layout for it.
    QFrame* frame = createColumnFrame(parent);

    auto label = new QLabel(frame);
    label->setText(title);
    frame->layout()->addWidget(label);

    auto addressWidget = createAddressWidget("Go", frame);
    frame->layout()->addWidget(addressWidget);

    auto scrollArea = new QScrollArea(frame);
    frame->layout()->addWidget(scrollArea);

    // TODO demo code - not for production
    auto item = createTimelineItemWidget(scrollArea);

    return frame;

}   // end createTimelineFrame

QWidget* MainWindow::createTimelineItemWidget(QWidget* parent)
{
    auto itemWidget = new QWidget(parent);

    auto mainLayout = new QHBoxLayout(itemWidget);
    itemWidget->setLayout(mainLayout);

    // TODO make a proper avatar
    auto avatar = new QLabel(itemWidget);
    avatar->setText("<avatar>");
    mainLayout->addWidget(avatar);

    auto textLayout = new QVBoxLayout(itemWidget);
    mainLayout->addLayout(textLayout);
    
    // TOOD add proper author info
    auto author = new QLabel(itemWidget);
    author->setText("<author>");
    textLayout->addWidget(author);

    // TODO add proper content info
    auto content = new QLabel(itemWidget);
    content->setText("<content>");
    textLayout->addWidget(content);

    return itemWidget;
}   // end createTimelineItemWidget

QWidget* MainWindow::createAddressWidget(
        const QString& buttonText,
        QWidget* parent)
{
    auto widget = new QWidget(parent);
    widget->setLayout(new QHBoxLayout(widget));

    auto addressLineEdit = new QLineEdit(widget);
    widget->layout()->addWidget(addressLineEdit);    

    auto pushButton = new QPushButton(widget);
    pushButton->setText(buttonText);
    widget->layout()->addWidget(pushButton);

    return widget;
}   // end createServerAddressWidget

QFrame* MainWindow::createColumnFrame(QWidget* parent)
{
    auto frame = new QFrame(parent);
    frame->setMinimumSize(500, 500);
    frame->setLayout(new QVBoxLayout(frame));

    return frame;
}   // end createColumnFrame
