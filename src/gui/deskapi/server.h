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

#include <QUrl>

#ifndef _td_gui_deskapi_h_included
#define _td_gui_deskapi_h_included

namespace TootDesk { namespace Api {

class Server
{

    // --- Public Interface ---

    public:

    // -- Public Subtypes --

    /**
     * \brief Initialise the server object with its URL
     *
     * \param url The URL of the Server
     */
    explicit Server(QUrl url = QUrl());

    /**
     * \brief Initialse with a string URL
     *
     * \param url The URL of the server
     */
    explicit Server(QString url);

    virtual ~Server(void) = default;    ///< Default destructor

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
    static bool isValid(const QString& url) { return isValid(QUrl(url)); }

    /**
     * \brief Determines whether or not the Server is valid for use.
     *
     * In this iteration, this method simple calls the static `isValid`
     * method overload with `this->m_url`.
     *
     * \return `true` if the Server object is valid
     */
    bool isValid(void) const;

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

    // --- Internal Declaratons ---

    protected:

    /**
     * \brief The URL of the server
     *
     * We take the `host` and `port` components (if specified) for the
     * Mastodon server
     */
    QUrl m_url;

};  // end Server class

}}  // end TootDesk::Api namespace

#endif
