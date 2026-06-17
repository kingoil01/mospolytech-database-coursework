#include "customerinfodialog.h"
#include "ui_customerinfodialog.h"
#include <QMessageBox>

CustomerInfoDialog::CustomerInfoDialog(QWidget *parent) : QDialog(parent), ui(new Ui::CustomerInfoDialog) {
    ui->setupUi(this);
}

CustomerInfoDialog::~CustomerInfoDialog() { delete ui; }

QString CustomerInfoDialog::getOrgName() const { return ui->editOrgName->text().trimmed(); }
QString CustomerInfoDialog::getAddress() const { return ui->editAddress->text().trimmed(); }
QString CustomerInfoDialog::getPhone() const { return ui->editPhone->text().trimmed(); }
QString CustomerInfoDialog::getContactPerson() const { return ui->editContactPerson->text().trimmed(); }

void CustomerInfoDialog::setInitialData(const QString &org, const QString &addr, const QString &phone, const QString &cp) {
    ui->editOrgName->setText(org);
    ui->editAddress->setText(addr);
    ui->editPhone->setText(phone);
    ui->editContactPerson->setText(cp);
}

void CustomerInfoDialog::on_btnSave_clicked() {
    if (getOrgName().isEmpty() || getAddress().isEmpty() || getPhone().isEmpty() || getContactPerson().isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Заполните все обязательные поля!");
        return;
    }
    accept(); // Закрывает окно со статусом QDialog::Accepted
}

void CustomerInfoDialog::on_btnCancel_clicked() {
    reject(); // Закрывает окно со статусом QDialog::Rejected
}