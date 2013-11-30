/**
 * @file  mainwindow.h
 * @author  Melanie Imough
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This is the header of the class that displays the first screen to the GUI. The first screen is a "Welcome" screen.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();

signals:
    void showOtherWindow();

public:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot;

};

#endif // MAINWINDOW_H
