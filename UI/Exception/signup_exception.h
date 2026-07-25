#ifndef SIGNUP_EXCEPTION_H
#define SIGNUP_EXCEPTION_H

#include "base_exception.h"

class SignupException : public BaseException {
public:
    explicit SignupException(const QString& msg): BaseException(msg, "Signup Error") {}
};

#endif // SIGNUP_EXCEPTION_H
