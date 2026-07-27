#include <QToolButton>
#include "signup.h"
#include "ui_signup.h"
#include "UI/Exception/signup_exception.h"

Signup::Signup(QWidget *parent) : QWidget(parent), ui(new Ui::Signup) {
    ui->setupUi(this);

    ui->label_char->setText("✗ At least 8 characters");
    ui->label_lower->setText("✗ At least one lowercase character (a-z)");
    ui->label_capital->setText("✗ At least one uppercase character (A-Z)");
    ui->label_number->setText("✗ At least one number (0-9)");
    ui->label_special->setText("✗ At least one special character");

    connect(ui->lineEdit_password, &QLineEdit::textChanged, this, &Signup::checkPasswordStrength);

    QToolButton *eye_button = new QToolButton(ui->lineEdit_password);
    eye_button->setIcon(QIcon(":/icons/Resources/Icons/eye_closed.png"));
    eye_button->setIconSize(QSize(24, 24));
    eye_button->setCursor(Qt::PointingHandCursor);
    eye_button->setStyleSheet("QToolButton {"
                              "border: none;"
                              "padding: 0px;"
                              "background: transparent;"
                              "}");

    QHBoxLayout *layout = new QHBoxLayout(ui->lineEdit_password);
    layout->setContentsMargins(0, 0, 8, 0);
    layout->addWidget(eye_button, 0, Qt::AlignRight | Qt::AlignVCenter);
    connect(eye_button, &QToolButton::clicked, this, [=]() {
        if (ui->lineEdit_password->echoMode() == QLineEdit::Password) {
            ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
            eye_button->setIcon(QIcon(":/icons/Resources/Icons/eye_open.png"));
        } else {
            ui->lineEdit_password->setEchoMode(QLineEdit::Password);
            eye_button->setIcon(QIcon(":/icons/Resources/Icons/eye_closed.png"));
        }
    });

    ui->lineEdit_name->setPlaceholderText("Enter your Name");
    ui->lineEdit_username->setPlaceholderText("Enter your Username");
    ui->lineEdit_email->setPlaceholderText("Enter your Email");
    ui->lineEdit_phonenumber->setPlaceholderText("e.g., 09123456789");
    ui->lineEdit_password->setPlaceholderText("Enter your password");
}

Signup::~Signup() {
    delete ui;
}

void Signup::setInitialValues(const QString& username, const QString& password) {
    ui->lineEdit_username->setText(username);
    ui->lineEdit_password->setText(password);
}

void Signup::clearFields() {
    ui->lineEdit_name->clear();
    ui->lineEdit_username->clear();
    ui->lineEdit_email->clear();
    ui->lineEdit_phonenumber->clear();
    ui->lineEdit_password->clear();
}

void Signup::on_pushButton_submit_clicked() {
    try {
        QString name = ui->lineEdit_name->text().trimmed();
        QString username = ui->lineEdit_username->text().trimmed();
        QString phone = ui->lineEdit_phonenumber->text().trimmed();
        QString email = ui->lineEdit_email->text().trimmed();
        QString password = ui->lineEdit_password->text().trimmed();
        if (name.isEmpty() || username.isEmpty() || phone.isEmpty() || email.isEmpty() || password.isEmpty())
            throw SignupException("Please fill out all fields.");

        emit signupRequested(name, username, email, phone, password);
    }
    catch (const BaseException& e) {
        e.showDialog(this);
    }
}

void Signup::on_pushButton_login_clicked() {
    emit navigateToLogin();
    clearFields();
}

void Signup::checkPasswordStrength(const QString &password) {
    bool hasLength = password.length() >= 8;
    bool hasLower = QRegularExpression("[a-z]").match(password).hasMatch();
    bool hasUpper = QRegularExpression("[A-Z]").match(password).hasMatch();
    bool hasNumber = QRegularExpression("[0-9]").match(password).hasMatch();
    bool hasSymbol = QRegularExpression("[^a-zA-Z0-9\\s]").match(password).hasMatch();

    updateRequirementLabel(ui->label_char, hasLength, "At least 8 characters");
    updateRequirementLabel(ui->label_lower, hasLower, "At least one lowercase character (a-z)");
    updateRequirementLabel(ui->label_capital, hasUpper, "At least one uppercase character (A-Z)");
    updateRequirementLabel(ui->label_number, hasNumber, "At least one number (0-9)");
    updateRequirementLabel(ui->label_special, hasSymbol, "At least one special character");
}

void Signup::updateRequirementLabel(QLabel *label, bool isValid, const QString &text) {
    if (isValid) {
        label->setText("✓ " + text);
        label->setStyleSheet("color: #13B51A;"
                             "background: transparent;"
                             "font-size: 10pt;"
                             "font-weight: bold;"
                             );
    } else {
        label->setText("✗ " + text);
        label->setStyleSheet("color: #3A2412;"
                             "background: transparent;"
                             "font-size: 10pt;"
                             "font-weight: bold;"
                             );
    }
}