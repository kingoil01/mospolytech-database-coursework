#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QString>
#include <QDate>

struct CartItem {
    int deliveryOptionId;
    int productId;
    QString productName;
    QString deliveryMethodName;
    int quantity;
};

struct DeliveryOption {
    int id;
    QString methodName;
};

struct OrderInfo {
    int id;
    QString docNumber;
    QDate orderDate;
    double totalAmount;
};

struct OrderItemInfo {
    QString productName;
    QString deliveryMethodName;
    int quantity;
};

class MainModel : public QObject {
    Q_OBJECT
public:
    explicit MainModel(QObject *parent = nullptr);

    // Работа с каталогом товаров
    QMap<QString, int> getProductCatalog();
    QVector<DeliveryOption> getDeliveryOptionsForProduct(int productId);
    QString getProductName(int productId);

    // Работа с корзиной
    QVector<CartItem> getCart() const;
    void addToCart(int productId, const QString &productName,
                   int deliveryOptionId, const QString &deliveryMethodName,
                   int quantity);
    void clearCart();
    bool isCartEmpty() const;
    int getCartTotalItems() const;

    // Оформление заказа
    bool placeOrder(int userId, QString &outDocNumber);

    // История заказов
    QVector<OrderInfo> getOrderHistory(int userId);

    QVector<OrderItemInfo> getOrderItems(int orderId);

    QString getLastError() const;

private:
    QVector<CartItem> m_cart;
    QString m_lastError;
};

#endif // MAINMODEL_H