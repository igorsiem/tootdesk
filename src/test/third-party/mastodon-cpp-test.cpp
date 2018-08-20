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

using Mastodon::Easy;

// This is a really basic test to verify that Mastodon-CPP can go online and
// get some statuses
TEST_CASE("mastodon-cpp", "[third-party][online]")
{
   
    Easy masto("mastodon.social", "");
    std::string answer;
    REQUIRE_NOTHROW(masto.get(Mastodon::API::v1::timelines_public, answer));
    REQUIRE(answer.empty() == false);

}   // end mastodon-cpp test
