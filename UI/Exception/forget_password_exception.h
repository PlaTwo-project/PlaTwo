#ifndef FORGOT_PASSWORD_EXCEPTION_H
#define FORGOT_PASSWORD_EXCEPTION_H

#include "base_exception.h"

class ForgotPasswordException : public BaseException {
public:
    explicit ForgotPasswordException(const QString& msg): BaseException(msg, "Password Reset Error") {}
};

#endif
