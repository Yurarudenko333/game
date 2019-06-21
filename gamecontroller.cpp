#include <QEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QMessageBox>
#include <QInputDialog>
#include "gamecontroller.h"
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTime>

GameController::GameController(SettingsData settings,QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    scene(scene),
    snake(new Snake(settings.snakeSpeed,settings.fieldSize,settings.snakeColor,*this)),//reating new snake
    isPause(false)
{
    //seed for random
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    timer.start( 1000/33 );
    set = settings;

    if(set.difficult == "Easy"){pointsForSimpleFood = 1;}
    else if(set.difficult == "Medium"){pointsForSimpleFood = 1;}
    else if(set.difficult == "Hard"){pointsForSimpleFood = 2;}
    else if(set.difficult == "Extreme"){pointsForSimpleFood = 3;}
    else {pointsForSimpleFood = 1;}

    addNewFood();

    addWalls();

    //add snake on the scene
    scene.addItem(snake);
    //An event filter is an object that
    //receives all events that are sent to this object.
    scene.installEventFilter(this);

    resume();

    connect(&timerForStopwatch,&QTimer::timeout,this,&GameController::SendStopwatchData);
    stopWatch = new stopwatch;//creates stopwatch
}

GameController::~GameController()
{
    //delete all walls
    foreach(Wall*wall,walls)
    {
        delete wall;
    }
    walls.clear();

    if(snake!=nullptr)
    {
       delete snake;
    }

    delete stopWatch;
}

//function which handles eating food by the snake
void GameController::snakeAteFood(Food *food)
{ 
    //if we ate food which makes us faster
    if(food->getFoodType() == FoodType::MakeFaster)
    {
        int snakeSpeed = snake->GetSpeed();//save old speed
        //set biggest speed for the snake
        if(snakeSpeed/2 == 0){ snake->SetSpeed(snakeSpeed/2 + 1);}
        else{snake->SetSpeed(snakeSpeed/2);}
        connect(&timerForFoodEffect,&QTimer::timeout,[=] { SetDefaultSpeed(snakeSpeed); });//connect timer with the slot,which set old speed for the snake
        timerForFoodEffect.start(8000);//start this timer
    }
    //if we ate food which makes us lower
    else if(food->getFoodType() == FoodType::MakeLower)
    {
        int snakeSpeed = snake->GetSpeed();//save old speed
        //set lowest speed for the snake
        snake->SetSpeed(snakeSpeed*2);
        connect(&timerForFoodEffect,&QTimer::timeout,[=] { SetDefaultSpeed(snakeSpeed); });//connect timer with the slot,which set old speed for the snake
        timerForFoodEffect.start(8000);//start this timer
    }
    if(food->getFoodType() == FoodType::Simple || food->getFoodType() == FoodType::MakeLower || food->getFoodType() == FoodType::MakeFaster)
    {
        score+=pointsForSimpleFood;
    }
    else//if we ate food which give more points to score
    {
        score+=pointsForMoreScoreFood;
    }
    scene.removeItem(food);//remove food from the scene

    addNewFood();//add new food
}

//function which handles eating wall by the snake
void GameController::snakeHitWall()
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

//function which handles eating itsels
void GameController::snakeAteItself()
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

//function which handles key,which was pressed on the keyboard
void GameController::handleKeyPressed(QKeyEvent *event,bool isTest)
{
    if (!isPause || isTest)
    {
        //if game isn`t started already and we starts it
        if(!gameIsStarted && (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right||event->key() == Qt::Key_Up||event->key() == Qt::Key_Down) && !isTest)
        {
            gameIsStarted = true;
            stopWatch->Start();
            timerForStopwatch.start(20);
        }
        switch (event->key())//set direction for the snake with the help of handled key
        {
            case Qt::Key_Left:
                snake->setMoveDirection(Snake::MoveLeft);
                break;
            case Qt::Key_Right:
                snake->setMoveDirection(Snake::MoveRight);
                break;
            case Qt::Key_Up:
                snake->setMoveDirection(Snake::MoveUp);
                break;
            case Qt::Key_Down:
                snake->setMoveDirection(Snake::MoveDown);
                break;
            case Qt::Key_Space:
                pause();
                break;
        }
    }
    //resume game if it was on pause mode
    else resume();
}

//function which add new food on the scene
void GameController::addNewFood()
{
    int x, y;
    bool collision;
    do {
        collision = false;
        //generate random coordinates of the food
        x = (int)(qrand() % 200) / 10 - 10;
        y = (int)(qrand() % 200) / 10 - 10;

        x *= 10;
        y *= 10;
        //chech the collision betwin this coordinates and walls
        foreach(Wall*wall,walls)
        {
            if(wall->shape().contains(wall->mapFromScene(QPointF(x + 5, y + 5))))
            {
                collision = true;
                break;
            }
        }//while is collision between snake and food generates coordinates again
    } while (snake->shape().contains(snake->mapFromScene(QPointF(x + 5, y + 5))) || collision);

    Food *food;

    if(current_step%FoodForSpeedInterval == 0)//if we should create food for speed change(every 8th food object)
    {
        int type = qrand()%2 + 1;
        if(type == 1)//make faster type
        {
            //check the colors so that they are not the same
            if(set.fieldColor == Qt::green)
            {
                food = new Food(set.fieldSize,x, y,FoodType::MakeFaster,Qt::cyan);
            }
            else
            {
                food = new Food(set.fieldSize,x, y,FoodType::MakeFaster,Qt::green);
            }
        }
        else//make lower type
        {
            //check the colors so that they are not the same
            if(set.fieldColor == Qt::black)
            {
                food = new Food(set.fieldSize,x, y,FoodType::MakeLower,Qt::darkGray);
            }
            else
            {
                food = new Food(set.fieldSize,x, y,FoodType::MakeLower,Qt::black);
            }
        }
    }
    else if(current_step%FoodForPointsInterval == 0)//if we should create food for score change(every 13th food object)
    {
        //check the colors so that they are not the same
        if(set.fieldColor == Qt::white)
        {
            food = new Food(set.fieldSize,x, y,FoodType::MorePointsToScore,Qt::darkBlue);
        }
        else
        {
            food = new Food(set.fieldSize,x, y,FoodType::MorePointsToScore,Qt::white);
        }
    }
    else//simple food object
    {
        //check the colors so that they are not the same
        if(set.fieldColor == Qt::red)
        {
            food = new Food(set.fieldSize,x, y,FoodType::Simple,Qt::yellow);
        }
        else
        {
            food = new Food(set.fieldSize,x, y,FoodType::Simple,Qt::red);
        }
    }
    current_step++;
    scene.addItem(food);//add vreated food on the scene
}

//adds walls on the scene
void GameController::addWalls()
{
    for(int i = 0; i < set.numOfWalls;i++)
    {
        int x, y;

        do {
            //generate random coordinates of the wall
            x = (int)(qrand() % 200) / 10 - 10;
            y = (int)(qrand() % 200) / 10 - 10;

            x *= 10;
            y *= 10;
        } while (snake->shape().contains(snake->mapFromScene(QPointF(x + 5, y + 5))));//while is collision between snake and wall

        int position = (int)qrand()%2;
        Wall *wall;
        if(set.fieldColor == Qt::green)
        {
            wall = new Wall(set.fieldSize,set.wallLength,(Wall::Position)position,x,y,Qt::blue);
        }
        else
        {
            wall = new Wall(set.fieldSize,set.wallLength,(Wall::Position)position,x,y,Qt::green);
        }

        walls.push_back(wall);
        scene.addItem(wall);
    }
}

void GameController::gameOver()
{
    //Этот слот продвигает сцену на один шаг вызывая QGraphicsItem::advance() для всех элементов на сцене.

    disconnect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
    resultTime = stopWatch->GetTime();
    timerForStopwatch.stop();
    stopWatch->Stop();//stops stopwatch

    //add result to the leaderboard file
    AddResultToLeaderboard();

    //reset all helpful values
    score = 0;
    current_step = 1;
    stopWatch->Reset();

    //if user want to play again
    if (QMessageBox::Yes == QMessageBox::information(NULL,
                            tr("Game Over"), tr("Again?"),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::Yes)) {
        connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
        scene.clear();

        qDebug()<<walls.size();

        walls.clear();


        //create new snake
        snake = new Snake(set.snakeSpeed,set.fieldSize,set.snakeColor,*this);
        scene.addItem(snake);

        //create new walls
        addWalls();

        //add new food
        addNewFood();

        gameIsStarted = false;
        emit sendStatusBarData("");
    } else {//if user dont want to play again
        emit closeWnd();//close this window
    }
}

//function which set default speed for the snake
void GameController::SetDefaultSpeed(int speed)
{
    snake->SetSpeed(speed);
    timerForFoodEffect.stop();
}

//function whish sends time and score to the game window
void GameController::SendStopwatchData()
{
    emit sendStatusBarData(stopWatch->GetTime() +  "    Score = " + QString::number(score));
}

//pause handler
void GameController::pause()
{
    if(gameIsStarted)
    {
        timerForStopwatch.stop();
        stopWatch->Stop();//stop stopwatch
    }
    //stop moving of the snake
    disconnect(&timer, SIGNAL(timeout()),
               &scene, SLOT(advance()));
    isPause = true;
}

void GameController::resume()
{
    if(gameIsStarted)
    {
        stopWatch->Start();//resume stopwatch
        timerForStopwatch.start(20);
    }
    //resume moving of the snake
    connect(&timer, SIGNAL(timeout()),
            &scene, SLOT(advance()));
    isPause = false;
}

//handler which handle key pressing
bool GameController::eventFilter(QObject *object, QEvent *event)
{
    //if key was pressed we handle it
    if (event->type() == QEvent::KeyPress) {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}

void GameController::AddResultToLeaderboard()
{
    bool bOk;
    QString str = QInputDialog::getText( nullptr, "Input", "Name:", QLineEdit::Normal, "Stas",  &bOk );//open input dialog window for entering name
    if (!bOk) {
        //cancel was pressed
    }
    else
    {
       LeaderboardData *data = new LeaderboardData;
       data->name = str;
       data->time = resultTime;
       data->difficult = set.difficult;
       data->score = score;
       data->fieldSize = set.fieldSize;

       QFile fileOut("leaderboard.txt"); // Связываем объект с файлом fileout.txt
       if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
       {
           QTextStream writeStream(&fileOut); // create ne object of QTextStream class
           //write all info into the file
           writeStream << data->name + "\n";
           writeStream << data->time + "\n";
           writeStream << data->difficult + "\n";
           writeStream << QString::number(data->score) + "\n";
           writeStream << QString::number(data->fieldSize) + "\n";
           fileOut.close();//close file
       }

    }
}
