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
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createLayout();
}   // end constructor

MainWindow::~MainWindow()
{
    delete ui;
}   // end destructor

void MainWindow::createLayout(void)
{

    // Set the main, central layout as an h-box
    auto centralLayout = new QHBoxLayout(ui->centralWidget);
    ui->centralWidget->setLayout(centralLayout);

    // Create two timeline frames
    //
    // TODO This is really only demo code, not for production
    auto tf1 = createTimelineFrame(
                "Federated Timeline",
                ui->centralWidget);
    centralLayout->addWidget(tf1);

    auto tf2 = createTimelineFrame(
                "Another Timeline",
                ui->centralWidget);
    centralLayout->addWidget(tf2);

}   // end createLayout method

QFrame* MainWindow::createTimelineFrame(
        const QString& title,
        QWidget* parent)
{

    // Create the frame and a layout for it.
    auto frame = new QFrame(parent);
    frame->setMinimumSize(500, 500);
    auto layout = new QVBoxLayout(frame);
    frame->setLayout(layout);

    // Create a title label, and a scroll area beneath it
    auto label = new QLabel(frame);
    label->setText(title);
    layout->addWidget(label);

    auto scrollArea = new QScrollArea(frame);
    layout->addWidget(scrollArea);

    return frame;
}   // end createTimelineFrame
