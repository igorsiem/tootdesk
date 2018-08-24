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

namespace TdApi = TootDesk::Api;

// Basic tests for the Server class
TEST_CASE("TootDesk::Api::Server basic", "[unit][tdapi]")
{

    TdApi::Server server;

    // An empty server URL - invalid
    SECTION("empty URL")
    {
        REQUIRE(!TdApi::Server::isValidUrl(""));
        REQUIRE(!server.isValid());

        // Name (derived) is empty
        REQUIRE(server.name().isEmpty());

        // Can explicitly set a name, though.
        server.setName("Example");
        REQUIRE(server.name() == "Example");
    }   // end empty URL section

    // An invalid non-empty URL
    SECTION("invalid non-empty URL")
    {
        QString url = "xyz";
        REQUIRE(!TdApi::Server::isValid(url));
        server.setUrl(url);
        REQUIRE(!server.isValid());

        // Name (derived) is empty
        REQUIRE(server.name().isEmpty());

        // Can explicitly set a name, though.
        server.setName("Example");
        REQUIRE(server.name() == "Example");
    }   // end invalid non-empty URL section

    // A valid URL with an invalid scheme
    SECTION("valid URL, invalid scheme")
    {
        QString url = "ftp://example.com";
        REQUIRE(!TdApi::Server::isValid(url));
        server.setUrl(url);
        REQUIRE(!server.isValid());

        // Name (derived) is empty
        REQUIRE(server.name().isEmpty());

        // Can explicitly set a name, though.
        server.setName("Example");
        REQUIRE(server.name() == "Example");
    }   // end valid URL, invalid scheme section

    SECTION("valid mastodon Server URL")
    {
        // A valid URL for a server
        QString url = "http://example.com:8080";
        REQUIRE(TdApi::Server::isValid(url));
        server.setUrl(url);
        REQUIRE(server.isValid());

        // For the mastodon library, extract the host and port number
        REQUIRE(server.mastodonAddress() == "example.com:8080");

        // Name has not been set, so the mastodon address is used.
        REQUIRE(server.name().toStdString() == server.mastodonAddress());

        // A name can be set explicitly
        server.setName("Example");
        REQUIRE(server.name() == "Example");
    }   // end valid Mastodon Server url section

}   // end Server basic test

// Verify Server object serialisation
TEST_CASE("TootDesk::Api::Server serialisation", "[unit][tdapi]")
{
    auto 
        s1 = std::make_shared<TdApi::Server>("s1", "http://example1.com")
        , s2 = std::make_shared<TdApi::Server>("s2", "https://example2.com")
        , s3 = std::make_shared<TdApi::Server>("s3", "http://example3.com")
        ;

    TdApi::ServerByNameMap servers {
        std::make_pair(s1->name(), s1)
        , std::make_pair(s2->name(), s2)
        , std::make_pair(s3->name(), s3)
    };

    QMap<QString, QVariant> serialisableServers =
        convertForSerialisation(servers);

    // We have 3 serialisable objects
    REQUIRE(serialisableServers.size() == 3);

    // Check server number 2
    REQUIRE(serialisableServers["s2"].isValid());
    REQUIRE(serialisableServers["s2"].toMap()["name"].toString() == "s2");
    REQUIRE(serialisableServers["s2"].toMap()["url"].toUrl() ==
        QUrl("https://example2.com"));

    // Now, deserialise the servers into a new list.
    TdApi::ServerByNameMap recoveredServers =
        TdApi::convertFromSerialisation(serialisableServers);
    
    // We should have three recovered server objects (same as before).
    REQUIRE(recoveredServers.size() == 3);

    // Check the values of server 3.
    REQUIRE(recoveredServers["s3"]->isValid());
    REQUIRE(recoveredServers["s3"]->name() == "s3");
    REQUIRE(recoveredServers["s3"]->url() == QUrl("http://example3.com"));

}   // end Server serialisation test
