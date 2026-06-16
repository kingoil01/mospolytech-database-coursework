#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../auth/customerinfodialog.h"
#include <QMessageBox>

MainWindow::MainWindow(const User &user, AuthModel *model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_currentUser(user), m_model(model) {
    ui->setupUi(this);
    updateInterfaceByRole();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::updateInterfaceByRole() {
    int customerId = 0;
    // Проверяем в БД наличие профиля организации
    bool hasProfile = m_model->checkCustomerProfileExists(m_currentUser.idUser(), customerId);

    if (!hasProfile) {
        // Профиля нет -> блокируем функции заказов
        ui->btnCreateOrder->setEnabled(false);
        ui->btnViewOrders->setEnabled(false);
        ui->btnCreateOrder->setToolTip("Доступно только после заполнения профиля организации");
        ui->btnViewOrders->setToolTip("Доступно только после заполнения профиля организации");

        ui->btnEditProfile->setText("Создать профиль организации");
    } else {
        // Профиль есть -> всё открыто
        ui->btnCreateOrder->setEnabled(true);
        ui->btnViewOrders->setEnabled(true);
        ui->btnCreateOrder->setToolTip("");
        ui->btnViewOrders->setToolTip("");

        ui->btnEditProfile->setText("Изменить профиль организации");
    }
}

void MainWindow::on_btnEditProfile_clicked() {
    int customerId = 0;
    if (!m_model->checkCustomerProfileExists(m_currentUser.idUser(), customerId)) {
        // Открываем диалог заполнения данных
        CustomerInfoDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            QString org = dialog.getOrgName();
            QString addr = dialog.getAddress();
            QString phone = dialog.getPhone();
            QString cp = dialog.getContactPerson();

            if (m_model->createCustomerProfile(m_currentUser.idUser(), org, addr, phone, cp)) {
                QMessageBox::information(this, "Успех", "Профиль успешно создан! Вам доступны функции заказа.");
                updateInterfaceByRole(); // Мгновенно разблокируем кнопки на форме!
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось сохранить профиль (возможно, телефон или организация уже заняты).");
            }
        }
    } else {
        QMessageBox::information(this, "Заглушка", "Тут будет форма UPDATE для редактирования существующего профиля.");
    }
}

void MainWindow::on_btnCreateOrder_clicked() {
    QMessageBox::information(this, "Заглушка", "Форма создания заказа.");
}

void MainWindow::on_btnViewOrders_clicked() {
    QMessageBox::information(this, "Заглушка", "Просмотр заказов.");
}

void MainWindow::on_btnLogout_clicked() {
    emit logoutRequested();
}