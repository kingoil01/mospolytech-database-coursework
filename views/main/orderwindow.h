#ifndef ORDERWINDOW_H
#define ORDERWINDOW_H

#include <QDialog>
#include <QMap>
#include <QListWidgetItem>  // <-- ДОБАВЛЯЕМ
#include "../../models/main/mainmodel.h"

namespace Ui { class OrderWindow; }

class OrderWindow : public QDialog {
    Q_OBJECT
public:
    explicit OrderWindow(int userId, MainModel *model, QWidget *parent = nullptr);
    ~OrderWindow();

private slots:
    void on_btnAddToCart_clicked();
    void on_btnClearCart_clicked();
    void on_btnConfirmOrder_clicked();
    void on_listCatalog_itemDoubleClicked(QListWidgetItem *item);
    void on_btnClose_clicked();

private:
    void loadCatalog();
    void showDeliveryAndQuantityDialog(int productId, const QString &fullProductText);
    void refreshCartUI();
    void showMessage(const QString &title, const QString &message, bool isError = false);

    Ui::OrderWindow *ui;
    MainModel *m_model;
    int m_userId;
    QMap<QString, int> m_productMap;
};

#endif // ORDERWINDOW_H