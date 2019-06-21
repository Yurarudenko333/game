#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QString>
#include <QColor>
#include<gtest\gtest.h>
namespace Ui {
class Settings;
}
struct SettingsData
{
    SettingsData(){}
    SettingsData(int snakeSpeed,int fieldSize,QColor snakeColor,QColor fieldColor,QString difficult,int numOfWalls = 0,int wallLength = 1)
    {
        this->snakeSpeed = snakeSpeed;
        this->fieldSize = fieldSize;
        this->snakeColor = snakeColor;
        this->fieldColor = fieldColor;
        this->difficult = difficult;
        this->numOfWalls = numOfWalls;
        this->wallLength = wallLength;
    }
    int snakeSpeed;
    int fieldSize;
    int numOfWalls;
    int wallLength;
    QColor snakeColor;
    QColor fieldColor;
    QString difficult;
};

class Settings : public QDialog
{
    Q_OBJECT

public:
    FRIEND_TEST(Settings,CheckDifficultSettings);

    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    QString Difficult();
private slots:


    void on_SaveButton_clicked();

    void on_rbCustom_clicked();

    void on_rbExtreme_clicked();

    void on_rbHard_clicked();

    void on_rbMedium_clicked();

    void on_rbEasy_clicked();

    void on_snakeColorBtn_clicked();

    void on_fieldColorBtn_clicked();

signals:
    void sendSettings(SettingsData settings);

private:
    Ui::Settings *ui;
    QColor snakeColor = Qt::darkMagenta;
    QColor fieldColor = Qt::gray;
    SettingsData settings;
    QString difficult;
};

#endif // SETTINGS_H
