#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "Logic/user.h"

class SessionManager
{
private:
    User current_user;
    bool is_logged_in;

    SessionManager();

public:
    static SessionManager& getInstance();

    void login(const User& user);
    void logout();

    User getCurrentUser() const;
    void updateCurrentUser(const User& updated_user);
    bool isLoggedIn() const;
};

#endif // SESSION_MANAGER_H