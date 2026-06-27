#include "User.h"

int User::id_counter = 1;

User::User() : id(id_counter++), name(""), phone_number(""), email(""), hashed_password("")
{
}

User::User(const std::string &name, const std::string &phone_number, const std::string &email, const std::string &hashed_password)
    : id(id_counter++), name(name), phone_number(phone_number), email(email), hashed_password(hashed_password)
{
}

int User::getId() const{
    return id;
}

std::string User::getName() const{
    return name;
}

std::string User::getPhoneNumber() const{
    return phone_number;
}

std::string User::getEmail() const{
    return email;
}

std::string User::getHashedPassword() const{
    return hashed_password;
}

void User::setName(const std::string &name){
    this->name = name;
}

void User::setPhoneNumber(const std::string &phone_number){
    this->phone_number = phone_number;
}

void User::setEmail(const std::string &email){
    this->email = email;
}

void User::setHashedPassword(const std::string &hashed_password){
    this->hashed_password = hashed_password;
}