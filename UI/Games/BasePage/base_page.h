#pragma once

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QResizeEvent>

class BasePage : public QWidget {
    Q_OBJECT

public:
    explicit BasePage(QWidget* parent = nullptr) : QWidget(parent), board_size(0), cell_spacing(70), margin_offset(90),
        is_input_enabled(false), first_player_score(0), second_player_score(0), turn_status_text("") {

        resign_button = new QPushButton("Resign", this);
        resign_button->setStyleSheet("background-color: #ff4d4d; color: white; font-weight: bold; border-radius: 5px;");
        connect(resign_button, &QPushButton::clicked, this, &BasePage::resignRequested);
    }

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

    virtual void setPlayerNames(const QString& name1, const QString& name2) {
        first_player_name = name1;
        second_player_name = name2;
        update();
    }

signals:
    void moveRequested(const int, const int, const int);
    void resignRequested();

protected:

    void resizeEvent(QResizeEvent* event) override {
        QWidget::resizeEvent(event);

        int button_width = 80;
        int button_height = 30;
        int x = width() - button_width - 20;
        int y = 20;

        resign_button->setGeometry(x, y, button_width, button_height);
    }

    int board_size;
    int cell_spacing;
    int margin_offset;
    bool is_input_enabled;

    int first_player_score;
    int second_player_score;
    QString turn_status_text;
    QString first_player_name;
    QString second_player_name;

    QPushButton* resign_button;
};