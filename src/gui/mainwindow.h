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
 * Declares the `MainWindow` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

///#include <tuple>
///#include <QListWidget>
///#include <QListWidgetItem>
#include <QMainWindow>
///#include <QPushButton>
///#include <mastodon-cpp/mastodon-cpp.hpp>
///#include <mastodon-cpp/easy/all.hpp>
///#include "timelinewidget.h"

#ifndef _td_gui_mainwindow_h_included
#define _td_gui_mainwindow_h_included

namespace Ui {
class MainWindow;
}

/**
 * \brief Encapsulates the main GUI window
 */
class MainWindow : public QMainWindow
{

    Q_OBJECT

    // --- External Interface ---

    public:

    // -- Construction / Destruction --

    /**
     * \brief Constructor, setting up all attributes
     *
     * \param parent The parent widget
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * \brief Destructor - destroys the internal UI structures
     */
    virtual ~MainWindow();

    // --- Internal Declarations ---

    private:

    // -- Helper Functions --

    // - Layout Stuff -

    void setup(void);

///    QFrame* createTimelineFrame(const QString& title, QWidget* parent);

///    static QFrame* createColumnFrame(QWidget* parent);

    // - Event Handlers -

    signals:

///    void statusDecoded(ConstStatusPtr status);

    // - Mastodon Stuff -

    private:

///    using StatusProcessorFn =
///        std::function<void(ConstStatusPtr status)>;
///
///    void getPublicTimeline(
///        const std::string& address,
///        StatusProcessorFn processorFn);

    // -- Attributes --

    /**
     * \brief Internal UI structures (managed by Qt framework
     */
    Ui::MainWindow *ui;

///    TimelineWidget* m_timelineWidget;

};  // end MainWindow class

#endif
