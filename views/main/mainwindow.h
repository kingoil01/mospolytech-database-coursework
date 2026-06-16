#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../models/user.h"
#include "../../models/authmodel.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const User &user, AuthModel *model, QWidget *parent = nullptr);
    ~MainWindow();

    void updateInterfaceByRole(); // Метод динамической настройки кнопок

signals:
    void logoutRequested();

private slots:
    void on_btnCreateOrder_clicked();
    void on_btnViewOrders_clicked();
    void on_btnEditProfile_clicked();
    void on_btnLogout_clicked();

private:
    Ui::MainWindow *ui;
    User m_currentUser;
    AuthModel *m_model;
};

#endif // MAINWINDOW_H