/**
  @file widgetlinechart.cpp
  @author Alex Clarke, Melanie Imough, Jen Stewart
  @version 1.0

  @class WidgetLineChart
  @brief Class for the display of data in a line graph
  */

#include "widgetlinechart.h"
#include "ui_widgetlinechart.h"
#include "measure.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>

#define PEN_WIDTH 3
#define GRAPH_HIGH_RATIO 1.2
#define GRAPH_LOW_RATIO 0.85

/**
 * Default Constructor
 * @param parent
 */
WidgetLineChart::WidgetLineChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetLineChart)
{
    ui->setupUi(this);
    this->setStyleSheet(
                "QPushButton#pushButtonSaveLineData {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonSaveLineData:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonSaveLineData:hover {"
                "background-color: rgb(130,130,180);}"

                "QPushButton#pushButtonRefresh {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonRefresh:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonRefresh:hover {"
                "background-color: rgb(130,130,180); }"
                );


    ui->listLineChartMunicipality->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listLineChartYears->setSelectionMode(QAbstractItemView::MultiSelection);
}

/**
 * Construct a line chart widget of a given parent, active measure, low and high range@brief WidgetLineChart::WidgetLineChart
 * @param parent    chosen parent
 * @param meas  active measure
 * @param range chosen range
 */
WidgetLineChart::WidgetLineChart(QWidget *parent, const Measure &meas, const std::pair<double, double> &range):QWidget(parent),ui(new Ui::WidgetLineChart),_meas(new Measure(meas))
{
    ui->setupUi(this);
    _range = range;


    this->setStyleSheet(
                "QPushButton#pushButtonSaveLineData {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonSaveLineData:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonSaveLineData:hover {"
                "background-color: rgb(130,130,180);}"

                "QPushButton#pushButtonRefresh {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonRefresh:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonRefresh:hover {"
                "background-color: rgb(130,130,180); }"
                );


    ui->listLineChartMunicipality->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listLineChartYears->setSelectionMode(QAbstractItemView::MultiSelection);
    //List Stuff
    QStringList mlist = _meas->stringMuniList();
    ui->listLineChartMunicipality->addItems(mlist);
    ui->listLineChartYears->addItems(_meas->yearRange());
    drawChart();
}
/**
 * Deconstructor
 */
WidgetLineChart::~WidgetLineChart()
{
    delete ui;
    delete _meas;

}

/**
 * @brief WidgetLineChart::drawYear draw a given year on the line chart
 * @param sel the index of the selected year
 */
void WidgetLineChart::drawYear(const int &sel) {
    QList<QListWidgetItem*> tempList = ui->listLineChartMunicipality->selectedItems();
    int i = 0;
    for(QList<QListWidgetItem*>::Iterator it = tempList.begin(); it != tempList.end(); ++it) {
        QListWidgetItem *tempItem = *it;
        Municipality tempMuni = _meas->findMuni(tempItem->text());
        try {
            ui->widgetLineChart_2->graph(i)->addData(_meas->yearRange().at(sel).toInt(),tempMuni.getYear(_meas->yearRange().at(sel)).value());
            ui->widgetLineChart_2->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        } catch(std::string s) {
        }
        ui->widgetLineChart_2->graph(i)->setName(tempMuni.name());
        ++i;
    }

}

/**
 * Draw a line graph using the selected years and Municipalities
 * @param meas  the active measure
 * @param lowrange the low range of the data
 * @param highrange the high range of the data
 */
void WidgetLineChart::drawChart()
{
    ui->widgetLineChart_2->clearPlottables();

    //Random Number stuff
    srand(time(NULL));

 //   QVector<QString> selectedMuniList;
    QList<QListWidgetItem*> tempMuniList = ui->listLineChartMunicipality->selectedItems();
    int j = 0;
    for(QList<QListWidgetItem*>::Iterator it = tempMuniList.begin(); it != tempMuniList.end(); ++it) {
   //     QListWidgetItem *tempItem = *it;
  //      selectedMuniList.append(_meas->findMuni(tempItem->text()).name());
        ui->widgetLineChart_2->addGraph();
        QPen tempPen(QColor(rand()%255,rand()%255,rand()%255,255));
        tempPen.setWidth(PEN_WIDTH);
        ui->widgetLineChart_2->graph(j)->setPen(tempPen);
        ++j;
    }
    ui->widgetLineChart_2->xAxis->setAutoTickStep(false);
    ui->widgetLineChart_2->xAxis->setTickStep(1.0);
    ui->widgetLineChart_2->xAxis->setTickLabelRotation(-60);
    if(_meas->yearRange().size() > 0 )ui->widgetLineChart_2->xAxis->setRange(_meas->yearRange().at(0).toInt(),_meas->yearRange().at(_meas->yearRange().size()-1).toInt());
    ui->widgetLineChart_2->yAxis->setLabel(_meas->name());
    ui->widgetLineChart_2->yAxis->setRange(_range.first*GRAPH_LOW_RATIO,_range.second*GRAPH_HIGH_RATIO);

    for(int j = 0; j < _meas->yearRange().size(); ++j)
        if(ui->listLineChartYears->item(j)->isSelected()) drawYear(j);

    if(ui->radioButtonMeanAllDataBar->isChecked()) {
        drawAverage();
    }

    ui->widgetLineChart_2->legend->setVisible(true);
    ui->widgetLineChart_2->replot();
}
/**
 * Draw a line representing the average
 */
void WidgetLineChart::drawAverage()
{
    QVector<double> tickList;
    tickList << 2008 << 2009 << 2010 << 2011 << 2012;
    QVector<double> avgData;
    double avg = _meas->measureAverage();
    //std::cout << avg << std::endl;
    avgData << avg << avg << avg << avg << avg;
    ui->widgetLineChart_2->addGraph();
    QPen tempPen;
    tempPen.setWidth(PEN_WIDTH);
    tempPen.setColor(QColor(0,0,0,255));

    ui->widgetLineChart_2->graph(ui->widgetLineChart_2->graphCount()-1)->setPen(tempPen);
    if(!_meas->yearRange().isEmpty()) {
        ui->widgetLineChart_2->graph(ui->widgetLineChart_2->graphCount()-1)->addData(_meas->yearRange().at(0).toInt(),avg);
        ui->widgetLineChart_2->graph(ui->widgetLineChart_2->graphCount()-1)->addData(_meas->yearRange().at(_meas->yearRange().size()-1).toInt(),avg);
    }
    ui->widgetLineChart_2->graph(ui->widgetLineChart_2->graphCount()-1)->setName("Average");
}
/**
 * Redraw the graph when a Municipality is selected or deselected
 */
void WidgetLineChart::on_listLineChartMunicipality_itemSelectionChanged()
{
    drawChart();

}
/**
 * Draw a line representing the average
 * @param checked
 */
void WidgetLineChart::on_radioButtonMeanAllDataBar_toggled(bool checked)
{
    drawChart();
}
/**
 * @author Jen Stewart
 *  Function to save the Line Chart as an image file
 *
 */
void WidgetLineChart::on_pushButtonSaveLineData_clicked()
{
    if(ui->listLineChartYears->selectedItems().isEmpty()
            || ui->listLineChartMunicipality->selectedItems().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please select data before saving.");
        msgBox.exec();
        return;
    }
      QString fileName = QFileDialog::getSaveFileName(this,"Save File", _dirname,"Images (*.png *.jpg *.bmp)");
      if(fileName.endsWith("png"))
      {
          ui->widgetLineChart_2->savePng(fileName);
          fileName.chop(fileName.length()-fileName.lastIndexOf("/"));
          _dirname = fileName;
      }
      else if(fileName.endsWith("jpg"))
      {
          ui->widgetLineChart_2->saveJpg(fileName);
          fileName.chop(fileName.length()-fileName.lastIndexOf("/"));
          _dirname = fileName;
      }
      else if(fileName.endsWith("bmp"))
      {
          ui->widgetLineChart_2->saveBmp(fileName);
          fileName.chop(fileName.length()-fileName.lastIndexOf("/"));
          _dirname = fileName;
      }
      else if (!fileName.isNull())
      {
          QMessageBox msgBox;
          msgBox.setText("Please type a filename of an image type ie .png .jpg .bmp");
          msgBox.exec();
      }

}

/**
 * Redraw the line chart
 */
void WidgetLineChart::on_pushButtonRefresh_clicked()
{
    drawChart();
}
/**
 * @brief WidgetLineChart::on_listLineChartYears_itemSelectionChanged Redraw the graph when year selection changes
 */
void WidgetLineChart::on_listLineChartYears_itemSelectionChanged()
{
    drawChart();
}
