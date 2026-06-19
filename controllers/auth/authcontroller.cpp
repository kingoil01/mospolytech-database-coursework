#include "authcontroller.h"
#include "../../controllers/main/maincontroller.h"
#include "../../controllers/admin/admincontroller.h"
#include "../../controllers/main/maincontroller.h"
#include "../../views/auth/regwindow.h"
#include <QMessageBox>

AuthController::AuthController(QObject *parent)
    : QObject(parent)
    , m_model(new AuthModel(this))
    , m_authWindow(nullptr)
    , m_regWindow(nullptr)
    , m_adminController(nullptr)
    , m_mainController(nullptr) {

    // Создаем окно авторизации при создании контроллера
    m_authWindow = new AuthWindow();

    connect(m_authWindow, &AuthWindow::loginSubmitted,
            this, &AuthController::handleLogin);
    connect(m_authWindow, &AuthWindow::openRegisterRequested,
            this, &AuthController::handleOpenRegister);
}

AuthController::~AuthController() {
    if (m_authWindow) {
        m_authWindow->deleteLater();
    }
    if (m_regWindow) {
        m_regWindow->deleteLater();
    }
    if (m_mainController) {
        m_mainController->deleteLater();
    }
}

void AuthController::start() {
    m_authWindow->show();
}

void AuthController::handleOpenRegister() {
    m_regWindow = new RegWindow(m_authWindow);
    connect(m_regWindow, &RegWindow::registrationSubmitted,
            this, &AuthController::handleRegistration);
    m_regWindow->exec();
}

void AuthController::handleRegistration(const QString &login, const QString &password, const QString &email) {
    if (m_model->registerUser(login, password, email)) {
        QMessageBox::information(m_regWindow, "Успех", "Регистрация прошла успешно! Теперь войдите в аккаунт.");
        m_regWindow->accept();
        m_authWindow->clearFields();
    } else {
        QMessageBox::critical(m_regWindow, "Ошибка", "Не удалось зарегистрироваться. Логин или Email уже заняты.");
    }
}

void AuthController::handleLogin(const QString &login, const QString &password) {
    User user;
    if (m_model->validateUser(login, password, user)) {
        m_authWindow->close();

        bool isAdmin = m_model->isAdmin(user.idUser());

        if (isAdmin) {
            showAdminWindow(user);
        } else {
            showMainWindow(user);
        }
    } else {
        QMessageBox::critical(m_authWindow, "Ошибка", "Неверный логин или пароль!");
    }
}

void AuthController::showMainWindow(const User &user) {

    if (m_mainController) {
        m_mainController->deleteLater();
    }

    m_mainController = new MainController(user, m_model, this);
    connect(m_mainController, &MainController::logout,
            this, &AuthController::handleMainLogout);
    m_mainController->start();
}

void AuthController::handleMainLogout() {
    if (m_mainController) {
        m_mainController->deleteLater();
        m_mainController = nullptr;
    }

    if (m_adminController) {
        m_adminController->deleteLater();
        m_adminController = nullptr;
    }

    m_authWindow->clearFields();
    m_authWindow->show();
}

void AuthController::showAdminWindow(const User &user) {
    if (m_adminController) {
        m_adminController->deleteLater();
    }

    m_adminController = new AdminController(user, m_model, this);
    connect(m_adminController, &AdminController::logout,
            this, &AuthController::handleMainLogout);
    m_adminController->start();
}