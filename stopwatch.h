#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QDialog>
#include <QTimer>
#include <QTime>
namespace Ui {
class stopwatch;
}

class stopwatch:public QDialog
{
    Q_OBJECT

public:
    explicit stopwatch(QWidget *parent = nullptr);
    ~stopwatch();

    QString GetTime();
    void Start();
    void Stop();
    void Reset();

private:
    bool running;
    QTimer timer;
    QTime time_on_stopwatch;//time which is on the stopwatch
    QTime last_start;//time of last start stopwatch

};

#endif // STOPWATCH_H
