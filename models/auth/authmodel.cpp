#include "authmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AuthModel::AuthModel(QObject *parent) : QObject(parent) {}

bool AuthModel::validateUser(const QString &login, const QString &password, User &outUser) {
    QSqlQuery query;
    // Тянем данные пользователя и проверяем, есть ли у него профиль заказчика
    query.prepare("SELECT u.id_user, u.login, u.email, (c.id_customer IS NOT NULL) AS has_profile "
                  "FROM users u "
                  "LEFT JOIN customers c ON u.id_user = c.id_user "
                  "WHERE u.login = :login AND u.password = :pass;");
    query.bindValue(":login", login);
    query.bindValue(":pass", password);

    if (query.exec() && query.next()) {
        outUser = User(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toBool()
        );
        return true;
    }
    return false;
}

bool AuthModel::registerUser(const QString &login, const QString &password, const QString &email) {
    QSqlQuery query;
    // Теперь здесь чистая вставка только в таблицу users
    query.prepare("INSERT INTO users (login, password, email) VALUES (:login, :pass, :email);");
    query.bindValue(":login", login);
    query.bindValue(":pass", password);
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Ошибка регистрации пользователя:" << query.lastError().text();
        return false;
    }
    return true;
}

bool AuthModel::checkCustomerProfileExists(int idUser, int &outCustomerId) {
    QSqlQuery query;
    query.prepare("SELECT id_customer FROM customers WHERE id_user = :id_user");
    query.bindValue(":id_user", idUser);

    if (query.exec() && query.next()) {
        outCustomerId = query.value(0).toInt();
        return true;
    }
    outCustomerId = 0;
    return false;
}

bool AuthModel::createCustomerProfile(int idUser, const QString &org, const QString &addr, const QString &phone, const QString &cp) {
    QSqlQuery query;
    query.prepare("INSERT INTO customers (id_user, org_name, address, phone, contact_person) "
                  "VALUES (:id_user, :org_name, :address, :phone, :contact_person)");
    query.bindValue(":id_user", idUser);
    query.bindValue(":org_name", org);
    query.bindValue(":address", addr);
    query.bindValue(":phone", phone);
    query.bindValue(":contact_person", cp);
    return query.exec();
}

bool AuthModel::getCustomerProfile(int idUser, QString &outOrg, QString &outAddr, QString &outPhone, QString &outCp) {
    QSqlQuery query;
    query.prepare("SELECT org_name, address, phone, contact_person FROM customers WHERE id_user = :id_user");
    query.bindValue(":id_user", idUser);

    if (query.exec() && query.next()) {
        outOrg = query.value(0).toString();
        outAddr = query.value(1).toString();
        outPhone = query.value(2).toString();
        outCp = query.value(3).toString();
        return true;
    }
    return false;
}

bool AuthModel::updateCustomerProfile(int idUser, const QString &org, const QString &addr, const QString &phone, const QString &cp) {
    QSqlQuery query;

    // Проверяем уникальность телефона
    if (!phone.isEmpty()) {
        query.prepare("SELECT id_user FROM customers WHERE phone = :phone AND id_user != :id_user");
        query.bindValue(":phone", phone);
        query.bindValue(":id_user", idUser);
        if (query.exec() && query.next()) {
            qDebug() << "Этот телефон уже привязан к другой организации!";
            return false;
        }
    }

    // Статичный запрос
    query.prepare(
        "UPDATE customers SET "
        "  org_name = COALESCE(NULLIF(:org, ''), org_name), "
        "  address = COALESCE(NULLIF(:addr, ''), address), "
        "  phone = COALESCE(NULLIF(:phone, ''), phone), "
        "  contact_person = COALESCE(NULLIF(:cp, ''), contact_person) "
        "WHERE id_user = :id_user"
    );

    query.bindValue(":org", org);
    query.bindValue(":addr", addr);
    query.bindValue(":phone", phone);
    query.bindValue(":cp", cp);
    query.bindValue(":id_user", idUser);

    if (!query.exec()) {
        qDebug() << "Ошибка обновления профиля customers:" << query.lastError().text();
        return false;
    }
    return true;
}

bool AuthModel::updateUserCredentials(int idUser, const QString &newLogin, const QString &newEmail, const QString &newPassword) {
    QSqlQuery query;

    // Проверка занятости логина
    if (!newLogin.isEmpty()) {
        query.prepare("SELECT id_user FROM users WHERE login = :login AND id_user != :id_user");
        query.bindValue(":login", newLogin);
        query.bindValue(":id_user", idUser);

        if (!query.exec()) {
            qDebug() << "Ошибка проверки уникальности логина:" << query.lastError().text();
            return false;
        }

        if (query.next()) {
            return false;
        }
    }

    // Статичный запрос
    query.prepare(
        "UPDATE users SET "
        "  login = COALESCE(NULLIF(:newLogin, ''), login), "
        "  email = COALESCE(NULLIF(:newEmail, ''), email), "
        "  password = CASE "
        "    WHEN :newPassword = '' THEN password "
        "    ELSE :newPassword "
        "  END "
        "WHERE id_user = :id_user"
    );

    // Привязываем переменные
    query.bindValue(":newLogin", newLogin);
    query.bindValue(":newEmail", newEmail);
    query.bindValue(":newPassword", newPassword);
    query.bindValue(":id_user", idUser);

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool AuthModel::isAdmin(int userId) const {
    // Проверяем по ID роли (1 - Администратор)
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM user_roles ur "
        "WHERE ur.id_user = :userId AND ur.id_role = 1"
    );
    query.bindValue(":userId", userId);

    if (!query.exec() || !query.next()) {
        return false;
    }

    bool result = query.value(0).toInt() > 0;
    return result;
}

bool AuthModel::isManager(int userId) const {
    // Проверяем по ID роли (4 - Менеджер)
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM user_roles ur "
        "WHERE ur.id_user = :userId AND ur.id_role = 4"
    );
    query.bindValue(":userId", userId);

    if (!query.exec() || !query.next()) {
        return false;
    }

    bool result = query.value(0).toInt() > 0;
    return result;
}

bool AuthModel::hasRole(int userId, const QString &roleName) const {
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM user_roles ur "
        "INNER JOIN roles r ON ur.id_role = r.id_role "
        "WHERE ur.id_user = :userId AND r.role_name = :roleName"
    );
    query.bindValue(":userId", userId);
    query.bindValue(":roleName", roleName);

    if (!query.exec() || !query.next()) {
        return false;
    }

    return query.value(0).toInt() > 0;
}

QVector<QPair<int, QString>> AuthModel::getUserRoles(int userId) const {
    QVector<QPair<int, QString>> roles;

    QSqlQuery query;
    query.prepare(
        "SELECT r.id_role, r.role_name FROM user_roles ur "
        "INNER JOIN roles r ON ur.id_role = r.id_role "
        "WHERE ur.id_user = :userId"
    );
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qDebug() << "Ошибка получения ролей пользователя:" << query.lastError().text();
        return roles;
    }

    while (query.next()) {
        roles.append({query.value(0).toInt(), query.value(1).toString()});
    }

    return roles;
}