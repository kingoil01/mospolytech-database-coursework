#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product {
public:
    Product() : m_id(0), m_basePrice(0.0) {}
    Product(int id, const QString &name, double basePrice)
        : m_id(id), m_name(name), m_basePrice(basePrice) {}

    int id() const { return m_id; }
    QString name() const { return m_name; }
    double basePrice() const { return m_basePrice; }

    QString getDisplayText() const {
        return QString("%1 — %2 руб.").arg(m_name).arg(m_basePrice);
    }

private:
    int m_id;
    QString m_name;
    double m_basePrice;
};

#endif // PRODUCT_H