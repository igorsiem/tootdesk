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
 * \file lambdarunnable.h
 * Declares some types related to Mastodon-cpp
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <memory>

#include <QMetaType>

#include <mastodon-cpp/mastodon-cpp.hpp>
#include <mastodon-cpp/easy/all.hpp>

#ifndef _td_gui_mastodontypes_h_included
#define _td_gui_mastodontypes_h_included

/**
 * \brief The mastodon status class
 */
using status_t = Mastodon::Easy::Easy::Status;

/**
 * \brief A shared pointer to a status object
 */
using StatusPtr = std::shared_ptr<status_t>;

/**
 * \brief A shared pointer to a const status object
 */
using ConstStatusPtr = std::shared_ptr<const status_t>;

// Set up Qt metatypes for a status pointer
Q_DECLARE_METATYPE(StatusPtr);
Q_DECLARE_METATYPE(ConstStatusPtr);

#endif
