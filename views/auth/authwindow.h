#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>

namespace Ui { class AuthWindow; }

class AuthWindow : public QWidget {
    Q_OBJECT
public:
    explicit AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

signals:
    void loginSubmitted(const QString &login, const QString &password);
    void openRegisterRequested();

private slots:
    void on_btnLogin_clicked();     // Имя должно строго совпадать с objectName кнопки в .ui
    void on_btnOpenReg_clicked();   // Имя должно строго совпадать с objectName кнопки в .ui
    void on_btnClose_clicked();

private:
    Ui::AuthWindow *ui;
};

#endif // AUTHWINDOW_H