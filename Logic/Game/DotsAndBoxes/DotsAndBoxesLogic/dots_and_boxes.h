#ifndef DOTS_AND_BOXES_H
#define DOTS_AND_BOXES_H

#include "Logic/Game/BaseLogicClasses/Game.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesBoard/dots_and_boxes_board.h"

class DotsAndBoxes : public Game {
public:
    DotsAndBoxes(int size, const User& player_one, const User& player_two);
    ~DotsAndBoxes() override;

    bool makeMove(const Move& main_move) override;
    GameStatus checkWin() override;
    void resetGame() override;
    QString serializeState() const override;
    void loadState(const QString& state_data) override;

    int getFirstPlayerScore() const;
    int getSecondPlayerScore() const;

protected:
    bool isValidMove(const Move& main_move) override;

private:
    User first_player;
    User second_player;
    int first_player_score;
    int second_player_score;
    int grid_size;
    DotsAndBoxesBoard* game_board;
};

#endif // DOTS_AND_BOXES_H