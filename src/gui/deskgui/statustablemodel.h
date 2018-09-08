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
 * \file statustablemodel.h
 * Declares the `TootDesk::Gui::StatusTableModel` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <QAbstractTableModel>
#include "../deskapi/status.h"

#ifndef _td_gui_deskgui_statustablemodel_h_included
#define _td_gui_deskgui_statustablemodel_h_included

namespace TootDesk { namespace Gui {

/**
 * \brief A Qt table data model for interacting with collections of Status
 * data
 */
class StatusTableModel : public QAbstractTableModel
{

    // --- External Interface ---

    public:
    
    /**
     * \brief Constructor, initialising from an external collection of
     * Status items
     * 
     * Note that the collection is a vector of shared pointers to Status
     * objects, so only the pointers are copied, not the objects themselves.
     * 
     * \param statuses The collection of status objects
     * 
     * \param parent The parent object (if any)
     */
    explicit StatusTableModel(
        Api::StatusVector statuses = Api::StatusVector(),
        QObject* parent = nullptr);

    virtual ~StatusTableModel(void) = default;

    /**
     * \brief Retrieve the number of data rows (number of status items)
     *
     * \parent The index of the parent - should normally be invalid, because
     * this is a table with no hierarchy
     * 
     * \return The number of status items, unless `parent` is valid, in which
     * case `0` is returned, because this is a table with no hierarchy
     */
    virtual int rowCount(
        const QModelIndex& parent = QModelIndex()) const override;

    /**
     * \brief Retrieve the number of columns in the table
     * 
     * This is equal to the number of attributes in the Status class that we
     * want to expose in this model.
     * 
     * \return The number of Status column
     */
    virtual int columnCount(
            const QModelIndex& parent = QModelIndex()) const override
        { return toInt(LastStatusColumnType)+1; }

    /**
     * \brief Retrieve an item of status data
     * 
     * This method retrieves a data item for the given row (Status) and
     * column (Status attribute).
     * 
     * \param index The model index of the data to retrieve; includes the
     * row and column identifying the Status and attribute
     * 
     * \param role The role of the data item; in this implementation, we
     * only support the Display role
     */
    virtual QVariant data(
        const QModelIndex& index,
        int role = Qt::DisplayRole) const override;

    /**
     * \brief Retrieve the data (name) for the given 'section' (data column)
     * 
     * This implementation returns a human-readable column name for the
     * given 'section' (treated as a column index).
     * 
     * \param section The column index
     * 
     * \param orientation The orientation of the data section; we only
     * retrieve information for columns (Qt::Horizontal)
     * 
     * \param role The display role; we only retrieve information for 
     * the Display Role in this read-only implementation
     */
    virtual QVariant headerData(
        int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole) const override;

    /**
     * \brief Set the status items in the data data
     * 
     * This method is thread-safe. It calls the `beginResetModel` and
     * `endResetModel` methods to ensure that components relying on this
     * model are properly notified.
     * 
     * \param status The status items to set; the shared pointers are copied
     */
    void setData(Api::StatusVector statuses);

    // --- Internal Declarations ---

    protected:

    /**
     * \brief Enumerate the data columns in a Status row
     * 
     * Note that when this is updated, the `LastStatusColumnType` and
     * `toString` members should be checked as well.
     */
    enum class StatusColumnType
    {
        AccountUserName = 0
        , AccountDisplayName = 1
        , CreatedAt = 2
        , Content = 3
        , Id = 4
    };  // end StatusColumnType

    /**
     * \brief Specify the last column in the column type enumeration
     */
    static const StatusColumnType LastStatusColumnType =
        StatusColumnType::Id;

    /**
     * \brief Translate a column type enumeration to an ordinary integer,
     * so that we can use it as an index
     * 
     * \param sct The Status Column Type to translate
     * 
     * \return The integer version of the `sct` enumerator
     */
    static constexpr int toInt(StatusColumnType sct)
        { return static_cast<int>(sct); }

    /**
     * \brief Convert an integer to a Status Column Type enumerator
     * 
     * \param i The integer to convert
     * 
     * \return The Status Columnn Type equivalent to `i`
     */
    static StatusColumnType toStatusColumnType(int i)
        { return static_cast<StatusColumnType>(i); }

    static QVariant toData(const Api::Status& status, StatusColumnType sct);

    /**
     * \brief Convert the given Status Column Type to a human-readable string
     * 
     * \param sct The StatusColumnType enumerator
     * 
     * \return A human-readable column name
     */
    static QString toString(StatusColumnType sct);

    /**
     * \brief The collection of status objects
     * 
     * This collection is protected by `m_statusesMtx`.
     */
    Api::StatusVector m_statuses;

    /**
     * \brief Thread-safety protection for `m_statuses`
     */
    mutable Api::Mutex m_statusesMtx;

};  // end StatusTableModel class

}}  // end TootDesk::Gui

#endif
