#include "dots_and_boxes_page.h"
#include <QPainter>
#include <QMouseEvent>
#include <QVariantAnimation>
#include <QEasingCurve>
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesLogic/dots_and_boxes.h"
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesMove/dots_and_boxes_move.h"

DotsAndBoxesPage::DotsAndBoxesPage(QWidget *parent) : BasePage(parent), is_animating(false), anim_progress(1.0), anim_type(0), anim_r(-1), anim_c(-1) {
    setMouseTracking(true);
    board_background.load(":/backgrounds/Board2.png");

    animation = new QVariantAnimation(this);
    animation->setDuration(600);
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
    horizontal_lines = QVector<QVector<PlayerSlot>>(board_size + 1, QVector<PlayerSlot>(board_size, PlayerSlot::NONE));
    vertical_lines = QVector<QVector<PlayerSlot>>(board_size, QVector<PlayerSlot>(board_size + 1, PlayerSlot::NONE));
    captured_boxes = QVector<QVector<PlayerSlot>>(board_size, QVector<PlayerSlot>(board_size, PlayerSlot::NONE));
    displayed_horizontal_lines = horizontal_lines;
    displayed_vertical_lines = vertical_lines;
    displayed_captured_boxes = captured_boxes;

    if (animation)
        animation->stop();

    is_animating = false;
    update();
}

void DotsAndBoxesPage::updateBoard(const QVector<QVector<PlayerSlot>> &new_h, const QVector<QVector<PlayerSlot>> &new_v, const QVector<QVector<PlayerSlot>> &new_b) {
    horizontal_lines = new_h;
    vertical_lines = new_v;
    captured_boxes = new_b;
    startAnimation(new_h, new_v, new_b);
}

void DotsAndBoxesPage::startAnimation(const QVector<QVector<PlayerSlot>> &new_h, const QVector<QVector<PlayerSlot>> &new_v, const QVector<QVector<PlayerSlot>> &new_b) {
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
            if (displayed_horizontal_lines[r][c] == PlayerSlot::NONE && new_h[r][c] != PlayerSlot::NONE) {
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
                if (displayed_vertical_lines[r][c] == PlayerSlot::NONE && new_v[r][c] != PlayerSlot::NONE) {
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
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QRect board_rect(margin_offset - 40, margin_offset - 25, (board_size * cell_spacing) + 80, (board_size * cell_spacing) + 100);
    drawBoardBackground(painter, board_rect, board_background);

    painter.setPen(Qt::black);
    painter.setFont(QFont("Bauhaus LT Demi", 12));
    painter.drawText(margin_offset, 30, turn_status_text);
    painter.drawText(margin_offset, 50, QString("%1's Score: %2  |  %3's Score: %4").arg(first_player_name).arg(first_player_score).arg(second_player_name).arg(second_player_score));
    painter.drawText(margin_offset, 70, QString("%1's Time: %2  |  %3's Time: %4").arg(first_player_name).arg(first_player_time_str).arg(second_player_name).arg(second_player_time_str));

    QPen empty_pen(QColor(180, 160, 140, 150), 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    QPen hover_pen(QColor(80, 80, 80, 200), 4, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);

    for (int r = 0; r < board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            if (captured_boxes[r][c] != PlayerSlot::NONE) {

                int pad = 5;
                QRect rect(margin_offset + c * cell_spacing + pad, 15 + margin_offset + r * cell_spacing + pad, cell_spacing - (2 * pad), cell_spacing - (2 * pad));

                QColor owner_color = (captured_boxes[r][c] == PlayerSlot::HOST) ? host_player_color : guest_player_color;

                qreal alpha = 210;
                if (is_animating && displayed_captured_boxes[r][c] == PlayerSlot::NONE)
                    alpha = 210 * anim_progress;

                QLinearGradient grad(rect.topLeft(), rect.bottomRight());
                QColor c_light = owner_color.lighter(120);
                QColor c_dark = owner_color.darker(110);
                c_light.setAlpha(static_cast<int>(alpha));
                c_dark.setAlpha(static_cast<int>(alpha));
                grad.setColorAt(0.0, c_light);
                grad.setColorAt(1.0, c_dark);


                painter.setPen(Qt::NoPen);
                painter.setBrush(grad);
                painter.drawRoundedRect(rect, 6, 6);

                QColor border_color = owner_color.lighter(140);
                border_color.setAlpha(static_cast<int>(alpha * 0.6));
                painter.setPen(QPen(border_color, 1));
                painter.setBrush(Qt::NoBrush);
                painter.drawRoundedRect(rect, 6, 6);

                painter.setFont(QFont("Bauhaus LT Demi", 15, QFont::Bold));
                painter.setPen(QColor(255, 255, 255, static_cast<int>(alpha)));
                QString letter = (captured_boxes[r][c] == PlayerSlot::HOST) ? first_player_name[0] : second_player_name[0];
                painter.drawText(rect, Qt::AlignCenter, letter);
            }
        }
    }

    for (int r = 0; r <= board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            int x1 = margin_offset + c * cell_spacing;
            int y1 = 15 + margin_offset + r * cell_spacing;
            if (is_animating && anim_type == 1 && r == anim_r && c == anim_c) {
                QColor owner_color = (horizontal_lines[r][c] == PlayerSlot::HOST) ? host_player_color : guest_player_color;
                painter.setPen(QPen(owner_color, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                int current_length = static_cast<int>(cell_spacing * anim_progress);
                painter.drawLine(x1, y1, x1 + current_length, y1);
            } else if (horizontal_lines[r][c] != PlayerSlot::NONE) {
                QColor owner_color = (horizontal_lines[r][c] == PlayerSlot::HOST) ? host_player_color : guest_player_color;
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
            int y1 = 15 + margin_offset + r * cell_spacing;
            if (is_animating && anim_type == 2 && r == anim_r && c == anim_c) {
                QColor owner_color = (vertical_lines[r][c] == PlayerSlot::HOST) ? host_player_color : guest_player_color;
                painter.setPen(QPen(owner_color, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                int current_length = static_cast<int>(cell_spacing * anim_progress);
                painter.drawLine(x1, y1, x1, y1 + current_length);
            } else if (vertical_lines[r][c] != PlayerSlot::NONE) {
                QColor owner_color = (vertical_lines[r][c] == PlayerSlot::HOST) ? host_player_color : guest_player_color;
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

    for (int r = 0; r <= board_size; ++r) {
        for (int c = 0; c <= board_size; ++c) {
            QPoint center(margin_offset + c * cell_spacing, 15 + margin_offset + r * cell_spacing);
            int dot_radius = 7;

            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(0, 0, 0, 90));
            painter.drawEllipse(center + QPoint(1, 2), dot_radius, dot_radius);
            painter.setBrush(QColor(25, 20, 15));
            painter.drawEllipse(center, dot_radius, dot_radius);
            painter.setBrush(QColor(255, 255, 255, 160));
            painter.drawEllipse(center - QPoint(2, 2), 2, 2);
        }
    }
}


void DotsAndBoxesPage::mousePressEvent(QMouseEvent *event) {
    if (!is_input_enabled || board_size == 0 || is_animating)
        return;

    int click_x = event->position().x();
    int click_y = event->position().y() - 15;
    for (int r = 0; r <= board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            int mid_x = margin_offset + c * cell_spacing + cell_spacing / 2;
            int mid_y = margin_offset + r * cell_spacing;
            if (abs(click_x - mid_x) < 30 && abs(click_y - mid_y) < 15) {
                emit moveRequested(r, c, static_cast<int>(lineDirection::HORIZONTAL));
                return;
            }
        }
    }

    for (int r = 0; r < board_size; ++r) {
        for (int c = 0; c <= board_size; ++c) {
            int mid_x = margin_offset + c * cell_spacing;
            int mid_y = margin_offset + r * cell_spacing + cell_spacing / 2;
            if (abs(click_x - mid_x) < 15 && abs(click_y - mid_y) < 30) {
                emit moveRequested(r, c, static_cast<int>(lineDirection::VERTICAL));
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
    int y = event->position().y() - 15;
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