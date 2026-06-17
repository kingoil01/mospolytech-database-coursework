#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QDialog>
#include <QMap>
#include <QVector>
#include "../../models/authmodel.h"

namespace Ui { class OrderWindow; }

// Структура позиции в корзине в соответствии с вашей СУБД
struct CartItem {
    int idDeliveryOption;      // ID из таблицы delivery_options
    int productId;             // ID товара
    QString productName;       // Название для вывода на экран
    QString deliveryMethodName;// Название доставки для вывода на экран
    int quantity;              // Количество
};

class OrderWindow : public QDialog {
    Q_OBJECT
public:
    explicit OrderWindow(int idUser, AuthModel *model, QWidget *parent = nullptr);
    ~OrderWindow();

private slots:
    void on_btnAddToCart_clicked();
    void on_btnClearCart_clicked();
    void on_btnConfirmOrder_clicked();
    void on_listCatalog_itemDoubleClicked(class QListWidgetItem *item);
    void on_btnClose_clicked();

private:
    void loadCatalog();
    void showDeliveryAndQuantityDialog(int productId, const QString &fullProductText);
    void refreshCartUI();

    Ui::OrderWindow *ui;
    AuthModel *m_model;
    int m_idUser;

    QMap<QString, int> m_productMap; // Текст строки -> id_product
    QVector<CartItem> m_cart;         // Корзина
};

#endif // ORDERWINDOW_H