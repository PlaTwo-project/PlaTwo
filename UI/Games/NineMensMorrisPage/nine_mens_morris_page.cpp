#include "nine_mens_morris_page.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisLogic/nine_mens_morris.h"
#include "Logic/Game/NineMensMorris/NineMensMorrisMove/nine_mens_morris_move.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QVariantAnimation>
#include <QEasingCurve>
#include <cmath>

using namespace std;

static const int POINT_RADIUS = 16;
static const int CLICK_THRESHOLD = 34;
static const int ANIMATION_DURATION_MS = 450;

NineMensMorrisPage::NineMensMorrisPage(QWidget* parent) : BasePage(parent), awaiting_removal(false), placed_count_p1(0), placed_count_p2(0), current_player_id(PlayerSlot::NONE),
    selected_position(-1), hovered_position(-1), is_animating(false), anim_progress(0.0), anim_move_from(-1), anim_move_to(-1), anim_removed_position(-1), anim_moving_player_id(PlayerSlot::NONE) {
    setMouseTracking(true);
    position_owners = QVector<PlayerSlot>(NineMensMorrisBoard::TOTAL_POSITIONS, PlayerSlot::NONE);

    move_animation = new QVariantAnimation(this);
    move_animation->setDuration(ANIMATION_DURATION_MS);
    move_animation->setStartValue(0.0);
    move_animation->setEndValue(1.0);
    move_animation->setEasingCurve(QEasingCurve::InOutQuad);

    connect(move_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
        anim_progress = value.toReal();
        update();
    });

    connect(move_animation, &QVariantAnimation::finished, this, &NineMensMorrisPage::finishAnimation);
}

void NineMensMorrisPage::setupBoard(const int size) {
    BasePage::setupBoard(size);
    position_owners = QVector<PlayerSlot>(NineMensMorrisBoard::TOTAL_POSITIONS, PlayerSlot::NONE);
    snapshot_board.setPositionOwners(position_owners);
    selected_position = -1;
    highlighted_positions.clear();

    displayed_owners = position_owners;
    pending_owners = position_owners;
    if (move_animation)
        move_animation->stop();

    is_animating = false;
    anim_move_from = -1;
    anim_move_to = -1;
    anim_removed_position = -1;
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
        if (is_animating) {
            move_animation->stop();
            displayed_owners = pending_owners;
            is_animating = false;
        }

        QVector<PlayerSlot> new_owners = game_board->getPositionOwners();
        position_owners = new_owners;
        snapshot_board.setPositionOwners(position_owners);
        awaiting_removal = game->getAwaitingRemoval();
        placed_count_p1 = game->getPlacedCount(PlayerSlot::HOST);
        placed_count_p2 = game->getPlacedCount(PlayerSlot::GUEST);
        current_player_id = game->getCurrentPlayerId();
        selected_position = -1;

        startAnimation(new_owners);
        updateHighlights();
        update();
    }
}

void NineMensMorrisPage::startAnimation(const QVector<PlayerSlot>& new_owners) {
    if (displayed_owners.size() != new_owners.size()) {
        displayed_owners = new_owners;
        return;
    }

    int from = -1;
    int to = -1;
    PlayerSlot moved_player = PlayerSlot::NONE;

    for (int i = 0; i < new_owners.size(); ++i) {
        if (displayed_owners[i] != PlayerSlot::NONE && new_owners[i] == PlayerSlot::NONE) {
            if (from == -1)
                from = i;
        } else if (displayed_owners[i] == PlayerSlot::NONE && new_owners[i] != PlayerSlot::NONE) {
            to = i;
            moved_player = new_owners[i];
        }
    }

    if (from == -1 && to == -1) {
        displayed_owners = new_owners;
        return;
    }

    pending_owners = new_owners;
    if (from != -1 && to != -1) { // move
        anim_move_from = from;
        anim_move_to = to;
        anim_removed_position = -1;
        anim_moving_player_id = moved_player;
    } else if (to != -1 && from == -1) { // place
        anim_move_from = -1;
        anim_move_to = to;
        anim_removed_position = -1;
        anim_moving_player_id = moved_player;
    } else if (from != -1 && to == -1) { // remove
        anim_move_from = -1;
        anim_move_to = -1;
        anim_removed_position = from;
    }

    anim_progress = 0.0;
    is_animating = true;
    move_animation->stop();
    move_animation->start();
}

void NineMensMorrisPage::finishAnimation() {
    displayed_owners = pending_owners;
    is_animating = false;
    anim_move_from = -1;
    anim_move_to = -1;
    anim_removed_position = -1;
    updateHighlights();
    update();
}

void NineMensMorrisPage::updateHighlights() {
    highlighted_positions.clear();
    if (!is_input_enabled)
        return;

    if (awaiting_removal) {
        PlayerSlot opponent_id;
        if (current_player_id == PlayerSlot::HOST)
            opponent_id = PlayerSlot::GUEST;
        else
            opponent_id = PlayerSlot::HOST;

        for (int position = 0; position < NineMensMorrisBoard::TOTAL_POSITIONS; ++position) {
            if (snapshot_board.isPieceRemovable(position, opponent_id))
                highlighted_positions.append(static_cast<PlayerSlot>(position));
        }

        return;
    }

    int my_placed_count;
    if (current_player_id == PlayerSlot::HOST)
        my_placed_count = placed_count_p1;
    else
        my_placed_count = placed_count_p2;

    bool placing_phase = my_placed_count < NineMensMorris::PIECES_PER_PLAYER;
    if (selected_position == -1) {
        if (placing_phase) {
            for (int position = 0; position < NineMensMorrisBoard::TOTAL_POSITIONS; ++position) {
                if (snapshot_board.isEmpty(position))
                    highlighted_positions.append(static_cast<PlayerSlot>(position));
            }
        }
        return;
    }

    bool is_flying = snapshot_board.getPieceCount(current_player_id) == NineMensMorris::FLYING_THRESHOLD;
    for (int position = 0; position < NineMensMorrisBoard::TOTAL_POSITIONS; ++position) {
        if (!snapshot_board.isEmpty(position))
            continue;

        if (is_flying || snapshot_board.isProximate(selected_position, position))
            highlighted_positions.append(static_cast<PlayerSlot>(position));
    }
}

void NineMensMorrisPage::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Bauhaus LT Demi", 12));
    painter.drawText(margin_offset, 18, turn_status_text);
    painter.drawText(margin_offset, 35, QString("%1's Score: %2  |  %3's Score: %4").arg(first_player_name).arg(first_player_score).arg(second_player_name).arg(second_player_score));
    painter.drawText(margin_offset, 52, QString("%1's Time: %2  |  %3's Time: %4").arg(first_player_name).arg(first_player_time_str).arg(second_player_name).arg(second_player_time_str));
    QString removal_state = "";
    if (awaiting_removal)
        removal_state = "   [Remove an opponent piece]";
    painter.drawText(margin_offset, 69, QString("Placed - %1: %2/9  |  %3: %4/9%5").arg(first_player_name).arg(placed_count_p1).arg(second_player_name).arg(placed_count_p2).arg(removal_state));

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
        if (highlighted_positions.contains(static_cast<PlayerSlot>(position))) {
            QColor highlight_color;
            if (awaiting_removal)
                highlight_color = QColor(231, 76, 60);
            else
                highlight_color = QColor(46, 204, 113);

            painter.setPen(QPen(highlight_color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(QColor(highlight_color.red(), highlight_color.green(), highlight_color.blue(), 70));
            painter.drawEllipse(p, POINT_RADIUS + 6, POINT_RADIUS + 6);
        }

        if (position == selected_position) {
            painter.setPen(QPen(QColor(255, 200, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(p, POINT_RADIUS + 6, POINT_RADIUS + 6);
        } else if (position == hovered_position && is_input_enabled) {
            painter.setPen(QPen(Qt::darkGray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(p, POINT_RADIUS + 4, POINT_RADIUS + 4);
        }

        PlayerSlot owner = displayed_owners.value(position, PlayerSlot::NONE);
        if (is_animating && (position == anim_move_from || position == anim_move_to)) {
            painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::white);
            painter.drawEllipse(p, 5, 5);
            continue;
        }

        if (is_animating && position == anim_removed_position) {
            qreal fade = 1.0 - anim_progress;
            QColor c;
            if (owner == PlayerSlot::HOST)
                c = QColor(100, 149, 237);
            else
                c = QColor(255, 99, 71);

            c.setAlphaF(fade);
            painter.setPen(Qt::NoPen);
            painter.setBrush(c);
            int r = static_cast<int>(POINT_RADIUS * (0.5 + 0.5 * fade));
            painter.drawEllipse(p, r, r);
            continue;
        }

        if (owner == PlayerSlot::NONE) {
            painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::white);
            painter.drawEllipse(p, 5, 5);
        } else {
            painter.setPen(Qt::black);
            if (owner == PlayerSlot::HOST)
                painter.setBrush(QColor(100, 149, 237));
            else
                painter.setBrush(QColor(255, 99, 71));

            painter.drawEllipse(p, POINT_RADIUS, POINT_RADIUS);
        }
    }

    if (is_animating && anim_move_to != -1) {
        QPointF current_point;
        if (anim_move_from != -1) {
            QPointF p1 = positionToCoordinates(anim_move_from);
            QPointF p2 = positionToCoordinates(anim_move_to);
            current_point = p1 + (p2 - p1) * anim_progress;
        } else {
            current_point = positionToCoordinates(anim_move_to);
            painter.setOpacity(anim_progress);
        }

        painter.setPen(Qt::black);

        if (anim_moving_player_id == PlayerSlot::HOST)
            painter.setBrush(QColor(100, 149, 237));
        else
            painter.setBrush(QColor(255, 99, 71));

        painter.drawEllipse(current_point, POINT_RADIUS, POINT_RADIUS);
        painter.setOpacity(1.0);
    }
}

void NineMensMorrisPage::mousePressEvent(QMouseEvent* event) {
    if (!is_input_enabled || is_animating)
        return;

    int clicked = getPositionClicked(event->position().toPoint());
    if (clicked < 0)
        return;

    if (awaiting_removal) {
        emit moveRequested(clicked, -1, static_cast<int>(MoveType::REMOVE));
        return;
    }

    if (selected_position == -1) {
        PlayerSlot owner = position_owners.value(clicked, PlayerSlot::NONE);
        if (owner == current_player_id) {
            selected_position = clicked;
            updateHighlights();
            update();
        }
        else if (owner == PlayerSlot::NONE)
            emit moveRequested(clicked, -1, static_cast<int>(MoveType::PLACE));

        return;
    }

    if (clicked == selected_position) {
        selected_position = -1;
        updateHighlights();
        update();
        return;
    }

    PlayerSlot owner = position_owners.value(clicked, PlayerSlot::NONE);
    if (owner == current_player_id) {
        selected_position = clicked;
        updateHighlights();
        update();
        return;
    }

    if (owner == PlayerSlot::NONE) {
        emit moveRequested(selected_position, clicked, static_cast<int>(MoveType::MOVE));
        selected_position = -1;
        updateHighlights();
    }
}

void NineMensMorrisPage::mouseMoveEvent(QMouseEvent* event) {
    if (is_animating)
        return;

    int new_hover = getPositionClicked(event->position().toPoint());
    if (new_hover != hovered_position) {
        hovered_position = new_hover;
        update();
    }
}