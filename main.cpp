#include <QApplication>
#include <QMessageBox>
#include "models/databasemanager.h" // Твой базовый класс-синглтон подключения к БД
#include "controllers/authcontroller.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Подставляем параметры подключения к твоей PostgreSQL компании
    if (!DatabaseManager::instance().connectToDb("localhost", 5432, "company", "postgres", "Adminka7")) {
        QMessageBox::critical(nullptr, "Ошибка БД", "Нет соединения с сервером PostgreSQL!");
        return -1;
    }

    AuthController controller;
    controller.start(); // Запуск бизнес-логики окон авторизации

    return a.exec();
}