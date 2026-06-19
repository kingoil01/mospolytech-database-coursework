#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "../../models/auth/user.h"

namespace Ui { class AdminWindow; }

class AdminWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

    void setAdminInfo(const User &user);
    void showMessage(const QString &title, const QString &message, bool isError = false);

signals:
    void logoutRequested();
    void createUserRequested();
    void deleteUserRequested();

private slots:
    void on_btnCreateUser_clicked();
    void on_btnDeleteUser_clicked();
    void on_btnLogout_clicked();

private:
    Ui::AdminWindow *ui;
};

#endif // ADMINWINDOW_H