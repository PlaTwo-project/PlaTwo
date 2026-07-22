#include "dots_and_boxes_page.h"
#include <QPainter>
#include <QMouseEvent>
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesLogic/dots_and_boxes.h"
DotsAndBoxesPage::DotsAndBoxesPage(QWidget *parent) : BasePage(parent) {
    setMouseTracking(true);
}

void DotsAndBoxesPage::setupBoard(int size) {
    BasePage::setupBoard(size);
    horizontal_lines.assign(board_size + 1, QVector<int>(board_size, 0));
    vertical_lines.assign(board_size, QVector<int>(board_size + 1, 0));
    captured_boxes.assign(board_size, QVector<int>(board_size, 0));
    update();
}

void DotsAndBoxesPage::updateBoard(const QVector<QVector<int>> &horizontal_lines, const QVector<QVector<int>> &vertical_lines, const QVector<QVector<int>> &captured_boxes) {
    this->horizontal_lines = horizontal_lines;
    this->vertical_lines = vertical_lines;
    this->captured_boxes = captured_boxes;
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
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(margin_offset, 30, turn_status_text);
    painter.drawText(margin_offset, 50, QString("%1's Score: %2  |  %3's Score: %4").arg(first_player_name).arg(first_player_score).arg(second_player_name).arg(second_player_score));

    QPen empty_pen(Qt::lightGray, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    QPen hover_pen(Qt::darkGray, 4, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);

    // captured squares
    for (int r = 0; r < board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            if (captured_boxes[r][c] != 0) {
                QRect rect(margin_offset + c * cell_spacing, margin_offset + r * cell_spacing, cell_spacing, cell_spacing);
                QColor owner_color = (captured_boxes[r][c] == 1) ? host_player_color : guest_player_color;
                QColor fill_color = owner_color;
                fill_color.setAlpha(100);
                painter.fillRect(rect, fill_color);
                painter.setPen(owner_color);
                painter.drawText(rect, Qt::AlignCenter, captured_boxes[r][c] == 1 ? first_player_name[0] : second_player_name[0]);
            }
        }
    }

    // horizontal lines
    for (int r = 0; r <= board_size; ++r) {
        for (int c = 0; c < board_size; ++c) {
            int x1 = margin_offset + c * cell_spacing;
            int y1 = margin_offset + r * cell_spacing;
            if (horizontal_lines[r][c] != 0) {
                QColor owner_color = (horizontal_lines[r][c] == 1) ? host_player_color : guest_player_color;
                painter.setPen(QPen(owner_color, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            } else if (r == hovered_h_row && c == hovered_h_col)
                painter.setPen(hover_pen);
            else
                painter.setPen(empty_pen);

            painter.drawLine(x1, y1, x1 + cell_spacing, y1);
        }
    }

    // vertical lines
    for (int r = 0; r < board_size; ++r) {
        for (int c = 0; c <= board_size; ++c) {
            int x1 = margin_offset + c * cell_spacing;
            int y1 = margin_offset + r * cell_spacing;
            if (vertical_lines[r][c] != 0) {
                QColor owner_color = (vertical_lines[r][c] == 1) ? host_player_color : guest_player_color;
                painter.setPen(QPen(owner_color, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            } else if (r == hovered_v_row && c == hovered_v_col)
                painter.setPen(hover_pen);
            else
                painter.setPen(empty_pen);

            painter.drawLine(x1, y1, x1, y1 + cell_spacing);
        }
    }

    // dots
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);

    for (int r = 0; r <= board_size; ++r)
        for (int c = 0; c <= board_size; ++c)
            painter.drawEllipse(QPoint(margin_offset + c * cell_spacing, margin_offset + r * cell_spacing), 5, 5);
}

void DotsAndBoxesPage::mousePressEvent(QMouseEvent *event) {
    if (!is_input_enabled || board_size == 0)
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

void DotsAndBoxesPage::mouseMoveEvent(QMouseEvent* event)
{
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

void DotsAndBoxesPage::updateFromGame(const Game *main_game)
{
    const DotsAndBoxes *game = static_cast<const DotsAndBoxes *>(main_game);
    if (game && game->getBoard()) {
        const DotsAndBoxesBoard *board = static_cast<DotsAndBoxesBoard *>(game->getBoard());
        updateBoard(board->getHorizontalLines(), board->getVerticalLines(), board->getCapturedBoxes());
    }
}