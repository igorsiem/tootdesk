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

#include <atomic>
#include <thread>

#include <boost/optional.hpp>

#include <QDebug>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QQueue>
#include <QString>
#include <QUrl>
#include <QVariant>
#include <QWaitCondition>

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
 * Each Server object runs a background thread for online operations. The
 * thread is created (and started) in the constructor, but sleeps until there
 * is a background task for the server to perform. We acknowledge that the
 * 'thread per Server' design is not particularly scalable, but the number of
 * Servers is expected to be 'small'.
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

    /**
     * \brief Destructor - ensures that background thread is properly shut
     * down and joined before exiting
     */
    virtual ~Server(void) noexcept;

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

    /**
     * \brief Check whether or not this server has an online operation in
     * progress
     * 
     * \return `true` if this Server is currently executing an online
     * operation
     */
    bool onlineOperationInProgress(void) const
        { return m_onlineOperationInProgress.load(); }

    /**
     * \brief Retrieve flag indicating whether instance data is current
     * 
     * Instance data is retrieved from the server (using the
     * `retrieveInstanceInfo` method). This flag is set to `true` after the
     * data is retrieved. It is cleared if the URL is changed.
     * 
     * \return True if the instance data is current.
     */
    bool instanceDataIsCurrent(void) const;

    /**
     * \brief Retrieve the instance data, if it is current
     * 
     * Instance data is retrieved from the server (using the
     * `retrieveInstanceInfo` method). This method retrieves the Title and
     * Description fields (as a tuple) if the data is current.
     */
    boost::optional<std::tuple<QString, QString> >
    instanceData(void) const;

    // -- Remote Operations --

    void retrieveInstanceInfo(void);

    signals:

    void instanceInfoRetrieved(void);

    // --- Internal Declaratons ---

    protected:

    // -- Internal Types --

    /**
     * \brief A callable object that is placed in a queue to be executed
     * as a task
     */
    using TaskFn = std::function<void(void)>;

    /**
     * \brief A structure intended for background execution, comprised of a
     * callable object and a 'done' flag
     */
    using Task = std::tuple<TaskFn>;

    TD_DECLARE_SHARED_POINTERS_FOR(Task);

    /**
     * \brief A queue of shared pointers to task objects
     */
    using TaskQueue = QQueue<TaskPtr>;

    // -- Helper Functions --

    void processTasks(void);

    void enqueue(TaskFn fn);

    // -- Attributes --

    // - Status / Housekeeping -

    /**
     * \brief Mutex protecting MT access to the basic data of our server
     */
    mutable Mutex m_dataMtx;

    /**
     * \brief Flag indicating that an online operation is currently in
     * progress
     */
    std::atomic<bool> m_onlineOperationInProgress;

    /**
     * \brief Flag indicating that the server is about to be destroyed - no
     * new operations should be started
     */
    std::atomic<bool> m_shuttingDown;

    // - Data About the Server -

    /**
     * \brief Human-readable name for the Instance
     *
     * This may be empty. If so, the `mastodonAddress` is used as the name.
     * This attribute is protected by `m_dataMtx`.
     */
    QString m_name;

    /**
     * \brief The URL of the server
     *
     * We take the `host` and `port` components (if specified) for the
     * Mastodon server. This attribute is protected by `m_dataMtx`.
     */
    QUrl m_url;

    // - Instance Information -

    /**
     * \brief Flag indicating that the instance data is up to date
     * 
     * This flag is set when the instance information is retrieve (just
     * before emitting `instanceInfoRetrieved`), and cleared when the URL
     * is changed. Access to this attribute is protected by `m_dataMtx`.
     */
    bool m_instanceDataIsCurrent;

    /**
     * \brief The human-readable Title of the Instance
     * 
     * This data is retrieved from the server using the
     * `retrieveInstanceInfo` method. The `m_instanceDataIsCurrent`
     * indicates whether it is current. Access to this attribute is protected
     * by `m_dataMtx`.
     */
    QString m_instanceTitle;

    /**
     * \brief The human-readable Description of the Instance
     * 
     * This data is retrieved from the server using the
     * `retrieveInstanceInfo` method. The `m_instanceDataIsCurrent`
     * indicates whether it is current. Access to this attribute is protected
     * by `m_dataMtx`.
     */
    QString m_instanceDescription;

    /**
     * \brief A background thread for executing queued tasks
     */
    SharedPtr<std::thread> m_workerThread;

    /**
     * \brief Mutex protecting the `m_tasks` queue
     */
    mutable Mutex m_tasksMtx;

    /**
     * \brief The queue of tasks for background execution
     */
    TaskQueue m_tasks;

    QWaitCondition m_timeToCheckForTasks;

};  // end Server class

TD_DECLARE_SHARED_POINTERS_FOR(Server)

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
