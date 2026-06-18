#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setUserInfo(const User &user) {
    // Используем существующие элементы UI или добавляем новые
    // Например, можно использовать statusBar или label
    // Если у вас нет labelUserInfo, можно использовать statusBar
    // statusBar()->showMessage(QString("Пользователь: %1 (%2)").arg(user.login()).arg(user.email()));
    // Или добавить label в UI
}

void MainWindow::setProfileButtonMode(bool hasProfile) {
    if (!hasProfile) {
        ui->btnEditProfile->setText("Создать профиль организации");
        ui->btnCreateOrder->setEnabled(false);
        ui->btnViewOrders->setEnabled(false);
        ui->btnCreateOrder->setToolTip("Доступно только после заполнения профиля организации");
        ui->btnViewOrders->setToolTip("Доступно только после заполнения профиля организации");
    } else {
        ui->btnEditProfile->setText("Изменить профиль организации");
        ui->btnCreateOrder->setEnabled(true);
        ui->btnViewOrders->setEnabled(true);
        ui->btnCreateOrder->setToolTip("");
        ui->btnViewOrders->setToolTip("");
    }
}

void MainWindow::setOrderButtonsEnabled(bool enabled) {
    ui->btnCreateOrder->setEnabled(enabled);
    ui->btnViewOrders->setEnabled(enabled);
}

void MainWindow::showMessage(const QString &title, const QString &message, bool isError) {
    if (isError) {
        QMessageBox::critical(this, title, message);
    } else {
        QMessageBox::information(this, title, message);
    }
}

void MainWindow::on_btnEditProfile_clicked() {
    emit editProfileRequested();
}

void MainWindow::on_btnCreateOrder_clicked() {
    emit createOrderRequested();
}

void MainWindow::on_btnViewOrders_clicked() {
    emit viewOrdersRequested();
}

void MainWindow::on_btnChangeCredentials_clicked() {
    emit changeCredentialsRequested();
}

void MainWindow::on_btnLogout_clicked() {
    emit logoutRequested();
}