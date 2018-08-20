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
 * \file curl-test.cpp
 * Simple test to verify the integration of the curl library
 * 
 * \author Igor Siemienowicz
 * 
 * \copyright GPL 3.0
 */

#include <catch/catch.hpp>
#include <curl/curl.h>

TEST_CASE("curl","[third-party]")
{
    REQUIRE(curl_global_init(CURL_GLOBAL_ALL) == 0);
    CURL* curl = curl_easy_init();
    REQUIRE(curl);

    // std::cout << "[DEBUG] cURL version: " << curl_version() << std::endl;

    curl_easy_cleanup(curl);
    curl_global_cleanup();
}   // end curl test
