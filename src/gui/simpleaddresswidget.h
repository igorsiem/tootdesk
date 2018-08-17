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
 * \file timelinewidget.h
 * Declares the `SimpleAddressWidget` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QLineEdit>
#include <QWidget>

#ifndef _td_gui_simpleaddress_h_included
#define _td_gui_simpleaddress_h_included

/**
 * \brief A widget with some basic controls for entering a URL and clicking
 * a button
 * 
 * The method invoked by the button is set as a standard C++ callable object.
 */
class SimpleAddressWidget : public QWidget
{

    Q_OBJECT

    // --- External Interface ---

    public:

    /**
     * \brief The callable type that is invoked by pushing the button, taking
     * the text of the editable address box as an argument
     */
    using Fn = std::function<void(const QString&)>;

    /**
     * \brief Constructor, initialising all internal properties
     * 
     * \param buttonText The text for the 'Go' button
     * 
     * \param fn The callable object that is called when the button is pushed
     * 
     * \param parent The parent widget
     */
    SimpleAddressWidget(const QString& buttonText, Fn fn, QWidget* parent);

    // --- Internal Declarations ---

    protected slots:

    /**
     * \brief Runs the callable object when the Go button is clicked
     */
    void handleGoButtonClicked(void);

    protected:

    /**
     * \brief The line edit control for the address
     */
    QLineEdit* m_addressLineEdit;

    /**
     * \brief Callable object that is run when the Go button is clicked
     */
    Fn m_fn;

};  // end SimpleAddressWidget class

#endif
