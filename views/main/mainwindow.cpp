#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../auth/customerinfodialog.h"
#include "../auth/changedatadialog.h"

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
    bool hasProfile = m_model->checkCustomerProfileExists(m_currentUser.idUser(), customerId);

    CustomerInfoDialog dialog(this);

    if (!hasProfile) {
        // --- РЕЖИМ ДОБАВЛЕНИЯ (Профиля еще нет) ---
        if (dialog.exec() == QDialog::Accepted) {
            if (m_model->createCustomerProfile(m_currentUser.idUser(), dialog.getOrgName(), dialog.getAddress(), dialog.getPhone(), dialog.getContactPerson())) {
                QMessageBox::information(this, "Успех", "Профиль успешно создан! Вам доступны функции заказа.");
                updateInterfaceByRole(); // Мгновенно разблокируем кнопки заказов!
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось создать профиль. Проверьте корректность данных.");
            }
        }
    } else {
        // --- РЕЖИМ РЕДАКТИРОВАНИЯ (Профиль уже существует) ---
        QString org, addr, phone, cp;

        // Загружаем текущие данные из БД для предзаполнения полей формы
        if (m_model->getCustomerProfile(m_currentUser.idUser(), org, addr, phone, cp)) {
            dialog.setInitialData(org, addr, phone, cp);
        }

        if (dialog.exec() == QDialog::Accepted) {
            if (m_model->updateCustomerProfile(m_currentUser.idUser(), dialog.getOrgName(), dialog.getAddress(), dialog.getPhone(), dialog.getContactPerson())) {
                QMessageBox::information(this, "Успех", "Данные организации успешно обновлены!");
                updateInterfaceByRole();
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось обновить профиль (возможно, телефон уже занят другой фирмой).");
            }
        }
    }
}

void MainWindow::on_btnCreateOrder_clicked() {
    QMessageBox::information(this, "Заглушка", "Форма создания заказа.");
}

void MainWindow::on_btnViewOrders_clicked() {
    QMessageBox::information(this, "Заглушка", "Просмотр заказов.");
}

void MainWindow::on_btnChangeCredentials_clicked() {
    ChangeDataDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        if (m_model->updateUserCredentials(m_currentUser.idUser(), dialog.getNewLogin(), dialog.getNewEmail(), dialog.getNewPassword())) {
            QMessageBox::information(this, "Успех", "Ваши данные успешно изменены!");
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось обновить данные (возможно, логин занят).");
        }
    }
}

void MainWindow::on_btnLogout_clicked() {
    emit logoutRequested();
}