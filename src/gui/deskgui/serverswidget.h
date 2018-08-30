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
     * \brief Signals that the Servers collection (for which the
     * `ServersWidget` object has a reference) has been updated by this
     * widget
     */
    void serversCollectionChanged(void);

    void newServerCreated(Api::ServerPtr server);

    // --- Internal Declarations ---

    protected slots:

    void serverInstanceInfoRetrieved(QString title, QString description)
    {
        setInstanceInfo(title, description);
    }

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

    void setupInstanceInfoWidget(void);

    // -- Helper Functions --

    void addNewServer(QString name, QString url);

    void clearInstanceInfo(void);

    void setInstanceInfoNone(QString message);

    void setInstanceInfo(QString instanceTitle, QString instanceDescription);

    void setInstanceInfoWaiting(void);

    Api::ServerPtr serverAtIndex(int index)
    {
        if ((index < 0) || (index >= m_servers.size())) return nullptr;

        auto serverItr = m_servers.begin();
        for (int i = 0; i < index; i++) serverItr++;

        return serverItr.value();
    }

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

    QWidget* m_instanceInfoWidget;

    QFormLayout* m_instanceInfoLayout;

    QLabel* m_instanceInfoNoneLabel;

    std::tuple<QMovie*, QLabel*> m_instanceInfoWaitingWidgets;

    std::tuple<QLabel*, QLabel*, QLabel*, QLabel*> m_instanceInfoDataWidgets;

    QMetaObject::Connection m_serverInstanceInfoConnection;

};  // end ServersWidget class

}}  // end TootDesk::Gui namespace

#endif
