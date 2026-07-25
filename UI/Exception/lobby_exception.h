#ifndef LOBBY_EXCEPTION_H
#define LOBBY_EXCEPTION_H

#include "base_exception.h"

class LobbyException : public BaseException {
public:
    explicit LobbyException(const QString& msg): BaseException(msg, "Lobby Error") {}
};

#endif