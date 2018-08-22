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
#include <QVBoxLayout>

#include "errorhandling.h"
#include "serverswidget.h"

namespace TootDesk { namespace Gui {

ServersWidget::ServersWidget(
        Api::ServerByNameMap& servers,
        QWidget* parent) :
    QWidget(parent)
    , m_servers(servers)
    , m_serverTableModel(nullptr)
{

    // Set up our layout as a V-Box with no margins
    setLayout(new QVBoxLayout(this));
    layout()->setContentsMargins(0, 0, 0, 0);

    // A simple label to say what we are
    layout()->addWidget(new QLabel(tr("Server instances"), parent));

    // The table view and model - the model wraps our reference to the
    // Servers container
    auto serverTableView = new QTableView(this);
    layout()->addWidget(serverTableView);
    m_serverTableModel =
        new Gui::ServerTableModel(m_servers, serverTableView);
    serverTableView->setModel(m_serverTableModel);

    // Create an add / edit section at the bottom of the table
    auto editingWidget = new QWidget(this);
    layout()->addWidget(editingWidget);
    auto editingLayout = new QGridLayout(editingWidget);
    editingWidget->setLayout(editingLayout);
    editingLayout->setContentsMargins(0, 0, 0, 0);

    // Editing section has a name, address and a button that says either
    // 'Add' or 'Update', depending on what we're doing
    editingLayout->addWidget(new QLabel(tr("Name"), editingWidget), 0, 0);
    editingLayout->addWidget(new QLabel(tr("Address"), editingWidget), 0, 1);

    auto nameLineEdit = new QLineEdit(editingWidget)
        , addressLineEdit = new QLineEdit(editingWidget);

    editingLayout->addWidget(nameLineEdit, 1, 0);
    editingLayout->addWidget(addressLineEdit, 1, 1);

    auto editingButton = new QPushButton(tr("Add"), editingWidget);

    connect(
        editingButton,
        &QPushButton::clicked,
        [this, nameLineEdit, addressLineEdit](bool)
        {
            TD_ACTION_TRY
            {
                // Add a new Server from the edit items
                //
                // TODO expand to work with updates as well
                addNewServer(nameLineEdit->text(), addressLineEdit->text());
            }
            TD_ACTION_CATCH_ALL_FROM("Adding something")
        });
    editingLayout->addWidget(editingButton, 1, 2);

}   // end constructor

void ServersWidget::addNewServer(QString name, QString url)
{
    // Make sure the URL begins with http or https.
    QString _url = url;
    if ((!_url.startsWith("http://")) && (!_url.startsWith("https://")))
        _url = QString("http://") + _url;

    // Check to see if the URL is valid
    if (!Api::Server::isValid(_url))
        TD_RAISE_API_ERROR(tr("invalid url for Server Instance: ") << _url);

    // Create the Server object
    auto newServer = std::make_shared<Api::Server>(name,  _url);

    // Check that we don't already have a server with the same name. We use
    // the Server object for this, because the derived Server object name
    // may be different from the `name` parameter
    if (m_servers.find(newServer->name()) == m_servers.end())
    {
        m_serverTableModel->beginServerCollectionChanged();
        m_servers[newServer->name()] = newServer;
        m_serverTableModel->endServerCollectionChanged();
    }
    else TD_RAISE_API_ERROR(
        tr("a Server instance with this name already exists: ") <<
        newServer->name());

     emit serversCollectionChanged();   
}   // end addNewServer method

}}  // end TootDesk::Gui namespace
