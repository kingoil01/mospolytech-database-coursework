#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QDialog>

namespace Ui { class RegWindow; }

class RegWindow : public QDialog {
    Q_OBJECT
public:
    explicit RegWindow(QWidget *parent = nullptr);
    ~RegWindow();

signals:
    void registrationSubmitted(const QString &login, const QString &password, const QString &email);

private slots:
    void on_btnRegister_clicked();
    void on_btnCansel_clicked();

private:
    Ui::RegWindow *ui;
    bool validatePassword(const QString &pass);
};

#endif // REGWINDOW_H