#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewindow.h"
#include "leaderboard.h"
#include <QPushButton>
#include <QLayout>

class QGraphicsScene;
class QGraphicsView;

class GameController;
class GameWindow;
class Settings;
struct SettingsData;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_newGameBtn_clicked();

    void on_leaderboardBtn_clicked();

    void on_settingsBtn_clicked();

    void on_quitBtn_clicked();

private:
    Ui::MainWindow *ui;


    //QBoxLayout*layout;
    //QPushButton*newGame;
    //QPushButton*quit;
    //QPushButton*leaderboard;
    //QPushButton*settings;

    GameWindow*wnd;
    Settings*settingsWindow;
    SettingsData settingsData;
    Leaderboard*leaderboardWindow;

    /*void NewGame();
    void CreateButtons();
    void OpenSettings();
    void ShowLeaderBoard()*/;
    void SaveSettings(SettingsData settings);


};

#endif // MAINWINDOW_H
