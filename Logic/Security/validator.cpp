#include "Validator.h"
#include <regex>

bool Validator::validateEmail(const std::string& email) {
    std::regex pattern(R"(^[^@\s]+@[^@\s]+\.[^@\s]{2,}$)");
    return std::regex_match(email, pattern);
}

bool Validator::validatePhone(const std::string& phone) {
    std::regex pattern(R"(^(09\d{9}|\+989\d{9})$)");
    return std::regex_match(phone, pattern);
}

bool Validator::validatePassword(const std::string& password) {
    std::regex pattern(R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[^A-Za-z\d]).{8,}$)");
    return std::regex_match(password, pattern);
}
