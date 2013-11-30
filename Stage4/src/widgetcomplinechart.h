/**
  @file widgetcomplinechar.h
  @author Alex Clarke, Melanie Imough, Jen Stewart
  @version 1.0

  @section DESCRIPTION

  Header for displaying a line graph comparing two measures

  */


#ifndef WIDGETCOMPLINECHART_H
#define WIDGETCOMPLINECHART_H

#include <QWidget>
#include "measure.h"

namespace Ui {
class WidgetCompLineChart;
}

class WidgetCompLineChart : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetCompLineChart(QWidget *parent = 0);
    WidgetCompLineChart(QWidget *parent, const Measure &meas, const Measure &meas2);
    ~WidgetCompLineChart();

private slots:
    void on_listLineChartMunicipality_4_itemSelectionChanged();
    void on_pushButtonRefresh_clicked();
    void on_pushButtonSaveLineData_clicked();

    void on_listLineChartYears_4_itemSelectionChanged();

private:
    void generateYears();
    void generateMunis();
    void drawChart();
    void drawYear(const int &sel);
    void drawYear2(const int &sel, const int &offset);

    QString _dirname;
    Ui::WidgetCompLineChart *ui;
    Measure *_meas; //Measure 1
    Measure *_meas2; //Measure 2
    QStringList *_mlist;    //Municipality list
    QStringList *_ylist;    //Year list
};

#endif // WIDGETCOMPLINECHART_H
