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

#include <QWidget>
#include "mastodontypes.h"

#ifndef _td_gui_statuswidget_h_included
#define _td_gui_statuswidget_h_included

/**
 * \brief A widget for displaying a Mastodon status item
 * 
 * \todo Expand this documentation
 * 
 * \todo This is just a barebones display of status text items - needs lots
 * of layout work
 */
class StatusWidget : public QWidget
{

    Q_OBJECT

    // --- External Interface ---

    public:

    // -- Constructor / Destructor --

    /**
     * \brief Constructor - initialises the status widget structure
     * 
     * \param status The mastodon status object, from which to populate the
     * the widget information
     * 
     * \param parent The parent widget
     */
    StatusWidget(
        const status_t& status,
        QWidget* parent);

    /**
     * \brief Trivial destructor
     */
    virtual ~StatusWidget(void) {}

    // --- Internal Declarations ---

    protected:

    // -- Helper Methods --

    /**
     * \brief Set up the various child objects of the status widget
     * 
     * All child objects are automatically destroyed by the Qt framework
     * when the parent is destroted.
     */
    void setup(const status_t& status);

};  // end StatusWidget class

#endif
