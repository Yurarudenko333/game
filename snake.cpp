#include <QPainter>
#include "constants.h"
#include "gamecontroller.h"
#include "snake.h"

Snake::Snake(const int&snakeSpeed,const int&snakeSize,QColor snakeColor,GameController &controller) :
    head(0, 0),
    growing(1),// start length of the snake
    moveDirection(NoMove),
    controller(controller),
    SNAKE_SIZE(snakeSize)
{
    SNAKE_COLOR = snakeColor;
    speed = snakeSpeed;
}

/*This pure virtual function defines the outer bounds of the item as a rectangle; all painting must be restricted
 * to inside an item's bounding rect. QGraphicsView uses this to determine whether the item requires redrawing.*/
QRectF Snake::boundingRect() const
{
    qreal minX = head.x();
    qreal minY = head.y();
    qreal maxX = head.x();
    qreal maxY = head.y();

    foreach (QPointF p, tail) {
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }

    QPointF tl = mapFromScene(QPointF(minX, minY));
    QPointF br = mapFromScene(QPointF(maxX, maxY));

    QRectF bound = QRectF(tl.x(),  // x
                          tl.y(),  // y
                          br.x() - tl.x() + SNAKE_SIZE,      // width
                          br.y() - tl.y() + SNAKE_SIZE       //height
                          );
    return bound;
}

/*Returns the shape of this item as a QPainterPath in local coordinates.
     *  The shape is used for many things, including collision detection, hit tests, and for the QGraphicsScene::items() functions.
        In our case,we use it for drawing  the snake object*/
QPainterPath Snake::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));

    foreach (QPointF p, tail) {
        QPointF itemp = mapFromScene(p);
        path.addRect(QRectF(itemp.x(), itemp.y(), SNAKE_SIZE, SNAKE_SIZE));
    }

    return path;
}

//this function draws the snake
void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();
    painter->fillPath(shape(), SNAKE_COLOR);
    painter->restore();
}

//this function sets snake direction
void Snake::setMoveDirection(Direction direction)
{
    if (moveDirection == MoveLeft && direction == MoveRight)
        return;
    if (moveDirection == MoveRight && direction == MoveLeft)
        return;
    if (moveDirection == MoveUp && direction == MoveDown)
        return;
    if (moveDirection == MoveDown && direction == MoveUp)
        return;
    moveDirection = direction;
}

Snake::Direction Snake::currentDirection()
{
    return moveDirection;
}

int Snake::GetLength()
{
    return tail.size();
}

int Snake::GetSpeed()
{
    return speed;
}

void Snake::SetSpeed(int speed)
{
    this->speed = speed;
}

int Snake::GetHeadXCoordinate()
{
    return head.rx();
}

int Snake::GetHeadYCoordinate()
{
    return head.ry();
}

QColor Snake::GetColor()
{
    return SNAKE_COLOR;
}

int Snake::TestMoveLeft()//returns new head x coordinate
{
    int current_x = head.rx();
    current_x-= SNAKE_SIZE;//new value after move
    return current_x;
}

int Snake::TestMoveRight()
{
    int current_x = head.rx();
    current_x+= SNAKE_SIZE;//new value after move
    return current_x;
}

int Snake::TestMoveUp()
{
    int current_y = head.ry();
    current_y-= SNAKE_SIZE;//new value after move
    return current_y;
}

int Snake::TestMoveDown()
{
    int current_y = head.ry();
    current_y+= SNAKE_SIZE;//new value after move
    return current_y;
}

//simple model of advance() function for testing program
int Snake::TestAdvance()
{
    //on the start of the game
    if (moveDirection == NoMove)
    {
        return -123;
    }
    //there we call move functions,which rotates our snake
    switch ((int)moveDirection)
    {
        case (int)MoveLeft:
            return TestMoveLeft();
        case (int)MoveRight:
            return TestMoveRight();
        case (int)MoveUp:
            return TestMoveUp();
        case (int)MoveDown:
            return TestMoveDown();
    }
}

//this function every time update snake position on the scene
void Snake::advance(int step)
{
    if (!step) {
        return;
    }
    //the more speed value so less speed of the snake,because function updates less times per time period
    if (tickCounter++ % speed != 0) {
        return;
    }
    //on the start of the game
    if (moveDirection == NoMove) {
        return;
    }

    //move
    if (growing > 0) {
		QPointF tailPoint = head;
        tail << tailPoint;
        growing -= 1;
    } else {
        tail.removeFirst();
        tail << head;
    }

    //there we call move functions,which rotates our snake
    switch (moveDirection) {
        case MoveLeft:
            moveLeft();
            break;
        case MoveRight:
            moveRight();
            break;
        case MoveUp:
            moveUp();
            break;
        case MoveDown:
            moveDown();
            break;
    }

    //Sets the position of the item to pos, which is in parent coordinates.
    //For items with no parent, pos is in scene coordinates.
    setPos(head);
    //check the collisions between items
    handleCollisions();
}

//next functions change snake position
void Snake::moveLeft()
{
    head.rx() -= SNAKE_SIZE;
    if (head.rx() < -100) {
        head.rx() = 95;
    }
}

void Snake::moveRight()
{
    head.rx() += SNAKE_SIZE;
    if (head.rx() > 95) {
        head.rx() = -100;
    }
}

void Snake::moveUp()
{
    head.ry() -= SNAKE_SIZE;
    if (head.ry() < -100) {
        head.ry() = 95;
    }
}

void Snake::moveDown()
{
    head.ry() += SNAKE_SIZE;
    if (head.ry() > 95) {
        head.ry() = -100;
    }
}

//function which handle collisions between snake and food/walls
void Snake::handleCollisions()
{
    QList<QGraphicsItem *> collisions = collidingItems();

    // Check collisions with other objects on screen
    foreach (QGraphicsItem *collidingItem, collisions)
    {
        if (collidingItem->data(GD_Type) == GO_Food)
        {
            // Let GameController handle the event by putting another food
            controller.snakeAteFood((Food *)collidingItem);
            growing += 1;
        }
        else if(collidingItem->data(GD_Type) == GO_Wall)//snake hits the wall
        {
            controller.snakeHitWall();
        }
    }

    // Check snake eating itself
    if (tail.contains(head))
    {
        controller.snakeAteItself();
    }
}
