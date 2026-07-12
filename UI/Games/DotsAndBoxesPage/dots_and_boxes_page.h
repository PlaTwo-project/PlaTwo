#ifndef DOTS_AND_BOXES_PAGE_H
#define DOTS_AND_BOXES_PAGE_H

#include "UI/Games/BasePage/base_page.h"
#include <QVector>

using namespace std;

class DotsAndBoxesPage : public BasePage {
    Q_OBJECT

public:
    explicit DotsAndBoxesPage(QWidget* parent = nullptr);
    ~DotsAndBoxesPage() override = default;

    void setupBoard(const int size) override;
    void updateBoard(const QVector<QVector<bool>>& horizontal_edges, const QVector<QVector<bool>>& vertical_edges, const QVector<QVector<int>>& captured_boxes);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QVector<QVector<bool>> horizontal_edges;
    QVector<QVector<bool>> vertical_edges;
    QVector<QVector<int>> captured_boxes;
};

#endif // DOTS_AND_BOXES_PAGE_H