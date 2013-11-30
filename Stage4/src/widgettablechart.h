/**
  @file widgetbarchart.h
  @author Alex Clarke, Melanie Imough
  @version 0.1

  @section DESCRIPTION

  Header for a window displaying a data table
  */

#ifndef WIDGETTABLECHART_H
#define WIDGETTABLECHART_H

#include <QWidget>
#include <QStandardItemModel>
#include "measure.h"
namespace Ui {
class WidgetTableChart;
}

class WidgetTableChart : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetTableChart(QWidget *parent = 0);
    WidgetTableChart(QWidget *parent, Measure &meas);
    ~WidgetTableChart();

private slots:
    void on_pushButtonLoadTable_clicked();
    void on_pushButtonSaveTableData_clicked();


private:
    void drawYear(const int &sel,const int &column);
    void drawTable();
    Ui::WidgetTableChart *ui;
    Measure *_meas; //active measure
    QStandardItemModel *_model;

    QString _dirname;
};

#endif // WIDGETTABLECHART_H
