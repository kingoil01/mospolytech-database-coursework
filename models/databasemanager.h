#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

class DatabaseManager {
private:
    DatabaseManager();
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;

public:
    static DatabaseManager& instance();

    // Методы управления подключением
    bool connectToDb(const QString& host, int port, const QString& dbName, const QString& user, const QString& pass);
    void disconnectFromDb();

    // Метод проверяет, открыто ли соединение сейчас
    bool isOpen() const;
};

#endif // DATABASEMANAGER_H