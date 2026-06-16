#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

class DatabaseManager {
private:
    // 1. Приватный конструктор и деструктор (запрещаем создание объектов извне)
    DatabaseManager();
    ~DatabaseManager();

    // 2. Удаляем конструктор копирования и оператор присваивания (запрещаем клонирование)
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db; // Единый объект подключения Qt к СУБД

public:
    // 3. Единственная точка доступа к экземпляру класса
    static DatabaseManager& instance();

    // Методы управления подключением
    bool connectToDb(const QString& host, int port, const QString& dbName, const QString& user, const QString& pass);
    void disconnectFromDb();

    // Вспомогательный метод: проверяет, открыто ли соединение сейчас
    bool isOpen() const;
};

#endif // DATABASEMANAGER_H