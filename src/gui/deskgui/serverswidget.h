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

#include <QDebug>
#include <QFormLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMovie>
#include <QResizeEvent>
#include <QSettings>
#include <QTableView>
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
     * \brief Signals that the Servers collection has been updated by this
     * widget
     */
    void serversCollectionChanged(void);

    /**
     * \brief Signal that a new server has been created
     * 
     * \param server The newly created server object. Note that this
     * *already* has been added to the `m_server` collection.
     */
    void newServerCreated(Api::ServerPtr server);

    // --- Internal Declarations ---

    protected slots:

    /**
     * \brief Display the Instance data for a server when it has been
     * retrieved
     * 
     * \param title The Instance Title
     * 
     * \param description A Description of the Instance
     */
    void serverInstanceInfoRetrieved(QString title, QString description)
    {
        setInstanceInfo(title, description);
    }

    /**
     * \brief Display information about an error that has occurred with a
     * Server
     * 
     * \param serverName The (user assigned) Name of the Server
     * 
     * \param errorMessage A human-readable error message 
     */
    void serverErrorOccurred(QString serverName, QString errorMsg)
    {
        setInstanceInfoNone(tr("Error retrieving instance data: ") +
            errorMsg);
    }

    protected:

    // -- Event-handler Overrides --

    /**
     * \brief Set table column widths when the widget is resized
     *
     * This method is called in response to a standard widget resize event.
     * It sets the widthds of the two data columns to half of the table
     * width.
     *
     * \todo This is a fairly 'brittle' method, in that it assumes that there
     * all the columns need to be of equal width. Consider an algorithm that
     * allows (and persists) column resizing, and maintains the relative
     * sizes of all columns.
     */
    virtual void resizeEvent(QResizeEvent* e) override
    {
        if (m_serverTableView)
        {
            auto numCols = m_serverTableModel->columnCount();

            for (int i = 0; i < numCols; i++)
                m_serverTableView->setColumnWidth(
                    i, (m_serverTableView->width() / numCols) - 1);
        }

        QWidget::resizeEvent(e);
    }   // end resizeEvent method

    // -- UI Setup Functions --

    /**
     * \brief Set up the Instance Information area widget
     */
    void setupInstanceInfoWidget(void);

    // -- Helper Functions --

    /**
     * \brief Create a new Server object, adding  it to the `m_servers`
     * collection
     * 
     * \param name A human-readable name for the Server
     * 
     * \param url The URL of the Server 
     */
    void addNewServer(QString name, QString url);

    /**
     * \brief Clear the Instance Info widget area
     */
    void clearInstanceInfo(void);

    /**
     * \brief Display a miscellaneous message in the Instance Info area when
     * there is no other data to  display
     */
    void setInstanceInfoNone(QString message);

    /**
     * \brief Display the Server's Instance data
     * 
     * \param instanceTitle The Title of the Instance
     * 
     * \param instanceDescription The Description of the Instance
     */
    void setInstanceInfo(QString instanceTitle, QString instanceDescription);

    /**
     * \brief Show a 'waiting' animation in the Instance Info area
     * 
     * This method is called when a 'retrieve instance info' operation
     * is taking place, and we are waiting for the result.
     */
    void setInstanceInfoWaiting(void);

    /**
     * \brief Retrieve the server at the given index in the `m_servers`
     * collection
     * 
     * This method counts through the `m_servers` collection from the
     * beginning (it's a map), and so is a tad inefficient, but we assume
     * that the collection of servers is 'small', so it's probably not a
     * problem. 
     * 
     * \param index The index of the server to retrieve
     * 
     * \return A shared pointer to the Server, or `nullptr` if `index` is out
     * of range
     */
    Api::ServerPtr serverAtIndex(int index)
    {
        if ((index < 0) || (index >= m_servers.size())) return nullptr;

        auto serverItr = m_servers.begin();
        for (int i = 0; i < index; i++) serverItr++;

        return serverItr.value();
    }   // end serverAtIndex method

    // -- Attributes --

    /**
     * \brief The external collection of Server instance objects, maintained
     * externally
     *
     * When this is updated, the collection is rewritten to `m_settings`.
     */
    Api::ServerByNameMap& m_servers;

    /**
     * \brief The view object for displaying Server instances in a table
     */
    QTableView* m_serverTableView;

    /**
     * \brief Data model for mediating between the `m_servers` collection and
     * the Table View of the model
     */
    Gui::ServerTableModel* m_serverTableModel;

    /**
     * \brief A container widget for the Instance data area
     */
    QWidget* m_instanceInfoWidget;

    /**
     * \brief The layout object for the Instance data area
     */
    QFormLayout* m_instanceInfoLayout;

    /**
     * \brief A label for use when there is no Server selected
     */
    QLabel* m_instanceInfoNoneLabel;

    /**
     * \brief A tuple of widgets for display when we are waiting for Instance
     * info to be retrieved
     */
    std::tuple<QMovie*, QLabel*> m_instanceInfoWaitingWidgets;

    /**
     * \brief Widgets for displaying Instance info
     */
    std::tuple<QLabel*, QLabel*, QLabel*, QLabel*> m_instanceInfoDataWidgets;

    /**
     * \brief The meta-object connection that ensures that the Servers
     * widget is notified when the Server has retrieved its instance info
     */
    QMetaObject::Connection m_serverInstanceInfoConnection;

};  // end ServersWidget class

}}  // end TootDesk::Gui namespace

#endif
