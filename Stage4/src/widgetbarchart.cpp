/**
  @file widgetlinechart.cpp
  @author Alex Clarke, Melanie Imough, Jen Stewart
  @version 1.0

  @class widgetBarChart
  @brief Class for the display of data in a bar chart
  */
#include "widgetbarchart.h"
#include "ui_widgetbarchart.h"
#include "qcustomplot.h"
#include <QVector>
#include <QString>
#include <QStringList>
#include <QMap>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>

#define BAR_WIDTH 0.25
#define PEN_WIDTH 1.2
#define GRAPH_HIGH_RATIO 1.2
#define GRAPH_LOW_RATIO 0.85
static const QColor COLOUR_LIST[5] = {QColor(255,0,0,100),QColor(0,255,0,100),QColor(0,0,255,100),QColor(255,255,0,100),QColor(255,0,255,100)};

/**
 * Default Constructor
 * @param parent
 */
widgetBarChart::widgetBarChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widgetBarChart)
{
    ui->setupUi(this);
    this->setStyleSheet(
                "QPushButton#pushButtonSaveBarData {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonSaveBarData:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonSaveBarData:hover {"
                "background-color: rgb(130,130,180);}"

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
    ui->listBarChartMunicipality->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listBarChartYears->setSelectionMode(QAbstractItemView::MultiSelection);

}
/**
 * Construct a line chart widget of a given parent, active measure, low and high range
 * @param parent chosen parent
 * @param meas  active measure
 * @param range low/high range of the data
 */
widgetBarChart::widgetBarChart(QWidget *parent, const Measure &meas, const std::pair<double, double> &range):QWidget(parent),ui(new Ui::widgetBarChart),_meas(new Measure(meas))
{
    ui->setupUi(this);
    _range =range;
    this->setStyleSheet(
                "QPushButton#pushButtonSaveBarData {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonSaveBarData:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonSaveBarData:hover {"
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
    ui->listBarChartMunicipality->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listBarChartYears->setSelectionMode(QAbstractItemView::MultiSelection);
    //List Stuff
    ui->listBarChartMunicipality->addItems(_meas->stringMuniList());
    ui->listBarChartYears->addItems(_meas->yearRange());
    drawChart();

}
/**
 * Deconstructor
 */
widgetBarChart::~widgetBarChart()
{
    delete ui;
    delete _meas;
}
/**
 * @brief widgetBarChart::drawYear draw a given year on a bar chart
 * @param sel the index of the selected year
 */
void widgetBarChart::drawYear(const int &sel) {
    QCPBars *bar = new QCPBars(ui->widgetBarChart_2->xAxis,ui->widgetBarChart_2->yAxis);
    bar->setName(_meas->yearRange().at(sel));
    bar->setWidth(BAR_WIDTH);
    QPen pen;
    bar->setBrush(COLOUR_LIST[sel%5]);
    pen.setWidthF(PEN_WIDTH);

    QVector<double> valueData, keyData;
    ui->widgetBarChart_2->addPlottable(bar);
    double i = 0.25 + 0.2*ui->widgetBarChart_2->xAxis->plottables().size();
    QList<QListWidgetItem*> tempList = ui->listBarChartMunicipality->selectedItems();
    for(QList<QListWidgetItem*>::Iterator it = tempList.begin(); it != tempList.end(); ++it) {
        QListWidgetItem *tempItem = *it;
        keyData << i;
        i+=1.5;
        try {
            valueData.append(_meas->findMuni(tempItem->text()).getYear(_meas->yearRange().at(sel)).value());
        } catch (std::string s) {
            valueData.append(0.0);
        }
    }
    bar->setData(keyData,valueData);
    bar->keyAxis()->setTickVector(keyData);
    ui->widgetBarChart_2->legend->setVisible(true);
    ui->widgetBarChart_2->replot();
}

/**
 * Draw a bar chart using the selected years and Municipalities
 */
void widgetBarChart::drawChart()
{
    ui->widgetBarChart_2->clearPlottables();
    QVector<QString> selectedMuniList;
    QList<QListWidgetItem*> tempMuniList = ui->listBarChartMunicipality->selectedItems();
    for(QList<QListWidgetItem*>::Iterator it = tempMuniList.begin(); it != tempMuniList.end(); ++it) {
        QListWidgetItem *tempItem = *it;
        selectedMuniList.append(_meas->findMuni(tempItem->text()).name());
    }
    QVector<double> keyData;
    double i = 0.25;
    for(QVector<QString>::Iterator it = selectedMuniList.begin(); it != selectedMuniList.end(); ++it) {
        keyData << i;
        i += 1.5;
    }
    ui->widgetBarChart_2->xAxis->setAutoTicks(false);
    ui->widgetBarChart_2->xAxis->setAutoTickLabels(false);
    ui->widgetBarChart_2->xAxis->setTickVector(keyData);
    ui->widgetBarChart_2->xAxis->setTickVectorLabels(selectedMuniList);
    ui->widgetBarChart_2->xAxis->setTickLabelRotation(-60);
    ui->widgetBarChart_2->xAxis->setRange(0.25,selectedMuniList.count()*1.5+1);
    ui->widgetBarChart_2->yAxis->setLabel(_meas->name());
    ui->widgetBarChart_2->yAxis->setRange(_range.first*GRAPH_LOW_RATIO,_range.second*GRAPH_HIGH_RATIO);

    //Check if average is selected
    if(ui->radioButtonMeanAllDataBar->isChecked())
        drawAverage(selectedMuniList.size()*1.5+1);

    //Draw applicable years
    for(int j = 0; j < _meas->yearRange().size(); ++j)
        if(ui->listBarChartYears->item(j)->isSelected()) drawYear(j);

    ui->widgetBarChart_2->legend->setVisible(true);
    ui->widgetBarChart_2->replot();

}
/**
 * Draw a line on the graph representing the average for the _measure
 * @param activeMunis
 */
void widgetBarChart::drawAverage(int activeMunis)
{
    double avg = _meas->measureAverage();
    //std::cout << avg << std::endl;
    //std::cout << activeMunis<< std::endl;
    ui->widgetBarChart_2->addGraph();
    QPen tempPen;
    tempPen.setWidth(3);
    tempPen.setColor(QColor(0,0,0,255));
    ui->widgetBarChart_2->graph(0)->setPen(tempPen);
    ui->widgetBarChart_2->graph(0)->addData(0.0,avg);
    ui->widgetBarChart_2->graph(0)->addData(activeMunis+1,avg);
    ui->widgetBarChart_2->graph(0)->setName("Average");
    ui->widgetBarChart_2->replot();
}
/**
 * Redraw the graph if a municipality is selected or deselected
 */
void widgetBarChart::on_listBarChartMunicipality_itemSelectionChanged()
{
    drawChart();

}
/**
 * Draw a line representing the average for the measure
 * @param checked
 */
void widgetBarChart::on_radioButtonMeanAllDataBar_toggled(bool checked)
{
   drawChart();

}
/**
 * Redraw the graph
 */
void widgetBarChart::on_pushButtonRefresh_clicked()
{
   drawChart();

}


/**
 * @author Jen Stewart
 *  Function to save the Bar Chart as an image file
 *
 */
void widgetBarChart::on_pushButtonSaveBarData_clicked()
{
    if(ui->listBarChartYears->selectedItems().isEmpty()
            || ui->listBarChartMunicipality->selectedItems().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please select data before saving.");
        msgBox.exec();
        return;
    }

      QString fileName = QFileDialog::getSaveFileName(this,"Save File", _dirname,"Images (*.png *.jpg *.bmp)");
      if(fileName.endsWith("png"))
      {
          ui->widgetBarChart_2->savePng(fileName);
          fileName.chop(fileName.length()-fileName.lastIndexOf("/"));
          _dirname = fileName;
      }
      else if(fileName.endsWith("jpg"))
      {
          ui->widgetBarChart_2->saveJpg(fileName);
          fileName.chop(fileName.length()-fileName.lastIndexOf("/"));
          _dirname = fileName;
      }
      else if(fileName.endsWith("bmp"))
      {
          ui->widgetBarChart_2->saveBmp(fileName);
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
 * @brief widgetBarChart::on_listBarChartYears_itemSelectionChanged Redraw the chart when a year is selected/deselected
 */
void widgetBarChart::on_listBarChartYears_itemSelectionChanged()
{
    drawChart();
}
