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
 * Declares the `TootDesk::Gui::ServerTableModel` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QAbstractTableModel>

#include "../deskapi/server.h"

#ifndef _td_gui_deskgui_servertablemodel_h_included
#define _td_gui_deskgui_servertablemodel_h_included

namespace TootDesk { namespace Gui {

/**
 * \brief A Qt table data model for interacting with Server data
 */
class ServerTableModel : public QAbstractTableModel
{
    
    // --- Externla Interface ---

    public:

    explicit ServerTableModel(
        Api::ServerByNameMap& servers,
        QObject* parent = nullptr);

    virtual ~ServerTableModel(void) = default;

    /**
     * \brief Returns the number of data rows
     *
     * \parent The index of the parent - should normally be invalid, because
     * this is a table with no hierarchy
     *
     * \return The number of servers, unless `parent` is valid, in which
     * case `0` is returned, because this is a table with no hierarchy
     */
    virtual int rowCount(
        const QModelIndex& parent = QModelIndex()) const override;

    /**
     * \brief Returns the number of columns for the data
     *
     * \parent The index of the parent - should normally be invalid, because
     * this is a table with no hierarchy
     *
     * \return Always 2
     */
    virtual int columnCount(
            const QModelIndex& parent = QModelIndex()) const override
        { return 2; }

    /**
     * \brief Retrieve the data item for the given index in the table
     *
     * \param The index of data to retrieve
     *
     * \param The role of the data; this iteration supports only the Display
     * Role
     *
     * \return The data item at the indexed row and column
     */
    virtual QVariant data(
        const QModelIndex& index,
        int role = Qt::DisplayRole) const override;

    /**
     * \brief Return the column header data for the table (no row headers)
     *
     * This basically implements hard-coded column names, and nothing for
     * the rows.
     *
     * \param section The integer section for which to retrieve data; this
     * is treated as a column number
     *
     * \param orientation We only returns a header for horizontal (column)
     *
     * \param role The role for the header; we only return data for the
     * `Qt::DisplatRole`
     *
     * \return The header data, or `QVariant()` when the section, orientation
     * or role are outside our parameters
     */
    virtual QVariant headerData(
        int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole) const override;

    /**
     * \brief Signal that the external collection of Server instance objects
     * will change
     */
    void beginServerCollectionChange(void) { beginResetModel(); }
    
    /**
     * \brief Signal that the external collection of Server instance objects
     * has finished changing
     */
    void endServerCollectionChange(void) { endResetModel(); }


    // --- Internal Declarations ---

    protected:

    /**
     * \brief A reference to the external container of Servers
     */
    Api::ServerByNameMap& m_servers;

};  // end ServerTableModel class

}}  // end TootDesk::Gui namespace


#endif
