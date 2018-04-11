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
 * Simple test to verify the jsoncpp library
 * 
 * \author Igor Siemienowicz
 * 
 * \copyright GPL 3.0
 */

#include <sstream>
#include <catch/catch.hpp>
#include <json/json.h>

TEST_CASE("jsoncpp","[third-party]")
{

    std::stringstream input;
    input << 
    "{" << std::endl <<
    "    \"encoding\" : \"UTF-8\"," << std::endl <<
    "    \"plug-ins\" : [" << std::endl <<
    "        \"python\"," << std::endl <<
    "        \"c++\"," << std::endl <<
    "        \"ruby\"" << std::endl <<
    "        ]," << std::endl <<
    "    \"indent\" : { \"length\" : 3, \"use_space\": true }" <<
        std::endl <<
    "}";
    
    // Parse into a root node.
    Json::Value root;
    input >> root;
    
    // Check some values.
    REQUIRE(root.get("encoding", "").asString() == "UTF-8");
    REQUIRE(root["plug-ins"].size() == 3);
    REQUIRE(root["plug-ins"][1].asString() == "c++");
    REQUIRE(root["indent"]["length"].asInt() == 3);
    REQUIRE(root["indent"]["use_space"].asBool() == true);
    
}   // end jsoncpp test
