#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include "../models/authmodel.h"
#include "../views/auth/authwindow.h"
#include "../views/auth/regwindow.h"

class AuthController : public QObject {
    Q_OBJECT
public:
    explicit AuthController(QObject *parent = nullptr);
    ~AuthController();

    void start(); // Запуск цепочки авторизации

private slots:
    void handleLogin(const QString &login, const QString &password);
    void handleOpenRegister();
    void handleRegistration(const QString &login, const QString &password, const QString &email);

private:
    AuthModel *m_model;
    AuthWindow *m_authWindow;
    RegWindow *m_regWindow;
};

#endif // AUTHCONTROLLER_H