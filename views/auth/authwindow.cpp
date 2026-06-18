#include "authwindow.h"
#include "ui_authwindow.h"
#include <QMessageBox>

AuthWindow::AuthWindow(QWidget *parent) : QWidget(parent), ui(new Ui::AuthWindow) {
    ui->setupUi(this);
}

AuthWindow::~AuthWindow() { delete ui; }

// <-- РЕАЛИЗУЕМ НОВЫЕ МЕТОДЫ
void AuthWindow::clearFields() {
    ui->editLogin->clear();
    ui->editPassword->clear();
}

void AuthWindow::showMessage(const QString &title, const QString &message) {
    QMessageBox::information(this, title, message);
}

void AuthWindow::on_btnLogin_clicked() {
    QString login = ui->editLogin->text().trimmed();
    QString password = ui->editPassword->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Заполните все поля!");
        return;
    }
    emit loginSubmitted(login, password);
}

void AuthWindow::on_btnOpenReg_clicked() {
    emit openRegisterRequested();
}

void AuthWindow::on_btnClose_clicked() {
    this->close();
}