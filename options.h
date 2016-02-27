#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT
    
public:
    int targetC;
    float strengthscale;
    int steepTime;



    explicit Options(QWidget *parent = 0);
    ~Options();
    
private:
    Ui::Options *ui;


 public slots:
    void acceptOpts();
    void sliderChangedTemp(int);
    void sliderChangedTime(int);
};

#endif // OPTIONS_H
