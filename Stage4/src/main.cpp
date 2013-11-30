/**
 * @file  main.cpp
 * @author  Melanie Imough
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class create the GUI.
 */

#include <QApplication>

#include "mainwindow.h"
#include "choosedata.h"

/**
 * Main method that initialises the GUI.
 *
 * @param argc, *argv[]
 *
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w1;
    ChooseData w2;

    //let the user switch to next mainwindow
    QObject::connect(&w1, SIGNAL(showOtherWindow()), &w2, SLOT(show()));
    QObject::connect(&w2, SIGNAL(showOtherWindow()), &w1, SLOT(show()));
    w1.show();  // only show one of them to start

    return a.exec();
}
