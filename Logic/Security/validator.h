#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
using namespace std;

class Validator
{
public:
    static bool validateEmail(const string &email);
    static bool validatePhone(const string &phone);
    static bool validatePassword(const string &password);
    static bool validateIP(const string& ip);
};

#endif
