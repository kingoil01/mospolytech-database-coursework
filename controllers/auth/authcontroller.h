#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include "../../models/auth/authmodel.h"
#include "../../views/auth/authwindow.h"

class MainController;
class AdminController;
class RegWindow;

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
    void handleMainLogout();

private:
    void showMainWindow(const User &user);
    void showAdminWindow(const User &user);

    AuthModel *m_model;
    AuthWindow *m_authWindow;
    RegWindow *m_regWindow;
    MainController *m_mainController;
    AdminController *m_adminController;

};

#endif // AUTHCONTROLLER_H