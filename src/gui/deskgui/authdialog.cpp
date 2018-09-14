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
 * \file authdialog.cpp
 * Implements the `AuthDialog` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QWebEngineView>
#include <QVBoxLayout>

#include "authdialog.h"

namespace TootDesk { namespace Gui {

AuthDialog::AuthDialog(QString url, QWidget* parent) :
    QDialog(parent)
{

    setFixedSize(640, 480);
    setLayout(new QVBoxLayout(this));
    auto webEngineView = new QWebEngineView(this);
    layout()->addWidget(webEngineView);

    connect(
        webEngineView
        , &QWebEngineView::loadFinished
        , [this, webEngineView](bool)
        {
            webEngineView->page()->toHtml([this](const QString& html)
            {
                qDebug() << html;
            });
        });

    webEngineView->load(QUrl(url));

}   // end constructor

}}  // end TootDesk::Gui namespace
