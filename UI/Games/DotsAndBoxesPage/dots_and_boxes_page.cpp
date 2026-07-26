#include "dots_and_boxes_page.h"
#include <QPainter>
#include <QMouseEvent>
#include <QVariantAnimation>
#include <QEasingCurve>
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesLogic/dots_and_boxes.h"

DotsAndBoxesPage::DotsAndBoxesPage(QWidget *parent) : BasePage(parent), is_animating(false), anim_progress(1.0), anim_type(0), anim_r(-1), anim_c(-1) {
    setMouseTracking(true);

    animation = new QVariantAnimation(this);
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::OutQuad);

    connect(animation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value) {
        anim_progress = value.toReal();
        update();
    });

    connect(animation, &QVariantAnimation::finished, this, &DotsAndBoxesPage::finishAnimation);
}

void DotsAndBoxesPage::setupBoard(int size) {
    BasePage::setupBoard(size);
    horizontal_lines = QVector<QVector<int>>(board_size + 1, QVector<int>(board_size, 0));
    vertical_lines = QVector<QVector<int>>(board_size, QVector<int>(board_size + 1, 0));
    captured_boxes = QVector<QVector<int>>(board_size, QVector<int>(board_size, 0));
    displayed_horizontal_lines = horizontal_lines;
    displayed_vertical_lines = vertical_lines;
    displayed_captured_boxes = captured_boxes;

    if (animation)
        animation->stop();

    is_animating = false;
    update();
}

void DotsAndBoxesPage::updateBoard(const QVector<QVector<int>> &new_h, const QVector<QVector<int>> &new_v, const QVector<QVector<int>> &new_b) {
    horizontal_lines = new_h;
    vertical_lines = new_v;
    captured_boxes = new_b;
    startAnimation(new_h, new_v, new_b);
}

void DotsAndBoxesPage::startAnimation(const QVector<QVector<int>> &new_h, const QVector<QVector<int>> &new_v, const QVector<QVector<int>> &new_b) {
    if (is_animating) {
        animation->stop();
        displayed_horizontal_lines = horizontal_lines;
        displayed_vertical_lines = vertical_lines;
        displayed_captured_boxes = captured_boxes;
        is_animating = false;
    }

    anim_type = 0;
    anim_r = -1;
    anim_c = -1;
    for (int r = 0; r <= board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            if (displayed_horizontal_lines[r][c] == 0 && new_h[r][c] != 0) {
                anim_type = 1;
                anim_r = r;
                anim_c = c;
                break;
            }
        }
        if (anim_type != 0)
            break;
    }

    if (anim_type == 0) {
        for (int r = 0; r < board_size; ++r) {
            for (int c = 0; c <= board_size; ++c) {
                if (displayed_vertical_lines[r][c] == 0 && new_v[r][c] != 0) {
                    anim_type = 2;
                    anim_r = r;
                    anim_c = c;
                    break;
                }
            }
            if (anim_type != 0)
                break;
        }
    }

    if (anim_type != 0) {
        is_animating = true;
        anim_progress = 0.0;
        animation->start();
    } else {
        displayed_horizontal_lines = new_h;
        displayed_vertical_lines = new_v;
        displayed_captured_boxes = new_b;
        update();
    }
}

void DotsAndBoxesPage::finishAnimation() {
    displayed_horizontal_lines = horizontal_lines;
    displayed_vertical_lines = vertical_lines;
    displayed_captured_boxes = captured_boxes;
    is_animating = false;
    anim_type = 0;
    update();
}

void DotsAndBoxesPage::setPlayerColors(const QColor& host_color, const QColor& guest_color) {
    host_player_color = host_color;
    guest_player_color = guest_color;
    update();
}

void DotsAndBoxesPage::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Bauhaus LT Demi", 12));
    painter.drawText(margin_offset, 30, turn_status_text);
    painter.drawText(margin_offset, 50, QString("%1's Score: %2  |  %3's Score: %4").arg(first_player_name).arg(first_player_score).arg(second_player_name).arg(second_player_score));
    painter.drawText(margin_offset, 70, QString("%1's Time: %2  |  %3's Time: %4").arg(first_player_name).arg(first_player_time_str).arg(second_player_name).arg(second_player_time_str));
    QPen empty_pen(Qt::lightGray, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    QPen hover_pen(Qt::darkGray, 4, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    for (int r = 0; r < board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            if (captured_boxes[r][c] != 0) {
                QRect rect(margin_offset + c * cell_spacing, margin_offset + r * cell_spacing, cell_spacing, cell_spacing);
                QColor owner_color;
                if (captured_boxes[r][c] == 1)
                    owner_color = host_player_color;
                else
                    owner_color = guest_player_color;

                QColor fill_color = owner_color;
                qreal alpha = 100;
                if (is_animating && displayed_captured_boxes[r][c] == 0)
                    alpha = 100 * anim_progress;

                fill_color.setAlpha(static_cast<int>(alpha));
                painter.fillRect(rect, fill_color);
                painter.setPen(owner_color);
                if (captured_boxes[r][c] == 1)
                    painter.drawText(rect, Qt::AlignCenter, first_player_name[0]);
                else
                    painter.drawText(rect, Qt::AlignCenter, second_player_name[0]);
            }
        }
    }

    for (int r = 0; r <= board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            int x1 = margin_offset + c * cell_spacing;
            int y1 = margin_offset + r * cell_spacing;
            if (is_animating && anim_type == 1 && r == anim_r && c == anim_c) {
                QColor owner_color;
                if (horizontal_lines[r][c] == 1)
                    owner_color = host_player_color;
                else
                    owner_color = guest_player_color;

                painter.setPen(QPen(owner_color, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                int current_length = static_cast<int>(cell_spacing * anim_progress);
                painter.drawLine(x1, y1, x1 + current_length, y1);
            } else if (horizontal_lines[r][c] != 0) {
                QColor owner_color;
                if (horizontal_lines[r][c] == 1)
                    owner_color = host_player_color;
                else
                    owner_color = guest_player_color;

                painter.setPen(QPen(owner_color, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawLine(x1, y1, x1 + cell_spacing, y1);
            } else if (!is_animating && r == hovered_h_row && c == hovered_h_col) {
                painter.setPen(hover_pen);
                painter.drawLine(x1, y1, x1 + cell_spacing, y1);
            } else {
                painter.setPen(empty_pen);
                painter.drawLine(x1, y1, x1 + cell_spacing, y1);
            }
        }
    }

    for (int r = 0; r < board_size; ++r) {
        for (int c = 0; c <= board_size; ++c) {
            int x1 = margin_offset + c * cell_spacing;
            int y1 = margin_offset + r * cell_spacing;
            if (is_animating && anim_type == 2 && r == anim_r && c == anim_c) {
                QColor owner_color;
                if (vertical_lines[r][c] == 1)
                    owner_color = host_player_color;
                else
                    owner_color = guest_player_color;

                painter.setPen(QPen(owner_color, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                int current_length = static_cast<int>(cell_spacing * anim_progress);
                painter.drawLine(x1, y1, x1, y1 + current_length);
            } else if (vertical_lines[r][c] != 0) {
                QColor owner_color;
                if (vertical_lines[r][c] == 1)
                    owner_color = host_player_color;
                else
                    owner_color = guest_player_color;

                painter.setPen(QPen(owner_color, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawLine(x1, y1, x1, y1 + cell_spacing);
            } else if (!is_animating && r == hovered_v_row && c == hovered_v_col) {
                painter.setPen(hover_pen);
                painter.drawLine(x1, y1, x1, y1 + cell_spacing);
            } else {
                painter.setPen(empty_pen);
                painter.drawLine(x1, y1, x1, y1 + cell_spacing);
            }
        }
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    for (int r = 0; r <= board_size; ++r)
        for (int c = 0; c <= board_size; ++c)
            painter.drawEllipse(QPoint(margin_offset + c * cell_spacing, margin_offset + r * cell_spacing), 5, 5);
}

void DotsAndBoxesPage::mousePressEvent(QMouseEvent *event) {
    if (!is_input_enabled || board_size == 0 || is_animating)
        return;

    int click_x = event->position().x();
    int click_y = event->position().y();
    for (int r = 0; r <= board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            int mid_x = margin_offset + c * cell_spacing + cell_spacing / 2;
            int mid_y = margin_offset + r * cell_spacing;
            if (abs(click_x - mid_x) < 30 && abs(click_y - mid_y) < 15) {
                emit moveRequested(r, c, 0);
                return;
            }
        }
    }

    for (int r = 0; r < board_size; ++r) {
        for (int c = 0; c <= board_size; ++c) {
            int mid_x = margin_offset + c * cell_spacing;
            int mid_y = margin_offset + r * cell_spacing + cell_spacing / 2;
            if (abs(click_x - mid_x) < 15 && abs(click_y - mid_y) < 30) {
                emit moveRequested(r, c, 1);
                return;
            }
        }
    }
}

void DotsAndBoxesPage::mouseMoveEvent(QMouseEvent* event) {
    if (is_animating)
        return;

    hovered_h_row = hovered_h_col = -1;
    hovered_v_row = hovered_v_col = -1;
    int x = event->position().x();
    int y = event->position().y();
    for (int r = 0; r <= board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            int mid_x = margin_offset + c * cell_spacing + cell_spacing / 2;
            int mid_y = margin_offset + r * cell_spacing;
            if (abs(x - mid_x) < 30 && abs(y - mid_y) < 15) {
                hovered_h_row = r;
                hovered_h_col = c;
                update();
                return;
            }
        }
    }

    for (int r = 0; r < board_size; ++r) {
        for (int c = 0; c <= board_size; ++c) {
            int mid_x = margin_offset + c * cell_spacing;
            int mid_y = margin_offset + r * cell_spacing + cell_spacing / 2;
            if (abs(x - mid_x) < 15 && abs(y - mid_y) < 30) {
                hovered_v_row = r;
                hovered_v_col = c;
                update();
                return;
            }
        }
    }

    update();
}

void DotsAndBoxesPage::updateFromGame(const Game *main_game) {
    const DotsAndBoxes *game = static_cast<const DotsAndBoxes *>(main_game);
    if (game && game->getBoard()) {
        const DotsAndBoxesBoard *board = static_cast<DotsAndBoxesBoard *>(game->getBoard());
        updateBoard(board->getHorizontalLines(), board->getVerticalLines(), board->getCapturedBoxes());
    }
}