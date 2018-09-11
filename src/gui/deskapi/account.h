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
 * \file account.h
 * Declares the Account class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <mastodon-cpp/mastodon-cpp.hpp>
#include <mastodon-cpp/easy/all.hpp>

#include "coretypes.h"

#ifndef _td_gui_deskapi_account_h_included
#define _td_gui_deskapi_account_h_included

namespace TootDesk { namespace Api {

/**
 * \brief An account on Mastodon (note this can be any Mastodon account, not
 * just the user's account)
 * 
 * This is a Qt-based wrapper for the `Mastodon::Easy::Easy::Account` class.
 * It encapsulates all the 'public' information about any account associated
 * with a Status.
 */
class Account
{
    // --- External Interface

    public:

    /**
     * \brief The Mastodon Easy Account structure, which wraps the raw JSON
     * string
     */
    using MastodonAccount = Mastodon::Easy::Easy::Account;

    TD_DECLARE_SHARED_POINTERS_FOR(MastodonAccount);

    // -- Constructor / Destructor --

    /**
     * \brief Construct an Account from a Mastodon Easy Account structure
     * 
     * \param A shared pointer to the underlying Mastodon Easy Account
     */
    explicit Account(MastodonAccountPtr mastodonAccount);

    /**
     * \brief Construct an account from a raw JSON string
     * 
     * \param json The JSON string to process
     */
    explicit Account(const std::string& json);

    /**
     * \brief Construct an empty (invalid) account structure
     */
    Account(void) : m_mastodonAccount(nullptr) {}

    virtual ~Account(void) = default;

    TD_DISABLE_COPY_AND_MOVE_SEMANTICS_FOR(Account)

    // -- Accessors --

    /**
     * \brief Set the underlying Mastodon Easy Account structure
     */
    void setMastodonAccount(MastodonAccountPtr m)
    {
        m_mastodonAccount = m;
    }

    /**
     * \brief Set the account data from a raw JSON string
     * 
     * \param json The raw JSON string from which to generate the account
     */
    void fromJson(const std::string& json)
    {
        setMastodonAccount(makeShared<MastodonAccount>(json));
    }

    /**
     * \brief Check that this is a valid, initialised account
     * 
     * \return `true` if the account structure is valid for use
     */
    bool isValid(void) const;

    /**
     * \brief Retrieve the User Name
     * 
     * This is a simple User Name if the Account is retrieved from its own
     * Server Instance, or `user@hostname` if from some other Instance
     * 
     * \return The User Name, or an empty string if the Account is not valid
     */
    QString userName(void) const;

    /**
     * \brief Retrieve the 'display name' of the account
     * 
     * \return The Display Name, or an empty string if the Account is not
     * valid
     */
    QString displayName(void) const;

    // --- Internal Declarations

    protected:

    MastodonAccountPtr m_mastodonAccount;

};  // end Account class

TD_DECLARE_SHARED_POINTERS_FOR(Account)

}}  // end TootDesk::Api namespace

#endif
