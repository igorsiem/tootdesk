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
 * \file timelinewidget.cpp
 * Implements the `TimelineWidget` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include "timelinewidget.h"

TimelineWidget::TimelineWidget(QWidget* parent) : QListWidget(parent)
{
}   // end constructor

void TimelineWidget::add(ConstStatusPtr status)
{

    auto statusWidget = new StatusWidget(*status, this);

    auto listWidgetItem = new QListWidgetItem;

    // TODO fix sizing, so that the status widget tells us what size it needs
    listWidgetItem->setSizeHint(
        QSize(
            statusWidget->size().width(),
            100));

    insertItem(0, listWidgetItem);
    setItemWidget(listWidgetItem, statusWidget);

}   // end add method
