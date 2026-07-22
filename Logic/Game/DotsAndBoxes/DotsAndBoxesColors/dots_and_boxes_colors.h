#ifndef DOTS_AND_BOXES_COLORS_H
#define DOTS_AND_BOXES_COLORS_H

#include <QColor>
#include <QVector>

namespace DotsAndBoxesColors {
    inline const QVector<QColor>& palette() {
        static const QVector<QColor> colors = {
            QColor(220, 53, 69), // Red
            QColor(0, 123, 255), // Blue
            QColor(40, 167, 69), // Green
            QColor(253, 126, 20), // Orange
            QColor(111, 66, 193), // Purple
            QColor(255, 193, 7), // Yellow
            QColor(32, 201, 151), // Teal
            QColor(214, 51, 132), // Pink
            QColor(121, 85, 72) // Brown
        };
        return colors;
    }

    inline QColor colorAt(int index) {
        const QVector<QColor>& colors = palette();
        return colors[index];
    }
}

#endif // DOTS_AND_BOXES_COLORS_H