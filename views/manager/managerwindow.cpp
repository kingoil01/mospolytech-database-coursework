#include "managerwindow.h"
#include "ui_managerwindow.h"
#include <QHeaderView>

ManagerWindow::ManagerWindow(const User &user, ManagerModel *model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ManagerWindow)
    , m_model(model)
    , m_user(user) {
    ui->setupUi(this);

    setWindowTitle(QString("Панель менеджера - %1").arg(m_user.login()));

    // Настраиваем дерево
    ui->treeOrders->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeOrders->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeOrders->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeOrders->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    loadOrders();
}

ManagerWindow::~ManagerWindow() {
    delete ui;
}

void ManagerWindow::loadOrders() {
    ui->treeOrders->clear();

    QVector<ManagerOrderInfo> orders = m_model->getAllOrders();

    if (orders.isEmpty()) {
        return;
    }

    for (const auto &order : orders) {
        // Родительский элемент - заказ
        QTreeWidgetItem *orderRoot = new QTreeWidgetItem(ui->treeOrders);
        orderRoot->setText(0, order.docNumber);
        orderRoot->setText(1, order.orderDate.toString("dd.MM.yyyy"));
        orderRoot->setText(2, order.organization);
        orderRoot->setText(3, QString::number(order.totalAmount, 'f', 2) + " руб.");

        // Жирный шрифт для заказа
        QFont rootFont = orderRoot->font(0);
        rootFont.setBold(true);
        orderRoot->setFont(0, rootFont);
        orderRoot->setFont(1, rootFont);
        orderRoot->setFont(2, rootFont);
        orderRoot->setFont(3, rootFont);

        // Дочерние элементы - товары в заказе
        QVector<ManagerOrderDetail> details = m_model->getOrderDetails(order.id);

        for (const auto &detail : details) {
            QTreeWidgetItem *childItem = new QTreeWidgetItem(orderRoot);
            childItem->setText(0, detail.productName);
            childItem->setText(1, "[" + detail.deliveryMethod + "]");
            childItem->setText(2, QString("%1 шт.").arg(detail.quantity));
            childItem->setText(3, QString::number(detail.totalPrice, 'f', 2) + " руб.");
        }

        // Автоматически раскрываем заказ
        orderRoot->setExpanded(true);
    }
}

void ManagerWindow::on_btnRefresh_clicked() {
    loadOrders();
}

void ManagerWindow::on_btnLogout_clicked() {
    emit logoutRequested();
}