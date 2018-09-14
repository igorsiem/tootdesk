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

# Cmake file for specifying how the GUI executable is built

# Sort out the UI files and their generated headers
file(GLOB UI_FILES "*.ui")
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# Specify composition of gui application
file(GLOB_RECURSE GUI_SRC *.cpp *.rc *.qrc)
add_executable(tootdesk-gui ${GUI_SRC} ${UI_HEADERS})

target_link_libraries(tootdesk-gui
    Qt5::Widgets
    Qt5::WebEngineWidgets
    mastodon-cpp
    curlpp
    ${CURL_LIB}
    jsoncpp
)
