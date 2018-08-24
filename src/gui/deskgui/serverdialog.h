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
 * \file serverdialog.h
 * Declares the `ServerDialog` class
 *
 * \author Igor Siemienowicz
 *
 * \copyright GPL 3.0
 */

#include <boost/optional.hpp>

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>

#ifndef _td_gui_deskgui_serverdialog_h_included
#define _td_gui_deskgui_serverdialog_h_included

namespace TootDesk { namespace Gui {

class ServerDialog : public QDialog
{

    // --- External Interface ---

    public:

    /**
     * \brief The data that this dialog is used to edit - the Name and URL
     * of the Server
     */
    using DialogData = std::tuple<QString, QString>;

    /**
     * \brief The result of executing the dialog - an optional structure of
     * edited data
     */
    using DialogResult = boost::optional<DialogData>;

    /**
     * \brief A callable object used to validate the dialog data - this
     * returns a `QString` with an error description if there are errors,
     * and `boost::none` if not
     */
    using errorCheckingFn =
        std::function<boost::optional<QString>(const DialogData&)>;

    /**
     * \brief Execute the dialog modally, returning the edited name and URL
     *
     * \param data The dialog data
     * 
     * \param errFn Error checking function to validate dialog data
     *
     * \param windowTitle The title to use for the Dialog
     *
     * \param parent The parent of the dialog
     *
     * \return Either the edited dialog data, or boost::none if the User
     * did not Accept the dialog
     */
    static DialogResult execute(
            DialogData data
            , errorCheckingFn errFn
            , QString windowTitle = tr("Server")
            , QWidget* parent = nullptr)
    {
        ServerDialog dlg(data, errFn, windowTitle, parent);
        if (dlg.exec() == QDialog::Accepted)
            return std::make_tuple(
                dlg.m_nameLineEdit->text(),
                dlg.m_urlLineEdit->text());
        else return boost::none;        
    }   // end execute method

    public slots:

    /**
     * \brief Close the dialog with an 'accept' result if the data is OK
     * 
     * This method calls the `checkForErrors` validation functor, and only
     * allows the dialog to accept if this does not return any errors.
     */
    virtual void accept(void) override
    {
        auto error =
            checkForErrors(
                DialogData(m_nameLineEdit->text(), m_urlLineEdit->text()));

        if (error) QMessageBox::critical(this, windowTitle(), *error);
        else QDialog::accept();
    }

    // --- Internal Declarations ---

    protected:

    /**
     * \brief Construct the dialog with its data (which may be blank)
     *
     * \param data The data of the dialog
     * 
     * \param errFn Error checking function to validate dialog data
     *
     * \param windowTitle The string to use as the dialog title
     *
     * \param parent The parent widget of the dialog
     */
    ServerDialog(
            DialogData data
            , errorCheckingFn errFn
            , QString windowTitle = tr("Server")
            , QWidget* parent = nullptr) :
        QDialog(parent)
        , m_nameLineEdit(nullptr)
        , m_urlLineEdit(nullptr)
        , checkForErrors(errFn)
    {
        setWindowTitle(windowTitle);

        auto layout = new QFormLayout(this);
        setLayout(layout);

        // The data fields
        m_nameLineEdit = new QLineEdit(this);
        m_nameLineEdit->setMinimumWidth(1000);
        m_nameLineEdit->setText(std::get<0>(data));
        m_urlLineEdit = new QLineEdit(this);
        m_urlLineEdit->setMinimumWidth(1000);
        m_urlLineEdit->setText(std::get<1>(data));

        layout->addRow(tr("Name:"), m_nameLineEdit);
        layout->addRow(tr("URL:"), m_urlLineEdit);

        // Buttons at the bottom of the dialog
        auto buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok
            | QDialogButtonBox::Cancel
            , this);

        connect(
            buttonBox,
            &QDialogButtonBox::accepted,
            this,
            &QDialog::accept);

        connect(
            buttonBox,
            &QDialogButtonBox::rejected,
            this,
            &QDialog::reject);

        layout->addRow(buttonBox);

    }   // end constructor

    /**
     * \brief A line edit control for the Server Name
     */
    QLineEdit* m_nameLineEdit;

    /**
     * \brief A line edit control for the URL of the Server
     */
    QLineEdit* m_urlLineEdit;

    /**
     * \brief Function object to test for errors in the dialog data
     */
    errorCheckingFn checkForErrors;

};  // end QDialog

}}  // end TootDesk::Gui

#endif
