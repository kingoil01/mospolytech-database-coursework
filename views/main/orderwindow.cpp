#include "orderwindow.h"
#include "ui_orderwindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QLabel>

// Подключаем элементы для создания встроенного окна без .ui файла
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>

OrderWindow::OrderWindow(int idUser, AuthModel *model, QWidget *parent)
    : QDialog(parent), ui(new Ui::OrderWindow), m_model(model), m_idUser(idUser) {
    ui->setupUi(this);
    loadCatalog();
}

OrderWindow::~OrderWindow() { delete ui; }

void OrderWindow::loadCatalog() {
    ui->listCatalog->clear();
    m_productMap.clear();

    QSqlQuery query("SELECT id_product, product_name, base_price FROM products");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        double price = query.value(2).toDouble();

        QString itemText = QString("%1 — %2 руб.").arg(name).arg(price);
        ui->listCatalog->addItem(itemText);
        m_productMap[itemText] = id;
    }
}

void OrderWindow::on_btnAddToCart_clicked() {
    if (auto *item = ui->listCatalog->currentItem()) {
        on_listCatalog_itemDoubleClicked(item);
    }
}

void OrderWindow::on_listCatalog_itemDoubleClicked(QListWidgetItem *item) {
    int productId = m_productMap[item->text()];
    showDeliveryAndQuantityDialog(productId, item->text());
}

// Вызов окна "Вариант доставки + Количество" в одном месте
void OrderWindow::showDeliveryAndQuantityDialog(int productId, const QString &fullProductText) {
    QString productName = fullProductText.split(" — ")[0];

    QSqlQuery query;

    // Меняем алиасы с зарезервированного "do" на безопасный "d_opt"
    QString sql = QString(
        "SELECT d_opt.id_delivery_option, d_meth.method_name "
        "FROM delivery_options d_opt "
        "INNER JOIN delivery_methods d_meth ON d_opt.id_delivery_method = d_meth.id_delivery_method "
        "WHERE d_opt.id_product = %1"
    ).arg(productId);

    QVector<QPair<int, QString>> options;

    if (!query.exec(sql)) {
        qDebug() << "!!! Ошибка выполнения SELECT !!!";
        qDebug() << "Текст ошибки БД:" << query.lastError().text();
    } else {
        while (query.next()) {
            options.append({query.value(0).toInt(), query.value(1).toString()});
        }
    }

    if (options.isEmpty()) {
        QMessageBox::warning(this, "Внимание",
            QString("Для товара \"%1\" (ID: %2) нет доступных вариантов доставки!\n\n"
                    "Проверьте таблицы в БД.").arg(productName).arg(productId));
        return;
    }

    // ... дальше твой код формы (popUp, QComboBox, QSpinBox) без изменений

    // 2. Создаем компактное кастомное диалоговое окно кодом
    QDialog popUp(this);
    popUp.setWindowTitle("Параметры позиции");
    QFormLayout layout(&popUp);

    QLabel *infoLabel = new QLabel(QString("Товар: <b>%1</b>").arg(productName), &popUp);
    layout.addRow(infoLabel);

    QComboBox *comboDelivery = new QComboBox(&popUp);
    for (const auto &opt : options) {
        comboDelivery->addItem(opt.second, opt.first); // Отображаем имя, внутрь пишем id_delivery_option
    }
    layout.addRow("Способ доставки:", comboDelivery);

    QSpinBox *spinQty = new QSpinBox(&popUp);
    spinQty->setRange(1, 9999); // Защита CHECK (quantity > 0)
    spinQty->setValue(1);
    layout.addRow("Количество:", spinQty);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &popUp);
    connect(buttons, &QDialogButtonBox::accepted, &popUp, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &popUp, &QDialog::reject);
    layout.addRow(buttons);

    // 3. Если пользователь нажал ОК — обрабатываем данные
    if (popUp.exec() == QDialog::Accepted) {
        int selectedOptionId = comboDelivery->currentData().toInt();
        QString selectedMethodName = comboDelivery->currentText();
        int quantity = spinQty->value();

        // Проверяем, есть ли уже в корзине позиция с ТАКИМ ЖЕ id_delivery_option
        bool found = false;
        for (int i = 0; i < m_cart.size(); ++i) {
            if (m_cart[i].idDeliveryOption == selectedOptionId) {
                m_cart[i].quantity += quantity; // Плюсуем количество
                found = true;
                break;
            }
        }

        if (!found) {
            CartItem item;
            item.idDeliveryOption = selectedOptionId;
            item.productId = productId;
            item.productName = productName;
            item.deliveryMethodName = selectedMethodName;
            item.quantity = quantity;
            m_cart.append(item);
        }

        refreshCartUI();
    }
}

void OrderWindow::refreshCartUI() {
    ui->listCart->clear();
    for (const auto &item : m_cart) {
        QString text = QString("%1 (%2) — %3 шт.")
            .arg(item.productName)
            .arg(item.deliveryMethodName)
            .arg(item.quantity);
        ui->listCart->addItem(text);
    }
}

void OrderWindow::on_btnClearCart_clicked() {
    m_cart.clear();
    ui->listCart->clear();
}

void OrderWindow::on_btnConfirmOrder_clicked() {
    if (m_cart.isEmpty()) {
        QMessageBox::warning(this, "Корзина пуста", "Добавьте товары в корзину!");
        return;
    }

    QSqlQuery query;
    query.exec("BEGIN;"); // Запускаем транзакцию

    // 1. Ищем id_customer по нашему id_user
    query.prepare("SELECT id_customer FROM customers WHERE id_user = :id_user");
    query.bindValue(":id_user", m_idUser);
    if (!query.exec() || !query.next()) {
        query.exec("ROLLBACK;");
        QMessageBox::critical(this, "Ошибка", "Профиль организации заказчика не найден.");
        return;
    }
    int idCustomer = query.value(0).toInt();

    // 2. Генерируем уникальный номер документа (doc_number) и берем текущую дату
    QString docNumber = QString("ЗАКАЗ-%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));
    QDate orderDate = QDate::currentDate();

    // Вставляем запись в orders
    query.prepare("INSERT INTO orders (id_customer, doc_number, order_date) "
                  "VALUES (:id_customer, :doc_number, :order_date) RETURNING id_order");
    query.bindValue(":id_customer", idCustomer);
    query.bindValue(":doc_number", docNumber);
    query.bindValue(":order_date", orderDate);

    if (!query.exec() || !query.next()) {
        query.exec("ROLLBACK;");
        QMessageBox::critical(this, "Ошибка СУБД", "Не удалось создать заказ: " + query.lastError().text());
        return;
    }
    int idOrder = query.value(0).toInt();

    // 3. Переносим позиции корзины в order_items
    for (const auto &item : m_cart) {
        query.prepare("INSERT INTO order_items (id_order, id_delivery_option, quantity) "
                      "VALUES (:id_order, :id_delivery_option, :quantity)");
        query.bindValue(":id_order", idOrder);
        query.bindValue(":id_delivery_option", item.idDeliveryOption);
        query.bindValue(":quantity", item.quantity);

        if (!query.exec()) {
            query.exec("ROLLBACK;");
            QMessageBox::critical(this, "Ошибка СУБД", "Ошибка добавления позиций: " + query.lastError().text());
            return;
        }
    }

    query.exec("COMMIT;"); // Фиксируем транзакцию
    QMessageBox::information(this, "Успех", QString("Заказ %1 успешно оформлен!").arg(docNumber));
    on_btnClearCart_clicked();
    accept();
}

void OrderWindow::on_btnClose_clicked() {
    reject();
}