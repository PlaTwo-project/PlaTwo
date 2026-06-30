#ifndef FORGOT_PASSWORD2_H
#define FORGOT_PASSWORD2_H

#include <QWidget>

namespace Ui {
class ForgotPassword2;
}

class ForgotPassword2 : public QWidget
{
    Q_OBJECT

public:
    explicit ForgotPassword2(QWidget *parent = nullptr);
    void clearFields();
    void setUserData(const QString& username, const QString& phone);
    ~ForgotPassword2();

signals:
    void resetPasswordRequested(const QString& username, const QString& phone, const QString& newPassword);
    void navigateToLogin();

private slots:
    void on_pushButton_reset_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::ForgotPassword2 *ui;
    QString phone_number;
    QString user_name;
};

#endif // FORGOT_PASSWORD2_H