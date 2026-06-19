#ifndef ADMINMODEL_H
#define ADMINMODEL_H

#include <QObject>
#include <QVector>

struct AdminUserInfo {
    int id;
    QString login;
    QString email;
    QString roles;          // Строка с ролями через запятую
    bool isAdmin;           // Есть ли роль админа
    bool hasCustomerProfile; // Есть ли профиль клиента
};

struct RoleInfo {
    int id;
    QString name;
};

class AdminModel : public QObject {
    Q_OBJECT
public:
    explicit AdminModel(QObject *parent = nullptr);

    // Получение списка всех пользователей (кроме удаленных)
    QVector<AdminUserInfo> getAllUsers();

    // Создание пользователя
    bool createUser(const QString &login, const QString &email,
                    const QString &password, bool isAdmin);

    // Удаление пользователя (с установкой NULL в customers)
    bool deleteUser(int userId);

    // Получение доступных ролей
    QVector<RoleInfo> getAvailableRoles();

    // Проверка существования пользователя
    bool userExists(int userId);

    QString getLastError() const;

private:
    QString m_lastError;
};

#endif // ADMINMODEL_H