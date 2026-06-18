#include "orderhistorydialog.h"
#include "ui_orderhistorydialog.h"
#include <QMessageBox>
#include <QTreeWidgetItem>

OrderHistoryDialog::OrderHistoryDialog(int userId, MainModel *model, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrderHistoryDialog)
    , m_model(model)
    , m_userId(userId) {
    ui->setupUi(this);
    loadHistory();
}

OrderHistoryDialog::~OrderHistoryDialog() {
    delete ui;
}

void OrderHistoryDialog::loadHistory() {
    QVector<OrderInfo> orders = m_model->getOrderHistory(m_userId);

    ui->treeOrders->clear();
    ui->treeOrders->setHeaderLabels({"ID", "Номер", "Дата", "Сумма"});

    for (const auto &order : orders) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeOrders);
        item->setText(0, QString::number(order.id));
        item->setText(1, order.docNumber);
        item->setText(2, order.orderDate.toString("dd.MM.yyyy"));
        item->setText(3, QString::number(order.totalAmount, 'f', 2) + " руб.");
    }
}