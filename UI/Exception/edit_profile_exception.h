#ifndef EDIT_PROFILE_EXCEPTION_H
#define EDIT_PROFILE_EXCEPTION_H

#include "base_exception.h"

class EditProfileException : public BaseException {
public:
    explicit EditProfileException(const QString& msg): BaseException(msg, "Profile Update Error") {}
};

#endif