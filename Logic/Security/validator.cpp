#include "Validator.h"
#include <regex>

bool Validator::validateEmail(const string& email) {
    regex pattern(R"(^[^@\s]+@[^@\s]+\.[^@\s]{2,}$)");
    return regex_match(email, pattern);
}

bool Validator::validatePhone(const string& phone) {
    regex pattern(R"(^(09\d{9}|\+989\d{9})$)");
    return regex_match(phone, pattern);
}

bool Validator::validatePassword(const string& password) {
    regex pattern(R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[^A-Za-z\d]).{8,}$)");
    return regex_match(password, pattern);
}

bool Validator::validateIP(const string& ip) {
    regex pattern(R"(^((25[0-5]|2[0-4]\d|1\d{2}|[1-9]?\d)\.){3}(25[0-5]|2[0-4]\d|1\d{2}|[1-9]?\d)$)");
    return regex_match(ip, pattern);
}