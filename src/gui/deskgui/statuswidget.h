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
 * Declares the StatusWidget class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QDebug>
#include <QWidget>

#include "../deskapi/status.h"

#ifndef _td_gui_deskgui_statuswidget_h_included
#define _td_gui_deskgui_statuswidget_h_included

namespace TootDesk { namespace Gui {

/**
 * \brief A widget for displaying a Status item
 */
class StatusWidget : public QWidget
{

    Q_OBJECT

    // --- External Interface ---

    public:

    explicit StatusWidget(
        const Api::Status& status,
        QWidget* parent = nullptr);

    // --- Internal Declarations ---

    protected:

};  // end StatusWidget class

}}  // end TootDesk::Gui namespace

#endif
