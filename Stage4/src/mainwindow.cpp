/**
 * @file  mainwindow.cpp
 * @author  Melanie Imough
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This class makes the first screen, "welcome" screen, The user can start filtering data by
 * pressing "go" button witch will activate the next screen.
 */

#include <string>
#include <QString>
#include <QStringList>
#include <QVector>
#include "filterdata.h"
#include <iostream>

#include "qcustomplot.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * Constructor that sets the first screen, "welcome" screen.
 *
 * @param *parent Pointer to the QWidget
 *
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet(
                "QPushButton#pushButton_2 {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButton_2:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButton_2:hover {"
                "background-color: rgb(130,130,180);}"

                "QGraphicsView#graphicsView_2 {"
                "border-style: inset;"
                "border-width: 2px;"
                "border-radius: 5px;"
                "border-color: rgb(150,150,175); }"

                "QGraphicsView#graphicsView {"
                "border: transparent;}"

                "QGraphicsView#graphicsView_3 {"
                "border-style: inset;"
                "border-width: 2px;"
                "border-radius: 5px;"
                "border-color: rgb(150,150,175); }"


                );



    //set the screen switching to get out of welcome screen
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(hide()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SIGNAL(showOtherWindow()));

    //insert the logo image on the welcome screen
    std :: string backgroundImage = "ombi-logo2.png";
    QPixmap pixmap1(backgroundImage.c_str());
    QRect rect = ui->graphicsView->rect();
    QSize sizeLogo(rect.width() , rect.height());
    QPixmap pixmap(pixmap1.scaled(sizeLogo));
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->addPixmap(pixmap);
    ui->graphicsView->setScene(scene);



}

/**
 * Destructor that deletes the GUI.
 *
 * @param *parent Pointer to the QWidget
 *
 */
MainWindow::~MainWindow()
{
    delete ui;

}

/**
 * Slots that activates the next screen, choosedata, so the user can start
 * filtering data.
 *
 */
void MainWindow::on_pushButton_2_clicked()
{

}
