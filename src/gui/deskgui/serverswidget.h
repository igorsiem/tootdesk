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
 * \file serverswidget.h
 * Declares the `ServersWidget` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QSettings>
#include <QWidget>

#include "../deskapi/server.h"
#include "servertablemodel.h"

#ifndef _td_gui_serverswidget_h_included
#define _td_gui_serverswidget_h_included

namespace TootDesk { namespace Gui {

class ServersWidget : public QWidget
{

    Q_OBJECT

    // --- External Interface ---

    public:

    /**
     * \brief Set up the widget, including all GUI components
     *
     * \param servers A reference to the container of Server objects
     *
     * \param parent The parent widget of self
     */
    explicit ServersWidget(
        Api::ServerByNameMap& servers,
        QWidget* parent = nullptr);

    signals:

    /**
     * \brief Signals that the Servers collection (for which the
     * `ServersWidget` object has a reference) has been updated by this
     * widget
     */
    void serversCollectionChanged(void);

    // --- Internal Declarations ---

    protected:

    void addNewServer(QString name, QString url);

    /**
     * \brief The external collection of Server instance objects, maintained
     * externally
     *
     * When this is updated, the collection is rewritten to `m_settings`.
     */
    Api::ServerByNameMap& m_servers;

    /**
     * \brief Data model for mediating between the `m_servers` collection and
     * the Table View of the model
     */
    Gui::ServerTableModel* m_serverTableModel;

};  // end ServersWidget class

}}  // end TootDesk::Gui namespace

#endif
