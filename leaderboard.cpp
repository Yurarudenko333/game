#include "leaderboard.h"
#include "ui_leaderboard.h"
#include "fstream"
#include <QDebug>
#include <QFile>
#include <QFont>
using std::string;

Leaderboard::Leaderboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Leaderboard)
{
    ui->setupUi(this);
    ShowLeaderboard();
    this->setMaximumSize(this->size());
    this->setMinimumSize(this->size());
}

Leaderboard::~Leaderboard()
{
    delete ui;
}

//reads leaderboard info from file
void Leaderboard::ShowLeaderboard()
{
       LeaderboardData data;

       //sets the font
       QFont f;
       f.setBold(true);
       f.setItalic(true);

       QFile file("leaderboard.txt");//opening of file
       if ((file.exists())&&(file.open(QIODevice::ReadOnly)))//checks existing of the file
       {
           while(!file.atEnd())
           {
               //adding info to the text Browser widget
               ui->leaderboard->setFont(f);
               ui->leaderboard->append("Name : " + file.readLine() + "\n");
               ui->leaderboard->append("Time : " + file.readLine() + "\n");
               ui->leaderboard->append("Difficult : " + file.readLine() + "\n");
               ui->leaderboard->append("Score : " + file.readLine() + "\n");
               ui->leaderboard->append("Field size : " + file.readLine() + "\n");
               ui->leaderboard->append("---------------------------------------------");
           }
       }
       file.close();
}
