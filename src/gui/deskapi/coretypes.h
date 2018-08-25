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

#include <QMutex>
#include <QReadLocker>
#include <QReadWriteLock>
#include <QSharedPointer>
#include <QWriteLocker>

#ifndef _td_gui_deskapi_coretypes_h_included
#define _td_gui_deskapi_coretypes_h_included

namespace TootDesk { namespace Api {

/**
 * \brief The shared pointer type used by the TootDesk API
 * 
 * \tparam T The type of the shared object
 */
template <typename T>
using SharedPtr = QSharedPointer<T>;

/**
 * \brief Create a new object as a shared pointer
 * 
 * \tparam T The type of the shared object
 * 
 * \tparam Args The argument types of the constructor
 * 
 * \param args The constructor arguments for the new object
 */
template <typename T, typename... Args>
auto makeShared(Args... args) { return SharedPtr<T>::create(args...); }

/**
 * \brief The mutex type used by the TD API
 * 
 * In this implementation, we use Qt's `QReadWriteLock`, which is roughly
 * equivalent to a `std::shared_mutex`.
 */
using Mutex = QReadWriteLock;

/**
 * \brief Mechanism for a non-exclusive lock on a mutex
 */
using ReadGuard = QReadLocker;

/**
 * \brief Mechanism for an exclusive lock on a mutex
 */
using WriteGuard = QWriteLocker;

}}  // end TootDesk::Api namespace

#endif
