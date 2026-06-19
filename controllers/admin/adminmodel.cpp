#include "adminmodel.h"
#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

AdminModel::AdminModel(QObject *parent) : QObject(parent) {}

QVector<AdminUserInfo> AdminModel::getAllUsers() {
    QVector<AdminUserInfo> users;

    QSqlQuery query(
        "SELECT u.id_user, u.login, u.email, "
        "c.id_customer IS NOT NULL as has_profile "
        "FROM users u "
        "LEFT JOIN customers c ON u.id_user = c.id_user "
        "ORDER BY u.id_user"
    );

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Ошибка получения пользователей:" << m_lastError;
        return users;
    }

    while (query.next()) {
        AdminUserInfo info;
        info.id = query.value(0).toInt();
        info.login = query.value(1).toString();
        info.email = query.value(2).toString();
        info.hasCustomerProfile = query.value(3).toBool();
        info.isAdmin = false;

        // Получаем роли пользователя
        QSqlQuery roleQuery;
        roleQuery.prepare(
            "SELECT r.role_name FROM user_roles ur "
            "INNER JOIN roles r ON ur.id_role = r.id_role "
            "WHERE ur.id_user = :userId"
        );
        roleQuery.bindValue(":userId", info.id);

        QStringList roles;
        if (roleQuery.exec()) {
            while (roleQuery.next()) {
                QString roleName = roleQuery.value(0).toString();
                roles.append(roleName);
                if (roleName.toLower() == "admin") {
                    info.isAdmin = true;
                }
            }
        }
        info.roles = roles.join(", ");

        users.append(info);
    }

    return users;
}

bool AdminModel::createUser(const QString &login, const QString &email,
                            const QString &password, bool isAdmin) {
    QSqlQuery query;
    query.exec("BEGIN;");

    // 1. Проверяем, не занят ли логин или email
    query.prepare("SELECT COUNT(*) FROM users WHERE login = :login OR email = :email");
    query.bindValue(":login", login);
    query.bindValue(":email", email);

    if (!query.exec() || !query.next()) {
        m_lastError = "Ошибка проверки существования пользователя";
        query.exec("ROLLBACK;");
        return false;
    }

    if (query.value(0).toInt() > 0) {
        m_lastError = "Пользователь с таким логином или email уже существует";
        query.exec("ROLLBACK;");
        return false;
    }

    // 2. Хэшируем пароль
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    QString passwordHash = hash.toHex();

    // 3. Создаем пользователя
    query.prepare(
        "INSERT INTO users (login, password, email) "
        "VALUES (:login, :password, :email) RETURNING id_user"
    );
    query.bindValue(":login", login);
    query.bindValue(":password", passwordHash);
    query.bindValue(":email", email);

    if (!query.exec() || !query.next()) {
        m_lastError = "Ошибка создания пользователя: " + query.lastError().text();
        query.exec("ROLLBACK;");
        return false;
    }

    int userId = query.value(0).toInt();

    // 4. Назначаем роль
    QString roleName = isAdmin ? "admin" : "user";
    query.prepare(
        "INSERT INTO user_roles (id_user, id_role) "
        "SELECT :userId, id_role FROM roles WHERE role_name = :roleName"
    );
    query.bindValue(":userId", userId);
    query.bindValue(":roleName", roleName);

    if (!query.exec()) {
        m_lastError = "Ошибка назначения роли: " + query.lastError().text();
        query.exec("ROLLBACK;");
        return false;
    }

    query.exec("COMMIT;");
    return true;
}

bool AdminModel::deleteUser(int userId) {
    // Проверяем, что пользователь существует
    if (!userExists(userId)) {
        m_lastError = "Пользователь не найден";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id_user = :userId");
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Ошибка удаления пользователя:" << m_lastError;
        return false;
    }

    return query.numRowsAffected() > 0;
}

QVector<RoleInfo> AdminModel::getAvailableRoles() {
    QVector<RoleInfo> roles;

    QSqlQuery query("SELECT id_role, role_name FROM roles ORDER BY role_name");

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        return roles;
    }

    while (query.next()) {
        RoleInfo info;
        info.id = query.value(0).toInt();
        info.name = query.value(1).toString();
        roles.append(info);
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