// Copyright (c) 2017-2018 The Triangles Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "editaddressdialog.h"
#include "ui_editaddressdialog.h"
#include "addresstablemodel.h"
#include "guiutil.h"
#include "thememanager.h"

#include <QDataWidgetMapper>
#include <QMessageBox>

extern ThemeManager *themeManager;

EditAddressDialog::EditAddressDialog(Mode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAddressDialog), mapper(0), mode(mode), model(0)
{
    ui->setupUi(this);
    setStyleSheet(themeManager->getCurrent()->getQFrameGeneralStyle());
    ui->labelEdit->setStyleSheet(themeManager->getCurrent()->getQLineEdit());
    ui->addressEdit->setStyleSheet(themeManager->getCurrent()->getQLineEdit());

    GUIUtil::setupAddressWidget(ui->addressEdit, this);

    switch(mode)
    {
    case NewReceivingAddress:
        setWindowTitle(tr("New receiving address"));
        ui->addressEdit->setEnabled(false);
        ui->addressEdit->setVisible(false);
        ui->addressEditLabel->setVisible(false);
        ui->stealthCB->setEnabled(true);
        ui->stealthCB->setVisible(true);
        break;
    case NewSendingAddress:
        setWindowTitle(tr("New sending address"));

		ui->stealthCB->setVisible(false);
        break;
    case EditReceivingAddress:
        setWindowTitle(tr("Edit receiving address"));
        ui->addressEdit->setEnabled(false);
        ui->addressEdit->setVisible(true);
        ui->stealthCB->setEnabled(false);
        ui->stealthCB->setVisible(true);
        break;
    case EditSendingAddress:
        setWindowTitle(tr("Edit sending address"));
        ui->stealthCB->setVisible(false);
        break;
    }

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

EditAddressDialog::~EditAddressDialog()
{
    delete ui;
}

void EditAddressDialog::setModel(AddressTableModel *model)
{
    this->model = model;
	if(!model)
	    return;
    mapper->setModel(model);
    mapper->addMapping(ui->labelEdit, AddressTableModel::Label);
    mapper->addMapping(ui->addressEdit, AddressTableModel::Address);
	mapper->addMapping(ui->stealthCB, AddressTableModel::Type);
	
	// disable SA related widgets for now
    if(model->beforeSaSwitch()) {
    	ui->stealthCB->setEnabled(false);
    	ui->stealthCB->setText("Stealth Address (disabled)");
    }
    else {
    	ui->stealthCB->setText("Stealth Address");
    }
}

void EditAddressDialog::loadRow(int row)
{
    mapper->setCurrentIndex(row);
}

bool EditAddressDialog::saveCurrentRow()
{
    if(!model)
        return false;
    switch(mode)
    {
    case NewReceivingAddress:
    case NewSendingAddress:
		{
	    int typeInd  = ui->stealthCB->isChecked() ? AddressTableModel::AT_Stealth : AddressTableModel::AT_Normal;
        address = model->addRow(
                mode == NewSendingAddress ? AddressTableModel::Send : AddressTableModel::Receive,
                ui->labelEdit->text(),
                ui->addressEdit->text(),
				typeInd);
		}
        break;
    case EditReceivingAddress:
    case EditSendingAddress:
        if(mapper->submit())
        {
            address = ui->addressEdit->text();
        }
        break;
    }
    return !address.isEmpty();
}

void EditAddressDialog::accept()
{
    if(!model)
        return;
    if(!saveCurrentRow())
    {
        switch(model->getEditStatus())
        {
		case AddressTableModel::OK:
            // Failed with unknown reason. Just reject.
            break;
        case AddressTableModel::NO_CHANGES:
            // No changes were made during edit operation. Just reject.
            break;
        case AddressTableModel::INVALID_ADDRESS:
            QMessageBox::warning(this, windowTitle(),
                tr("The entered address \"%1\" is not a valid Triangles address.").arg(ui->addressEdit->text()),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case AddressTableModel::DUPLICATE_ADDRESS:
            QMessageBox::warning(this, windowTitle(),
                tr("The entered address \"%1\" is already in the address book.").arg(ui->addressEdit->text()),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case AddressTableModel::WALLET_UNLOCK_FAILURE:
            QMessageBox::critical(this, windowTitle(),
                tr("Could not unlock wallet."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        case AddressTableModel::KEY_GENERATION_FAILURE:
            QMessageBox::critical(this, windowTitle(),
                tr("New key generation failed."),
                QMessageBox::Ok, QMessageBox::Ok);
            break;
        }
        return;
    }
    QDialog::accept();
}

QString EditAddressDialog::getAddress() const
{
    return address;
}

void EditAddressDialog::setAddress(const QString &address)
{
    this->address = address;
    ui->addressEdit->setText(address);
}
