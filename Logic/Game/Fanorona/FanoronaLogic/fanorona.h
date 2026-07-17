#ifndef FANORONA_H
#define FANORONA_H

#include "Logic/Game/BaseLogicClasses/Game.h"
#include "Logic/Game/Fanorona/FanoronaBoard/fanorona_board.h"

class Fanorona : public Game
{
public:
    Fanorona(const User& player_one, const User& player_two);
    ~Fanorona() override;

    bool makeMove(const Move& main_move) override;
    GameStatus checkWin() override;
    void resetGame() override;
    QString serializeState() const override;
    void loadState(const QString& state_data) override;

    int getFirstPlayerScore() const override;
    int getSecondPlayerScore() const override;
    Board* getBoard() const override;

    bool isChainActive() const;
    int getChainPosition() const;
    bool mustCapture() const;
    int currentPlayerId() const;

protected:
    bool isValidMove(const Move& main_move) override;

private:
    User first_player;
    User second_player;
    int first_player_score;
    int second_player_score;
    FanoronaBoard* game_board;

    bool chain_active;
    int chain_position;
    QVector<int> chain_visited;
    int chain_last_dr;
    int chain_last_dc;

    int idOf(const User& user) const;
    int opponentId(int player_id) const;
    void switchTurn();
    void resetChainState();

    FanoronaCaptureType resolveCaptureType(int from, int to, FanoronaCaptureType requested, bool& is_ambiguous) const;
    bool hasContinuation(int position, int incoming_dr, int incoming_dc, const QVector<int>& visited) const;
};

#endif // FANORONA_H