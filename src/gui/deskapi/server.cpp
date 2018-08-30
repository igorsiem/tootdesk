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
 * \file server.cpp
 * Implements the `TootDesk::Api::Server` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <sstream>

#include <QDebug>
#include <QThreadPool>

#include <mastodon-cpp/mastodon-cpp.hpp>
#include <mastodon-cpp/easy/all.hpp>

#include "mastodonerror.h"
#include "server.h"

namespace TootDesk { namespace Api {

Server::Server(QString name, QUrl url, QObject* parent) :
    QObject(parent)
    , m_dataMtx()
    , m_onlineOperationInProgress(false)
    , m_shuttingDown(false)
    , m_name(std::move(name))
    , m_url(std::move(url))
    , m_instanceDataIsCurrent(false)
    , m_instanceTitle()
    , m_instanceDescription()
    , m_workerThread(nullptr)
    , m_tasksMtx()
    , m_tasks()
    , m_timeToCheckForTasks()
{
    if (!isValid()) qWarning() << "Server address" << url.toString() <<
        "is invalid";

    // Start the worker thread for this server
    m_workerThread =
        makeShared<std::thread>([this](void) { processTasks(); });
}   // end constructor

Server::~Server(void) noexcept
{
    // Tell the worker thread that it's time to stop, wake it up if
    // necessary, then wait for it to finish.
    m_shuttingDown = true;
    m_timeToCheckForTasks.wakeAll();
    m_workerThread->join();

    qDebug() << __FUNCTION__ << "- Server" << name() << "has shut down";

}   // end destructor

bool Server::isValid(const QUrl& url)
{
    // First, check whether the URL is valid according to Qt's rules.
    if (!url.isValid()) return false;

    // Must be http or https scheme.
    if ((url.scheme() != "http") && (url.scheme() != "https"))
        return false;

    return true;
}   // end isValid

bool Server::isValid(void) const
{
    ReadGuard grd(&m_dataMtx);
    return isValid(m_url);
}   // end isValid

QString Server::name(void) const
{
    ReadGuard grd(&m_dataMtx);

    if (m_name.isEmpty())
    {
        if (isValid())
            return QString::fromStdString(mastodonAddress());
        else return "";
    }

    return m_name;
}   // end name method

void Server::setUrl(QUrl url)
{
    WriteGuard grd(&m_dataMtx);
    m_url = std::move(url);

    // Our instance data is no longer currnet
    m_instanceDataIsCurrent = false;

    grd.unlock();

    if (!isValid()) qWarning() << "Server address" << url.toString() <<
        "is invalid";
}   // end setUrl method

void Server::setUrl(QString url)
{
    WriteGuard grd(&m_dataMtx);
    m_url = QUrl(std::move(url));
    m_instanceDataIsCurrent = false;
    grd.unlock();
    if (!isValid()) qWarning() << "Server address" << url << "is invalid";
}   // end setUrl

std::string Server::mastodonAddress(void) const
{
    if (!isValid())
        TD_RAISE_API_ERROR(QObject::tr("server address not valid: ") <<
            m_url.toString());

    ReadGuard grd(&m_dataMtx);
    std::stringstream strm;
    strm << m_url.host().toStdString();

    // Add the port if specified.
    if (m_url.port() > 0) strm << ":" << m_url.port();

    return std::move(strm.str());
}   // end mastodonAddress

bool Server::instanceDataIsCurrent(void) const
{
    ReadGuard grd(&m_dataMtx);
    return m_instanceDataIsCurrent;    
}

boost::optional<std::tuple<QString, QString> >
Server::instanceData(void) const
{
    ReadGuard grd(&m_dataMtx);
    if (m_instanceDataIsCurrent)
        return std::make_tuple(m_instanceTitle, m_instanceDescription);
    else return boost::none;
}

void Server::retrieveInstanceInfo(void)
{

    enqueue([this](void)
    {

        qDebug() << "starting retrieve instance info task -" <<
            QString::fromStdString(mastodonAddress());

        m_onlineOperationInProgress = true;

        Mastodon::Easy masto(mastodonAddress(), "");    // no need to auth
        std::string response;

        m_onlineOperationInProgress = false;

        // Get the info, checking for errors
        qDebug() << "    " << name() << "commencing get...";
        auto result = masto.get(Mastodon::API::v1::instance, response);
        if (result) MastodonError(result).raise();
        qDebug() << "    ... " << name() << "get completed";

        Mastodon::Easy::Instance instanceData(response);

        WriteGuard grd(&m_dataMtx);
        m_instanceTitle = QString::fromStdString(instanceData.title());
        m_instanceDescription =
            QString::fromStdString(instanceData.description());
        m_instanceDataIsCurrent = true;
        grd.unlock();

        emit instanceInfoRetrieved(
            m_instanceTitle,
            m_instanceDescription);

        qDebug() << "finished retrieve instance info task -" <<
            QString::fromStdString(mastodonAddress());

    });

}   // end retrieveInstanceInfo

void Server::processTasks(void)
{
    // TODO exception handling

    qDebug() << "Server" << name() << "background processing begins";

    // Loop until the 'shutting down' flag is raised
    while (m_shuttingDown.load() == false)
    {
        // Wait until there are tasks in the queue (or it's time to shut
        // down)
        WriteGuard tasksGrd(&m_tasksMtx);

        if (m_tasks.empty() && (m_shuttingDown.load() == false))
            m_timeToCheckForTasks.wait(&m_tasksMtx);

        // We've woken up - is it time to stop?
        if (m_shuttingDown.load()) break;

        // Get the next task from the queue
        auto task = m_tasks.dequeue();

        tasksGrd.unlock();

        // Execute the task, catching all errors. These are emitted as
        // signals from the Server component (as well as logged to
        // qCritical).
        try
        {
            std::get<0>(*task)();
        }
        catch (const Error& error)
        {
            qCritical() << "Server" << name() << "encountered the "
                "following error:" << error.message();

            emit errorOccurred(name(), error.message());
        }
        catch (const std::exception& error)
        {
            auto message = QString::fromStdString(error.what());

            qCritical() << "Server" << name() << "encountered the "
                "following error:" << message;

            emit errorOccurred(name(), message);
        }
        catch (...)
        {
            QString message("unrecognised exception / error condition");

            qCritical() << "Server" << name() << "encountered the "
                "following error:" << message;

            emit errorOccurred(name(), message);
        }
    }

    qDebug() << "Server" << name() << "background processing ends";
}   // end process tasks

void Server::enqueue(TaskFn fn)
{
    auto t = makeShared<Task>(fn);

    WriteGuard grd(&m_tasksMtx);
    m_tasks.enqueue(t);
    grd.unlock();

    m_timeToCheckForTasks.wakeAll();
}   // end enqueue

QMap<QString, QVariant>& convertForSerialisation(
        const ServerByNameMap& servers,
        QMap<QString, QVariant>& serialisable)
{

    for (auto itr = servers.begin(); itr != servers.end(); ++itr)
    {

        // Check for validity
        if (itr.value()->isValid() == false)
            TD_RAISE_API_ERROR(
                QObject::tr(
                    "encountered invalid Server object while serialising"));

        QVariant name(itr.value()->name()), url(itr.value()->url());

        QMap<QString, QVariant> serverMap;
        serverMap["name"] = name;
        serverMap["url"] = url;

        serialisable[itr.key()] = serverMap;
    }

    return serialisable;
}   // end convertForSerialisation function

QMap<QString, QVariant> convertForSerialisation(
        const ServerByNameMap& servers)
{
    QMap<QString, QVariant> serialisable;
    convertForSerialisation(servers, serialisable);
    return std::move(serialisable);
}   // end convertForSerialisation function

ServerByNameMap& convertFromSerialisation(
        const QMap<QString, QVariant>& deserialised,
        ServerByNameMap& servers)
{
    for (auto itr : deserialised)
    {
        if (itr.isValid() == false)
            TD_RAISE_API_ERROR(
                QObject::tr("invalid Server object encountered while "
                    "deserialising"));

        QMap<QString, QVariant> serverMap = itr.toMap();
        auto newServer = Api::makeShared<Server>(
            serverMap["name"].toString(),
            serverMap["url"].toUrl());

        if (!newServer->isValid())
            TD_RAISE_API_ERROR(
                QObject::tr("invalid Server object encountered while "
                    "deserialisaing; name \"") <<
                    serverMap["name"].toString() <<
                    QObject::tr("\"; URL \"") <<
                    serverMap["url"].toString() << "\"");

        servers[newServer->name()] = newServer;
    }

    return servers;
}   // end convertFromSerialisation

ServerByNameMap convertFromSerialisation(
        const QMap<QString, QVariant>& deserialised)
{
    ServerByNameMap servers;
    convertFromSerialisation(deserialised, servers);
    return servers;
}   // end convertFromSerialisation function

}}  // end TootDesk::Api
