#include "orderwindow.h"
#include "ui_orderwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>

OrderWindow::OrderWindow(int userId, MainModel *model, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrderWindow)
    , m_model(model)
    , m_userId(userId) {
    ui->setupUi(this);
    loadCatalog();
}

OrderWindow::~OrderWindow() {
    delete ui;
}

void OrderWindow::showMessage(const QString &title, const QString &message, bool isError) {
    if (isError) {
        QMessageBox::critical(this, title, message);
    } else {
        QMessageBox::information(this, title, message);
    }
}

void OrderWindow::loadCatalog() {
    ui->listCatalog->clear();
    m_productMap.clear();

    QMap<QString, int> catalog = m_model->getProductCatalog();
    for (const QString &itemText : catalog.keys()) {
        ui->listCatalog->addItem(itemText);
        m_productMap[itemText] = catalog[itemText];
    }
}

void OrderWindow::on_btnAddToCart_clicked() {
    if (auto *item = ui->listCatalog->currentItem()) {
        on_listCatalog_itemDoubleClicked(item);
    }
}

void OrderWindow::on_listCatalog_itemDoubleClicked(QListWidgetItem *item) {
    int productId = m_productMap[item->text()];
    showDeliveryAndQuantityDialog(productId, item->text());
}

void OrderWindow::showDeliveryAndQuantityDialog(int productId, const QString &fullProductText) {
    QString productName = fullProductText.split(" — ")[0];

    QVector<DeliveryOption> options = m_model->getDeliveryOptionsForProduct(productId);

    if (options.isEmpty()) {
        showMessage("Внимание",
            QString("Для товара \"%1\" нет доступных вариантов доставки!").arg(productName));
        return;
    }

    QDialog popUp(this);
    popUp.setWindowTitle("Параметры позиции");
    QFormLayout layout(&popUp);

    QLabel *infoLabel = new QLabel(QString("Товар: <b>%1</b>").arg(productName), &popUp);
    layout.addRow(infoLabel);

    QComboBox *comboDelivery = new QComboBox(&popUp);
    for (const auto &opt : options) {
        comboDelivery->addItem(opt.methodName, opt.id);
    }
    layout.addRow("Способ доставки:", comboDelivery);

    QSpinBox *spinQty = new QSpinBox(&popUp);
    spinQty->setRange(1, 9999);
    spinQty->setValue(1);
    layout.addRow("Количество:", spinQty);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &popUp);
    connect(buttons, &QDialogButtonBox::accepted, &popUp, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &popUp, &QDialog::reject);
    layout.addRow(buttons);

    if (popUp.exec() == QDialog::Accepted) {
        int selectedOptionId = comboDelivery->currentData().toInt();
        QString selectedMethodName = comboDelivery->currentText();
        int quantity = spinQty->value();

        m_model->addToCart(productId, productName, selectedOptionId, selectedMethodName, quantity);
        refreshCartUI();
    }
}

void OrderWindow::refreshCartUI() {
    ui->listCart->clear();

    QVector<CartItem> cart = m_model->getCart();
    for (const auto &item : cart) {
        QString text = QString("%1 (%2) — %3 шт.")
            .arg(item.productName)
            .arg(item.deliveryMethodName)
            .arg(item.quantity);
        ui->listCart->addItem(text);
    }
}

void OrderWindow::on_btnClearCart_clicked() {
    m_model->clearCart();
    ui->listCart->clear();
}

void OrderWindow::on_btnConfirmOrder_clicked() {
    if (m_model->isCartEmpty()) {
        showMessage("Корзина пуста", "Добавьте товары в корзину!");
        return;
    }

    QString docNumber;
    if (m_model->placeOrder(m_userId, docNumber)) {
        showMessage("Успех", QString("Заказ %1 успешно оформлен!").arg(docNumber));
        accept();
    } else {
        showMessage("Ошибка", "Не удалось оформить заказ: " + m_model->getLastError(), true);
    }
}

void OrderWindow::on_btnClose_clicked() {
    reject();
}