#include "managermodel.h"
#include "../../models/databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ManagerModel::ManagerModel(QObject *parent) : QObject(parent) {}

QVector<ManagerOrderInfo> ManagerModel::getAllOrders() {
    QVector<ManagerOrderInfo> orders;

    QSqlQuery query;
    query.exec(
        "SELECT o.id_order, o.doc_number, o.order_date, "
        "c.org_name, "
        "COALESCE(SUM(oi.quantity * p.base_price), 0) as total "
        "FROM orders o "
        "INNER JOIN customers c ON o.id_customer = c.id_customer "
        "INNER JOIN order_items oi ON o.id_order = oi.id_order "
        "INNER JOIN delivery_options d ON oi.id_delivery_option = d.id_delivery_option "
        "INNER JOIN products p ON d.id_product = p.id_product "
        "GROUP BY o.id_order, o.doc_number, o.order_date, c.org_name "
        "ORDER BY o.order_date DESC"
    );

    if (!query.lastError().isValid() && query.isActive()) {
        while (query.next()) {
            ManagerOrderInfo info;
            info.id = query.value(0).toInt();
            info.docNumber = query.value(1).toString();
            info.orderDate = query.value(2).toDate();
            info.organization = query.value(3).toString();
            info.totalAmount = query.value(4).toDouble();
            orders.append(info);
        }
    } else {
        m_lastError = query.lastError().text();
        qDebug() << "Ошибка получения заказов:" << m_lastError;
    }

    return orders;
}

QVector<ManagerOrderDetail> ManagerModel::getOrderDetails(int orderId) {
    QVector<ManagerOrderDetail> details;

    QSqlQuery query;
    query.prepare(
        "SELECT p.product_name, dm.method_name, oi.quantity, p.base_price "
        "FROM order_items oi "
        "INNER JOIN delivery_options d ON oi.id_delivery_option = d.id_delivery_option "
        "INNER JOIN products p ON d.id_product = p.id_product "
        "INNER JOIN delivery_methods dm ON d.id_delivery_method = dm.id_delivery_method "
        "WHERE oi.id_order = :orderId"
    );
    query.bindValue(":orderId", orderId);

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        qDebug() << "Ошибка получения деталей заказа:" << m_lastError;
        return details;
    }

    while (query.next()) {
        ManagerOrderDetail detail;
        detail.productName = query.value(0).toString();
        detail.deliveryMethod = query.value(1).toString();
        detail.quantity = query.value(2).toInt();
        detail.price = query.value(3).toDouble();
        detail.totalPrice = detail.quantity * detail.price;
        details.append(detail);
    }

    return details;
}

QString ManagerModel::getLastError() const {
    return m_lastError;
}