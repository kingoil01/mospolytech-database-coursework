#ifndef DELETEUSERDIALOG_H
#define DELETEUSERDIALOG_H

#include <QDialog>
#include <QVector>

struct UserListItem {
    int id;
    QString login;
    QString email;
    bool isAdmin;
};

namespace Ui { class DeleteUserDialog; }

class DeleteUserDialog : public QDialog {
    Q_OBJECT
public:
    explicit DeleteUserDialog(QWidget *parent = nullptr);
    ~DeleteUserDialog();

    // Методы для управления списком
    void setUserList(const QVector<UserListItem> &users);
    int getSelectedUserId() const;
    void clearSelection();

    // Методы для сообщений
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