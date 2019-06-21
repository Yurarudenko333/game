#include <QPainter>

#include "constants.h"
#include "food.h"

Food::Food(const int&foodSize,qreal x, qreal y):FOOD_SIZE(foodSize)
{
    setPos(x, y);
    setData(GD_Type, GO_Food);
    this->setFoodType(FoodType::Simple);
    this->setFoodColor(Qt::red);
}

Food::Food(const int &foodSize, qreal x, qreal y, FoodType foodType, QColor color):FOOD_SIZE(foodSize)
{
    //Sets the position of the item to pos, which is in parent coordinates.
    //For items with no parent, pos is in scene coordinates.
    setPos(x, y);

    //Sets this item's custom data for the key key to value.
    setData(GD_Type, GO_Food);
    this->setFoodType(foodType);
    this->setFoodColor(color);
}

/*This pure virtual function defines the outer bounds of the item as a rectangle; all painting must be restricted
 * to inside an item's bounding rect. QGraphicsView uses this to determine whether the item requires redrawing.*/
QRectF Food::boundingRect() const
{
    return QRectF(-FOOD_SIZE,    -FOOD_SIZE,
                   FOOD_SIZE * 2, FOOD_SIZE * 2 );
}

//this function draw the food on the scene
void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), foodColor);

    painter->restore();
}

/*Returns the shape of this item as a QPainterPath in local coordinates.
     *  The shape is used for many things, including collision detection, hit tests, and for the QGraphicsScene::items() functions.
        In our case,we use it for drawing food object*/
QPainterPath Food::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(FOOD_SIZE / 2 + 1, FOOD_SIZE / 2 + 1), FOOD_SIZE/2, FOOD_SIZE/2);
    return p;
}

void Food::setFoodType(FoodType foodType)
{
    this->foodType = foodType;
}

FoodType Food::getFoodType()
{
    return foodType;
}

void Food::setFoodColor(QColor color)
{
    this->foodColor = color;
}

QColor Food::getFoodColor()
{
    return this->foodColor;
}
