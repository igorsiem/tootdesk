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
 * \file statuswidget.h
 * Declares the `StatusWidget` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "statuswidget.h"

///StatusWidget::StatusWidget(
///        const status_t& status,
///        QWidget* parent) :
///    QWidget(parent)
///{
///    setup(status);
///}   // end constructor
///
///void StatusWidget::setup(const status_t& status)
///{
///
///    auto mainLayout = new QHBoxLayout(this);
///    setLayout(mainLayout);
///
///    // TODO make a proper avatar
///    auto avatarLabel = new QLabel(this);
///    avatarLabel->setText(QString::fromStdString("<avatar>"));
///    avatarLabel->setMinimumSize(100, 100);
///    avatarLabel->setMaximumSize(100, 100);
///    mainLayout->addWidget(avatarLabel);
///
///    auto textLayout = new QVBoxLayout();
///    mainLayout->addItem(textLayout);
///
///    auto authorLabel = new QLabel(this);
///    authorLabel->setText(QString::fromStdString(status.account().acct()));
///    textLayout->addWidget(authorLabel);
///
///    auto contentLabel = new QLabel(this);
///    contentLabel->setText(QString::fromStdString(status.content()));
///    textLayout->addWidget(contentLabel);
///
///}   // end constructorLayout method
