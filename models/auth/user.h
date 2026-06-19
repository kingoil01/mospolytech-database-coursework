#ifndef USER_H
#define USER_H

#include <QString>

class User {
public:
    User() : m_idUser(0), m_hasCustomerProfile(false) {}
    User(int id, const QString &login, const QString &email, bool hasProfile)
        : m_idUser(id), m_login(login), m_email(email), m_hasCustomerProfile(hasProfile) {}

    int idUser() const { return m_idUser; }
    QString login() const { return m_login; }
    QString email() const { return m_email; }
    bool hasCustomerProfile() const { return m_hasCustomerProfile; }

private:
    int m_idUser;
    QString m_login;
    QString m_email;
    bool m_hasCustomerProfile;
};

#endif // USER_H