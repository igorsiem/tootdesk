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
 * \file mainwindow.h
 * Declares the `mainwindow` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#ifndef _td_gui_mainwindow_h
#define _td_gui_mainwindow_h

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

/**
 * @brief Encapsulates the main GUI window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

    /**
     * @brief Constructor, setting up all attributes
     *
     * @param parent The parent widget
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief Destructor - destroys the internal UI structures
     */
    ~MainWindow();

    private:

    /**
     * @brief Internal UI structures (managed by Qt framework
     */
    Ui::MainWindow *ui;

};  // end MainWindow class

#endif
