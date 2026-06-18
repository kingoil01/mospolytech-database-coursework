#include "changedatadialog.h"
#include "ui_changedatadialog.h"
#include <QRegularExpression>

ChangeDataDialog::ChangeDataDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeDataDialog) {
    ui->setupUi(this);
}

ChangeDataDialog::~ChangeDataDialog() {
    delete ui;
}

QString ChangeDataDialog::getNewLogin() const {
    return ui->editNewLogin->text().trimmed();
}

QString ChangeDataDialog::getNewEmail() const {
    return ui->editNewEmail->text().trimmed();
}

QString ChangeDataDialog::getNewPassword() const {
    return ui->editNewPassword->text();
}

void ChangeDataDialog::setCurrentLogin(const QString &login) {
    // Если в UI нет labelCurrentLogin, используем placeholder или другой способ
    ui->editNewLogin->setPlaceholderText("Текущий: " + login);
}

void ChangeDataDialog::setCurrentEmail(const QString &email) {
    ui->editNewEmail->setPlaceholderText("Текущий: " + email);
}

bool ChangeDataDialog::isValidEmail(const QString &email) const {
    QRegularExpression emailRegExp("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return emailRegExp.match(email).hasMatch();
}

void ChangeDataDialog::on_btnSave_clicked() {
    accept();
}

void ChangeDataDialog::on_btnCancel_clicked() {
    reject();
}