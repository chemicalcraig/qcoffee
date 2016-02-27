#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);

    /** Connections **/
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptOpts()));
    connect(ui->horizontalSlider_temp,SIGNAL(valueChanged(int)), this, SLOT(sliderChangedTemp(int)));
    connect(ui->horizontalSlider_time,SIGNAL(valueChanged(int)), this, SLOT(sliderChangedTime(int)));

}

/************************************************
 *  Functions
 ************************************************/
void Options::sliderChangedTemp(int n) {
    ui->label_temp->setText(QString::number(n));
}

void Options::sliderChangedTime(int n) {
    ui->label_time->setText(QString::number(n));
}

Options::~Options()
{
    delete ui;
}

void Options::acceptOpts() {
    this->targetC = ui->horizontalSlider_temp->value();
    this->steepTime = ui->horizontalSlider_time->value();
}
