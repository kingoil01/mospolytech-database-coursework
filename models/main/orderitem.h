#ifndef ORDERITEM_H
#define ORDERITEM_H

class OrderItem {
public:
    OrderItem() : m_orderId(0), m_deliveryOptionId(0), m_quantity(0) {}
    OrderItem(int orderId, int deliveryOptionId, int quantity)
        : m_orderId(orderId), m_deliveryOptionId(deliveryOptionId), m_quantity(quantity) {}

    int orderId() const { return m_orderId; }
    int deliveryOptionId() const { return m_deliveryOptionId; }
    int quantity() const { return m_quantity; }

private:
    int m_orderId;
    int m_deliveryOptionId;
    int m_quantity;
};

#endif // ORDERITEM_H