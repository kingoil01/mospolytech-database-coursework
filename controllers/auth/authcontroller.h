#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include "../../models/authmodel.h"
#include "../../models/user.h"
#include "../../views/auth/authwindow.h"
#include "../../views/auth/regwindow.h"
#include "../../views/auth/customerinfodialog.h"
#include "../../views/main/mainwindow.h"

class AuthController : public QObject {
    Q_OBJECT
public:
    explicit AuthController(QObject *parent = nullptr);
    ~AuthController();
    void start();

private slots:
    void handleLogin(const QString &login, const QString &password);
    void handleOpenRegister();
    void handleRegistration(const QString &login, const QString &password, const QString &email);
    void handleLogout();

private:
    AuthModel *m_model;
    AuthWindow *m_authWindow;
    RegWindow *m_regWindow;
    CustomerInfoDialog *m_infoDialog;
    MainWindow *m_mainWindow;

    User m_currentUser; // Храним текущего юзера сессии
};

#endif // AUTHCONTROLLER_H