#include "authcontroller.h"
#include <QMessageBox>

AuthController::AuthController(QObject *parent)
    : QObject(parent), m_model(new AuthModel(this)), m_authWindow(nullptr), m_regWindow(nullptr) {}

AuthController::~AuthController() {
    if (m_authWindow) m_authWindow->deleteLater();
}

void AuthController::start() {
    m_authWindow = new AuthWindow();

    // Связываем сигналы View со слотами Controller'а
    connect(m_authWindow, &AuthWindow::loginSubmitted, this, &AuthController::handleLogin);
    connect(m_authWindow, &AuthWindow::openRegisterRequested, this, &AuthController::handleOpenRegister);

    m_authWindow->show();
}

void AuthController::handleLogin(const QString &login, const QString &password) {
    User authenticatedUser;
    if (m_model->validateUser(login, password, authenticatedUser)) {
        QMessageBox::information(m_authWindow, "Успех", "Авторизация прошла успешно!");
        m_authWindow->close();

        // Проверяем твой сценарий эксплойта
        if (!authenticatedUser.hasCustomerProfile()) {
            // TODO: Открыть Окно Заполнения Информации (CustomerInfoDialog)
            // Если профиль успешно создан -> открыть MainWindow
            // Если диалог закрыли крестиком -> завершить приложение
        } else {
            // TODO: Сразу открыть Основное окно (MainWindow) пользователя
        }
    } else {
        QMessageBox::critical(m_authWindow, "Ошибка", "Неверный логин или пароль!");
    }
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