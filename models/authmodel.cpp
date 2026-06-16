#include "authmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AuthModel::AuthModel(QObject *parent) : QObject(parent) {}

bool AuthModel::validateUser(const QString &login, const QString &password, User &outUser) {
    QSqlQuery query;
    // Тянем данные пользователя и проверяем через LEFT JOIN, есть ли у него профиль заказчика
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
