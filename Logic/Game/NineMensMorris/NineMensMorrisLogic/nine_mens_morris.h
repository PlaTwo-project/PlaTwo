#ifndef NINE_MENS_MORRIS_H
#define NINE_MENS_MORRIS_H

#include "Logic/Game/BaseLogicClasses/Game.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisBoard/nine_mens_morris_board.h"

class NineMensMorris : public Game
{
public:
    NineMensMorris(const User& player_one, const User& player_two);
    ~NineMensMorris();

    bool makeMove(const Move& main_move) override;
    GameStatus checkWin() override;
    void resetGame() override;
    QString serializeState() const override;
    void loadState(const QString& state_data) override;
    bool isValidMove(const Move& main_move) override;

    int getFirstPlayerScore() const override;
    int getSecondPlayerScore() const override;
    Board* getBoard() const override;
    int getPlacedCount(int player_id) const;
    bool getAwaitingRemoval() const;
    bool isFlying(int player_id) const;

    static const int PIECES_PER_PLAYER = 9;
    static const int FLYING_THRESHOLD = 3;

private:
    User first_player;
    User second_player;
    int first_player_score;
    int second_player_score;
    NineMensMorrisBoard* game_board;
    int placed_count[2];
    bool awaiting_removal;

    int getCurrentPlayerId() const;
    int getOpponentId(int player_id) const;
    void switchTurn();

};

#endif // NINE_MENS_MORRIS_H