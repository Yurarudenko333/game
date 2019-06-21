#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QDialog>
#include "gamecontroller.h"
#include <QString>
struct LeaderboardData;
namespace Ui {
class Leaderboard;
}

class Leaderboard : public QDialog
{
    Q_OBJECT

public:
    explicit Leaderboard(QWidget *parent = nullptr);
    ~Leaderboard();

private:
    Ui::Leaderboard *ui;
    void ShowLeaderboard();
};

#endif // LEADERBOARD_H
