#include "maincontroller.h"
#include "../../views/auth/customerinfodialog.h"
#include "../../views/auth/changedatadialog.h"
#include "../../views/main/orderwindow.h"
#include "../../views/main/orderhistorydialog.h"
#include <QMessageBox>

MainController::MainController(const User &user, AuthModel *authModel, QObject *parent)
    : QObject(parent)
    , m_view(new MainWindow())
    , m_authModel(authModel)
    , m_mainModel(new MainModel(this))
    , m_user(user) {

    // Связываем сигналы View со слотами Controller
    connect(m_view, &MainWindow::editProfileRequested,
            this, &MainController::handleEditProfile);
    connect(m_view, &MainWindow::createOrderRequested,
            this, &MainController::handleCreateOrder);
    connect(m_view, &MainWindow::viewOrdersRequested,
            this, &MainController::handleViewOrders);
    connect(m_view, &MainWindow::changeCredentialsRequested,
            this, &MainController::handleChangeCredentials);
    connect(m_view, &MainWindow::logoutRequested,
            this, &MainController::handleLogout);

    // Обновляем UI
    m_view->setUserInfo(m_user);
    updateUI();
}

MainController::~MainController() {
    if (m_view) {
        m_view->deleteLater();
    }
}

void MainController::start() {
    m_view->show();
}

void MainController::updateUI() {
    bool hasProfile = hasCustomerProfile();
    m_view->setProfileButtonMode(hasProfile);
}

bool MainController::hasCustomerProfile() const {
    int customerId = 0;
    return m_authModel->checkCustomerProfileExists(m_user.idUser(), customerId);
}

void MainController::showError(const QString &message) {
    m_view->showMessage("Ошибка", message, true);
}

void MainController::showSuccess(const QString &message) {
    m_view->showMessage("Успех", message, false);
}

void MainController::handleAddProfile() {
    CustomerInfoDialog dialog(m_view);

    if (dialog.exec() == QDialog::Accepted) {
        if (m_authModel->createCustomerProfile(m_user.idUser(),
                                               dialog.getOrgName(),
                                               dialog.getAddress(),
                                               dialog.getPhone(),
                                               dialog.getContactPerson())) {
            showSuccess("Профиль успешно создан! Вам доступны функции заказа.");
            updateUI();
        } else {
            showError("Не удалось создать профиль. Проверьте корректность данных.");
        }
    }
}

void MainController::handleEditProfileData() {
    CustomerInfoDialog dialog(m_view);

    // Загружаем текущие данные для предзаполнения
    QString org, addr, phone, cp;
    if (m_authModel->getCustomerProfile(m_user.idUser(), org, addr, phone, cp)) {
        dialog.setInitialData(org, addr, phone, cp);
    }

    if (dialog.exec() == QDialog::Accepted) {
        if (m_authModel->updateCustomerProfile(m_user.idUser(),
                                               dialog.getOrgName(),
                                               dialog.getAddress(),
                                               dialog.getPhone(),
                                               dialog.getContactPerson())) {
            showSuccess("Данные организации успешно обновлены!");
            updateUI();
        } else {
            showError("Не удалось обновить профиль.");
        }
    }
}

void MainController::handleEditProfile() {
    if (hasCustomerProfile()) {
        handleEditProfileData();
    } else {
        handleAddProfile();
    }
}

void MainController::handleCreateOrder() {
    OrderWindow orderWin(m_user.idUser(), m_mainModel, m_view);
    orderWin.exec();
}

void MainController::handleViewOrders() {
    OrderHistoryDialog historyWin(m_user.idUser(), m_mainModel, m_view);
    historyWin.exec();
}

void MainController::handleChangeCredentials() {
    ChangeDataDialog dialog(m_view);

    // Устанавливаем текущие данные
    dialog.setCurrentLogin(m_user.login());
    dialog.setCurrentEmail(m_user.email());

    if (dialog.exec() == QDialog::Accepted) {
        QString newLogin = dialog.getNewLogin();
        QString newEmail = dialog.getNewEmail();
        QString newPassword = dialog.getNewPassword();

        // Проверяем, есть ли изменения
        bool hasChanges = !newLogin.isEmpty() || !newEmail.isEmpty() || !newPassword.isEmpty();

        if (!hasChanges) {
            m_view->showMessage("Внимание", "Вы не ввели ни одного нового параметра!", false);
            return;
        }

        // Валидация
        if (!newLogin.isEmpty() && newLogin.length() < 3) {
            m_view->showMessage("Внимание", "Логин должен содержать минимум 3 символа!", false);
            return;
        }

        if (!newEmail.isEmpty() && !dialog.isValidEmail(newEmail)) {
            m_view->showMessage("Внимание", "Неверный формат email!", false);
            return;
        }

        if (!newPassword.isEmpty() && newPassword.length() < 6) {
            m_view->showMessage("Внимание", "Пароль должен содержать минимум 6 символов!", false);
            return;
        }

        // Обновляем данные
        if (m_authModel->updateUserCredentials(m_user.idUser(),
                                               newLogin.isEmpty() ? m_user.login() : newLogin,
                                               newEmail.isEmpty() ? m_user.email() : newEmail,
                                               newPassword)) {
            // Обновляем локальные данные пользователя
            m_user = User(m_user.idUser(),
                         newLogin.isEmpty() ? m_user.login() : newLogin,
                         newEmail.isEmpty() ? m_user.email() : newEmail,
                         m_user.hasCustomerProfile());

            m_view->setUserInfo(m_user);
            showSuccess("Ваши данные успешно изменены!");
        } else {
            showError("Не удалось обновить данные.");
        }
    }
}

void MainController::handleLogout() {
    emit logout();
}