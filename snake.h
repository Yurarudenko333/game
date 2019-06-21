#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QRectF>
#include "settings.h"
#include <QColor>
#include <gtest/gtest.h>

class GameController;

class Snake :public QObject, public QGraphicsItem
{
public:

    FRIEND_TEST(Snake,Move);//this test if frien for this class

    //enumeration for snake direction
    enum Direction {
        NoMove,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown
    };

    Snake(const int&snakeSpeed,const int&snakeSize ,QColor snakeColor,GameController & controller);

    //Three next functions are virtual,so we should to override them,because class Food derived from QGraphicsItem

    /*This pure virtual function defines the outer bounds of the item as a rectangle; all painting must be restricted
     * to inside an item's bounding rect. QGraphicsView uses this to determine whether the item requires redrawing.*/
    QRectF boundingRect() const;

    /*Returns the shape of this item as a QPainterPath in local coordinates.
         *  The shape is used for many things, including collision detection, hit tests, and for the QGraphicsScene::items() functions.
            In our case,we use it for drawing  the snake object*/
    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);//this function draws the snake

    void setMoveDirection(Direction direction);
	Direction currentDirection();

    int GetLength();

    int GetSpeed();
    void SetSpeed(int speed);

    int GetHeadXCoordinate();
    int GetHeadYCoordinate();
    QColor GetColor();

    //test move functions for testing program
    int TestMoveLeft();
    int TestMoveRight();
    int TestMoveUp();
    int TestMoveDown();

    int TestAdvance();

protected:
    /*This virtual function is called twice for all items by the QGraphicsScene::advance() slot. In the first phase,
    all items are called with phase == 0, indicating that items on the scene are about to advance, and then all items are called with phase == 1.
    We reimplement this function to update snake position on the scene*/
    void advance(int step);

private:

    //move functions
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    //function which handle collisions between snake and food/walls
    void handleCollisions();

    QPointF        head;
    int            growing;
    int            speed;
    QList<QPointF> tail;
    int            tickCounter;
    Direction      moveDirection;
    GameController &controller;

    const int SNAKE_SIZE;
    QColor SNAKE_COLOR;
};

#endif // SNAKE_H
