#include <QApplication>
#include <QMessageBox>
#include "models/databasemanager.h" // Твой базовый класс-синглтон подключения к БД
#include "controllers/authcontroller.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Подключаемся к БД
    DatabaseManager &db = DatabaseManager::instance();
    if (!DatabaseManager::instance().connectToDb("localhost", 5432, "company", "postgres", "Adminka7")) {
        QMessageBox::critical(nullptr, "Ошибка БД", "Нет соединения с сервером PostgreSQL!");
        return -1;
    }

    // Запускаем контроллер авторизации
    AuthController controller;
    controller.start();

    int result = a.exec();

    // Отключаемся от БД
    db.disconnectFromDb();

    return result;
}