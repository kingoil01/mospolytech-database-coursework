#ifndef MANAGERMODEL_H
#define MANAGERMODEL_H

#include <QObject>
#include <QVector>
#include <QDate>

struct ManagerOrderInfo {
    int id;
    QString docNumber;
    QDate orderDate;
    QString organization;
    double totalAmount;
};

struct ManagerOrderDetail {
    QString productName;
    QString deliveryMethod;
    int quantity;
    double price;
    double totalPrice;
};

class ManagerModel : public QObject {
    Q_OBJECT
public:
    explicit ManagerModel(QObject *parent = nullptr);

    QVector<ManagerOrderInfo> getAllOrders();
    QVector<ManagerOrderDetail> getOrderDetails(int orderId);

    QString getLastError() const;

private:
    QString m_lastError;
};

#endif // MANAGERMODEL_H