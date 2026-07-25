#ifndef LOGIN_EXCEPTION_H
#define LOGIN_EXCEPTION_H

#include "base_exception.h"

class LoginException : public BaseException {
public:
    explicit LoginException(const QString& msg): BaseException(msg, "Login Error") {}
};


#endif // LOGIN_EXCEPTION_H
