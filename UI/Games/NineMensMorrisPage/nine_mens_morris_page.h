#ifndef NINE_MENS_MORRIS_PAGE_H
#define NINE_MENS_MORRIS_PAGE_H

#include "UI/Games/BasePage/base_page.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisBoard/nine_mens_morris_board.h"
#include <QVector>

class NineMensMorrisPage : public BasePage
{
    Q_OBJECT

public:
    explicit NineMensMorrisPage(QWidget* parent = nullptr);
    ~NineMensMorrisPage() override = default;

    void setupBoard(const int size) override;
    void updateFromGame(const class Game* main_game) override;

private:
    QVector<int> position_owners;
    NineMensMorrisBoard snapshot_board;
    bool awaiting_removal;
    int placed_count_p1;
    int placed_count_p2;
    int current_player_id;

    int selected_position;
    int hovered_position;
    QVector<int> highlighted_positions;

    int getPositionClicked(const QPoint& point) const;
    QPoint positionToCoordinates(int position) const;
    void updateHighlights();

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    // (position, -1, 0) = PLACE, (from, to, 1) = MOVE, (position, -1, 2) = REMOVE
};

#endif // NINE_MENS_MORRIS_PAGE_H