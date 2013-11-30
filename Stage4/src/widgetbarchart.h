/**
  @file widgetbarchart.h
  @author Alex Clarke, Melanie Imough, Jen Stewart
  @version 0.1

  @section DESCRIPTION

  Header for a window displaying a bar chart
  */

#ifndef WIDGETBARCHART_H
#define WIDGETBARCHART_H

#include <QWidget>
#include <QVector>
#include <QColor>
#include "measure.h"
#include "qcustomplot.h"

namespace Ui {
class widgetBarChart;
}

class widgetBarChart : public QWidget
{
    Q_OBJECT
    
public:
    explicit widgetBarChart(QWidget *parent = 0);
    widgetBarChart(QWidget *parent, const Measure &meas,const std::pair<double,double> &range);
    ~widgetBarChart();
    
    void drawChartDefault();

private slots:

    void on_listBarChartMunicipality_itemSelectionChanged();
    void on_listBarChartYears_itemSelectionChanged();
    void on_radioButtonMeanAllDataBar_toggled(bool checked);
    void on_pushButtonRefresh_clicked();
    void on_pushButtonSaveBarData_clicked();

    void titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title);
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void contextMenuRequest(QPoint pos);
    void selectImageBackground();
    void selectImageAxisBackground();
    void changeLegendColor();
    void changeColorLine();
    void changeAxisColor();
    void changeBackgroundColor();
    void moveLegend();

signals:
    void plottableClick(QCPAbstractPlottable*,QMouseEvent*);

private:
    void drawChart();
    void drawYear(const int &sel);
    void drawAverage(int activeMunis);

    Ui::widgetBarChart *ui;
    Measure *_meas; //active measure
    QString _dirname;
    std::pair<double,double> _range;

};

#endif // WIDGETBARCHART_H
