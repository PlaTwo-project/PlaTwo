#include "User.h"

User::User() : name(""), username(""), phone_number(""), email(""), hashed_password("")
{}

User::User(const QString& name, const QString& username, const QString& phone_number, const QString& email, const QString& hashed_password)
    : name(name), username(username), phone_number(phone_number), email(email), hashed_password(hashed_password)
{}

QString User::getName() const {
    return name;
}

QString User::getUsername() const {
    return username;
}

QString User::getPhoneNumber() const {
    return phone_number;
}

QString User::getEmail() const {
    return email;
}

QString User::getHashedPassword() const {
    return hashed_password;
}

void User::setName(const QString& name) {
    this->name = name;
}

void User::setUsername(const QString& username) {
    this->username = username;
}

void User::setPhoneNumber(const QString& phone_number) {
    this->phone_number = phone_number;
}

void User::setEmail(const QString& email) {
    this->email = email;
}

void User::setHashedPassword(const QString& hashed_password) {
    this->hashed_password = hashed_password;
}
