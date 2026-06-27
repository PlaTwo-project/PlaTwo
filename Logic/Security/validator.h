#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>

class Validator
{
public:
    static bool validateEmail(const std::string &email);
    static bool validatePhone(const std::string &phone);
    static bool validatePassword(const std::string &password);
};

#endif
