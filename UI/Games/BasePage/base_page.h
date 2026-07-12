#pragma once

#include <QWidget>
#include <QString>

class BasePage : public QWidget {
    Q_OBJECT

public:
    explicit BasePage(QWidget* parent = nullptr) : QWidget(parent), board_size(0), cell_spacing(50), margin_offset(60),
        is_input_enabled(false), first_player_score(0), second_player_score(0), turn_status_text("") {}

    virtual ~BasePage() = default;

    virtual void setupBoard(const int size) {
        board_size = size;
    }

    virtual void updateFromGame(const class Game* game) = 0;

    virtual void updateScores(const int score1, const int score2) {
        first_player_score = score1;
        second_player_score = score2;
        update();
    }

    virtual void setTurnInfo(const QString& text, const bool is_active) {
        turn_status_text = text;
        is_input_enabled = is_active;
        update();
    }

signals:
    void moveRequested(const int row, const int col, const int direction);

protected:
    int board_size;
    int cell_spacing;
    int margin_offset;
    bool is_input_enabled;

    int first_player_score;
    int second_player_score;
    QString turn_status_text;
};