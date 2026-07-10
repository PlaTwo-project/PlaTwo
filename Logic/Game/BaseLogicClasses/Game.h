#ifndef GAME_H
#define GAME_H

#include "Logic/Game/BaseLogicClasses/Move.h"
#include "Logic/Game/BaseLogicClasses/Board.h"
#include "Logic/User/user.h"

enum class GameStatus {
    ONGOING,
    PLAYER_ONE_WIN,
    PLAYER_TWO_WIN,
    DRAW
};

class Game {
public:
    virtual ~Game() = default;
    virtual bool makeMove(const Move& move) = 0;
    virtual GameStatus checkWinCondition() = 0;
    virtual void resetGame() = 0;

    virtual QString serializeState() const = 0;
    virtual void loadState(const QString& state_data) = 0;

    User getCurrentPlayer() const {
        return current_player;
    }

protected:
    virtual bool isValidMove(const Move& move) = 0;

    User current_player;
};

#endif // GAME_H