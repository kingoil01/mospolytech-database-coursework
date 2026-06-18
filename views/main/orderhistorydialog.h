#ifndef ORDERHISTORYDIALOG_H
#define ORDERHISTORYDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "../../models/main/mainmodel.h"

namespace Ui { class OrderHistoryDialog; }

class OrderHistoryDialog : public QDialog {
    Q_OBJECT
public:
    explicit OrderHistoryDialog(int userId, MainModel *model, QWidget *parent = nullptr);
    ~OrderHistoryDialog();

private slots:
    void on_btnClose_clicked();

private:
    void loadHistory();

    Ui::OrderHistoryDialog *ui;
    MainModel *m_model;
    int m_userId;
};

#endif // ORDERHISTORYDIALOG_H