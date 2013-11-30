/**
  @file displaywindow.cpp
  @author Alex Clarke, Melanie Imough
  @version 0.1

  @section DESCRIPTION
  Class for a generic tabbed data viewing window

  */

#include "displaywindow.h"
#include "ui_displaywindow.h"
/**
 * @brief DisplayWindow::DisplayWindow Default Constructor
 * @param parent
 */
DisplayWindow::DisplayWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DisplayWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setMovable(true);
}
/**
 * @brief DisplayWindow::~DisplayWindow Deconstructor
 */
DisplayWindow::~DisplayWindow()
{
    delete ui;

}
/**
 * @brief DisplayWindow::addTab Add a tab, the display it
 * @param newTab the tab to add
 * @param name the tab's name
 */

void DisplayWindow::addTab(QWidget *newTab, const QString &name)
{
    ui->tabWidget->addTab(newTab,name);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}
/**
 * @brief DisplayWindow::on_tabWidget_tabCloseRequested Remove a tab if you close it
 * @param index
 */
void DisplayWindow::on_tabWidget_tabCloseRequested(int index)
{
    delete ui->tabWidget->widget(index);
}
