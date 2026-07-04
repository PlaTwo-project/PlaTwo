#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include <QMetaEnum>
#include "Logic/match_record.h"

namespace Ui {
class History;
}

class History : public QWidget
{
    Q_OBJECT

public:
    enum class GameName { BoxesAndDots, NineMensMorris, Fanorona };
    Q_ENUM(GameName)

    explicit History(QWidget *parent = nullptr);
    void setHistory(const QList<MatchRecord>& list, int current_user_id, const GameName& game_type);
    ~History();

signals:
    void navigateToGameMenu();

private slots:
    void on_pushButton_back_clicked();

private:
    Ui::History *ui;

    QString getRole(const MatchRecord& rec, int currentUserId) const;
    QString getWinner(const MatchRecord& rec, int currentUserId) const;
};

#endif // HISTORY_H
