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
 * \file account.cpp
 * Implements the Account class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QDebug>
#include "account.h"

namespace TootDesk { namespace Api {

Account::Account(MastodonAccountPtr mastodonAccount) :
    m_mastodonAccount(mastodonAccount)
{}

Account::Account(const std::string& json) :
    Account(makeShared<MastodonAccount>(json))
{}

bool Account::isValid(void) const
{
    if (!m_mastodonAccount) return false;
    return m_mastodonAccount->valid();
}   // end isValid method

QString Account::userName(void) const
{
    if (isValid())
        return std::move(
            QString::fromStdString(m_mastodonAccount->acct()));
    return QString();
}   // end userName method

QString Account::displayName(void) const
{
    if (isValid())
        return std::move(
            QString::fromStdString(m_mastodonAccount->display_name()));
    return QString();
}   // end displayName method

}}  // end TootDesk::Api namespace
