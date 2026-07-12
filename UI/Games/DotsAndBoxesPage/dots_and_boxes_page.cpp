#include "dots_and_boxes_page.h"
#include <QPainter>
#include <QMouseEvent>
#include "Logic/Game/DotsAndBoxes/DotsAndBoxesLogic/dots_and_boxes.h"
DotsAndBoxesPage::DotsAndBoxesPage(QWidget *parent) : BasePage(parent) {}

void DotsAndBoxesPage::setupBoard(int size)
{
    BasePage::setupBoard(size);
    horizontal_lines.assign(board_size + 1, QVector<bool>(board_size, false));
    vertical_lines.assign(board_size, QVector<bool>(board_size + 1, false));
    captured_boxes.assign(board_size, QVector<int>(board_size, 0));

    update();
}

void DotsAndBoxesPage::updateBoard(const QVector<QVector<bool>> &horizontal_lines, const QVector<QVector<bool>> &vertical_lines, const QVector<QVector<int>> &captured_boxes)
{
    this->horizontal_lines = horizontal_lines;
    this->vertical_lines = vertical_lines;
    this->captured_boxes = captured_boxes;
    update();
}

void DotsAndBoxesPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(margin_offset, 30, QString("Status: %1").arg(turn_status_text));
    painter.drawText(margin_offset, 50, QString("Player 1: %1  |  Player 2: %2").arg(first_player_score).arg(second_player_score));

    for (int r = 0; r < board_size; ++r)
    {
        for (int c = 0; c < board_size; ++c)
        {
            if (captured_boxes[r][c] != 0)
            {
                QRect rect(margin_offset + c * cell_spacing, margin_offset + r * cell_spacing, cell_spacing, cell_spacing);
                QColor box_color;

                if (captured_boxes[r][c] == 1)
                    box_color = QColor(100, 149, 237, 100);
                else
                    box_color = QColor(255, 99, 71, 100);

                painter.fillRect(rect, box_color);

                if (captured_boxes[r][c] == 1)
                    painter.setPen(Qt::blue);
                else
                    painter.setPen(Qt::red);

                painter.drawText(rect, Qt::AlignCenter, (captured_boxes[r][c] == 1) ? "P1" : "P2");
            }
        }
    }

    QPen active_line_pen(Qt::black, 4);
    QPen empty_pen(Qt::lightGray, 2, Qt::DashLine);

    for (int r = 0; r <= board_size; ++r)
    {
        for (int c = 0; c < board_size; ++c)
        {
            int x1 = margin_offset + c * cell_spacing;
            int y1 = margin_offset + r * cell_spacing;

            if (horizontal_lines[r][c])
                painter.setPen(active_line_pen);
            else
                painter.setPen(empty_pen);

            painter.drawLine(x1, y1, x1 + cell_spacing, y1);
        }
    }

    for (int r = 0; r < board_size; ++r)
    {
        for (int c = 0; c <= board_size; ++c)
        {
            int x1 = margin_offset + c * cell_spacing;
            int y1 = margin_offset + r * cell_spacing;

            if (vertical_lines[r][c])
                painter.setPen(active_line_pen);
            else
                painter.setPen(empty_pen);

            painter.drawLine(x1, y1, x1, y1 + cell_spacing);
        }
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);

    for (int r = 0; r <= board_size; ++r)
    {
        for (int c = 0; c <= board_size; ++c)
        {
            painter.drawEllipse(QPoint(margin_offset + c * cell_spacing, margin_offset + r * cell_spacing), 5, 5);
        }
    }
}

void DotsAndBoxesPage::mousePressEvent(QMouseEvent *event)
{
    if (!is_input_enabled || board_size == 0)
        return;

    int click_x = event->position().x();
    int click_y = event->position().y();
    for (int r = 0; r <= board_size; ++r)
        for (int c = 0; c < board_size; ++c)
        {
            int mid_x = margin_offset + c * cell_spacing + cell_spacing / 2;
            int mid_y = margin_offset + r * cell_spacing;
            if (abs(click_x - mid_x) < 20 && abs(click_y - mid_y) < 10)
            {
                emit moveRequested(r, c, 0);
                return;
            }
        }

    for (int r = 0; r < board_size; ++r)
        for (int c = 0; c <= board_size; ++c)
        {
            int mid_x = margin_offset + c * cell_spacing;
            int mid_y = margin_offset + r * cell_spacing + cell_spacing / 2;
            if (abs(click_x - mid_x) < 10 && abs(click_y - mid_y) < 20)
            {
                emit moveRequested(r, c, 1);
                return;
            }
        }
}

void DotsAndBoxesPage::updateFromGame(const Game *game)
{

    const DotsAndBoxes *db_game = static_cast<const DotsAndBoxes *>(game);
    if (db_game && db_game->getBoard())
    {
        const DotsAndBoxesBoard *db_board = static_cast<DotsAndBoxesBoard *>(db_game->getBoard());
        updateBoard(db_board->getHorizontalLines(), db_board->getVerticalLines(), db_board->getCapturedBoxes());
    }
}
