#ifndef MANAGERCONTROLLER_H
#define MANAGERCONTROLLER_H

#include <QObject>
#include "../../models/auth/user.h"
#include "../../models/auth/authmodel.h"
#include "../../models/manager/managermodel.h"
#include "../../views/manager/managerwindow.h"

class ManagerController : public QObject {
    Q_OBJECT
public:
    explicit ManagerController(const User &managerUser, AuthModel *authModel, QObject *parent = nullptr);
    ~ManagerController();

    void start();

signals:
    void logout();

private slots:
    void handleLogout();

private:
    ManagerWindow *m_view;
    ManagerModel *m_managerModel;
};

#endif // MANAGERCONTROLLER_H