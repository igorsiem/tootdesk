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
 * \file error.h
 * Declares the `TootDesk::Api::MastodonError` exception class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QObject>
#include "error.h"

#ifndef _td_gui_mastodonerror_h_included
#define _td_gui_mastodonerror_h_included

namespace TootDesk { namespace Api {

class MastodonError : public Error
{

    public:

    /**
     * \brief Generate a human-readable message from a given Mastodon-Cpp
     * error code
     * 
     * \param c The error code
     * 
     * \return A human-readable message describing the error
     */
    static QString generateMessage(uint16_t c)
    {
        if (c == 0) return QObject::tr("no error");
        else if (c == 11) return QObject::tr("invalid call");
        else if (c == 12) return QObject::tr("not implemented");
        else if (c == 13)
            return QObject::tr("URL changed (HTTP 301 or 308)");
        else if (c == 14) return QObject::tr("cancelled by user");
        else if (c == 15)
            return QObject::tr("network error (curlpp exception)");
        else if ((c >= 100) && (c <= 999))
            return QObject::tr("HTTP status ") + QString::number(c);
        else if (c == 65535) return QObject::tr("unknown error");
        else return QObject::tr("Mastodon unrecognised error code");
    }   // end generateMessage

    /**
     * \brief Constructor, taking the Mastodon-Cpp error code
     * 
     * \param c The error code
     */
    explicit MastodonError(uint16_t c) : Error(generateMessage(c)) {}

    /**
     * \brief Raise the exception
     */    
    virtual void raise(void) const override { throw *this; }

    /**
     * \brief Clone the exception object
     *
     * \return A pointer to a new `Error` exception object
     */
    virtual QException* clone(void) const override
        { return new MastodonError(*this); }

    /**
     * \brief Retrieve the Mastodon error code
     * 
     * \return The code; this may be tranlated into a human-readable message
     * using `generateMessage`
     */
    uint16_t code(void) const { return m_code; }

    protected:

    /**
     * \brief The Mastodon error code
     */
    uint16_t m_code;

};  // end MastodonError class

}}  // end TootDesk::Api namespace

/**
 * \brief Raise a Mastodon Error exception
 * 
 * \param c The error code of the exception
 */
#define TD_RAISE_API_MASTODON_ERROR( c ) do { \
    ::TootDesk::Api::MastodonError(c).raise(); \
} while (false)

#endif
