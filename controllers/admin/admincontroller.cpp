#include "admincontroller.h"
#include "../../views/admin/createuserdialog.h"
#include "../../views/admin/deleteuserdialog.h"
#include <QMessageBox>

AdminController::AdminController(const User &adminUser, AuthModel *authModel, QObject *parent)
    : QObject(parent)
    , m_view(new AdminWindow())
    , m_authModel(authModel)
    , m_adminModel(new AdminModel(this))
    , m_adminUser(adminUser) {

    connect(m_view, &AdminWindow::createUserRequested,
            this, &AdminController::handleCreateUser);
    connect(m_view, &AdminWindow::deleteUserRequested,
            this, &AdminController::handleDeleteUser);
    connect(m_view, &AdminWindow::logoutRequested,
            this, &AdminController::handleLogout);

    m_view->setAdminInfo(m_adminUser);
}

AdminController::~AdminController() {
    if (m_view) {
        m_view->deleteLater();
    }
}

void AdminController::start() {
    m_view->show();
}

void AdminController::handleCreateUser() {
    CreateUserDialog dialog(m_view);

    if (dialog.exec() == QDialog::Accepted) {
        QString login = dialog.getLogin();
        QString email = dialog.getEmail();
        QString password = dialog.getPassword();
        bool isAdmin = dialog.isAdminRole();

        if (login.isEmpty() || email.isEmpty() || password.isEmpty()) {
            dialog.showError("Все поля должны быть заполнены!");
            return;
        }

        if (login.length() < 3) {
            dialog.showError("Логин должен содержать минимум 3 символа!");
            return;
        }

        if (password.length() < 6) {
            dialog.showError("Пароль должен содержать минимум 6 символов!");
            return;
        }

        if (!email.contains('@') || !email.contains('.')) {
            dialog.showError("Введите корректный email!");
            return;
        }

        if (m_adminModel->createUser(login, email, password, isAdmin)) {
            m_view->showMessage("Успех", QString("Пользователь '%1' успешно создан!").arg(login));
            loadUsersForDeletion();
        } else {
            m_view->showMessage("Ошибка",
                "Не удалось создать пользователя: " + m_adminModel->getLastError(),
                true);
        }
    }
}

void AdminController::handleDeleteUser() {
    DeleteUserDialog dialog(m_view);

    QVector<AdminUser> users = m_adminModel->getAllUsers();

    QVector<AdminUser> filteredUsers;
    for (const auto &user : users) {
        if (user.idUser() != m_adminUser.idUser()) {
            filteredUsers.append(user);
        }
    }

    if (filteredUsers.isEmpty()) {
        m_view->showMessage("Информация", "Нет пользователей для удаления.");
        return;
    }

    dialog.setUserList(filteredUsers);

    if (dialog.exec() == QDialog::Accepted) {
        int userId = dialog.getSelectedUserId();
        if (userId == -1) {
            dialog.showWarning("Пожалуйста, выберите пользователя для удаления.");
            return;
        }

        QString userLogin;
        for (const auto &user : filteredUsers) {
            if (user.idUser() == userId) {
                userLogin = user.login();
                break;
            }
        }

        if (m_adminModel->deleteUser(userId)) {
            m_view->showMessage("Успех", QString("Пользователь '%1' успешно удален!").arg(userLogin));
            loadUsersForDeletion();
        } else {
            m_view->showMessage("Ошибка",
                "Не удалось удалить пользователя: " + m_adminModel->getLastError(),
                true);
        }
    }
}

void AdminController::loadUsersForDeletion() {
    m_cachedUsers = m_adminModel->getAllUsers();
}

void AdminController::handleLogout() {
    emit logout();
}

void AdminController::updateUserList() {
    loadUsersForDeletion();
}