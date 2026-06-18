#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QDate>
#include <QVector>
#include "orderitem.h"

class Order {
public:
    Order() : m_id(0), m_customerId(0) {}
    Order(int id, int customerId, const QString &docNumber, const QDate &orderDate)
        : m_id(id), m_customerId(customerId), m_docNumber(docNumber), m_orderDate(orderDate) {}

    int id() const { return m_id; }
    int customerId() const { return m_customerId; }
    QString docNumber() const { return m_docNumber; }
    QDate orderDate() const { return m_orderDate; }
    QVector<OrderItem> items() const { return m_items; }

    void addItem(const OrderItem &item) { m_items.append(item); }
    void setItems(const QVector<OrderItem> &items) { m_items = items; }

private:
    int m_id;
    int m_customerId;
    QString m_docNumber;
    QDate m_orderDate;
    QVector<OrderItem> m_items;
};

#endif // ORDER_H