#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "../../models/auth/user.h"
#include "../../models/auth/authmodel.h"
#include "../../models/main/mainmodel.h"
#include "../../views/main/mainwindow.h"

class MainController : public QObject {
    Q_OBJECT
public:
    explicit MainController(const User &user, AuthModel *authModel, QObject *parent = nullptr);
    ~MainController();

    void start();

signals:
    void logout();

private slots:
    // Обработчики главного окна
    void handleEditProfile();
    void handleCreateOrder();
    void handleViewOrders();
    void handleChangeCredentials();
    void handleLogout();

private:
    // Вспомогательные методы
    void updateUI();
    bool hasCustomerProfile() const;
    void showError(const QString &message);
    void showSuccess(const QString &message);

    // Методы для работы с профилем
    void handleAddProfile();
    void handleEditProfileData();

    MainWindow *m_view;
    AuthModel *m_authModel;
    MainModel *m_mainModel;
    User m_user;
};

#endif // MAINCONTROLLER_H