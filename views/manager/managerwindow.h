#ifndef MANAGERWINDOW_H
#define MANAGERWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "../../models/auth/user.h"
#include "../../models/manager/managermodel.h"

namespace Ui { class ManagerWindow; }

class ManagerWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit ManagerWindow(const User &user, ManagerModel *model, QWidget *parent = nullptr);
    ~ManagerWindow();

signals:
    void logoutRequested();

private slots:
    void on_btnRefresh_clicked();
    void on_btnLogout_clicked();

private:
    void loadOrders();

    Ui::ManagerWindow *ui;
    ManagerModel *m_model;
    User m_user;
};

#endif // MANAGERWINDOW_H