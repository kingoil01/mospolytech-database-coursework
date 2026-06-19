#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <QMessageBox>

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow) {
    ui->setupUi(this);
}

AdminWindow::~AdminWindow() {
    delete ui;
}

void AdminWindow::setAdminInfo(const User &user) {
    setWindowTitle(QString("Административная панель - %1").arg(user.login()));
}

void AdminWindow::showMessage(const QString &title, const QString &message, bool isError) {
    if (isError) {
        QMessageBox::critical(this, title, message);
    } else {
        QMessageBox::information(this, title, message);
    }
}

void AdminWindow::on_btnCreateUser_clicked() {
    emit createUserRequested();
}

void AdminWindow::on_btnDeleteUser_clicked() {
    emit deleteUserRequested();
}

void AdminWindow::on_btnLogout_clicked() {
    emit logoutRequested();
}