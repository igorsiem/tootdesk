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
 * \file coretypes.h
 * Core type declarations used by the TootDesk API
 * 
 * These declarations are for basic structures like memory management and
 * multi-threading primitives that have more than one implementation that we
 * could potentially use in TootDesk. These choices are made in this header
 * so that they can be grouped and documented in a single place.
 * 
 * For example, both Qt and the C++ standard library have the concept of a
 * shared pointer. We use this header to choose between them so that we
 * could potentially change this later with minimal effort.
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QSharedPointer>

#ifndef _td_gui_deskapi_coretypes_h_included
#define _td_gui_deskapi_coretypes_h_included

namespace TootDesk { namespace Api {

template <typename T>
using SharedPtr = QSharedPointer<T>;

template <typename T, typename... Args>
auto makeShared(Args... args) { return SharedPtr<T>::create(args...); }

}}  // end TootDesk::Api namespace

#endif
