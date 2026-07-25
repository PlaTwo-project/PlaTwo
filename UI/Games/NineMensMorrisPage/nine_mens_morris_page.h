#ifndef NINE_MENS_MORRIS_PAGE_H
#define NINE_MENS_MORRIS_PAGE_H

#include "UI/Games/BasePage/base_page.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisBoard/nine_mens_morris_board.h"
#include <QVector>

class QVariantAnimation;

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

    // <animations>
    QVector<int> displayed_owners;
    QVector<int> pending_owners;
    QVariantAnimation* move_animation;
    bool is_animating;
    qreal anim_progress;
    int anim_move_from;
    int anim_move_to;
    int anim_removed_position;
    int anim_moving_player_id;

    void startAnimation(const QVector<int>& new_owners);
    void finishAnimation();
    // </animations>

    int getPositionClicked(const QPoint& point) const;
    QPoint positionToCoordinates(int position) const;
    void updateHighlights();

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

signals:
    void moveRequested(int from, int to, int type); // (position, -1, 0) = PLACE, (from, to, 1) = MOVE, (position, -1, 2) = REMOVE
};

#endif // NINE_MENS_MORRIS_PAGE_H