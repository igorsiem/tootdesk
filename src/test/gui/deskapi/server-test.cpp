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
 * \file server-test.cpp
 * Tests for the `TootDesk::Api::Server` class
 * 
 * \author Igor Siemienowicz
 * 
 * \copyright GPL 3.0
 */

#include <catch/catch.hpp>
#include <gui/deskapi/deskapi.h>

namespace tdapi = TootDesk::Api;

// Basic tests for the Server class
TEST_CASE("TootDesk::Api::Server", "[unit][tdapi]")
{

    tdapi::Server server;

    // An empty server URL - invalid
    SECTION("empty URL")
    {
        REQUIRE(!tdapi::Server::isValid(""));
        REQUIRE(!server.isValid());
    }   // end empty URL section

    // An invalid non-empty URL
    SECTION("invalid non-empty URL")
    {
        QString url = "xyz";
        REQUIRE(!tdapi::Server::isValid(url));
        server.setUrl(url);
        REQUIRE(!server.isValid());
    }   // end invalid non-empty URL section

    // A valid URL with an invalid scheme
    SECTION("valid URL, invalid scheme")
    {
        QString url = "ftp://example.com";
        REQUIRE(!tdapi::Server::isValid(url));
        server.setUrl(url);
        REQUIRE(!server.isValid());
    }   // end valid URL, invalid scheme section

    SECTION("valid mastodon Server URL")
    {
        // A valid URL for a server
        QString url = "http://example.com:8080";
        REQUIRE(tdapi::Server::isValid(url));
        server.setUrl(url);
        REQUIRE(server.isValid());

        // For the mastodon library, extract the host and port number
        REQUIRE(server.mastodonAddress() == "example.com:8080");
    }   // end valid Mastodon Server url section

}   // end Server test
