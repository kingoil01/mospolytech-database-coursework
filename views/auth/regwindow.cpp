#include "regwindow.h"
#include "ui_regwindow.h"
#include <QMessageBox>
#include <QRegularExpression>

RegWindow::RegWindow(QWidget *parent) : QDialog(parent), ui(new Ui::RegWindow) {
    ui->setupUi(this);
    setModal(true);
}

RegWindow::~RegWindow() { delete ui; }

bool RegWindow::validatePassword(const QString &pass) {
    // Длина от 8 символов, 1 цифра, 1 заглавная буква, 1 спецсимвол
    QRegularExpression regex("^(?=.*[A-Z])(?=.*\\d)(?=.*[!@#$%^&*()_+{}|:\"<>?\\-=\\[\\]\\\\;',./]).{8,}$");
    return regex.match(pass).hasMatch();
}

void RegWindow::on_btnRegister_clicked() {
    QString login = ui->editLogin->text().trimmed();
    QString password = ui->editPassword->text();
    QString email = ui->editEmail->text().trimmed();

    if (login.isEmpty() || password.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Заполните все поля!");
        return;
    }

    if (!validatePassword(password)) {
        QMessageBox::warning(this, "Слабый пароль",
                             "Пароль должен быть длиннее 8 символов, содержать заглавную букву, цифру и спецсимвол!");
        return;
    }
    emit registrationSubmitted(login, password, email);
}

void RegWindow::on_btnCansel_clicked() {
    this->reject(); // Автоматически закроет диалоговое окно регистрации и вернет QDialog::Rejected в контроллер
}