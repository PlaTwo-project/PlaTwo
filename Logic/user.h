#ifndef USER_H
#define USER_H

#include <string>

class User
{
private:
    static int id_counter;

    int id;
    std::string name;
    std::string phone_number;
    std::string email;
    std::string hashed_password;

public:
    User();
    User(const std::string &name, const std::string &phone_number, const std::string &email, const std::string &hashed_password);

    int getId() const;
    std::string getName() const;
    std::string getPhoneNumber() const;
    std::string getEmail() const;
    std::string getHashedPassword() const;

    void setName(const std::string &name);
    void setPhoneNumber(const std::string &phone_number);
    void setEmail(const std::string &email);
    void setHashedPassword(const std::string &hashed_password);
};

#endif
