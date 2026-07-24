#include "fanorona_page.h"
#include "Logic/Game/Fanorona/FanoronaLogic/fanorona.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <cmath>

static const int PIECE_RADIUS = 16;
static const int CLICK_THRESHOLD = 30;

FanoronaPage::FanoronaPage(QWidget* parent) : BasePage(parent), chain_active(false), chain_position(-1), current_player_id(0), selected_position(-1), hovered_position(-1) {
    setMouseTracking(true);
}

void FanoronaPage::setupBoard(const int size) {
    BasePage::setupBoard(size);
    snapshot_board.clear();
    selected_position = -1;
    chain_active = false;
    chain_position = -1;
    highlighted_positions.clear();
}

QPoint FanoronaPage::pixelOf(int position) const {
    int row = FanoronaBoard::rowOf(position);
    int col = FanoronaBoard::colOf(position);
    return QPoint(margin_offset + col * cell_spacing, margin_offset + row * cell_spacing);
}

int FanoronaPage::positionAt(const QPoint& point) const {
    for (int position = 0; position < FanoronaBoard::TOTAL_POSITIONS; ++position){
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

    snapshot_board.setOccupants(board->getOccupants());
    chain_active = fanorona_game->isChainActive();
    chain_position = fanorona_game->getChainPosition();
    current_player_id = fanorona_game->currentPlayerId();
    selected_position = -1;

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
    painter.setPen(Qt::black);
    painter.setFont(QFont("LTe50403.ttf", 12));
    painter.drawText(margin_offset, 25, turn_status_text);
    painter.drawText(margin_offset, 43, QString("Captured - %1's Score: %2  |  %3's Score: %4").arg(first_player_name).arg(first_player_score).arg(second_player_name).arg(second_player_score));
    painter.drawText(margin_offset, 61, QString("%1's Time: %2  |  %3's Time: %4").arg(first_player_name).arg(first_player_time_str).arg(second_player_name).arg(second_player_time_str));

    if (chain_active)
        painter.drawText(margin_offset, 79, "Capture chain in progress - continue capturing");

    painter.setPen(QPen(Qt::darkGray, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for (int position = 0; position < FanoronaBoard::TOTAL_POSITIONS; ++position)
        for (int neighbour : snapshot_board.getNeighbours(position))
            if (neighbour > position)
                painter.drawLine(pixelOf(position), pixelOf(neighbour));

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

        int occupant = snapshot_board.getOccupant(position);
        if (occupant == 0) {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::white);
            painter.drawEllipse(p, 4, 4);
        } else {
            painter.setPen(Qt::black);
            if (occupant == 1)
                painter.setBrush(QColor(255, 99, 71));
            else
                painter.setBrush(QColor(100, 149, 237));

            painter.drawEllipse(p, PIECE_RADIUS, PIECE_RADIUS);
        }
    }
}

void FanoronaPage::tryEmitMove(int from, int to) {
    bool can_approach = snapshot_board.canApproachCapture(from, to);
    bool can_withdrawal = snapshot_board.canWithdrawalCapture(from, to);
    int capture_choice = -1;
    if (can_approach && can_withdrawal){
        QMessageBox box(this);
        box.setWindowTitle("Choose Capture");
        box.setText("Both an approach and a withdrawal capture are available. Which one would you like to play?");
        QPushButton* approach_button = box.addButton("Approach", QMessageBox::AcceptRole);
        QPushButton* withdrawal_button = box.addButton("Withdrawal", QMessageBox::RejectRole);
        box.exec();

        if (box.clickedButton() == approach_button)
            capture_choice = 0;
        else if (box.clickedButton() == withdrawal_button)
            capture_choice = 1;
        else
            return;
    }

    emit moveRequested(from, to, capture_choice);
}

void FanoronaPage::mousePressEvent(QMouseEvent* event) {

    if (!is_input_enabled)
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

    if (snapshot_board.getOccupant(clicked) == 0) {
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