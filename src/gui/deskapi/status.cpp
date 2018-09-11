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
 * \file status.cpp
 * Implements the Status class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <chrono>
#include "status.h"

namespace TootDesk { namespace Api {

Status::Status(MastodonStatusPtr mastodonStatus) :
        m_mastodonStatus(nullptr),
        m_account(nullptr)
{
    setMastodonStatus(mastodonStatus);
}

Status::Status(const std::string& json) :
    Status(makeShared<MastodonStatus>(json))
{
}

bool Status::isValid(void) const
{
    if (!m_mastodonStatus) return false;
    return m_mastodonStatus->valid();
}

AccountPtr Status::account(void) const
{
    return m_account;
}   // end account method

QDateTime Status::createdAt(void) const
{
    if (isValid())
    {
        auto msSinceEpoch =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                m_mastodonStatus->created_at().time_since_epoch()).count();

        return QDateTime::fromMSecsSinceEpoch(msSinceEpoch);
    }
    else return QDateTime::fromMSecsSinceEpoch(0);
}   // end createdAt

QString Status::content(void) const
{
    if (isValid())
    {
        return std::move(
            QString::fromStdString(m_mastodonStatus->content()));
    }
    else return QString();
}   // end content method

uint_fast64_t Status::id(void) const
{
    if (isValid()) return m_mastodonStatus->id();
    else return 0;
}   // end id method

}}  // end TootDesk::Api
