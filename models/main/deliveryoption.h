#ifndef DELIVERYOPTION_H
#define DELIVERYOPTION_H

#include <QString>
#include "deliverymethod.h"

class DeliveryOption {
public:
    DeliveryOption() : m_id(0), m_productId(0) {}
    DeliveryOption(int id, int productId, const DeliveryMethod &method)
        : m_id(id), m_productId(productId), m_method(method) {}

    int id() const { return m_id; }
    int productId() const { return m_productId; }
    DeliveryMethod method() const { return m_method; }

    QString getDisplayName() const {
        return m_method.name();
    }

private:
    int m_id;
    int m_productId;
    DeliveryMethod m_method;
};

#endif // DELIVERYOPTION_H