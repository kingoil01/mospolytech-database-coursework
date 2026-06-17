#ifndef ORDERHISTORYDIALOG_H
#define ORDERHISTORYDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui { class OrderHistoryDialog; }

class OrderHistoryDialog : public QDialog {
    Q_OBJECT
public:
    explicit OrderHistoryDialog(int idUser, QWidget *parent = nullptr);
    ~OrderHistoryDialog();

private slots:
    void on_btnClose_clicked();

private:
    void loadOrderHistory();
    Ui::OrderHistoryDialog *ui;
    int m_idUser;
};

#endif // ORDERHISTORYDIALOG_H