#include "settings.h"
#include "ui_settings.h"
#include <QColorDialog>
#include <cmath>
Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    //setting range of the widgets
    ui->snakeSpeed->setEnabled(false);
    ui->fieldSize->setEnabled(false);
    ui->snakeSpeed->setMaximum(10);
    ui->snakeSpeed->setMinimum(1);
    ui->fieldSize->setMaximum(8);
    ui->fieldSize->setMinimum(3);
    ui->rbEasy->setChecked(true);

    ui->numOfWalls->setMinimum(0);
    ui->numOfWalls->setMaximum(10);
    ui->wallLength->setMinimum(1);
    ui->wallLength->setMaximum(6);

    //setting color of the buttons
    int red = snakeColor.red();
    int green = snakeColor.green();
    int blue = snakeColor.blue();
    ui->snakeColorBtn->setStyleSheet("background-color: rgb(" + QString::number(red) + ',' + QString::number(green) + ',' + QString::number(blue) + ')');

    red = fieldColor.red();
    green = fieldColor.green();
    blue = fieldColor.blue();
    ui->fieldColorBtn->setStyleSheet("background-color: rgb(" + QString::number(red) + ',' + QString::number(green) + ',' + QString::number(blue) + ')');
}

Settings::~Settings()
{
    delete ui;
}

QString Settings::Difficult()
{
    return difficult;
}

//save button handler(function save settings,which were chosed)
void Settings::on_SaveButton_clicked()
{
    if(ui->rbEasy->isChecked())
    {
        settings = SettingsData(6,8,snakeColor,fieldColor,"Easy",ui->numOfWalls->value(),ui->wallLength->value());
        difficult = "Easy";
        emit sendSettings(settings);//send settings to the mainwindow
    }
    else if(ui->rbMedium->isChecked())
    {
        settings = SettingsData(4,7,snakeColor,fieldColor,"Medium",ui->numOfWalls->value(),ui->wallLength->value());
        difficult = "Medium";
        emit sendSettings(settings);//send settings to the mainwindow
    }
    else if(ui->rbHard->isChecked())
    {
        settings = SettingsData(2,5,snakeColor,fieldColor,"Hard",ui->numOfWalls->value(),ui->wallLength->value());
        difficult = "Hard";
        emit sendSettings(settings);//send settings to the mainwindow
    }
    else if(ui->rbExtreme->isChecked())
    {
        settings = SettingsData(1,5,snakeColor,fieldColor,"Extreme",ui->numOfWalls->value(),ui->wallLength->value());
        difficult = "Extreme";
        emit sendSettings(settings);//send settings to the mainwindow
    }
    else if(ui->rbCustom->isChecked())
    {
        //edit speed and size values for program(because user chooses other values than program needs)
        int speed = abs(ui->snakeSpeed->value() - 10) + 1;
        int size = abs(ui->fieldSize->value()-10) + 1 ;

        settings = SettingsData(speed,size,snakeColor,fieldColor,"Custom",ui->numOfWalls->value(),ui->wallLength->value());
        difficult = "Custom";
        emit sendSettings(settings);//send settings to the mainwindow
    }
    close();
}

//handlers for radiobuttons

void Settings::on_rbCustom_clicked()
{
    ui->snakeSpeed->setEnabled(true);
    ui->fieldSize->setEnabled(true);
}

void Settings::on_rbExtreme_clicked()
{

     ui->snakeSpeed->setEnabled(false);
     ui->fieldSize->setEnabled(false);
}

void Settings::on_rbHard_clicked()
{
     ui->snakeSpeed->setEnabled(false);
     ui->fieldSize->setEnabled(false);
}

void Settings::on_rbMedium_clicked()
{
    ui->snakeSpeed->setEnabled(false);
    ui->fieldSize->setEnabled(false);
}

void Settings::on_rbEasy_clicked()
{
    ui->snakeSpeed->setEnabled(false);
    ui->fieldSize->setEnabled(false);
}

//snake color button handler
void Settings::on_snakeColorBtn_clicked()
{
    QColor color = QColorDialog::getColor();//opens dialog window,where user choose snake color
    snakeColor = color;
    ui->snakeColorBtn->setAutoFillBackground(true);
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    //set button style
    ui->snakeColorBtn->setStyleSheet("background-color: rgb(" + QString::number(red) + ',' + QString::number(green) + ',' + QString::number(blue) + ')');
}

//field color button handler
void Settings::on_fieldColorBtn_clicked()
{
    QColor color = QColorDialog::getColor();//opens dialog window,where user choose field color
    fieldColor = color;
    ui->fieldColorBtn->setAutoFillBackground(true);
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    //set button style
    ui->fieldColorBtn->setStyleSheet("background-color: rgb(" + QString::number(red) + ',' + QString::number(green) + ',' + QString::number(blue) + ')');
}
