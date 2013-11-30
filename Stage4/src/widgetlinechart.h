/**
  @file widgetlinechart.h
  @author Alex Clarke, Melanie Imough, Jen Stewart
  @version 1.0

  @section DESCRIPTION

  Header for a window displaying a line chart
  */

#ifndef WIDGETLINECHART_H
#define WIDGETLINECHART_H

#include <QWidget>
#include "measure.h"
namespace Ui {
class WidgetLineChart;
}

class WidgetLineChart : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetLineChart(QWidget *parent = 0);
    WidgetLineChart(QWidget *parent, const Measure &meas, const std::pair<double,double> &range);
    ~WidgetLineChart();
    
private slots:
    void on_listLineChartMunicipality_itemSelectionChanged();
    void on_radioButtonMeanAllDataBar_toggled(bool checked);
    void on_pushButtonSaveLineData_clicked();
    void on_pushButtonRefresh_clicked();

    void on_listLineChartYears_itemSelectionChanged();

private:
    void drawChart();
    void drawYear(const int &sel);

    void drawAverage();

    Ui::WidgetLineChart *ui;
    Measure *_meas; //active measure
    QString _dirname;
    std::pair<double,double> _range;
    int _lowrange; //low range of the data
    int _highrange; //high range of the data
};

#endif // WIDGETLINECHART_H
