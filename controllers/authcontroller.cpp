#include "authcontroller.h"
#include <QMessageBox>

AuthController::AuthController(QObject *parent)
    : QObject(parent), m_model(new AuthModel(this)), m_authWindow(nullptr), m_regWindow(nullptr) {}

AuthController::~AuthController() {
    if (m_authWindow) m_authWindow->deleteLater();
    if (m_mainWindow) m_mainWindow->deleteLater();
}

void AuthController::start() {
    m_authWindow = new AuthWindow();

    // Связываем сигналы View со слотами Controller'а
    connect(m_authWindow, &AuthWindow::loginSubmitted, this, &AuthController::handleLogin);
    connect(m_authWindow, &AuthWindow::openRegisterRequested, this, &AuthController::handleOpenRegister);

    m_authWindow->show();
}

void AuthController::handleOpenRegister() {
    m_regWindow = new RegWindow(m_authWindow);
    connect(m_regWindow, &RegWindow::registrationSubmitted, this, &AuthController::handleRegistration);
    m_regWindow->exec(); // Открываем диалоговое окно модально
}

void AuthController::handleRegistration(const QString &login, const QString &password, const QString &email) {
    if (m_model->registerUser(login, password, email)) {
        QMessageBox::information(m_regWindow, "Успех", "Регистрация прошла успешно! Теперь войдите в аккаунт.");
        m_regWindow->accept(); // Закрывает диалог регистрации
    } else {
        QMessageBox::critical(m_regWindow, "Ошибка", "Не удалось зарегистрироваться. Логин или Email уже заняты.");
    }
}

void AuthController::handleLogin(const QString &login, const QString &password) {
    if (m_model->validateUser(login, password, m_currentUser)) {
        m_authWindow->close(); // Закрываем окно входа

        // Создаем главное окно и передаем ему данные пользователя и ссылку на модель
        m_mainWindow = new MainWindow(m_currentUser, m_model);
        connect(m_mainWindow, &MainWindow::logoutRequested, this, &AuthController::handleLogout);
        m_mainWindow->show();
    } else {
        QMessageBox::critical(m_authWindow, "Ошибка", "Неверный логин или пароль!");
    }
}

void AuthController::handleLogout() {
    m_mainWindow->close();
    m_mainWindow->deleteLater();
    m_mainWindow = nullptr;

    start(); // Возврат на форму авторизации
}
