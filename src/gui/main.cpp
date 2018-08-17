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
 * \file main.cpp
 * Entry point for the GUI executable
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QApplication>
#include <QSettings>

#include "mainwindow.h"
///#include "mastodontypes.h"

/**
 * @brief Entry point to the GUI executable
 *
 * @param argc The number of command-line arguments
 *
 * @param argv The vector of command-line arguments
 *
 * @return Non-zero on error
 */
int main(int argc, char *argv[])
{

///    // Declare our Qt metatypes
///    qRegisterMetaType<StatusPtr>("StatusPtr");
///    qRegisterMetaType<ConstStatusPtr>("ConstStatusPtr");

    QApplication a(argc, argv);
    QSettings settings("Igor Siemienowicz", "TootDesk");
    MainWindow w(settings);
///    w.showMaximized();
    w.show();

    return a.exec();
}   // end main function

