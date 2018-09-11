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
 * \file statuswidget.cpp
 * Implements the StatusWidget class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "statuswidget.h"

namespace TootDesk { namespace Gui {

StatusWidget::StatusWidget(
        const Api::Status& status,
        QWidget* parent) :
    QWidget(parent)
{

    // Set up the layouts
    auto mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    // Add a label as a placeholder for the icon
    auto iconLabel = new QLabel("[TODO icon]", this);
    iconLabel->setMinimumSize(QSize(100, 100));
    iconLabel->setMaximumSize(QSize(100, 100));
    iconLabel->setWordWrap(true);
    iconLabel->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(iconLabel, 0, Qt::AlignTop);

    // Add a vertical layout for the rest of the items in this control
    auto textLayout = new QVBoxLayout();
    layout()->addItem(textLayout);

    // Add text items
    auto createdAtLabel = new QLabel(status.createdAt().toString(), this);
    textLayout->addWidget(createdAtLabel);

    auto accountLabel =
        new QLabel(
            QString("<b>")
            + status.account()->displayName()
            + "(" + status.account()->userName() + ")"
            + QString("</b>"),
            this);
    textLayout->addWidget(accountLabel);

    auto contentLabel = new QLabel(status.content(), this);
    contentLabel->setWordWrap(true);
    contentLabel->setSizePolicy(
        QSizePolicy::Expanding, QSizePolicy::Preferred);
    textLayout->addWidget(contentLabel);

}   // end constructor

}}  // end TootDesk::Gui namespace
