#include <QToolButton>
#include "login.h"
#include "ui_login.h"
#include "UI/Exception/login_exception.h"

Login::Login(QWidget *parent) : QWidget(parent), ui(new Ui::Login) {
    ui->setupUi(this);

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

    ui->lineEdit_username->setPlaceholderText("Enter your Username");
    ui->lineEdit_password->setPlaceholderText("Enter your Password");
}

Login::~Login() {
    delete ui;
}

void Login::clearFields() {
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
}

void Login::on_pushButton_login_clicked() {
    try {
        QString username = ui->lineEdit_username->text().trimmed();
        QString password = ui->lineEdit_password->text().trimmed();
        if (username.isEmpty() || password.isEmpty())
            throw LoginException("Please fill out all fields.");

        emit loginRequested(username, password);
    }
    catch (const BaseException& e) {
        e.showDialog(this);
    }
}

void Login::on_pushButton_signup_clicked() {
    emit navigateToSignup(ui->lineEdit_username->text(), ui->lineEdit_password->text());
}

void Login::on_pushButton_forgotPassword_clicked() {
    emit navigateToForgotPassword(ui->lineEdit_username->text());
}

void Login::showEvent(QShowEvent *event) { // set focus on username-field
    QWidget::showEvent(event);
    ui->lineEdit_username->setFocus();
}