#ifndef DELIVERYMETHOD_H
#define DELIVERYMETHOD_H

#include <QString>

class DeliveryMethod {
public:
    DeliveryMethod() : m_id(0) {}
    DeliveryMethod(int id, const QString &name)
        : m_id(id), m_name(name) {}

    int id() const { return m_id; }
    QString name() const { return m_name; }

private:
    int m_id;
    QString m_name;
};

#endif // DELIVERYMETHOD_H