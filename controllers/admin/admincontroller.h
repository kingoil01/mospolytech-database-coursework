#ifndef ADMINCONTROLLER_H
#define ADMINCONTROLLER_H

#include <QObject>
#include "../../models/auth/user.h"
#include "../../models/auth/authmodel.h"
#include "../../models/admin/adminmodel.h"
#include "../../models/admin/adminuser.h"
#include "../../views/admin/adminwindow.h"

class AdminController : public QObject {
    Q_OBJECT
public:
    explicit AdminController(const User &adminUser, AuthModel *authModel, QObject *parent = nullptr);
    ~AdminController();

    void start();

signals:
    void logout();

private slots:
    void handleCreateUser();
    void handleDeleteUser();
    void handleLogout();

private:
    void loadUsersForDeletion();
    void updateUserList();

    AdminWindow *m_view;
    AuthModel *m_authModel;
    AdminModel *m_adminModel;
    User m_adminUser;

    QVector<AdminUser> m_cachedUsers;
};

#endif // ADMINCONTROLLER_H