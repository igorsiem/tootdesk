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
 * Declares the `LambdaRunnable` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <functional>
#include <QRunnable>

#ifndef _td_gui_deskapi_lambdarunnable_h_included
#define _td_gui_deskapi_lambdarunnable_h_included

namespace TootDesk { namespace Api {

/**
 * \brief An implementation of QRunnable that runs an ordinary C++ lambda
 * function
 * 
 * This class is based around passing a callable object (i.e. anything on
 * which `operator()` is defined) to the constructor. This 'run function' is
 * called by the `QRunnable::run` method when the object is passed to the
 * `QThreadPool`.
 * 
 * All of the traditional caveats associated with using callable objects,
 * functions and lambdas apply here. The "run function" cannot be changed
 * after construction.
 */
class LambdaRunnable : public QRunnable
{

    // --- External Interface ---

    public:

    /**
     * \brief The callable type that is run by the runnable object
     */
    using RunFn = std::function<void(void)>;

    /**
     * \brief Constructor - optionally initialises the run function
     * 
     * Note that the `QRunnable` `autoDelete` property is set by default.
     * 
     * \brief runFn The callable object to run when the task is executabed.
     */
    explicit LambdaRunnable(RunFn runFn) : QRunnable(), m_runFn(runFn) {}

    /**
     * \brief Call the run function
     */
    virtual void run(void) override { m_runFn(); }

    protected:

    /**
     * \brief The callable object that is run by the thread pool
     */
    RunFn m_runFn;

};  // end LambdaRunnable class

}}  // end TootDesk::Api namespace

#endif
