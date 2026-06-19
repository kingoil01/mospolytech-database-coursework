#ifndef ADMINUSER_H
#define ADMINUSER_H

#include <QString>
#include "../auth/user.h"

class AdminUser : public User {
public:
    AdminUser() : User(), m_isAdmin(false) {}

    AdminUser(int id, const QString &login, const QString &email,
              bool hasProfile, bool isAdmin, const QString &roles = "")
        : User(id, login, email, hasProfile)
        , m_isAdmin(isAdmin)
        , m_roles(roles) {}

    // Геттеры
    bool isAdmin() const { return m_isAdmin; }
    QString roles() const { return m_roles; }

    // Сеттеры (для удобства)
    void setIsAdmin(bool isAdmin) { m_isAdmin = isAdmin; }
    void setRoles(const QString &roles) { m_roles = roles; }

    // Дополнительные методы
    QString getDisplayText() const {
        QString text = login() + " (" + email() + ")";
        if (m_isAdmin) {
            text += " [АДМИН]";
        }
        return text;
    }

private:
    bool m_isAdmin;
    QString m_roles;
};

#endif // ADMINUSER_H