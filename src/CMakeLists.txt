# TootDesk - Desktop Mastodon Client
# Copyright (C) 2018 Igor Siemienowicz
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

# This is the top-level cmake file

cmake_minimum_required(VERSION 3.10)
project(tootdesk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCURL_STATICLIB -pthread")
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCURL_STATICLIB")

# Qt Stuff
# Run moc and RCC as needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Tell Cmake where to find Qt stuff
set(CMAKE_PREFIX_PATH $ENV{QT_DIR}/lib/cmake/Qt5Widgets)
message("CMAKE_PREFIX_PATH set to '${CMAKE_PREFIX_PATH}'")

# Find QtWidgets
find_package(Qt5Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find cURL manually for windows
#
# TODO Figure out why FindCURL doesn't work
SET(CURL_LIB "curl")
if (WIN32)
    set(CURL_LIB "$ENV{CURL_LIBRARY}")
    set(CURLPP_LIB "$ENV{CURLPP_LIBRARY}")
endif()

include_directories(
    .
    third-party
    third-party/jsoncpp
    third-party/curlpp/include
    third-party/mastodon-cpp
    $ENV{CURL_INCLUDE_DIR}      # only needed for Windows
)

add_subdirectory(gui)
add_subdirectory(test)
add_subdirectory(third-party)
