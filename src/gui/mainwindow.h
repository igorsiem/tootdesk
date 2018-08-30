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
#include <QMessageBox>
#include <QSettings>
///#include <QPushButton>
///#include <mastodon-cpp/mastodon-cpp.hpp>
///#include <mastodon-cpp/easy/all.hpp>

#include "deskapi/deskapi.h"

///#include "timelinewidget.h"

#ifndef _td_gui_mainwindow_h_included
#define _td_gui_mainwindow_h_included

namespace Ui {
class MainWindow;
}

namespace Td = TootDesk;
namespace TdApi = Td::Api;

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
     * \param settings Persistent settings object for this application
     *
     * \param parent The parent widget
     */
    explicit MainWindow(QSettings& settings, QWidget *parent = 0);

    /**
     * \brief Destructor - destroys the internal UI structures
     */
    virtual ~MainWindow();

    // --- Internal Declarations ---

    protected:

    // -- Event Handler Overrides --

    /**
     * \brief Write window state and geometry to persistent storage
     * 
     * \param event The event object (passed to base-class implementation) 
     */
    virtual void closeEvent(QCloseEvent *event) override;

    private:

    // -- Helper Functions --

    // - Layout Stuff -

    /**
     * \brief Top-level user interface setup
     */
    void setupUi(void);

    /**
     * \brief Set up the main widget of the window
     */
    void setupCentralWidget(void);

    /**
     * \brief Set up the widget for the 'left half' of the main splitter
     *
     * \param parent The widget to use for for the parent (owns the new
     * widget)
     *
     * \return The widget for the left split
     */
    QWidget* setupLeftSplit(QWidget* parent);

    /**
     * \brief Create a widget containing a table view of the Servers
     * collection
     *
     * \param parent Widget to use for the parent (owns the new widget) 
     *
     * \return The new widget containing Servers table
     */
    QWidget* createServersTableWidget(QWidget* parent);

///    QFrame* createTimelineFrame(const QString& title, QWidget* parent);

///    static QFrame* createColumnFrame(QWidget* parent);

    // - Event Handling -
  
    signals:

///    void statusDecoded(ConstStatusPtr status);

    protected slots:

    void serverErrorOccurred(QString serverName, QString errorMessage)
    {
        QMessageBox::critical(
            this,
            tr("Server: ") + serverName,
            tr("Error: ") + errorMessage);
    }

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

    /**
     * \brief Persistent settings object for recording various app settings
     * such as window geometry
     */
    QSettings& m_settings;

    /**
     * \brief The collection of Server instances
     */
    TdApi::ServerByNameMap m_servers;

///    TimelineWidget* m_timelineWidget;

};  // end MainWindow class

#endif
