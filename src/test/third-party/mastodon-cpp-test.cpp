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
 * \file mastodon-cpp-test.cpp
 * Simple test to verify the mastodon-cpp library
 * 
 * \author Igor Siemienowicz
 * 
 * \copyright GPL 3.0
 */

#include <catch/catch.hpp>
#include <mastodon-cpp/mastodon-cpp.hpp>
#include <mastodon-cpp/easy/all.hpp>

// #include <iostream>

using Mastodon::Easy;

// This is a really basic test to verify that Mastodon-CPP can go online and
// get some statuses and other info.
TEST_CASE("mastodon-cpp basic", "[third-party][online][current]")
{

///    std::string serverAddress("mastodon.social");
    std::string serverAddress("freeradical.zone");
    std::string accessToken(
        "ed085b15ba69ea433d869101a5074f2bf10650f3f21b71f0b91d96e71ba3ce5a");
    Easy masto(serverAddress, accessToken);
    std::string response;

    std::string appName = "TootDesk/0.0.0.0";
    masto.set_useragent(appName);

    auto result = masto.get(
        Mastodon::API::v1::accounts_verify_credentials
        , response);

    REQUIRE(result == 0);
    std::cout
        << std::endl << "[DEBUG] verify: " << response
        << std::endl;

    // Check instance info
///    REQUIRE_NOTHROW(masto.get(Mastodon::API::v1::instance, response));
///    Easy::Instance ins(response);

    // std::cout
    //     << std::endl << "[DEBUG] title:       " << ins.title()
    //     << std::endl << "[DEBUG] description: " << ins.description()
    //     << std::endl;

    // Retrieve the timeline
///    REQUIRE_NOTHROW(
///        masto.get(Mastodon::API::v1::timelines_public, response));
///    REQUIRE(response.empty() == false);

///    masto.get(Mastodon::API::v1::accounts_verify_credentials, response);
///    std::cout
///        << std::endl << "[DEBUG] verify credentials: " << response
///        << std::endl;

    
///    std::string clientId, clientSecret, url;
///
///    auto ret = masto.register_app1(
///        appName
///        , "urn:ietf:wg:oauth:2.0:oob"
///        , "read write follow push"
///        , ""
///        , clientId
///        , clientSecret
///        , url);
///
///    REQUIRE(ret == 0);
///
///    std::cout
///        << std::endl << "[DEBUG] client ID: " << clientId
///        << std::endl << "[DEBUG] client secret: " << clientSecret
///        << std::endl << "[DEBUG] visit " << url << " to get the code"
///        << std::endl;

///    std::string
///        clientId("891f94248a5f6d97c51e700f8058f0f210f3d8205ccc39b32cabe903782b3afc")
///        , clientSecret("ee117c06201df8f6ba29222b7bdccad43539cccaf7d400a521e141e5bf6c0af1")
///        , code("0db151119541c7c306103a008e034660387f0128c91e220841c8f8bdfdc193d6")
///        , accessToken;
///
///    auto ret = masto.register_app2(
///        clientId
///        , clientSecret
///        , "urn:ietf:wg:oauth:2.0:oob"
///        , code
///        , accessToken);
///
///    REQUIRE(ret == 0);
///
///    std::cout
///        << std::endl << "[DEBUG] access token: " << accessToken
///        << std::endl;

}   // end mastodon-cpp test
