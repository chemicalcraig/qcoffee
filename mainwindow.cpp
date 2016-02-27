#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "brewers.h"
#include "brewparams.h"
#include "cups.h"
#include "strength.h"
#include "wiringPiI2C.h"
#include "wiringPi.h"
#include "options.h"

/***************************
* Brewing parameters
****************************/
Brewers brewer;
cups cup;
Strength strength;
BrewParams params;

/** GPIO pins **/
int tempPin = 0;

/** Set up I2C **/
int i2cDevice = wiringPiI2CSetup(0x08);
int grinding = 0;
int brewing = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /** Connections **/
    connect(ui->pushButton_chemex, SIGNAL(pressed()) , this, SLOT(brewbuttonPushed()) );
    connect(ui->pushButton_hario, SIGNAL(pressed()) , this, SLOT(brewbuttonPushed()) );
    connect(ui->pushButton_grinder, SIGNAL(pressed()) , this, SLOT(brewbuttonPushed()) );
    connect(ui->pushButton_cup1,SIGNAL(pressed()), this, SLOT(cupbuttonPushed()));
    connect(ui->pushButton_cup2,SIGNAL(pressed()), this, SLOT(cupbuttonPushed()));
    connect(ui->pushButton_cup3,SIGNAL(pressed()), this, SLOT(cupbuttonPushed()));
    connect(ui->pushButton_cup4,SIGNAL(pressed()), this, SLOT(cupbuttonPushed()));
    connect(ui->pushButton_strength1,SIGNAL(pressed()),this,SLOT(strbuttonPushed()));
    connect(ui->pushButton_strength2,SIGNAL(pressed()),this,SLOT(strbuttonPushed()));
    connect(ui->pushButton_strength3,SIGNAL(pressed()),this,SLOT(strbuttonPushed()));
    connect(ui->pushButton_strength4,SIGNAL(pressed()),this,SLOT(strbuttonPushed()));
    connect(ui->pushButton_strength5,SIGNAL(pressed()),this,SLOT(strbuttonPushed()));
    connect(ui->pushButton_brew, SIGNAL(pressed()), this, SLOT(brewitPushed()));
    connect(ui->pushButton_reset, SIGNAL(pressed()), this, SLOT(resetPushed()));

    /** Options **/
    connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(options()));



    /** Populate lists **/
    brewer.buttons<<ui->pushButton_hario<<ui->pushButton_chemex<<ui->pushButton_grinder;
    cup.buttons<<ui->pushButton_cup1<<ui->pushButton_cup2<<ui->pushButton_cup3<<ui->pushButton_cup4;
    strength.buttons<<ui->pushButton_strength1<<ui->pushButton_strength2<<ui->pushButton_strength3
                      <<ui->pushButton_strength4<<ui->pushButton_strength5;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*****************************************************
 *  Functions
 *****************************************************/

void MainWindow::options() {
    Options *ops = new Options(this);

    int res = ops->exec();

    if (res == QDialog::Accepted) {
        params.strengthScale = ops->strengthscale;
        params.steepTime = ops->steepTime;
        params.setTempC = ops->targetC;
    }

}

void MainWindow::resetPushed() {
    grinding = 0;
    brewing = 0;So
    ui->pushButton_brew->setEnabled(true);
}

void MainWindow::brewbuttonPushed() {
    for (int i=0; i<brewer.buttons.length(); i++) {
        if (((QPushButton*)sender()) == brewer.buttons.at(i) ) {
            brewer.buttons.at(i)->setEnabled(false);
            brewer.buttons.at(i)->setChecked(true);
            params.brewer = i;
        } else if (brewer.buttons.at(i)->isChecked()) {
            brewer.buttons.at(i)->setEnabled(true);
            brewer.buttons.at(i)->setChecked(false);
        }
    }
}

void MainWindow::cupbuttonPushed() {
    for (int i=0; i<cup.buttons.length(); i++) {
        if (((QPushButton*)sender()) == cup.buttons.at(i) ) {
            cup.buttons.at(i)->setEnabled(false);
            cup.buttons.at(i)->setChecked(true);
            params.ncups = i;
        } else if (cup.buttons.at(i)->isChecked()) {
            cup.buttons.at(i)->setEnabled(true);
            cup.buttons.at(i)->setChecked(false);
        }
    }
}

void MainWindow::strbuttonPushed() {
    for (int i=0; i<strength.buttons.length(); i++) {
        if (((QPushButton*)sender()) == strength.buttons.at(i) ) {
            strength.buttons.at(i)->setEnabled(false);
            strength.buttons.at(i)->setChecked(true);
             params.strength = i;
        } else if (strength.buttons.at(i)->isChecked()) {
            strength.buttons.at(i)->setEnabled(true);
            strength.buttons.at(i)->setChecked(false);
        }
    }
}

void MainWindow::grindCoffee(const int which, const int gt) {
    wiringPiI2CWrite(i2cDevice,which);
    wiringPiI2CWrite(i2cDevice,gt);

    /** delay for grindTime **/
    delay(gt+500);
}

float MainWindow::getCurrentTempC() {
    return 0.;
}

void MainWindow::brewitPushed() {

    /** Disable Brew button **/
    ui->pushButton_brew->setEnabled(false);

    /** calculate grind time in ms **/
    params.setgrindMS();

    /** Grinder only, no coffee **/
    if (params.brewer == 2) {
        if (grinding == 0) {
            grindCoffee(1,params.grindTime);
            grinding = 1;
        }
    } else
    /** Full Brew **/
    {
        brewing = 1;

        /** grind coffee **/
        grindCoffee(1,params.grindTime);
    
        /** Heat up water to set point **/
        float tempC = getCurrentTempC();

        /** Turn on heater through I2C **/
        wiringPiI2CWrite(i2cDevice,2);
        wiringPiI2CWrite(i2cDevice,0);

        /** get current temperature **/
        while (tempC < params.setTempC) {
            /** read temperature every second **/
            delay(1000);
            tempC = getCurrentTempC();
        }

        /** Turn off heater through I2C **/
        wiringPiI2CWrite(i2cDevice,2);
        wiringPiI2CWrite(i2cDevice,1);

        /** Set Brew Parameters **/
        params.setSendData();

        /** Send Brew Parameters **/
        wiringPiI2CWrite(i2cDevice,3);
        for (int i=0; i<sizeof(params)/sizeof(int); i++ ) {
            wiringPiI2CWrite(i2cDevice,params.sendData[i]);
        }

        /** Reenable brewing **/
        //ui->pushButton_brew->setEnabled(true);
    }
}
