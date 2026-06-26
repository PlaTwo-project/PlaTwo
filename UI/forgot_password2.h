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
    ~ForgotPassword2();

private slots:
    void on_pushButton_reset_clicked();

    void on_pushButton_cancel_clicked();

signals:
    void backToLogin();

private:
    Ui::ForgotPassword2 *ui;
};

#endif // FORGOT_PASSWORD2_H
