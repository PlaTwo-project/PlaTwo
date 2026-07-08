#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include "Logic/match_record.h"
#include "Logic/Game/game_name.h"

namespace Ui {
class History;
}

class History : public QWidget
{
    Q_OBJECT

public:
    explicit History(QWidget *parent = nullptr);
    ~History();

    void setHistory(const QList<MatchRecord>& list, int current_user_id, GameName game_type);

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
