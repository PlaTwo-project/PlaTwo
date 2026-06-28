#include "User.h"

User::User() : name(""), phone_number(""), email(""), hashed_password("")
{
}

User::User(const QString &name, const QString &phone_number, const QString &email, const QString &hashed_password)
    : id(id_counter++), name(name), phone_number(phone_number), email(email), hashed_password(hashed_password)
{
}

int User::getId() const{
    return id;
}

QString User::getName() const{
    return name;
}

QString User::getPhoneNumber() const{
    return phone_number;
}

QString User::getEmail() const{
    return email;
}

QString User::getHashedPassword() const{
    return hashed_password;
}

void User::setName(const QString &name){
    this->name = name;
}

void User::setPhoneNumber(const QString &phone_number){
    this->phone_number = phone_number;
}

void User::setEmail(const QString &email){
    this->email = email;
}

void User::setHashedPassword(const QString &hashed_password){
    this->hashed_password = hashed_password;
}
