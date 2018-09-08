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
 * \file statustablemodel.cpp
 * Implements the `TootDesk::Gui::StatusTableModel` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include "statustablemodel.h"

namespace TootDesk { namespace Gui {

StatusTableModel::StatusTableModel(
        Api::StatusVector statuses,
        QObject* parent) :
    QAbstractTableModel(parent)
    , m_statuses(std::move(statuses))
    , m_statusesMtx()
{
}

int StatusTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;

    Api::ReadGuard grd(&m_statusesMtx);
    return m_statuses.size();
}   // end rowCount

QVariant StatusTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    Api::ReadGuard grd(&m_statusesMtx);

    if ((index.row() < 0)
            || (index.row() >= m_statuses.size())
            || (index.column() < 0)
            || (index.column() >= columnCount()))
        return QVariant();

    auto status = m_statuses[index.row()];

    return toData(
        *status,
        toStatusColumnType(index.column()));
}   // end data method

QVariant StatusTableModel::headerData(
        int section,
        Qt::Orientation orientation,
        int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();

    return toString(toStatusColumnType(section));
}   // end headerData

void StatusTableModel::setData(Api::StatusVector statuses)
{
    beginResetModel();

    Api::WriteGuard grd(&m_statusesMtx);
    m_statuses = std::move(statuses);
    grd.unlock();

    endResetModel();
}   // end setData method

QVariant StatusTableModel::toData(
        const Api::Status& status,
        StatusColumnType sct)
{

    switch (sct)
    {

        case StatusColumnType::AccountUserName:
            return status.account()->userName();

        case StatusColumnType::AccountDisplayName:
            return status.account()->displayName();

        case StatusColumnType::CreatedAt:
            return status.createdAt();

        case StatusColumnType::Content:
            return status.content();

        case StatusColumnType::Id:
            return (qlonglong)status.id();

        default:
            return tr("*** unknown column");

    }   // switch on SCT

}   // end toData

QString StatusTableModel::toString(StatusColumnType sct)
{
    switch (sct)
    {
        case StatusColumnType::AccountUserName:
            return tr("Account User Name");

        case StatusColumnType::AccountDisplayName:
            return tr("Account Display Name");

        case StatusColumnType::CreatedAt: return tr("Created At");
        case StatusColumnType::Content: return tr("Content");
        case StatusColumnType::Id: return tr("ID");

        default: return tr("Unknown Column");
    }   // switch on SCT
}   // end toString method

}}  // end TootDesk::Gui namespace
