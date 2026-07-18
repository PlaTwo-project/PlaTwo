#include "nine_mens_morris_page.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisLogic/nine_mens_morris.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>

using namespace std;

static const int POINT_RADIUS = 10;
static const int CLICK_THRESHOLD = 16;

NineMensMorrisPage::NineMensMorrisPage(QWidget* parent)
    : BasePage(parent), awaiting_removal(false), placed_count_p1(0), placed_count_p2(0), selected_position(-1), hovered_position(-1) {
    setMouseTracking(true);
    position_owners.assign(NineMensMorrisBoard::TOTAL_POSITIONS, 0);
}

void NineMensMorrisPage::setupBoard(const int size) {
    BasePage::setupBoard(size);
    position_owners.assign(NineMensMorrisBoard::TOTAL_POSITIONS, 0);
    selected_position = -1;
    update();
}

QPoint NineMensMorrisPage::positionToCoordinates(int position) const {
    QPair<int, int> coordinates = NineMensMorrisBoard::getCoordinates(position);
    return QPoint(margin_offset + coordinates.first * cell_spacing, margin_offset + coordinates.second * cell_spacing);
}

int NineMensMorrisPage::getPositionClicked(const QPoint& point) const {
    for (int position = 0; position < NineMensMorrisBoard::TOTAL_POSITIONS; ++position) {
        QPoint p = positionToCoordinates(position);
        double distance = hypot(point.x() - p.x(), point.y() - p.y());
        if (distance <= CLICK_THRESHOLD)
            return position;
    }

    return -1;
}

void NineMensMorrisPage::updateFromGame(const Game* main_game) {
    const NineMensMorris* game = static_cast<const NineMensMorris*>(main_game);
    if (game && game->getBoard()) {
        const NineMensMorrisBoard* game_board = static_cast<NineMensMorrisBoard*>(game->getBoard());
        position_owners = game_board->getPositionOwners();
        awaiting_removal = game->getAwaitingRemoval();
        placed_count_p1 = game->getPlacedCount(1);
        placed_count_p2 = game->getPlacedCount(2);
        selected_position = -1;
        update();
    }
}

void NineMensMorrisPage::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(margin_offset, 25, QString("Status: %1").arg(turn_status_text));
    painter.drawText(margin_offset, 45, QString("Player 1: %1  |  Player 2: %2").arg(first_player_score).arg(second_player_score));
    painter.drawText(margin_offset, 65, QString("Placed - P1: %1/9  P2: %2/9%3").arg(placed_count_p1).arg(placed_count_p2).arg(awaiting_removal ? "   [Remove an opponent piece]" : ""));

    // board lines
    QPen line_pen(Qt::darkGray, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(line_pen);
    for (int from = 0; from < NineMensMorrisBoard::TOTAL_POSITIONS; ++from) {
        for (int to : NineMensMorrisBoard::getProximityList()[from]) {
            if (to <= from)
                continue;

            painter.drawLine(positionToCoordinates(from), positionToCoordinates(to));
        }
    }

    // points and pieces
    for (int position = 0; position < NineMensMorrisBoard::TOTAL_POSITIONS; ++position) {
        QPoint p = positionToCoordinates(position);
        if (position == selected_position) {
            painter.setPen(QPen(QColor(255, 200, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(p, POINT_RADIUS + 6, POINT_RADIUS + 6);
        }
        else if (position == hovered_position && is_input_enabled) {
            painter.setPen(QPen(Qt::darkGray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(p, POINT_RADIUS + 4, POINT_RADIUS + 4);
        }

        int owner = position_owners.value(position, 0);
        if (owner == 0) {
            painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::white);
            painter.drawEllipse(p, 5, 5);
        }
        else {
            painter.setPen(Qt::black);
            if (owner == 1)
                painter.setBrush(QColor(100, 149, 237));
            else
                painter.setBrush(QColor(255, 99, 71));
            painter.drawEllipse(p, POINT_RADIUS, POINT_RADIUS);
        }
    }
}

void NineMensMorrisPage::mousePressEvent(QMouseEvent* event) {
    if (!is_input_enabled)
        return;

    int clicked = getPositionClicked(event->position().toPoint());
    if (clicked < 0)
        return;

    if (awaiting_removal) {
        emit moveRequested(clicked, -1, 2); // remove
        return;
    }

    if (selected_position == -1) {
        if (position_owners.value(clicked, 0) != 0) {
            selected_position = clicked;
            update();
        }
        else
            emit moveRequested(clicked, -1, 0); // place
        return;
    }

    if (clicked == selected_position) {
        selected_position = -1;
        update();
        return;
    }

    if (position_owners.value(clicked, 0) != 0) {
        selected_position = clicked;
        update();
        return;
    }

    emit moveRequested(selected_position, clicked, 1); // move
    selected_position = -1;
}

void NineMensMorrisPage::mouseMoveEvent(QMouseEvent* event) {
    int new_hover = getPositionClicked(event->position().toPoint());
    if (new_hover != hovered_position) {
        hovered_position = new_hover;
        update();
    }
}