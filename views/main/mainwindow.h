#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../models/auth/user.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUserInfo(const User &user);
    void setProfileButtonMode(bool hasProfile);
    void setOrderButtonsEnabled(bool enabled);
    void showMessage(const QString &title, const QString &message, bool isError = false);

signals:
    void editProfileRequested();
    void createOrderRequested();
    void viewOrdersRequested();
    void changeCredentialsRequested();
    void logoutRequested();

private slots:
    void on_btnEditProfile_clicked();
    void on_btnCreateOrder_clicked();
    void on_btnViewOrders_clicked();
    void on_btnChangeCredentials_clicked();
    void on_btnLogout_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H