#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QLabel>
#include <QRegularExpression>

namespace Ui {
class Signup;
}

class Signup : public QWidget
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    void setInitialValues(const QString& username, const QString& password);
    void clearFields();
    ~Signup();

signals:
    void signupRequested(const QString& name, const QString& username, const QString& email, const QString& phone, const QString& password);
    void navigateToLogin();

private slots:
    void on_pushButton_submit_clicked();
    void on_pushButton_login_clicked();
    void checkPasswordStrength(const QString &password);

private:
    Ui::Signup *ui;

    void updateRequirementLabel(QLabel *label, bool isValid, const QString &text);
};

#endif // SIGNUP_H