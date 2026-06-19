#include "createuserdialog.h"
#include "ui_createuserdialog.h"
#include <QMessageBox>
#include <QRegularExpression>

CreateUserDialog::CreateUserDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateUserDialog) {
    ui->setupUi(this);
}

CreateUserDialog::~CreateUserDialog() {
    delete ui;
}

QString CreateUserDialog::getLogin() const {
    return ui->editLogin->text().trimmed();
}

QString CreateUserDialog::getEmail() const {
    return ui->editEmail->text().trimmed();
}

QString CreateUserDialog::getPassword() const {
    return ui->editPassword->text();
}

bool CreateUserDialog::isAdminRole() const {
    return ui->checkAdmin->isChecked();
}

void CreateUserDialog::clearFields() {
    ui->editLogin->clear();
    ui->editEmail->clear();
    ui->editPassword->clear();
    ui->checkAdmin->setChecked(false);
}

void CreateUserDialog::showError(const QString &message) {
    QMessageBox::critical(this, "Ошибка", message);
}

void CreateUserDialog::showSuccess(const QString &message) {
    QMessageBox::information(this, "Успех", message);
}

void CreateUserDialog::on_btnCreate_clicked() {
    accept();
}

void CreateUserDialog::on_btnCancel_clicked() {
    reject();
}