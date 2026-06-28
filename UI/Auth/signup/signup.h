#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>

namespace Ui {
class Signup;
}

class Signup : public QWidget
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    void setInitialValues(const QString &username, const QString &password);
    ~Signup();

private slots:
    void on_pushButton_submit_clicked();
    void on_pushButton_login_clicked();

signals:
    void backToLogin();

private:
    Ui::Signup *ui;
};

#endif // SIGNUP_H
