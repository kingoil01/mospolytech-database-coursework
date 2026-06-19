#ifndef DELETEUSERDIALOG_H
#define DELETEUSERDIALOG_H

#include <QDialog>
#include <QVector>
#include "../../models/admin/adminuser.h"

namespace Ui { class DeleteUserDialog; }

class DeleteUserDialog : public QDialog {
    Q_OBJECT
public:
    explicit DeleteUserDialog(QWidget *parent = nullptr);
    ~DeleteUserDialog();

    void setUserList(const QVector<AdminUser> &users);
    int getSelectedUserId() const;
    void clearSelection();

    void showError(const QString &message);
    void showWarning(const QString &message);
    bool askConfirmation(const QString &message);

private slots:
    void on_btnDelete_clicked();
    void on_btnCancel_clicked();
    void on_listUsers_itemSelectionChanged();

private:
    Ui::DeleteUserDialog *ui;
};

#endif // DELETEUSERDIALOG_H