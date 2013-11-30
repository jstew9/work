/**
  @file displaywindow.h
  @author Alex Clarke, Melanie Imough
  @version 1.0

  @section DESCRIPTION
  Header for a generic tabbed data viewing window

  */
#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QMainWindow>

namespace Ui {
class DisplayWindow;
}

class DisplayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DisplayWindow(QWidget *parent = 0);
    ~DisplayWindow();
    void addTab(QWidget *newTab, const QString &name);



private slots:
    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::DisplayWindow *ui;
};

#endif // DISPLAYWINDOW_H
