#ifndef CHANGEDATADIALOG_H
#define CHANGEDATADIALOG_H

#include <QDialog>

namespace Ui { class ChangeDataDialog; }

class ChangeDataDialog : public QDialog {
    Q_OBJECT
public:
    explicit ChangeDataDialog(QWidget *parent = nullptr);
    ~ChangeDataDialog();

    QString getNewLogin() const;
    QString getNewEmail() const;
    QString getNewPassword() const;

private slots:
    void on_btnSave_clicked();
    void on_btnCancel_clicked();

private:
    Ui::ChangeDataDialog *ui;
};

#endif // CHANGEDATADIALOG_H