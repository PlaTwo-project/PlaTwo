#include "fanorona_page.h"
#include "Logic/Game/Fanorona/FanoronaLogic/fanorona.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QVariantAnimation>
#include <QEasingCurve>

static const int PIECE_RADIUS = 20;
static const int CLICK_THRESHOLD = 35;
static const int MOVE_ANIMATION_DURATION_MS = 450;

FanoronaPage::FanoronaPage(QWidget* parent) : BasePage(parent), chain_active(false), chain_position(-1), current_player_id(PlayerSlot::NONE),
    selected_position(-1), hovered_position(-1), is_animating(false), anim_progress(0.0), anim_move_from(-1), anim_move_to(-1), anim_moving_player_id(PlayerSlot::NONE) {
    setMouseTracking(true);
    redPieceTexture.load(":/textures/Red.png");
    bluePieceTexture.load(":/textures/Blue.png");
    board_background.load(":/backgrounds/Board3.png");

    move_animation = new QVariantAnimation(this);
    move_animation->setDuration(MOVE_ANIMATION_DURATION_MS);
    move_animation->setStartValue(0.0);
    move_animation->setEndValue(1.0);
    move_animation->setEasingCurve(QEasingCurve::InOutQuad);

    connect(move_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value) {
        anim_progress = value.toReal();
        update();
    });

    connect(move_animation, &QVariantAnimation::finished, this, &FanoronaPage::finishAnimation);
}

void FanoronaPage::setupBoard(const int size) {
    BasePage::setupBoard(size);
    snapshot_board.clear();

    displayed_occupants = snapshot_board.getOccupants();
    pending_occupants = displayed_occupants;
    move_animation->stop();
    is_animating = false;
    anim_move_from = -1;
    anim_move_to = -1;
    anim_captured_positions.clear();

    selected_position = -1;
    chain_active = false;
    chain_position = -1;
    highlighted_positions.clear();
}

QPoint FanoronaPage::pixelOf(int position) const {
    int row = FanoronaBoard::rowOf(position);
    int col = FanoronaBoard::colOf(position);
    return QPoint(margin_offset + col * cell_spacing, 28 + margin_offset + row * cell_spacing);
}

int FanoronaPage::positionAt(const QPoint& point) const {
    for (int position = 0; position < FanoronaBoard::TOTAL_POSITIONS; ++position) {
        QPoint p = pixelOf(position);
        double distance = std::hypot(point.x() - p.x(), point.y() - p.y());
        if (distance <= CLICK_THRESHOLD)
            return position;
    }
    return -1;
}

void FanoronaPage::resizeEvent(QResizeEvent* event) {
    BasePage::resizeEvent(event);
}

void FanoronaPage::updateFromGame(const Game* game) {
    const Fanorona* fanorona_game = dynamic_cast<const Fanorona*>(game);
    if (!fanorona_game || !fanorona_game->getBoard())
        return;

    const FanoronaBoard* board = dynamic_cast<const FanoronaBoard*>(fanorona_game->getBoard());
    if (!board)
        return;

    if (is_animating) {
        move_animation->stop();
        displayed_occupants = pending_occupants;
        is_animating = false;
    }

    QVector<PlayerSlot> new_occupants = board->getOccupants();
    snapshot_board.setOccupants(new_occupants);

    chain_active = fanorona_game->isChainActive();
    chain_position = fanorona_game->getChainPosition();
    current_player_id = fanorona_game->currentPlayerId();
    selected_position = -1;

    startMoveAnimation(new_occupants);

    updateHighlights();
    update();
}

void FanoronaPage::startMoveAnimation(const QVector<PlayerSlot>& new_occupants) {
    if (displayed_occupants.size() != new_occupants.size()) {
        displayed_occupants = new_occupants;
        return;
    }

    int to = -1;
    PlayerSlot moved_player = PlayerSlot::NONE;
    for (int position = 0; position < new_occupants.size(); ++position) {
        if (displayed_occupants[position] == PlayerSlot::NONE && new_occupants[position] != PlayerSlot::NONE) {
            to = position;
            moved_player = new_occupants[position];
            break;
        }
    }

    if (to == -1) {
        displayed_occupants = new_occupants;
        return;
    }

    int from = -1;
    QVector<PlayerSlot> captured;
    for (int position = 0; position < new_occupants.size(); ++position) {
        if (displayed_occupants[position] != PlayerSlot::NONE && new_occupants[position] == PlayerSlot::NONE) {
            if (displayed_occupants[position] == moved_player)
                from = position;
            else
                captured.append(static_cast<PlayerSlot>(position));
        }
    }

    if (from == -1) {
        displayed_occupants = new_occupants;
        return;
    }

    pending_occupants = new_occupants;
    anim_move_from = from;
    anim_move_to = to;
    anim_moving_player_id = moved_player;
    anim_captured_positions = captured;
    anim_progress = 0.0;
    is_animating = true;

    move_animation->stop();
    move_animation->start();
}

void FanoronaPage::finishAnimation() {
    displayed_occupants = pending_occupants;
    is_animating = false;
    anim_move_from = -1;
    anim_move_to = -1;
    anim_captured_positions.clear();
    updateHighlights();
    update();
}

void FanoronaPage::updateHighlights() {
    highlighted_positions.clear();
    if (!is_input_enabled)
        return;

    if (chain_active) {
        highlighted_positions = computeChainTargets();
        return;
    }

    if (selected_position != -1)
        highlighted_positions = computeLegalTargets(selected_position);
}

QVector<int> FanoronaPage::computeLegalTargets(int from) const {
    QVector<int> targets;
    bool force_capture = snapshot_board.hasAnyCaptureAvailable(current_player_id);
    for (int neighbour : snapshot_board.getNeighbours(from)) {
        if (!snapshot_board.isEmpty(neighbour))
            continue;

        bool captures = snapshot_board.canApproachCapture(from, neighbour) || snapshot_board.canWithdrawalCapture(from, neighbour);
        if (force_capture && !captures)
            continue;

        targets.append(neighbour);
    }

    return targets;
}

QVector<int> FanoronaPage::computeChainTargets() const {
    QVector<int> targets;
    for (int neighbour : snapshot_board.getNeighbours(chain_position)) {
        if (!snapshot_board.isEmpty(neighbour))
            continue;

        if (snapshot_board.canApproachCapture(chain_position, neighbour) || snapshot_board.canWithdrawalCapture(chain_position, neighbour))
            targets.append(neighbour);
    }

    return targets;
}

void FanoronaPage::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QRect board_rect(margin_offset - 90, margin_offset - 105, ((FanoronaBoard::COLS - 1) * cell_spacing) + 180, ((FanoronaBoard::ROWS - 1) * cell_spacing) + 280);
    drawBoardBackground(painter, board_rect, board_background);

    painter.setPen(Qt::black);
    painter.setFont(QFont("Bauhaus LT Demi", 12));
    painter.drawText(margin_offset, 25, turn_status_text);
    painter.drawText(margin_offset, 43, QString("Captured - %1's Score: %2  |  %3's Score: %4").arg(first_player_name).arg(first_player_score).arg(second_player_name).arg(second_player_score));
    painter.drawText(margin_offset, 61, QString("%1's Time: %2  |  %3's Time: %4").arg(first_player_name).arg(first_player_time_str).arg(second_player_name).arg(second_player_time_str));
    if (chain_active)
        painter.drawText(margin_offset, 79, "Capture chain in progress - continue capturing");

    painter.setPen(QPen(Qt::darkGray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for (int position = 0; position < FanoronaBoard::TOTAL_POSITIONS; ++position) {
        for (int neighbour : snapshot_board.getNeighbours(position)) {
            if (neighbour > position)
                painter.drawLine(pixelOf(position), pixelOf(neighbour));
        }
    }

    int offset = 15, offset2 = -7;
    for (int position = 0; position < FanoronaBoard::TOTAL_POSITIONS; ++position) {
        QPoint p = pixelOf(position);
        if (position == selected_position || (chain_active && position == chain_position)) {
            painter.setPen(QPen(QColor(255, 200, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(p, PIECE_RADIUS + 6, PIECE_RADIUS + 6);
        } else if (position == hovered_position && is_input_enabled) {
            painter.setPen(QPen(Qt::darkGray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(p, PIECE_RADIUS + 4, PIECE_RADIUS + 4);
        }

        if (highlighted_positions.contains(position)) {
            painter.setPen(QPen(QColor(46, 204, 113), 3));
            painter.setBrush(QColor(46, 204, 113, 80));
            painter.drawEllipse(p, PIECE_RADIUS + 3, PIECE_RADIUS + 3);
        }

        if (is_animating && (position == anim_move_from || position == anim_move_to)) {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::white);
            painter.drawEllipse(p, 4, 4);
            continue;
        }

        if (is_animating && anim_captured_positions.contains(static_cast<PlayerSlot>(position))) {
            qreal fade = 1.0 - anim_progress;
            int radius = static_cast<int>(PIECE_RADIUS * (0.5 + 0.5 * fade));

            QColor color;
            if (displayed_occupants[position] == PlayerSlot::HOST)
                color = QColor(255, 99, 71);
            else
                color = QColor(100, 149, 237);

            painter.setOpacity(fade);
            if (displayed_occupants[position] == PlayerSlot::HOST)
                painter.drawPixmap(p.x() - radius + offset2, p.y() - radius + offset2 - 2, offset + radius * 2, offset + radius * 2, redPieceTexture);
            else
                painter.drawPixmap(p.x() - radius + offset2, p.y() - radius + offset2 - 2, offset + radius * 2, offset + radius * 2, bluePieceTexture);

            painter.setOpacity(1.0);
            continue;
        }

        PlayerSlot occupant = displayed_occupants[position];
        if (occupant == PlayerSlot::NONE) {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::white);
            painter.drawEllipse(p, 4, 4);
        } else {
            if (occupant == PlayerSlot::HOST)
                painter.drawPixmap(p.x() - PIECE_RADIUS + offset2, p.y() - PIECE_RADIUS + offset2 - 2, offset + PIECE_RADIUS * 2, offset + PIECE_RADIUS * 2, redPieceTexture);
            else
                painter.drawPixmap(p.x() - PIECE_RADIUS + offset2, p.y() - PIECE_RADIUS + offset2 - 2, offset + PIECE_RADIUS * 2, offset + PIECE_RADIUS * 2, bluePieceTexture);
        }
    }

    if (is_animating) {
        QPointF from_point = pixelOf(anim_move_from);
        QPointF to_point = pixelOf(anim_move_to);
        QPointF current_point = from_point + (to_point - from_point) * anim_progress;

        if (anim_moving_player_id == PlayerSlot::HOST)
            painter.drawPixmap(current_point.x() - PIECE_RADIUS + offset2, current_point.y() - PIECE_RADIUS + offset2 - 2, offset + PIECE_RADIUS * 2, offset + PIECE_RADIUS * 2, redPieceTexture);
        else
            painter.drawPixmap(current_point.x() - PIECE_RADIUS + offset2, current_point.y() - PIECE_RADIUS + offset2 - 2, offset + PIECE_RADIUS * 2, offset + PIECE_RADIUS * 2, bluePieceTexture);
    }
}

void FanoronaPage::tryEmitMove(int from, int to) {
    bool can_approach = snapshot_board.canApproachCapture(from, to);
    bool can_withdrawal = snapshot_board.canWithdrawalCapture(from, to);
    int capture_choice = static_cast<int>(FanoronaCaptureType::NONE);

    if (can_approach && can_withdrawal){
        QMessageBox box(this);
        box.setWindowTitle("Choose Capture");
        box.setText("Both an approach and a withdrawal capture are available. Which one would you like to play?");
        QPushButton* approach_button = box.addButton("Approach", QMessageBox::AcceptRole);
        QPushButton* withdrawal_button = box.addButton("Withdrawal", QMessageBox::RejectRole);
        box.exec();

        if (box.clickedButton() == approach_button)
            capture_choice = static_cast<int>(FanoronaCaptureType::APPROACH);
        else if (box.clickedButton() == withdrawal_button)
            capture_choice = static_cast<int>(FanoronaCaptureType::WITHDRAWAL);
        else
            return;
    }

    emit moveRequested(from, to, capture_choice);
}

void FanoronaPage::mousePressEvent(QMouseEvent* event) {
    if (!is_input_enabled || is_animating)
        return;

    int clicked = positionAt(event->position().toPoint());
    if (clicked < 0)
        return;

    if (chain_active){
        if (clicked == chain_position)
            return;

        if (snapshot_board.isEmpty(clicked) && (snapshot_board.canApproachCapture(chain_position, clicked) || snapshot_board.canWithdrawalCapture(chain_position, clicked)))
            tryEmitMove(chain_position, clicked);

        return;
    }

    if (selected_position == -1) {
        if (snapshot_board.getOccupant(clicked) == current_player_id){
            selected_position = clicked;
            updateHighlights();
            update();
        }
        return;
    }

    if (clicked == selected_position) {
        selected_position = -1;
        updateHighlights();
        update();
        return;
    }

    if (snapshot_board.getOccupant(clicked) == current_player_id) {
        selected_position = clicked;
        updateHighlights();
        update();
        return;
    }

    if (snapshot_board.getOccupant(clicked) == PlayerSlot::NONE) {
        tryEmitMove(selected_position, clicked);
        selected_position = -1;
        updateHighlights();
    }
}

void FanoronaPage::mouseMoveEvent(QMouseEvent* event) {
    int new_hover = positionAt(event->position().toPoint());
    if (new_hover != hovered_position) {
        hovered_position = new_hover;
        update();
    }
}