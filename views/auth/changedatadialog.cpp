#include "changedatadialog.h"
#include "ui_changedatadialog.h"
#include <QMessageBox>
#include <QRegularExpression>

ChangeDataDialog::ChangeDataDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ChangeDataDialog) {
    ui->setupUi(this);
}

ChangeDataDialog::~ChangeDataDialog() { delete ui; }

QString ChangeDataDialog::getNewLogin() const { return ui->editNewLogin->text().trimmed(); }
QString ChangeDataDialog::getNewEmail() const { return ui->editNewEmail->text().trimmed(); }
QString ChangeDataDialog::getNewPassword() const { return ui->editNewPassword->text(); }

void ChangeDataDialog::on_btnSave_clicked() {
    // Если все поля пустые, то и менять нечего
    if (getNewLogin().isEmpty() && getNewEmail().isEmpty() && getNewPassword().isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Вы не ввели ни одного нового параметра для изменения!");
        return;
    }

    // Проверка пароля (если он введен). Допустим, ограничение: минимум 6 символов
    if (!getNewPassword().isEmpty() && getNewPassword().length() < 6) {
        QMessageBox::warning(this, "Внимание", "Новый пароль должен быть не менее 6 символов!");
        return;
    }

    // Проверка почты регулярным выражением (если введена)
    if (!getNewEmail().isEmpty()) {
        QRegularExpression emailRegExp("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        if (!emailRegExp.match(getNewEmail()).hasMatch()) {
            QMessageBox::warning(this, "Внимание", "Неверный формат новой почты!");
            return;
        }
    }

    // Запрашиваем подтверждение, как ты и просил
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение",
        "Вы действительно хотите изменить свои регистрационные данные?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        accept();
    }
}

void ChangeDataDialog::on_btnCancel_clicked() { reject(); }