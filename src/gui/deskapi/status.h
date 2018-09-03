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
 * \file status.h
 * Declares the Status class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QDateTime>

#include <mastodon-cpp/mastodon-cpp.hpp>
#include <mastodon-cpp/easy/all.hpp>

#include "account.h"
#include "coretypes.h"

#ifndef _td_gui_deskapi_status_h_included
#define _td_gui_deskapi_status_h_included

namespace TootDesk { namespace Api {

/**
 * \brief A single status item in a timeline
 * 
 * This class is a Qt-style wrapper for the `Mastodon::Easy::Easy::Status`
 * class.
 */
class Status
{

    // --- External Interface ---

    public:

    // -- Public Sub-types --

    /**
     * \brief The underlying Mastodon Status type
     */
    using MastodonStatus = Mastodon::Easy::Easy::Status;

    TD_DECLARE_SHARED_POINTERS_FOR(MastodonStatus)

    // -- Constructor / Destructor

    /**
     * \brief Construct a Status object from a Mastodon Easy Status structure
     * 
     * \param mastodonStatus The Mastodon Easy Status structure that this
     * object will wrap
     */
    explicit Status(MastodonStatusPtr mastodonStatus);

    /**
     * \brief Construct a Status object from a raw JSON structure
     * 
     * \param json The raw JSON string from which to construct the object
     */
    explicit Status(const std::string& json);

    /**
     * \brief Construct an empty, invalid Status object
     */
    explicit Status(void) : m_mastodonStatus(nullptr), m_account(nullptr) {}

    virtual ~Status(void) = default;

    TD_DISABLE_COPY_AND_MOVE_SEMANTICS_FOR(Status)

    // -- Accessors --

    /**
     * \brief Set the underluing Mastodon Easy Status structure
     * 
     * \param mastodonStatus The Mastodon Status object to set
     */
    void setMastodonStatus(MastodonStatusPtr mastodonStatus)
    {
        m_mastodonStatus = mastodonStatus;

        m_account = makeShared<Account>(
            makeShared<Account::MastodonAccount>(
                mastodonStatus->account()));
    }

    /**
     * \brief Set up the Status object from a raw JSON string
     * 
     * \param json The raw string from which to set the Status data
     */
    void fromJson(const std::string& json)
    {
        setMastodonStatus(makeShared<MastodonStatus>(json));
    }

    /**
     * \brief Check whether the status object is valid
     * 
     * \return `true` if the Status object is valid and can be used
     */
    bool isValid(void) const;

    /**
     * \brief Retrieve the Account associated with this Status
     * 
     * \return A shared pointer to the Account; this will be `nullptr` if
     * the Status is invalid
     */
    AccountPtr account(void) const;

    /**
     * \brief Retrieve the creation date/time
     * 
     * \return the Date / Time the status object was created, if the status
     * is valid, or 0:00:00 1 Jan 1970 if not.
     */
    QDateTime createdAt(void) const;

    /**
     * \brief Retrieve the content of the status
     * 
     * \return The status content, or an empty string if the status is
     * invalid
     */
    QString content(void) const;

    /**
     * \brief Retrieve the ID of the status
     * 
     * \return The ID, or zero if Status object is invalid
     */
    uint_fast64_t id(void) const;

    // --- Internal Declarations ---

    protected:

    /**
     * \brief The Mastodon status object that we are wrapping
     */
    MastodonStatusPtr m_mastodonStatus;

    /**
     * \brief A pointer to the Account object associated with this Status
     */
    AccountPtr m_account;

};  // end Status class

TD_DECLARE_SHARED_POINTERS_FOR(Status)

}}  // end TootDesk::Api namespace

#endif
