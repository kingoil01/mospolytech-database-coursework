#include "mainmodel.h"
#include "../../models/databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

MainModel::MainModel(QObject *parent) : QObject(parent) {}

QMap<QString, int> MainModel::getProductCatalog() {
    QMap<QString, int> productMap;

    QSqlQuery query("SELECT id_product, product_name, base_price FROM products");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        double price = query.value(2).toDouble();

        QString itemText = QString("%1 — %2 руб.").arg(name).arg(price);
        productMap[itemText] = id;
    }

    return productMap;
}

QVector<DeliveryOption> MainModel::getDeliveryOptionsForProduct(int productId) {
    QVector<DeliveryOption> options;

    QSqlQuery query;
    QString sql = QString(
        "SELECT d_opt.id_delivery_option, d_meth.method_name "
        "FROM delivery_options d_opt "
        "INNER JOIN delivery_methods d_meth ON d_opt.id_delivery_method = d_meth.id_delivery_method "
        "WHERE d_opt.id_product = %1"
    ).arg(productId);

    if (!query.exec(sql)) {
        qDebug() << "Ошибка получения вариантов доставки:" << query.lastError().text();
        m_lastError = query.lastError().text();
        return options;
    }

    while (query.next()) {
        DeliveryOption opt;
        opt.id = query.value(0).toInt();
        opt.methodName = query.value(1).toString();
        options.append(opt);
    }

    return options;
}

QString MainModel::getProductName(int productId) {
    QSqlQuery query;
    query.prepare("SELECT product_name FROM products WHERE id_product = :id");
    query.bindValue(":id", productId);

    if (!query.exec() || !query.next()) {
        return QString();
    }

    return query.value(0).toString();
}

QVector<CartItem> MainModel::getCart() const {
    return m_cart;
}

void MainModel::addToCart(int productId, const QString &productName,
                          int deliveryOptionId, const QString &deliveryMethodName,
                          int quantity) {
    bool found = false;
    for (int i = 0; i < m_cart.size(); ++i) {
        if (m_cart[i].deliveryOptionId == deliveryOptionId) {
            m_cart[i].quantity += quantity;
            found = true;
            break;
        }
    }

    if (!found) {
        CartItem item;
        item.deliveryOptionId = deliveryOptionId;
        item.productId = productId;
        item.productName = productName;
        item.deliveryMethodName = deliveryMethodName;
        item.quantity = quantity;
        m_cart.append(item);
    }
}

void MainModel::clearCart() {
    m_cart.clear();
}

bool MainModel::isCartEmpty() const {
    return m_cart.isEmpty();
}

int MainModel::getCartTotalItems() const {
    int total = 0;
    for (const auto &item : m_cart) {
        total += item.quantity;
    }
    return total;
}

bool MainModel::placeOrder(int userId, QString &outDocNumber) {
    if (m_cart.isEmpty()) {
        m_lastError = "Корзина пуста";
        return false;
    }

    QSqlQuery query;
    query.exec("BEGIN;");

    // 1. Ищем id_customer по нашему id_user
    query.prepare("SELECT id_customer FROM customers WHERE id_user = :id_user");
    query.bindValue(":id_user", userId);
    if (!query.exec() || !query.next()) {
        query.exec("ROLLBACK;");
        m_lastError = "Профиль организации заказчика не найден.";
        return false;
    }
    int idCustomer = query.value(0).toInt();

    // 2. Генерируем уникальный номер документа
    QString docNumber = QString("ЗАКАЗ-%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss"));
    QDate orderDate = QDate::currentDate();

    query.prepare("INSERT INTO orders (id_customer, doc_number, order_date) "
                  "VALUES (:id_customer, :doc_number, :order_date) RETURNING id_order");
    query.bindValue(":id_customer", idCustomer);
    query.bindValue(":doc_number", docNumber);
    query.bindValue(":order_date", orderDate);

    if (!query.exec() || !query.next()) {
        query.exec("ROLLBACK;");
        m_lastError = "Не удалось создать заказ: " + query.lastError().text();
        return false;
    }
    int idOrder = query.value(0).toInt();

    // 3. Переносим позиции корзины в order_items
    for (const auto &item : m_cart) {
        query.prepare("INSERT INTO order_items (id_order, id_delivery_option, quantity) "
                      "VALUES (:id_order, :id_delivery_option, :quantity)");
        query.bindValue(":id_order", idOrder);
        query.bindValue(":id_delivery_option", item.deliveryOptionId);
        query.bindValue(":quantity", item.quantity);

        if (!query.exec()) {
            query.exec("ROLLBACK;");
            m_lastError = "Ошибка добавления позиций: " + query.lastError().text();
            return false;
        }
    }

    query.exec("COMMIT;");
    outDocNumber = docNumber;
    m_cart.clear();
    return true;
}

QVector<OrderInfo> MainModel::getOrderHistory(int userId) {
    QVector<OrderInfo> orders;

    QSqlQuery query;
    query.prepare(
        "SELECT o.id_order, o.doc_number, o.order_date, "
        "COALESCE(SUM(oi.quantity * p.base_price), 0) as total "
        "FROM orders o "
        "INNER JOIN customers c ON o.id_customer = c.id_customer "
        "INNER JOIN order_items oi ON o.id_order = oi.id_order "
        "INNER JOIN delivery_options d ON oi.id_delivery_option = d.id_delivery_option "
        "INNER JOIN products p ON d.id_product = p.id_product "
        "WHERE c.id_user = :userId "
        "GROUP BY o.id_order, o.doc_number, o.order_date "
        "ORDER BY o.order_date DESC"
    );
    query.bindValue(":userId", userId);

    if (!query.exec()) {
        qDebug() << "Ошибка получения истории заказов:" << query.lastError().text();
        return orders;
    }

    while (query.next()) {
        OrderInfo order;
        order.id = query.value(0).toInt();
        order.docNumber = query.value(1).toString();
        order.orderDate = query.value(2).toDate();
        order.totalAmount = query.value(3).toDouble();
        orders.append(order);
    }

    return orders;
}

QString MainModel::getLastError() const {
    return m_lastError;
}