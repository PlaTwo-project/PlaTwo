#ifndef FANORONA_PAGE_H
#define FANORONA_PAGE_H

#include "UI/Games/BasePage/base_page.h"
#include "Logic/Game/Fanorona/FanoronaBoard/fanorona_board.h"
#include <QVector>

class QPushButton;

class FanoronaPage : public BasePage
{
    Q_OBJECT

public:
    explicit FanoronaPage(QWidget* parent = nullptr);
    ~FanoronaPage() override = default;

    void setupBoard(const int size) override;
    void updateFromGame(const class Game* game) override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    FanoronaBoard snapshot_board;
    bool chain_active;
    int chain_position;
    int current_player_id;

    int selected_position;
    int hovered_position;
    QVector<int> highlighted_positions;

    int positionAt(const QPoint& point) const;
    QPoint pixelOf(int position) const;
    void tryEmitMove(int from, int to);
    void updateHighlights();
    QVector<int> computeLegalTargets(int from) const;
    QVector<int> computeChainTargets() const;

signals:
    void moveRequested(int from, int to, int captureChoice);
};

#endif // FANORONA_PAGE_H