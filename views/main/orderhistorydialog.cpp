#include "orderhistorydialog.h"
#include "ui_orderhistorydialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QDate>

OrderHistoryDialog::OrderHistoryDialog(int idUser, QWidget *parent)
    : QDialog(parent), ui(new Ui::OrderHistoryDialog), m_idUser(idUser) {
    ui->setupUi(this);

    // Настраиваем красивое растягивание колонок дерева
    ui->treeOrders->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeOrders->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeOrders->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    loadOrderHistory();
}

OrderHistoryDialog::~OrderHistoryDialog() { delete ui; }

void OrderHistoryDialog::loadOrderHistory() {
    ui->treeOrders->clear();

    // 1. Сначала находим id_customer для текущего id_user
    QSqlQuery query;
    query.prepare("SELECT id_customer FROM customers WHERE id_user = :id_user");
    query.bindValue(":id_user", m_idUser);

    if (!query.exec() || !query.next()) {
        return; // Если профиля заказчика нет, то и истории заказов быть не может
    }
    int idCustomer = query.value(0).toInt();

    // 2. Вытаскиваем все заказы этого клиента (помним про капризный "do", берем "d_opt")
    // Запрос собираем сразу в строку, чтобы QPSQL не ругался на подготовленные операторы
    QString ordersSql = QString(
        "SELECT id_order, doc_number, order_date FROM orders "
        "WHERE id_customer = %1 ORDER BY order_date DESC"
    ).arg(idCustomer);

    if (!query.exec(ordersSql)) {
        qDebug() << "Ошибка загрузки заказов:" << query.lastError().text();
        return;
    }

    // Буфер для хранения заказов, чтобы вторым запросом вытащить их состав
    struct OrderData { int id; QString doc; QString date; };
    QVector<OrderData> userOrders;
    while (query.next()) {
        userOrders.append({
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toDate().toString("dd.MM.yyyy")
        });
    }

    // 3. Для каждого заказа вытаскиваем его состав (товары и способы доставки)
    for (const auto &order : userOrders) {
        // Создаем родительский элемент дерева (сам Заказ)
        QTreeWidgetItem *orderRoot = new QTreeWidgetItem(ui->treeOrders);
        orderRoot->setText(0, order.doc);  // В первую колонку пишем Номер документа
        orderRoot->setText(1, order.date); // Во вторую — Дату

        // Делаем текст заказа жирным для визуального отличия
        QFont rootFont = orderRoot->font(0);
        rootFont.setBold(true);
        orderRoot->setFont(0, rootFont);
        orderRoot->setFont(1, rootFont);

        // Тянем состав этого конкретного заказа из order_items + delivery_options
        QString itemsSql = QString(
            "SELECT p.product_name, dm.method_name, oi.quantity "
            "FROM order_items oi "
            "INNER JOIN delivery_options d_opt ON oi.id_delivery_option = d_opt.id_delivery_option "
            "INNER JOIN products p ON d_opt.id_product = p.id_product "
            "INNER JOIN delivery_methods dm ON d_opt.id_delivery_method = dm.id_delivery_method "
            "WHERE oi.id_order = %1"
        ).arg(order.id);

        QSqlQuery itemQuery;
        if (itemQuery.exec(itemsSql)) {
            while (itemQuery.next()) {
                QString prodName = itemQuery.value(0).toString();
                QString delMethod = itemQuery.value(1).toString();
                int qty = itemQuery.value(2).toInt();

                // Создаем дочернюю строку (товар внутри заказа)
                QTreeWidgetItem *childItem = new QTreeWidgetItem(orderRoot);
                childItem->setText(0, prodName);             // Товар
                childItem->setText(1, "[" + delMethod + "]"); // Способ доставки
                childItem->setText(2, QString("%1 шт.").arg(qty)); // Количество
            }
        }

        // Автоматически раскрываем каждый заказ, чтобы пользователь сразу видел состав
        orderRoot->setExpanded(true);
    }
}

void OrderHistoryDialog::on_btnClose_clicked() {
    accept();
}