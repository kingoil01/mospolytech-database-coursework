#include "orderhistorydialog.h"
#include "ui_orderhistorydialog.h"
#include <QHeaderView>
#include <QMessageBox>

OrderHistoryDialog::OrderHistoryDialog(int userId, MainModel *model, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrderHistoryDialog)
    , m_model(model)
    , m_userId(userId) {
    ui->setupUi(this);

    ui->treeOrders->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeOrders->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeOrders->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    loadHistory();
}

OrderHistoryDialog::~OrderHistoryDialog() {
    delete ui;
}

void OrderHistoryDialog::loadHistory() {
    ui->treeOrders->clear();

    // Получаем список заказов из модели
    QVector<OrderInfo> orders = m_model->getOrderHistory(m_userId);

    if (orders.isEmpty()) {
        return;
    }

    // Для каждого заказа создаем элемент дерева
    for (const auto &order : orders) {
        QTreeWidgetItem *orderRoot = new QTreeWidgetItem(ui->treeOrders);
        orderRoot->setText(0, order.docNumber);
        orderRoot->setText(1, order.orderDate.toString("dd.MM.yyyy"));
        orderRoot->setText(2, QString::number(order.totalAmount, 'f', 2) + " руб.");

        QFont rootFont = orderRoot->font(0);
        rootFont.setBold(true);
        orderRoot->setFont(0, rootFont);
        orderRoot->setFont(1, rootFont);
        orderRoot->setFont(2, rootFont);

        // Получаем позиции заказа из модели
        QVector<OrderItemInfo> items = m_model->getOrderItems(order.id);

        for (const auto &item : items) {
            QTreeWidgetItem *childItem = new QTreeWidgetItem(orderRoot);
            childItem->setText(0, item.productName);
            childItem->setText(1, "[" + item.deliveryMethodName + "]");
            childItem->setText(2, QString("%1 шт.").arg(item.quantity));
        }

        orderRoot->setExpanded(true);
    }
}

void OrderHistoryDialog::on_btnClose_clicked() {
    accept();
}