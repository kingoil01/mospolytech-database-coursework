#ifndef CREATEUSERDIALOG_H
#define CREATEUSERDIALOG_H

#include <QDialog>

namespace Ui { class CreateUserDialog; }

class CreateUserDialog : public QDialog {
    Q_OBJECT
public:
    explicit CreateUserDialog(QWidget *parent = nullptr);
    ~CreateUserDialog();

    // Геттеры для данных
    QString getLogin() const;
    QString getEmail() const;
    QString getPassword() const;
    bool isAdminRole() const;

    // Методы для управления UI
    void clearFields();
    void showError(const QString &message);
    void showSuccess(const QString &message);

private slots:
    void on_btnCreate_clicked();
    void on_btnCancel_clicked();

private:
    Ui::CreateUserDialog *ui;
};

#endif // CREATEUSERDIALOG_H