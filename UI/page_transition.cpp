#include "page_transition.h"

#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>

void pageTransition(QStackedWidget *stack, QWidget *targetWidget, QObject *owner) {
    if (!stack || !targetWidget) return;

    QWidget *currentWidget = stack->currentWidget();

    if (!currentWidget || currentWidget == targetWidget) {
        stack->setCurrentWidget(targetWidget);
        return;
    }

    int width = stack->width();
    int height = stack->height();

    currentWidget->setGraphicsEffect(nullptr);
    targetWidget->setGraphicsEffect(nullptr);

    auto *effCurrent = new QGraphicsOpacityEffect(currentWidget);
    auto *effTarget = new QGraphicsOpacityEffect(targetWidget);

    currentWidget->setGraphicsEffect(effCurrent);
    targetWidget->setGraphicsEffect(effTarget);

    effTarget->setOpacity(0.0);

    targetWidget->setGeometry(0, 0, width, height);
    targetWidget->move(width * 0.12, 0);
    targetWidget->show();
    targetWidget->raise();

    auto *animNextPos = new QPropertyAnimation(targetWidget, "pos");
    animNextPos->setDuration(230);
    animNextPos->setStartValue(QPoint(width * 0.12, 0));
    animNextPos->setEndValue(QPoint(0, 0));
    animNextPos->setEasingCurve(QEasingCurve::OutCubic);

    auto *animNextFade = new QPropertyAnimation(effTarget, "opacity");
    animNextFade->setDuration(200);
    animNextFade->setStartValue(0.0);
    animNextFade->setEndValue(1.0);

    auto *animPrevFade = new QPropertyAnimation(effCurrent, "opacity");
    animPrevFade->setDuration(150);
    animPrevFade->setStartValue(1.0);
    animPrevFade->setEndValue(0.0);

    auto *group = new QParallelAnimationGroup(owner ? owner : stack);
    group->addAnimation(animNextPos);
    group->addAnimation(animNextFade);
    group->addAnimation(animPrevFade);

    QObject::connect(group, &QParallelAnimationGroup::finished, [stack, currentWidget, targetWidget, group]() {
        stack->setCurrentWidget(targetWidget);

        currentWidget->setGraphicsEffect(nullptr);
        targetWidget->setGraphicsEffect(nullptr);

        currentWidget->move(0, 0);
        targetWidget->move(0, 0);

        group->deleteLater();
    });

    group->start();
}