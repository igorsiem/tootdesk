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
 * \file status-and-account-test.cpp
 * Tests for the Status and Account classes
 * 
 * \author Igor Siemienowicz
 * 
 * \copyright GPL 3.0
 */

#include <iostream>
#include <catch/catch.hpp>
#include <gui/deskapi/deskapi.h>

namespace TdApi = TootDesk::Api;

TEST_CASE("Status and Account", "[unit][tdapi][online]")
{

    // Get the public timeline of mastodon.social, and take its first
    // status
    Mastodon::Easy masto("mastodon.social", "");
    std::string response;
    masto.get(Mastodon::API::v1::timelines_public, response);
    auto firstStatus =
        Mastodon::Easy::Easy::json_array_to_vector(response).front();

    // Parse a status object from the raw JSON
    TdApi::Status status(firstStatus);

    // TODO get additional status info

    // We can get a valid Account from the status.
    auto account = status.account();
    REQUIRE(account);
    REQUIRE(account->isValid());
    
    // Status is valid.
    REQUIRE(status.isValid());

    // std::cout
    //     << std::endl << "[DEBUG] user name:    " <<
    //         account->userName().toStdString()
    //     << std::endl << "[DEBUG] display name: " <<
    //         account->displayName().toStdString()
    //     << std::endl;

    // std::cout
    //     << std::endl << "[DEBUG] status created at: " <<
    //         status.createdAt().toString().toStdString()
    //     << std::endl << "[DEBUG] content: " <<
    //         status.content().toStdString()
    //     << std::endl;

    // Basic checks on information retrieved - it will be different each
    // time the test is run.
    REQUIRE(!account->userName().isEmpty());
    REQUIRE(!account->displayName().isEmpty());
    REQUIRE(status.createdAt() > QDateTime::fromMSecsSinceEpoch(0));
    REQUIRE(!status.content().isEmpty());

}   // end Status and Account tests
