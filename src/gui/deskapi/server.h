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
 * \file server.h
 * Declares the `TootDesk::Api::Server` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QMap>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVariant>

#include "coretypes.h"

#ifndef _td_gui_deskapi_server_h_included
#define _td_gui_deskapi_server_h_included

namespace TootDesk { namespace Api {

/**
 * \brief Encapsulates a Mastodon server instance that can be queried for
 * status items (Toots)
 * 
 * This class implements a Qt-style interace for the `Mastodon::Easy::Easy`
 * and `Mastodon::Easy::Instance` classes.
 * 
 * `Server` methods are thread-safe.
 *
 * \todo Expand and tidy up this doco
 */
class Server : public QObject
{

    Q_OBJECT

    // --- Public Interface ---

    public:

    // -- Construction / Destruction --

    /**
     * \brief Initialise the server object with its URL
     *
     * \brief A human-readable name for the server instance
     *
     * \param url The URL of the Server
     * 
     * \param parent The parent (owner) of the object in the hierarchy
     */
    Server(QString name, QUrl url, QObject* parent = nullptr);

    /**
     * \brief Initialise with a string URL
     *
     * \param url The URL of the server
     */
    explicit Server(QString url = "") : Server("", QUrl(url)) {}

    /**
     * \brief Initialise with two strings - name and URL
     *
     * \brief A human-readable name for the server instance
     *
     * \param url The URL of the Server (as a string)
     */
    Server(QString name, QString url) : Server(name, QUrl(url)) {}

    virtual ~Server(void) = default;    ///< Default destructor

    // -- Validation --

    /**
     * \brief Determine whether the given URL is valid for use as a Mastodon
     * server address
     *
     * In this iteration, this method checks to see if the URL is valid as a
     * generic URL, and also if it uses the `http` or `https` schemes.
     *
     * This can be used to pre-validate an address before passing it to a
     * `Server` object
     *
     * \param url The URL to check
     *
     * \return `true` if the URL is valid
     */
    static bool isValid(const QUrl& url);

    /**
     * \brief Determine whether the given URL is valid for use as a Mastodon
     * server address
     *
     * In this iteration, this method checks to see if the URL is valid as a
     * generic URL, and also if it uses the `http` or `https` schemes.
     *
     * This can be used to pre-validate an address before passing it to a
     * `Server` object
     *
     * \param url The URL to check
     *
     * \return `true` if the URL is valid
     */
    static bool isValidUrl(const QString& url) { return isValid(QUrl(url)); }

    /**
     * \brief Determines whether or not the Server is valid for use.
     *
     * In this iteration, this method simple calls the static `isValid`
     * method overload with `this->m_url`.
     *
     * \return `true` if the Server object is valid
     */
    bool isValid(void) const;

    // -- Accessors --

    /**
     * \brief Retrieve the human-readable name of the Server
     *
     * Note that if a human-readable name has not been set, the result of
     * `mastodonAddress` is returned instead (as a `QString`).
     *
     * \return The human-readable name of the Server
     */
    QString name(void) const;

    /**
     * \brief Set the human-readable name of the Server
     *
     * If this is set to an empty string, the human-readable name will be the
     * result of `mastodonAddress` instead.
     *
     * \param name The name to set
     */
    void setName(QString name) { m_name = std::move(name); }

    /**
     * \brief Retrieve the Server URL
     *
     * \return The Server URL
     */
    QUrl url(void) const { return m_url; }

    /**
     * \brief Set the Server URL
     *
     * Note that a validity check is run for the URL, but it only logs a
     * debug message if the URL is invalid. Call `isValid` after setting to
     * check validity.
     *
     * \param url The URL to set
     */
    void setUrl(QUrl url);

    /**
     * \brief Set the Server URL as a string
     *
     * Note that a validity check is run for the URL, but it only logs a
     * debug message if the URL is invalid. Call `isValid` after setting to
     * check validity.
     *
     * \param url The URL to set
     */
    void setUrl(QString url);

    /**
     * \brief Retrieve the Server address in the form expected by the
     * Mastodon API 
     *
     * \return The server address as a standard string
     *
     * \throws TootDesk::Api::Error If the Server URL is not valid
     */
    std::string mastodonAddress(void) const;

    // -- Access Data --



    // --- Internal Declaratons ---

    protected:

    /**
     * \brief MT synchronised access to the internals of a Server object
     */
    mutable Mutex m_mtx;

    /**
     * \brief Human-readable name for the Instance
     *
     * This may be empty. If so, the `mastodonAddress` is used as the name.
     */
    QString m_name;

    /**
     * \brief The URL of the server
     *
     * We take the `host` and `port` components (if specified) for the
     * Mastodon server
     */
    QUrl m_url;

};  // end Server class

/**
 * \brief A shared pointer to a Server object
 */
using ServerPtr = SharedPtr<Server>;

/**
 * \brief A shared pointer to a const Server object
 */
using ConstServerPtr = SharedPtr<const Server>;

/**
 * \brief A std::map of shared pointers to `Server` objects, indexed by Name
 */
using ServerByNameMap = QMap<QString, ServerPtr>;

/**
 * \brief Convert a container of Server objects to a form that can be
 * serialised by the Qt settings object
 *
 * \param servers The collection of Server objects
 *
 * \param serialisable The serialisable collection (a `QMap` of strings to
 * variants) that will be filled with data; note that this is not emptied
 * prior to adding data
 *
 * \return A reference to `serialisable`
 *
 * \throws TootDesk::Api::Error One of the Server objects was invalid
 */
extern QMap<QString, QVariant>& convertForSerialisation(
    const ServerByNameMap& servers,
    QMap<QString, QVariant>& serialisable);

/**
 * \brief Convert a container of Server objects to a form that can be
 * serialised by the Qt settings object (using a temporary)
 *
 * \param servers The collection of Server objects
 *
 * \param serialisable The serialisable collection (a `QMap` of strings to
 * variants) that will be filled with data; note that this is not emptied
 * prior to adding data
 *
 * \return The serialisable collection (a `QMap` of strings to variants)
 *
 * \throws TootDesk::Api::Error One of the Server objects was invalid
 */
extern QMap<QString, QVariant> convertForSerialisation(
    const ServerByNameMap& servers);

/**
 * \brief Convert a serialisable container to Server objects
 *
 * \param deserialised A `QMap` of strings to variants containing the Server
 * data
 *
 * \param servers The container of Server objects to populate; note that this
 * is not emptied prior to adding new data
 *
 * \return A reference to `servers`
 *
 * \throws TootDesk::Api::Error Invalid server data was encountered in the
 * deserialised data
 */
extern ServerByNameMap& convertFromSerialisation(
    const QMap<QString, QVariant>& deserialised,
    ServerByNameMap& servers);

/**
 * \brief Convert a serialisable container to Server objects (using a
 * temporary)
 *
 * \param deserialised A `QMap` of strings to variants containing the Server
 * data
 *
 * \return The newly created Server objects
 *
 * \throws TootDesk::Api::Error Invalid server data was encountered in the
 * deserialised data
 */
extern ServerByNameMap convertFromSerialisation(
    const QMap<QString, QVariant>& deserialised);

}}  // end TootDesk::Api namespace

#endif
