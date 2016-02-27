#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void grindCoffee(const int which, const int gt);
    float getCurrentTempC();

    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

public slots:
    void brewbuttonPushed();
    void cupbuttonPushed();
    void strbuttonPushed();
    void brewitPushed();
    void resetPushed();
    void options();

};

#endif // MAINWINDOW_H
