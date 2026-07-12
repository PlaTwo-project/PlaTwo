#ifndef FORGOT_PASSWORD_H
#define FORGOT_PASSWORD_H

#include <QWidget>

namespace Ui {
class ForgotPassword;
}

class ForgotPassword : public QWidget
{
    Q_OBJECT

public:
    explicit ForgotPassword(QWidget *parent = nullptr);
    void setInitialValues(const QString& username);
    void setUserData(const QString& username, const QString& phone);
    void switchToVerifyPage();
    void switchToResetPage();
    void clearFields();
    ~ForgotPassword();

signals:
    void forgotPasswordStep2Requested(const QString& username, const QString& phone);
    void resetPasswordRequested(const QString& username, const QString& phone, const QString& newPassword);
    void navigateToLogin();

private slots:
    void on_pushButton_next_2_clicked();
    void on_pushButton_back_2_clicked();
    void on_pushButton_reset_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::ForgotPassword *ui;
    QString phone_number;
    QString user_name;
};

#endif // FORGOT_PASSWORD_H