#ifndef DOTS_AND_BOXES_PAGE_H
#define DOTS_AND_BOXES_PAGE_H

#include "UI/Games/BasePage/base_page.h"
#include <QVector>

using namespace std;

class DotsAndBoxesPage : public BasePage
{
    Q_OBJECT

public:
    explicit DotsAndBoxesPage(QWidget *parent = nullptr);
    ~DotsAndBoxesPage() override = default;

    void setupBoard(const int size) override;
    void updateBoard(const QVector<QVector<bool>> &horizontal_lines, const QVector<QVector<bool>> &vertical_lines, const QVector<QVector<int>> &captured_boxes);
    void updateFromGame(const class Game *game) override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QVector<QVector<bool>> horizontal_lines;
    QVector<QVector<bool>> vertical_lines;
    QVector<QVector<int>> captured_boxes;
};

#endif // DOTS_AND_BOXES_PAGE_H