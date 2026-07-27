#ifndef PAGE_TRANSITION_H
#define PAGE_TRANSITION_H

#include <QStackedWidget>
#include <QWidget>
#include <QObject>

void pageTransition(QStackedWidget *stack, QWidget *targetWidget, QObject *owner = nullptr);

#endif // PAGE_TRANSITION_H