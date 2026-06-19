#include "adminmodel.h"
#include "../../models/databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

AdminModel::AdminModel(QObject *parent) : QObject(parent) {}

QVector<AdminUser> AdminModel::getAllUsers() {
    QVector<AdminUser> users;

    QSqlQuery query;
    query.exec(
        "SELECT u.id_user, u.login, u.email, "
        "c.id_customer IS NOT NULL as has_profile "
        "FROM users u "
        "LEFT JOIN customers c ON u.id_user = c.id_user "
        "ORDER BY u.id_user"
    );

    if (!query.lastError().isValid() && query.isActive()) {
        while (query.next()) {
            int id = query.value(0).toInt();
            QString login = query.value(1).toString();
            QString email = query.value(2).toString();
            bool hasProfile = query.value(3).toBool();

            // Создаем объект AdminUser
            AdminUser user(id, login, email, hasProfile, false);

            // Получаем роли пользователя
            QSqlQuery roleQuery;
            roleQuery.prepare(
                "SELECT r.role_name FROM user_roles ur "
                "INNER JOIN roles r ON ur.id_role = r.id_role "
                "WHERE ur.id_user = :userId"
            );
            roleQuery.bindValue(":userId", id);

            QStringList roles;
            bool isAdmin = false;

            if (roleQuery.exec()) {
                while (roleQuery.next()) {
                    QString roleName = roleQuery.value(0).toString();
                    roles.append(roleName);
                    if (roleName == "Администратор" || roleName.toLower() == "admin") {
                        isAdmin = true;
                    }
                }
            }

            user.setIsAdmin(isAdmin);
            user.setRoles(roles.join(", "));

            users.append(user);
        }
    }
    else {
        m_lastError = query.lastError().text();
        qDebug() << "Ошибка получения пользователей:" << m_lastError;
    }

    return users;
}

bool AdminModel::createUser(const QString &login, const QString &email,
                            const QString &password, bool isAdmin) {
    QSqlQuery query;

    // Начинаем транзакцию
    if (!query.exec("BEGIN;")) {
        m_lastError = "Не удалось начать транзакцию: " + query.lastError().text();
        qDebug() << m_lastError;
        return false;
    }

    // 1. Проверяем, не занят ли логин или email
    query.prepare("SELECT COUNT(*) FROM users WHERE login = :login OR email = :email");
    query.bindValue(":login", login);
    query.bindValue(":email", email);

    if (!query.exec()) {
        m_lastError = "Ошибка проверки существования пользователя: " + query.lastError().text();
        query.exec("ROLLBACK;");
        qDebug() << m_lastError;
        return false;
    }

    if (!query.next()) {
        m_lastError = "Ошибка получения результата проверки";
        query.exec("ROLLBACK;");
        return false;
    }

    if (query.value(0).toInt() > 0) {
        m_lastError = "Пользователь с таким логином или email уже существует";
        query.exec("ROLLBACK;");
        qDebug() << m_lastError;
        return false;
    }

    // 2. Создаем пользователя
    QSqlQuery insertQuery;
    insertQuery.prepare(
        "INSERT INTO users (login, password, email) "
        "VALUES (:login, :password, :email) RETURNING id_user"
    );
    insertQuery.bindValue(":login", login);
    insertQuery.bindValue(":password", password);
    insertQuery.bindValue(":email", email);

    if (!insertQuery.exec()) {
        m_lastError = "Ошибка создания пользователя: " + insertQuery.lastError().text();
        query.exec("ROLLBACK;");
        qDebug() << m_lastError;
        return false;
    }

    if (!insertQuery.next()) {
        m_lastError = "Не удалось получить ID созданного пользователя";
        query.exec("ROLLBACK;");
        return false;
    }

    int userId = insertQuery.value(0).toInt();

    // 3. Получаем ID роли
    QString roleName = isAdmin ? "Администратор" : "Пользователь";
    QSqlQuery roleIdQuery;
    roleIdQuery.prepare("SELECT id_role FROM roles WHERE role_name = :roleName");
    roleIdQuery.bindValue(":roleName", roleName);

    if (!roleIdQuery.exec()) {
        m_lastError = "Ошибка получения ID роли: " + roleIdQuery.lastError().text();
        query.exec("ROLLBACK;");
        qDebug() << m_lastError;
        return false;
    }

    if (!roleIdQuery.next()) {
        m_lastError = QString("Роль '%1' не найдена в таблице roles").arg(roleName);
        query.exec("ROLLBACK;");
        qDebug() << m_lastError;
        return false;
    }

    int roleId = roleIdQuery.value(0).toInt();

    // 4. Проверяем, есть ли уже такая роль у пользователя
    QSqlQuery checkQuery;
    checkQuery.prepare(
        "SELECT COUNT(*) FROM user_roles WHERE id_user = :userId AND id_role = :roleId"
    );
    checkQuery.bindValue(":userId", userId);
    checkQuery.bindValue(":roleId", roleId);

    if (!checkQuery.exec()) {
        m_lastError = "Ошибка проверки существующей роли: " + checkQuery.lastError().text();
        query.exec("ROLLBACK;");
        qDebug() << m_lastError;
        return false;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {}
    else {
        // 5. Назначаем роль
        QSqlQuery roleQuery;
        roleQuery.prepare(
            "INSERT INTO user_roles (id_user, id_role) "
            "VALUES (:userId, :roleId)"
        );
        roleQuery.bindValue(":userId", userId);
        roleQuery.bindValue(":roleId", roleId);

        if (!roleQuery.exec()) {
            m_lastError = "Ошибка назначения роли: " + roleQuery.lastError().text();
            query.exec("ROLLBACK;");
            qDebug() << m_lastError;
            return false;
        }
    }

    // 6. Фиксируем транзакцию
    if (!query.exec("COMMIT;")) {
        m_lastError = "Ошибка фиксации транзакции: " + query.lastError().text();
        qDebug() << m_lastError;
        return false;
    }

    return true;
}

bool AdminModel::deleteUser(int userId) {
    if (!userExists(userId)) {
        m_lastError = "Пользователь не найден";
        return false;
    }

    QSqlQuery query;
    query.exec("BEGIN;");

    // Удаляем связи с ролями
    QSqlQuery roleDelete;
    roleDelete.prepare("DELETE FROM user_roles WHERE id_user = :userId");
    roleDelete.bindValue(":userId", userId);
    if (!roleDelete.exec()) {
        m_lastError = "Ошибка удаления ролей: " + roleDelete.lastError().text();
        query.exec("ROLLBACK;");
        return false;
    }

    // Удаляем пользователя
    QSqlQuery userDelete;
    userDelete.prepare("DELETE FROM users WHERE id_user = :userId");
    userDelete.bindValue(":userId", userId);

    if (!userDelete.exec()) {
        m_lastError = userDelete.lastError().text();
        query.exec("ROLLBACK;");
        qDebug() << "Ошибка удаления пользователя:" << m_lastError;
        return false;
    }

    if (userDelete.numRowsAffected() == 0) {
        m_lastError = "Пользователь не был удален";
        query.exec("ROLLBACK;");
        return false;
    }

    query.exec("COMMIT;");
    return true;
}

QVector<Role> AdminModel::getAvailableRoles() {
    QVector<Role> roles;

    QSqlQuery query("SELECT id_role, role_name FROM roles ORDER BY role_name");

    if (!query.lastError().isValid() && query.isActive()) {
        while (query.next()) {
            Role role(
                query.value(0).toInt(),
                query.value(1).toString()
            );
            roles.append(role);
        }
    } else {
        m_lastError = query.lastError().text();
        qDebug() << "Ошибка получения ролей:" << m_lastError;
    }

    return roles;
}

bool AdminModel::userExists(int userId) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM users WHERE id_user = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec() || !query.next()) {
        return false;
    }

    return query.value(0).toInt() > 0;
}

QString AdminModel::getLastError() const {
    return m_lastError;
}