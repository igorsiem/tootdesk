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
 * \file timelinewidget.h
 * Declares the `TimelineWidget` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QListWidget>
#include "statuswidget.h"

#ifndef _td_gui_timelinewidget_h_included
#define _td_gui_timelinewidget_h_included

////**
/// * \brief A widget for displaying statuses (stati?) in a vertical 'timeline'
/// * format
/// * 
/// * \todo Add a `clear` method for emptying the timeline
/// * 
/// * \param Add additional options for ordering / filtering
/// * 
/// * \param Expand this documentation
/// *
/// * \todo Consider deprecating / removing this class in favour of a Model /
/// * View design
/// */
///class TimelineWidget : public QListWidget
///{
///
///    Q_OBJECT
///
///    // --- External Interface ---
///
///    public:
///
///    // -- Construction / Destruction --
///
///    explicit TimelineWidget(QWidget* parent);
///
///    /**
///     * \brief Trivial destructor
///     */
///    virtual ~TimelineWidget(void) {}
///
///    // -- Status Item Management --
///
///    public slots:
///
///    /**
///     * \brief Add a new status item to the displayed timeline
///     * 
///     * This method creates a new StatusWidget object from the given status,
///     * and places it in the displayed timeline.
///     * 
///     * \param status The Mastodon status item
///     */
///    void add(ConstStatusPtr status);
///
///    // --- Internal Declarations ---
///
///    protected:
///
///    // -- Helper Methods --
///
///    
///};  // end TimelineWidget class

#endif
