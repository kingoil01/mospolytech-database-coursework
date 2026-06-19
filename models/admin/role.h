#ifndef ROLE_H
#define ROLE_H

#include <QString>

class Role {
public:
    Role() : m_id(0) {}
    Role(int id, const QString &name)
        : m_id(id), m_name(name) {}

    int id() const { return m_id; }
    QString name() const { return m_name; }

    void setId(int id) { m_id = id; }
    void setName(const QString &name) { m_name = name; }

    QString getDisplayText() const {
        return m_name;
    }

private:
    int m_id;
    QString m_name;
};

#endif // ROLE_H