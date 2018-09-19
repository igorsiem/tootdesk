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
 * \file authdialog.h
 * Declares the `AuthDialog` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QDialog>

#ifndef _td_gui_deskgui_authdialog_h_included
#define _td_gui_deskgui_authdialog_h_included

namespace TootDesk { namespace Gui {

/**
 * \brief Dialog for executing the OAuth process, allowing the User to access
 * a Server to enter their credentials and retrieve an access token
 * 
 * \todo Expand documentation about this class
 */
class AuthDialog : public QDialog
{

    // --- External Interface ---

    public:

    explicit AuthDialog(QString url, QWidget* parent = nullptr);

    virtual ~AuthDialog(void) = default;

    // --- Internal Declarations ---

    protected:



};  // end AuthDialog class

}}  // end TootDesk::Gui namespace

#endif
