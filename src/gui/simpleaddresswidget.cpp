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
 * Implements the `SimpleAddressWidget` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QHBoxLayout>
#include <QPushButton>
#include "simpleaddresswidget.h"

SimpleAddressWidget::SimpleAddressWidget(
        const QString& buttonText,
        Fn fn,
        QWidget* parent) :
    QWidget(parent),
    m_addressLineEdit(nullptr),
    m_fn(fn)
{

    setLayout(new QHBoxLayout());
    m_addressLineEdit = new QLineEdit(this);
    layout()->addWidget(m_addressLineEdit);

    auto goButton = new QPushButton(this);
    goButton->setText(buttonText);
    layout()->addWidget(goButton);

    connect(
        goButton,
        SIGNAL(released()),
        this,
        SLOT(handleGoButtonClicked()));

}   // end constructor

void SimpleAddressWidget::handleGoButtonClicked(void)
{
    m_fn(m_addressLineEdit->text());
}   // end handleGoButtonClicked
