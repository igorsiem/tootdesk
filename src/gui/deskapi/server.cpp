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

#include "error.h"
#include "server.h"

namespace TootDesk { namespace Api {

Server::Server(QString name, QUrl url, QObject* parent) :
    QObject(parent)
    , m_mtx()
    , m_name(std::move(name))
    , m_url(std::move(url))
{
    if (!isValid()) qWarning() << "Server address" << url.toString() <<
        "is invalid";
}   // end constructor

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
    ReadGuard grd(&m_mtx);
    return isValid(m_url);
}   // end isValid

QString Server::name(void) const
{
    ReadGuard grd(&m_mtx);

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
    WriteGuard grd(&m_mtx);
    m_url = std::move(url);
    grd.unlock();
    if (!isValid()) qWarning() << "Server address" << url.toString() <<
        "is invalid";
}   // end setUrl method

void Server::setUrl(QString url)
{
    WriteGuard grd(&m_mtx);
    m_url = QUrl(std::move(url));
    grd.unlock();
    if (!isValid()) qWarning() << "Server address" << url << "is invalid";
}   // end setUrl

std::string Server::mastodonAddress(void) const
{
    if (!isValid())
        TD_RAISE_API_ERROR(QObject::tr("server address not valid: ") <<
            m_url.toString());

    ReadGuard grd(&m_mtx);
    std::stringstream strm;
    strm << m_url.host().toStdString();

    // Add the port if specified.
    if (m_url.port() > 0) strm << ":" << m_url.port();

    return std::move(strm.str());
}   // end mastodonAddress

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
