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
    bool checkCustomerProfileExists(int idUser, int &outCustomerId);
    bool createCustomerProfile(int idUser, const QString &org, const QString &addr, const QString &phone, const QString &cp);
    bool updateUserCredentials(int idUser, const QString &newLogin, const QString &newEmail, const QString &newPassword);
    bool getCustomerProfile(int idUser, QString &outOrg, QString &outAddr, QString &outPhone, QString &outCp);
    bool updateCustomerProfile(int idUser, const QString &org, const QString &addr, const QString &phone, const QString &cp);

};

#endif // AUTHMODEL_H