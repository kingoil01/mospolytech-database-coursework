#ifndef ADMINMODEL_H
#define ADMINMODEL_H

#include <QObject>
#include <QVector>
#include "adminuser.h"
#include "role.h"

class AdminModel : public QObject {
    Q_OBJECT
public:
    explicit AdminModel(QObject *parent = nullptr);

    // Получение списка всех пользователей
    QVector<AdminUser> getAllUsers();

    // Создание пользователя
    bool createUser(const QString &login, const QString &email,
                    const QString &password, bool isAdmin);

    // Удаление пользователя
    bool deleteUser(int userId);

    // Получение доступных ролей
    QVector<Role> getAvailableRoles();

    // Проверка существования пользователя
    bool userExists(int userId);

    QString getLastError() const;

private:
    QString m_lastError;
};

#endif // ADMINMODEL_H