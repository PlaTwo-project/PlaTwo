#ifndef DOTS_AND_BOXES_PAGE_H
#define DOTS_AND_BOXES_PAGE_H

#include "UI/Games/BasePage/base_page.h"
#include <QVector>
#include <QColor>

using namespace std;

class QVariantAnimation;

class DotsAndBoxesPage : public BasePage
{
    Q_OBJECT

public:
    explicit DotsAndBoxesPage(QWidget *parent = nullptr);
    ~DotsAndBoxesPage() override = default;

    void setupBoard(const int size) override;
    void updateBoard(const QVector<QVector<int>> &horizontal_lines, const QVector<QVector<int>> &vertical_lines, const QVector<QVector<int>> &captured_boxes);
    void updateFromGame(const Game *main_game) override;
    void setPlayerColors(const QColor& host_color, const QColor& guest_color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    QVector<QVector<int>> horizontal_lines;
    QVector<QVector<int>> vertical_lines;
    QVector<QVector<int>> captured_boxes;

    QVector<QVector<int>> displayed_horizontal_lines;
    QVector<QVector<int>> displayed_vertical_lines;
    QVector<QVector<int>> displayed_captured_boxes;

    QColor host_player_color = QColor(100, 149, 237);
    QColor guest_player_color = QColor(255, 99, 71);

    int hovered_h_row = -1;
    int hovered_h_col = -1;
    int hovered_v_row = -1;
    int hovered_v_col = -1;

    // <animations>
    QVariantAnimation* animation;
    bool is_animating;
    qreal anim_progress;
    int anim_type;
    int anim_r;
    int anim_c;

    void startAnimation(const QVector<QVector<int>> &new_h, const QVector<QVector<int>> &new_v, const QVector<QVector<int>> &new_b);
    void finishAnimation();
    // </animations>
};

#endif // DOTS_AND_BOXES_PAGE_H