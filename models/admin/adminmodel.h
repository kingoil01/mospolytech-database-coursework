#ifndef ADMINMODEL_H
#define ADMINMODEL_H

#include <QObject>
#include <QVector>

struct AdminUserInfo {
    int id;
    QString login;
    QString email;
    QString roles;
    bool isAdmin;
    bool hasCustomerProfile;
};

struct RoleInfo {
    int id;
    QString name;
};

class AdminModel : public QObject {
    Q_OBJECT
public:
    explicit AdminModel(QObject *parent = nullptr);

    QVector<AdminUserInfo> getAllUsers();
    bool createUser(const QString &login, const QString &email, const QString &password, bool isAdmin);
    bool deleteUser(int userId);
    QVector<RoleInfo> getAvailableRoles();
    bool userExists(int userId);
    QString getLastError() const;

private:
    QString m_lastError;
};

#endif // ADMINMODEL_H