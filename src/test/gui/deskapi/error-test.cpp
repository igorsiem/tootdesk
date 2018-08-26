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
 * \file error-test.cpp
 * Tests for the `TootDesk::Api::Error` class
 * 
 * \author Igor Siemienowicz
 * 
 * \copyright GPL 3.0
 */

#include <catch/catch.hpp>
#include <gui/deskapi/deskapi.h>

namespace TdApi = TootDesk::Api;

// Test the basic operation of the Error class and the TD_RAISE_API_ERROR
// macro
TEST_CASE("TootDesk::Api::Error", "[unit][tdapi]")
{

    try
    {
        TD_RAISE_API_ERROR("test");
        FAIL("Error exception should have been thrown before this");
    }
    catch (const TdApi::Error& error)
    {
        REQUIRE(error.message() == "test");
    }
    catch (...)
    {
        FAIL("Error exception should have been thrown, but some other "
            "exception was thrown instead");
    }

}   // end error test

// Simple test of the MastodonError exception class - checking that the macro
// throws as we expect, and that the human-readable message is generated
// correctly.
TEST_CASE("TootDesk::Api::MastodonError", "[unit][tdapi]")
{
    try
    {
        TD_RAISE_API_MASTODON_ERROR(11);
        FAIL("Mastodon Error exception should have been thrown before this");
    }
    catch (const TdApi::MastodonError& error)
    {
        REQUIRE(error.message() == QObject::tr("invalid call"));
    }
    catch (...)
    {
        FAIL("Mastodon Error exception should have been thrown, but some "
            "other exception was thrown instead");

    }
}   // end MastodonError test
