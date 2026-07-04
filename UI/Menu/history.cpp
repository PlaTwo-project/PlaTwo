#include "history.h"
#include "ui_history.h"

History::History(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::History)
{
    ui->setupUi(this);
}

History::~History()
{
    delete ui;
}

void History::on_pushButton_back_clicked()
{
    emit navigateToGameMenu();
}

void History::setHistory(const QList<MatchRecord>& list, int current_user_id, const GameName& game_type)
{
    ui->tableWidget_history->setRowCount(list.size());
    for (int i = 0; i < list.size(); i++) {
        const MatchRecord& rec = list[i];
        if (QString::fromLatin1(QMetaEnum::fromType<GameName>().valueToKey(static_cast<int>(game_type))) != rec.getGameType()) // convert 'GameName' to 'QString'
            continue;

        int opponent_id, your_score, opponent_score;
        QString role = getRole(rec, current_user_id), winner = getWinner(rec, current_user_id);

        if (rec.getHostId() == current_user_id) {
            opponent_id = rec.getGuestId();
            your_score = rec.getHostScore();
            opponent_score = rec.getGuestScore();
        }
        else {
            opponent_id = rec.getHostId();
            your_score = rec.getGuestScore();
            opponent_score = rec.getHostScore();
        }

        ui->tableWidget_history->setItem(i, 0, new QTableWidgetItem(QString::number(opponent_id)));
        ui->tableWidget_history->setItem(i, 1, new QTableWidgetItem(rec.getDate().toString("yyyy-MM-dd hh:mm")));
        ui->tableWidget_history->setItem(i, 2, new QTableWidgetItem(role));
        ui->tableWidget_history->setItem(i, 3, new QTableWidgetItem(QString::number(your_score)));
        ui->tableWidget_history->setItem(i, 4, new QTableWidgetItem(QString::number(opponent_score)));
        ui->tableWidget_history->setItem(i, 5, new QTableWidgetItem(winner));
    }
}

QString History::getRole(const MatchRecord& rec, int currentUserId) const
{
    if (rec.getHostId() == currentUserId)
        return "Host";
    else
        return "Guest";
}

QString History::getWinner(const MatchRecord& rec, int currentUserId) const
{
    if (rec.getWinnerId() == currentUserId)
        return "You";
    else
        return "Opponent";
}