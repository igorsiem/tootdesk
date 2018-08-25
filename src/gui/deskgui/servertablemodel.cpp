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
 * \file servertablemodel.h
 * Implements the `TootDesk::Gui::ServerTableModel` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include "servertablemodel.h"

namespace TootDesk { namespace Gui {

ServerTableModel::ServerTableModel(
        Api::ServerByNameMap& servers,
        QObject* parent) :
    QAbstractTableModel(parent)
    , m_servers(servers)
{
}   // end constructor

int ServerTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    else return static_cast<int>(m_servers.size());
}   // end rowCount

QVariant ServerTableModel::data(
        const QModelIndex& index,
        int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    // Count through the servers to get to the row number - this is a bit
    // inefficient, but the number of servers is 'small'.
    if (index.row() >= rowCount()) return QVariant();
    auto serverItr = m_servers.begin();
    for (int i = 0; i < index.row(); i++) serverItr++;

    switch (index.column())
    {
        case 0: return serverItr.value()->name();
        case 1:
            return QString::fromStdString(
                serverItr.value()->mastodonAddress());
        default: return QVariant();
    }
}   // end data method

QVariant ServerTableModel::headerData(
        int section,
        Qt::Orientation orientation,
        int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    switch (section)
    {
        case 0: return tr("Name");
        case 1: return tr("Address");
        default: return QVariant();
    }
}   // end headerData

}}  // end TootDesk::Gui
