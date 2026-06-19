#include "authcontroller.h"
#include "../../controllers/main/maincontroller.h"
#include "../../controllers/admin/admincontroller.h"
#include "../../views/auth/regwindow.h"
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QDialogButtonBox>

AuthController::AuthController(QObject *parent)
    : QObject(parent)
    , m_model(new AuthModel(this))
    , m_authWindow(nullptr)
    , m_regWindow(nullptr)
    , m_adminController(nullptr)
    , m_mainController(nullptr) {

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
    if (m_adminController) {
        m_adminController->deleteLater();
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

        // Получаем все роли пользователя
        QVector<QPair<int, QString>> roles = m_model->getUserRoles(user.idUser());

        if (roles.isEmpty()) {
            QMessageBox::critical(m_authWindow, "Ошибка", "У пользователя нет ролей!");
            return;
        }

        // Если только одна роль - входим сразу
        if (roles.size() == 1) {
            QString roleName = roles.first().second;
            if (roleName == "Администратор" || roleName.toLower() == "admin") {
                showAdminWindow(user);
            } else {
                showMainWindow(user);
            }
        } else {
            // Несколько ролей - показываем мини-диалог выбора
            showRoleSelectionDialog(user, roles);
        }
    } else {
        QMessageBox::critical(m_authWindow, "Ошибка", "Неверный логин или пароль!");
    }
}

void AuthController::showRoleSelectionDialog(const User &user, const QVector<QPair<int, QString>> &roles) {
    // Создаем мини-диалог
    QDialog popUp(m_authWindow);
    popUp.setWindowTitle("Выбор роли");
    popUp.setModal(true);
    popUp.setMinimumWidth(350);

    // Основной вертикальный layout
    QVBoxLayout *mainLayout = new QVBoxLayout(&popUp);

    // Информационная метка
    QLabel *infoLabel = new QLabel(QString("Пользователь: <b>%1</b>\nВыберите роль для входа:").arg(user.login()), &popUp);
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);

    // Выпадающий список с ролями
    QComboBox *comboRoles = new QComboBox(&popUp);
    for (const auto &role : roles) {
        comboRoles->addItem(role.second, role.first);
    }
    mainLayout->addWidget(comboRoles);

    // Кнопки OK/Cancel
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &popUp);
    connect(buttons, &QDialogButtonBox::accepted, &popUp, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &popUp, &QDialog::reject);
    mainLayout->addWidget(buttons);

    // Показываем диалог
    if (popUp.exec() == QDialog::Accepted) {
        QString selectedRole = comboRoles->currentText();

        if (selectedRole == "Администратор" || selectedRole.toLower() == "admin") {
            showAdminWindow(user);
        } else {
            showMainWindow(user);
        }
    } else {
        // Пользователь отменил выбор - возвращаем на окно авторизации
        m_authWindow->clearFields();
        m_authWindow->show();
    }
}

void AuthController::showMainWindow(const User &user) {
    m_authWindow->close();

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
    m_authWindow->close();

    if (m_adminController) {
        m_adminController->deleteLater();
    }

    m_adminController = new AdminController(user, m_model, this);
    connect(m_adminController, &AdminController::logout,
            this, &AuthController::handleMainLogout);
    m_adminController->start();
}