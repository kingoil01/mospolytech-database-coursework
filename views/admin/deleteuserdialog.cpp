#include "deleteuserdialog.h"
#include "ui_deleteuserdialog.h"
#include <QMessageBox>

DeleteUserDialog::DeleteUserDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteUserDialog) {
    ui->setupUi(this);

    // Изначально кнопка удаления неактивна
    ui->btnDelete->setEnabled(false);
}

DeleteUserDialog::~DeleteUserDialog() {
    delete ui;
}

void DeleteUserDialog::setUserList(const QVector<UserListItem> &users) {
    ui->listUsers->clear();

    for (const auto &user : users) {
        QString displayText = QString("%1 (%2)").arg(user.login).arg(user.email);
        if (user.isAdmin) {
            displayText += " [АДМИН]";
        }
        QListWidgetItem *item = new QListWidgetItem(displayText, ui->listUsers);
        item->setData(Qt::UserRole, user.id);
    }
}

int DeleteUserDialog::getSelectedUserId() const {
    QListWidgetItem *item = ui->listUsers->currentItem();
    if (!item) {
        return -1;
    }
    return item->data(Qt::UserRole).toInt();
}

void DeleteUserDialog::clearSelection() {
    ui->listUsers->clearSelection();
    ui->btnDelete->setEnabled(false);
}

void DeleteUserDialog::showError(const QString &message) {
    QMessageBox::critical(this, "Ошибка", message);
}

void DeleteUserDialog::showWarning(const QString &message) {
    QMessageBox::warning(this, "Внимание", message);
}

bool DeleteUserDialog::askConfirmation(const QString &message) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Подтверждение",
        message,
        QMessageBox::Yes | QMessageBox::No
    );
    return reply == QMessageBox::Yes;
}

void DeleteUserDialog::on_btnDelete_clicked() {
    int userId = getSelectedUserId();
    if (userId == -1) {
        showWarning("Пожалуйста, выберите пользователя для удаления.");
        return;
    }

    // Запрашиваем подтверждение
    if (askConfirmation("Вы действительно хотите удалить выбранного пользователя?\n"
                        "Его профиль клиента будет сохранен без привязки к пользователю.")) {
        accept();
    }
}

void DeleteUserDialog::on_btnCancel_clicked() {
    reject();
}

void DeleteUserDialog::on_listUsers_itemSelectionChanged() {
    // Активируем кнопку удаления только если выбран пользователь
    ui->btnDelete->setEnabled(ui->listUsers->currentItem() != nullptr);
}