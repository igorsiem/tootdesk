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
 * \file statuslistwidget.h
 * Declares the StatusListWidget class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QListWidget>
#include "../deskapi/status.h"

#ifndef _td_gui_deskgui_statuslistwidget_h_included
#define _td_gui_deskgui_statuslistwidget_h_included

namespace TootDesk { namespace Gui {

class StatusListWidget : public QListWidget
{

    Q_OBJECT

    // --- External Interface ---

    public:

    StatusListWidget(QWidget* parent = nullptr);

    virtual ~StatusListWidget(void) = default;

    void setStatusItems(Api::StatusVector statusItems);

    signals:
    
    void statusItemsUpdated(void);

    // --- Internal Declarations ---

    protected slots:

    void refillStatusItemsList(void);

    protected:

    Api::StatusVectorPtr m_statusItems;

    mutable Api::Mutex m_statusItemsMtx;

};  // end StatusListWidget class

}}  // end TootDesk::Gui namespace

#endif
