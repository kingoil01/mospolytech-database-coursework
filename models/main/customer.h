#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QString>

class Customer {
public:
    Customer() : m_id(0), m_userId(0) {}
    Customer(int id, int userId, const QString &organization,
             const QString &address, const QString &phone, const QString &contactPerson)
        : m_id(id), m_userId(userId), m_organization(organization),
          m_address(address), m_phone(phone), m_contactPerson(contactPerson) {}

    int id() const { return m_id; }
    int userId() const { return m_userId; }
    QString organization() const { return m_organization; }
    QString address() const { return m_address; }
    QString phone() const { return m_phone; }
    QString contactPerson() const { return m_contactPerson; }

private:
    int m_id;
    int m_userId;
    QString m_organization;
    QString m_address;
    QString m_phone;
    QString m_contactPerson;
};

#endif // CUSTOMER_H