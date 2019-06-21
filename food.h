#ifndef FOOD_H
#define FOOD_H

#include <QGraphicsItem>
#include <QColor>
#include <gtest/gtest.h>

//enumeration fo food object
enum FoodType
{
    Simple,
    MakeFaster,
    MakeLower,
    MorePointsToScore
};

class Food : public QGraphicsItem
{
public:
    FRIEND_TEST(Food,SettertGetters);//this test if frien for this class

    Food(const int& foodSize,qreal x, qreal y);
    Food(const int& foodSize,qreal x, qreal y,FoodType foodType,QColor color);

    //Three next functions are virtual,so we should to override them,because class Food derived from QGraphicsItem

    /*This pure virtual function defines the outer bounds of the item as a rectangle; all painting must be restricted
     * to inside an item's bounding rect. QGraphicsView uses this to determine whether the item requires redrawing.*/
    QRectF boundingRect() const;   
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);//this function draw the food on the scene

    /*Returns the shape of this item as a QPainterPath in local coordinates.
     *  The shape is used for many things, including collision detection, hit tests, and for the QGraphicsScene::items() functions.
        In our case,we use it for drawing food object*/
    QPainterPath shape() const;

    void setFoodType(FoodType foodType);
    FoodType getFoodType();

    void setFoodColor(QColor color);
    QColor getFoodColor();

private:
    const int FOOD_SIZE;
    FoodType foodType;
    QColor foodColor;
};

#endif // FOOD_H
