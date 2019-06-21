#include <QGraphicsView>
#include <QTimer>
#include <qaction.h>
#include <qmenubar.h>
#include <qapplication.h>
#include <qmessagebox.h>

#include "constants.h"
#include "gamecontroller.h"
#include "mainwindow.h"
#include"ui_mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setting size for the window
    this->setMaximumSize(this->size());
    this->setMinimumSize(this->size());
    //CreateButtons();
    settingsData = SettingsData(5,5,Qt::darkMagenta,Qt::gray,"Medium");//default settings of the game
}

MainWindow::~MainWindow()
{
    
}

/*void MainWindow::NewGame()
{
    wnd = new GameWindow(settingsData);
    wnd->show();
}

void MainWindow::OpenSettings()
{
    settingsWindow = new Settings;
    connect(settingsWindow,&Settings::sendSettings,this,&MainWindow::SaveSettings);
    settingsWindow->show();
}

void MainWindow::ShowLeaderBoard()
{
    leaderboardWindow = new Leaderboard;
    leaderboardWindow->show();
}

void MainWindow::CreateButtons()
{
    newGame = new QPushButton("New Game",this);
    connect(newGame, &QPushButton::clicked, this, &MainWindow::NewGame);

    leaderboard = new QPushButton("Leaderboard",this);
    connect(leaderboard, &QPushButton::clicked, this, &MainWindow::ShowLeaderBoard);

    settings = new QPushButton("Settings",this);
    connect(settings, &QPushButton::clicked, this, &MainWindow::OpenSettings);

    quit = new QPushButton("Quit",this);
    connect(quit, &QPushButton::clicked, this, &QApplication::quit);

    this->layout = new QVBoxLayout; // or QVBoxLayout if you prefer
    this->layout->addWidget(newGame);
    this->layout->addWidget(leaderboard);
    this->layout->addWidget(settings);
    this->layout->addWidget(quit);

    // set central widget of the main window
    QWidget *central_widget = new QWidget;
    central_widget->setLayout(this->layout);
    this->setCentralWidget(central_widget);
}*/

//function which receives and saves settings from setting window
void MainWindow::SaveSettings(SettingsData settings)
{
    settingsData = settings;
}

//new game button handler
void MainWindow::on_newGameBtn_clicked()
{
    wnd = new GameWindow(settingsData);
    wnd->show();
}

//leaderboard button handler
void MainWindow::on_leaderboardBtn_clicked()
{
    leaderboardWindow = new Leaderboard;
    leaderboardWindow->show();
}

//settings button handler
void MainWindow::on_settingsBtn_clicked()
{
    settingsWindow = new Settings;
    connect(settingsWindow,&Settings::sendSettings,this,&MainWindow::SaveSettings);
    settingsWindow->show();
}

//quit button handler
void MainWindow::on_quitBtn_clicked()
{
    QApplication::quit();
}
