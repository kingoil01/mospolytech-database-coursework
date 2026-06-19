#ifndef CUSTOMERINFODIALOG_H
#define CUSTOMERINFODIALOG_H

#include <QDialog>

namespace Ui { class CustomerInfoDialog; }

class CustomerInfoDialog : public QDialog {
    Q_OBJECT
public:
    explicit CustomerInfoDialog(QWidget *parent = nullptr);
    void setInitialData(const QString &org, const QString &addr, const QString &phone, const QString &cp);

    ~CustomerInfoDialog();

    QString getOrgName() const;
    QString getAddress() const;
    QString getPhone() const;
    QString getContactPerson() const;

private slots:
    void on_btnSave_clicked();
    void on_btnCancel_clicked();

private:
    Ui::CustomerInfoDialog *ui;
};

#endif // CUSTOMERINFODIALOG_H