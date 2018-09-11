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
 * \file jsoncpp-test.cpp
 * Simple test to verify the integration of the curlcpp library
 * 
 * \author Igor Siemienowicz
 * 
 * \copyright GPL 3.0
 */

#include <catch/catch.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

// Simple cURLpp test
TEST_CASE("curlpp", "[third-party]")
{

    // Initialises cURL in constructor, and terminates in destructor.
    cURLpp::Cleanup curl_cleanup;

    cURLpp::Easy easy_handle;
    easy_handle.setOpt(cURLpp::Options::Url("http://example.com/"));  
    // REQUIRE_NOTHROW(easy_handle.perform());  

}   // end curlpp test
