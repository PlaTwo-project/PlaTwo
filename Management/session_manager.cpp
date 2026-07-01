#include "session_manager.h"

SessionManager::SessionManager() : is_logged_in(false)
{
}

SessionManager& SessionManager::getInstance()
{
    static SessionManager instance;
    return instance;
}

void SessionManager::login(const User& user)
{
    current_user = user;
    is_logged_in = true;
}

void SessionManager::logout()
{
    current_user = User();
    is_logged_in = false;
}

User SessionManager::getCurrentUser() const
{
    return current_user;
}

void SessionManager::updateCurrentUser(const User& updated_user)
{
    if (is_logged_in && current_user.getId() == updated_user.getId())
        current_user = updated_user;
}

bool SessionManager::isLoggedIn() const
{
    return is_logged_in;
}