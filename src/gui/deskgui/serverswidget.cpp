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
 * Implements the `ServersWidget` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QToolBar>
#include <QVBoxLayout>

#include "errorhandling.h"
#include "serverdialog.h"
#include "serverswidget.h"

namespace TootDesk { namespace Gui {

ServersWidget::ServersWidget(
        Api::ServerByNameMap& servers,
        QWidget* parent) :
    QWidget(parent)
    , m_servers(servers)
    , m_serverTableView(nullptr)
    , m_serverTableModel(nullptr)
{

    // Set up our layout as a V-Box with no margins
    setLayout(new QVBoxLayout(this));
    layout()->setContentsMargins(0, 0, 0, 0);

    // A simple label to say what we are
    layout()->addWidget(new QLabel(tr("Server instances"), parent));

    // The table view, data model and selection model - the model wraps our
    // reference to the Servers container
    m_serverTableView = new QTableView(this);
    layout()->addWidget(m_serverTableView);
    m_serverTableModel =
        new Gui::ServerTableModel(m_servers, m_serverTableView);
    m_serverTableView->setModel(m_serverTableModel);
    m_serverTableView->setSelectionModel(
        new QItemSelectionModel(m_serverTableModel));
    m_serverTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_serverTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    m_serverTableView->selectRow(0);

    // Put a toolbar at the bottom of the widget for editing functions.
    auto toolBar = new QToolBar(this);
    layout()->addWidget(toolBar);

    // Action for creating a new Server object
    toolBar->addAction(
        tr("New Server"),
        [this](void)
        {
            TD_ACTION_TRY
            {

                // Execute the Server Dialog, with a validation function that
                // checks the URL, and that there are no duplicate names.
                auto newServerData = ServerDialog::execute(
                    ServerDialog::DialogData(),
                    [this](const ServerDialog::DialogData& data) ->
                            boost::optional<QString>
                    {
                        // Make sure it is not a duplicate name
                        if (m_servers.find(std::get<0>(data)) !=
                                m_servers.end())
                            return tr("A Server with this name already "
                                "exists");

                        // Make sure the URL is legit
                        if (!Api::Server::isValid(std::get<1>(data)))
                            return tr("The Server URL is not valid");

                        return boost::none;
                    },
                    tr("New Server"),
                    this);

                // If we got a result, then legit data was entered and
                // accepted.
                if (newServerData)
                    addNewServer(
                        std::get<0>(*newServerData),
                        std::get<1>(*newServerData));

            }
            TD_ACTION_CATCH_ALL_FROM(tr("New Server"))
        });

    // Lambda for editing the selected Server
    auto editServerFn = 
        [this](void)
        {
            TD_ACTION_TRY
            {

                // Get the index of the selected row in the table
                auto selRows =
                    m_serverTableView->selectionModel()->selectedRows();
                if (selRows.size() == 0) return;

                int selRow = selRows[0].row();

                // Get the Server object by iterating through its list
                // (inefficient, but number of Servers is 'small').
                auto serverItr = m_servers.begin();
                for (int i = 0; i < selRow; i++) serverItr++;

                auto selectedServer = serverItr.value();

                ServerDialog::DialogData
                    dataToEdit(
                        selectedServer->name(),
                        selectedServer->url().toString());

                QString oldName = selectedServer->name();

                // Execute the Server Dialog, with a validation function that
                // checks the URL, and that there are no duplicate names
                // but allows the existing name to be retained
                auto newServerData = ServerDialog::execute(
                    dataToEdit,
                    [this,&oldName](const ServerDialog::DialogData& data) ->
                            boost::optional<QString>
                    {
                        if (std::get<0>(data) != oldName)
                        {
                            if (m_servers.find(std::get<0>(data)) !=
                                    m_servers.end())
                                return tr("A Server with this name already "
                                    "exists");
                        }

                        // Make sure the URL is legit
                        if (!Api::Server::isValid(std::get<1>(data)))
                            return tr("The Server URL is not valid");

                        return boost::none;
                    },
                    tr("Edit Server"),
                    this);        

                // If we got legit new data, update the servers list.
                if (newServerData)
                {
                    QString newName = std::get<0>(*newServerData),
                        newUrl = std::get<1>(*newServerData);

                    // Make sure the new URL begins with HTTP or HTTPS
                    if (
                            (!newUrl.startsWith(
                                "http://",
                                Qt::CaseInsensitive))
                            && (!newUrl.startsWith(
                                "https://",
                                Qt::CaseInsensitive)))
                        newUrl = QString("http://") + newUrl;

                    // Remove the Server object from the collection, update
                    // its data, and then reinsert.
                    m_serverTableModel->beginServerCollectionChange();
                    m_servers.erase(serverItr);
                    selectedServer->setName(newName);
                    selectedServer->setUrl(QUrl(newUrl));
                    m_servers[newName] = selectedServer;
                    m_serverTableModel->endServerCollectionChange();

                    emit serversCollectionChanged();
                }   // end if we have some new server data

            }
            TD_ACTION_CATCH_ALL_FROM(tr("Edit Server"))
        };

    // Attach editing function from both the button and the table
    // double-click
    toolBar->addAction(
        tr("Edit Server"),
        editServerFn);

    connect(
        m_serverTableView,
        &QTableView::doubleClicked,
        editServerFn);

    // Action for deleting the selected Server
    toolBar->addAction(
        tr("Delete Server"),
        [this](void)
        {
            TD_ACTION_TRY
            {
                // Get the index of the selected row in the table
                auto selRows =
                    m_serverTableView->selectionModel()->selectedRows();
                if (selRows.size() == 0) return;

                int selRow = selRows[0].row();

                // Get the Server object by iterating through its list
                // (inefficient, but number of Servers is 'small').
                auto serverItr = m_servers.begin();
                for (int i = 0; i < selRow; i++) serverItr++;

                auto selectedServer = serverItr.value();

                // Get user confirmation
                if (QMessageBox::question(
                            this,
                            tr("Delete Server"),
                            tr("Please confirm deletion of Server \"") +
                                selectedServer->name() + QString("\""),
                            QMessageBox::Ok | QMessageBox::Cancel) ==
                        QMessageBox::Ok)
                {
                    // User has confirmed - execute deletion and notification
                    m_serverTableModel->beginServerCollectionChange();
                    m_servers.erase(serverItr);
                    m_serverTableModel->endServerCollectionChange();

                    emit serversCollectionChanged();
                }
            }
            TD_ACTION_CATCH_ALL_FROM(tr("Delete Server"))
        });

}   // end constructor

void ServersWidget::addNewServer(QString name, QString url)
{
    // Make sure the URL begins with http or https.
    QString _url = url;
    if ((!_url.startsWith("http://", Qt::CaseInsensitive))
            && (!_url.startsWith("https://", Qt::CaseInsensitive)))
        _url = QString("http://") + _url;

    // Create the Server object - we've already checked for validity and
    // duplicate names - and add it to the servers collection
///    auto newServer = std::make_shared<Api::Server>(name,  _url);
    auto newServer = Api::makeShared<Api::Server>(name, _url);

    m_serverTableModel->beginServerCollectionChange();
    m_servers[newServer->name()] = newServer;
    m_serverTableModel->endServerCollectionChange();
    
     emit serversCollectionChanged();
}   // end addNewServer method

}}  // end TootDesk::Gui namespace
