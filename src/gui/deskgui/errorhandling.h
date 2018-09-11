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
 * \file errorhandling.h
 * Declares some handy top-level error handling macros
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <stdexcept>
#include <QMessageBox>
#include "../deskapi/error.h"

#ifndef _td_gui_deskgui_errorhandling_h_included
#define _td_gui_deskgui_errorhandling_h_included

#define TD_SIGNAL_ERROR( headingText, descriptionText ) do { \
    QMessageBox::critical(this, headingText, descriptionText); \
} while (false)

#define TD_ACTION_TRY try

#define TD_ACTION_CATCH_ALL_FROM( action ) \
catch (const ::TootDesk::Api::Error& err) \
{ \
    TD_SIGNAL_ERROR(action, tr("Error: ") + err.message()); \
} \
catch (const std::exception& err) \
{ \
    TD_SIGNAL_ERROR(action, tr("Error: ") + QString(err.what())); \
} \
catch (...) \
{ \
    TD_SIGNAL_ERROR( \
        action, \
        tr("An unrecognised error condition was encountered.")); \
}

#endif
