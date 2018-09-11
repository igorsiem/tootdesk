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
 * \file statuslistwidget.cpp
 * Implements the StatusListWidget class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QListWidgetItem>
#include "statuslistwidget.h"
#include "statuswidget.h"

namespace TootDesk { namespace Gui {

StatusListWidget::StatusListWidget(QWidget* parent) :
    QListWidget(parent)
    , m_statusItems(nullptr)
    , m_statusItemsMtx()
{

    // Allow self-notification when list data has been updated
    connect(
        this,
        &StatusListWidget::statusItemsUpdated,
        this,
        &StatusListWidget::refillStatusItemsList);

}   // end constructor

void StatusListWidget::setStatusItems(Api::StatusVector statusItems)
{
    Api::WriteGuard grd(&m_statusItemsMtx);
    if (!m_statusItems) m_statusItems = Api::makeShared<Api::StatusVector>();
    (*m_statusItems) = std::move(statusItems);
    grd.unlock();

    emit statusItemsUpdated();
}   // end setStatusItems method

void StatusListWidget::refillStatusItemsList(void)
{
    clear();

    Api::ReadGuard grd(&m_statusItemsMtx);
    if (m_statusItems)
    {
        for (auto status : *m_statusItems)
        {
            // addItem(status->content());
            auto statusWidget = new StatusWidget(*status);

            auto item = new QListWidgetItem(this);
            item->setSizeHint(statusWidget->sizeHint());
            addItem(item);
            setItemWidget(item, statusWidget);
        }
    }   // end if we have some status items

}   // end refillStatusItemsList method

}}  // end TootDesk::Gui namespace
