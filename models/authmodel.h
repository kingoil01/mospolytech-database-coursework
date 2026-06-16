#ifndef AUTHMODEL_H
#define AUTHMODEL_H

#include <QObject>
#include "user.h"

class AuthModel : public QObject {
    Q_OBJECT
public:
    explicit AuthModel(QObject *parent = nullptr);

    // Бизнес-логика авторизации и регистрации
    bool validateUser(const QString &login, const QString &password, User &outUser);
    bool registerUser(const QString &login, const QString &password, const QString &email);
};

#endif // AUTHMODEL_H