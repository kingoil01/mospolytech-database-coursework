#include "databasemanager.h"
#include <QDebug>

// Конструктор инициализирует драйвер PostgreSQL
DatabaseManager::DatabaseManager() {
    // Создаем дефолтное подключение приложения к СУБД
    m_db = QSqlDatabase::addDatabase("QPSQL");
}

// Деструктор гарантирует закрытие сессии при выходе из программы
DatabaseManager::~DatabaseManager() {
    disconnectFromDb();
}

// Реализация глобальной точки доступа
DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance; // Создастся ровно один раз при первом вызове
    return instance;
}

// Метод для открытия сессии с переданными параметрами
bool DatabaseManager::connectToDb(const QString& host, int port, const QString& dbName, const QString& user, const QString& pass) {
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(pass);

    if (!m_db.open()) {
        qDebug() << "Критическая ошибка подключения к PostgreSQL:" << m_db.lastError().text();
        return false;
    }

    qDebug() << "Успешное Singleton-подключение к базе данных:" << dbName;
    return true;
}

// Метод принудительного закрытия соединения
void DatabaseManager::disconnectFromDb() {
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "Подключение к БД успешно закрыто.";
    }
}

bool DatabaseManager::isOpen() const {
    return m_db.isOpen();
}