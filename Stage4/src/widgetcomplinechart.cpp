/**
  @file widgetcomplinechar.cpp
  @author Alex Clarke, Melanie Imough, Jen Stewart
  @version 0.1

  @section DESCRIPTION

  Class to display a line graph comparing two measures

  */

#include "widgetcomplinechart.h"
#include "ui_widgetcomplinechart.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>

#define GRAPH_LOW_RATIO 0.85
#define GRAPH_HIGH_RATIO 1.2
#define PEN_WIDTH 3
/**
 * @brief WidgetCompLineChart::WidgetCompLineChart default constructor
 * @param parent
 */
WidgetCompLineChart::WidgetCompLineChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetCompLineChart), _meas(new Measure()), _meas2(new Measure()),_mlist(new QStringList()), _ylist(new QStringList())
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
}
/**
 * @brief WidgetCompLineChart::WidgetCompLineChart Construct a WidgetCompLineChart with given measures
 * @param parent parent object
 * @param meas first measure
 * @param meas2 second measure
 */
WidgetCompLineChart::WidgetCompLineChart(QWidget *parent, const Measure &meas, const Measure &meas2):QWidget(parent),ui(new Ui::WidgetCompLineChart),
    _meas(new Measure(meas)),_meas2(new Measure(meas2)),_mlist(new QStringList()), _ylist(new QStringList())
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
    generateMunis();
    generateYears();
    ui->listLineChartMunicipality_4->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listLineChartYears_4->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listLineChartMunicipality_4->addItems(*_mlist);
    ui->listLineChartYears_4->addItems(*_ylist);
    drawChart();

}
/**
 * @brief WidgetCompLineChart::~WidgetCompLineChart deconstructor
 */
WidgetCompLineChart::~WidgetCompLineChart()
{
    delete ui;
    delete _meas;
    delete _meas2;
    delete _ylist;
    delete _mlist;
}
/**
 * @brief WidgetCompLineChart::generateYears save only those years which belong to both measures
 */
void WidgetCompLineChart::generateYears()
{
    for(QStringList::const_iterator it = _meas->yearRange().begin(); it != _meas->yearRange().end(); ++it)
        if(_meas2->yearRange().contains(*it)) _ylist->append(*it);

}
/**
 * @brief WidgetCompLineChart::generateMunis save only those Municipalities which belong to both measures
 */
void WidgetCompLineChart::generateMunis()
{
    QStringList tempList = _meas2->stringMuniList();
    for(QStringList::const_iterator it = tempList.begin(); it != tempList.end(); ++it)
        if(_meas->stringMuniList().contains(*it))
            _mlist->append(*it);
}
/**
 * @brief WidgetCompLineChart::drawChart draw the chart
 */
void WidgetCompLineChart::drawChart()
{

    ui->widgetCompLineChart_2->clearPlottables();
    srand(time(NULL));
    int j = 0;
    QList<QListWidgetItem*> tempMuniList = ui->listLineChartMunicipality_4->selectedItems();
    for(QList<QListWidgetItem*>::Iterator it = tempMuniList.begin(); it != tempMuniList.end(); ++it) {
        ui->widgetCompLineChart_2->addGraph();
        QPen tempPen(QColor(rand()%255,rand()%255,rand()%255,255));
        tempPen.setWidth(PEN_WIDTH);
        ui->widgetCompLineChart_2->graph(j)->setPen(tempPen);
        ++j;
    }
    int k = 0;
    for(QList<QListWidgetItem*>::Iterator it = tempMuniList.begin(); it != tempMuniList.end(); ++it) {
        ui->widgetCompLineChart_2->addGraph(ui->widgetCompLineChart_2->xAxis2,ui->widgetCompLineChart_2->yAxis2);
        QPen tempPen(QColor(rand()%255,rand()%255,rand()%255,255));
        tempPen.setWidth(PEN_WIDTH);
        ui->widgetCompLineChart_2->graph(k+j)->setPen(tempPen);
        ++k;
    }
    ui->widgetCompLineChart_2->xAxis->setAutoTickStep(false);
    ui->widgetCompLineChart_2->xAxis->setTickStep(1.0);
    ui->widgetCompLineChart_2->xAxis->setTickLabelRotation(-60);
    ui->widgetCompLineChart_2->xAxis2->setAutoTickStep(false);
    ui->widgetCompLineChart_2->xAxis2->setTickStep(1.0);
    ui->widgetCompLineChart_2->xAxis2->setTickLabelRotation(-60);
    if(_ylist->size() > 0) {
        ui->widgetCompLineChart_2->xAxis->setRange(_ylist->at(0).toInt(),_ylist->at(_ylist->size()-1).toInt());
        ui->widgetCompLineChart_2->xAxis2->setRange(_ylist->at(0).toInt(),_ylist->at(_ylist->size()-1).toInt());
    }
    ui->widgetCompLineChart_2->yAxis->setLabel(_meas->id()+":"+_meas->name());
    ui->widgetCompLineChart_2->yAxis2->setLabel(_meas2->id()+":"+_meas2->name());
    ui->widgetCompLineChart_2->yAxis->setRange(_meas->maxmin().first*GRAPH_LOW_RATIO,_meas->maxmin().second*GRAPH_HIGH_RATIO);
    ui->widgetCompLineChart_2->yAxis2->setRange(_meas2->maxmin().first*GRAPH_LOW_RATIO,_meas2->maxmin().second*GRAPH_HIGH_RATIO);
    ui->widgetCompLineChart_2->yAxis2->setVisible(true);

    for(int i = 0; i < _meas->yearRange().size(); ++i) {
        if(ui->listLineChartYears_4->item(i)->isSelected()) {
            drawYear(i);
            drawYear2(i,k);
        }

    }

    ui->widgetCompLineChart_2->legend->setVisible(true);
    ui->widgetCompLineChart_2->replot();
}
/**
 * @brief WidgetCompLineChart::drawYear draw a year of the first measure
 * @param sel the index of the year to draw
 */
void WidgetCompLineChart::drawYear(const int &sel)
{
    QList<QListWidgetItem*> tempList = ui->listLineChartMunicipality_4->selectedItems();
    int i = 0;
    for(QList<QListWidgetItem*>::Iterator it = tempList.begin(); it != tempList.end(); ++it) {
        QListWidgetItem *tempItem = *it;
        Municipality tempMuni = _meas->findMuni(tempItem->text());
        try {
            ui->widgetCompLineChart_2->graph(i)->addData(_meas->yearRange().at(sel).toInt(),tempMuni.getYear(_meas->yearRange().at(sel)).value());
        } catch(std::string s) {
        }
        ui->widgetCompLineChart_2->graph(i)->setName(tempMuni.name()+ ":" + _meas->id());
        ++i;
    }

}
/**
 * @brief WidgetCompLineChart::drawYear2 draw year for the second measure
 * @param sel the index of the year to draw
 * @param offset the offset in the list of graphs
 */
void WidgetCompLineChart::drawYear2(const int &sel,const int &offset)
{
    QList<QListWidgetItem*> tempList = ui->listLineChartMunicipality_4->selectedItems();
    int i = 0;
    for(QList<QListWidgetItem*>::Iterator it = tempList.begin(); it != tempList.end(); ++it) {
        QListWidgetItem *tempItem = *it;
        Municipality tempMuni = _meas2->findMuni(tempItem->text());
        try {
            ui->widgetCompLineChart_2->graph(i+offset)->addData(_ylist->at(sel).toInt(),tempMuni.getYear(_meas2->yearRange().at(sel)).value());
        } catch(std::string s) {
        }
        ui->widgetCompLineChart_2->graph(i+offset)->setName(tempMuni.name()+ ":" + _meas2->id());
        ++i;
    }

}
/**
 * @brief WidgetCompLineChart::on_listLineChartMunicipality_4_itemSelectionChanged reload the chart
 */
void WidgetCompLineChart::on_listLineChartMunicipality_4_itemSelectionChanged()
{
    drawChart();
}
/**
 * @brief WidgetCompLineChart::on_pushButtonRefresh_clicked reload the chart
 */
void WidgetCompLineChart::on_pushButtonRefresh_clicked()
{
    drawChart();
}

/**
 * @author Jen Stewart
 *  Function to save the Line Chart as an image file
 *
 */
void WidgetCompLineChart::on_pushButtonSaveLineData_clicked()
{
    if(ui->listLineChartYears_4->selectedItems().isEmpty()
            || ui->listLineChartMunicipality_4->selectedItems().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please select data before saving.");
        msgBox.exec();
        return;
    }
      QString fileName = QFileDialog::getSaveFileName(this,"Save File", _dirname,"Images (*.png *.jpg *.bmp)");
      if(fileName.endsWith("png"))
      {
          ui->widgetCompLineChart_2->savePng(fileName);
          fileName.chop(fileName.length()-fileName.lastIndexOf("/"));
          _dirname = fileName;
      }
      else if(fileName.endsWith("jpg"))
      {
          ui->widgetCompLineChart_2->saveJpg(fileName);
          fileName.chop(fileName.length()-fileName.lastIndexOf("/"));
          _dirname = fileName;
      }
      else if(fileName.endsWith("bmp"))
      {
          ui->widgetCompLineChart_2->saveBmp(fileName);
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
 * @brief WidgetCompLineChart::on_listLineChartYears_4_itemSelectionChanged reload the graph
 */
void WidgetCompLineChart::on_listLineChartYears_4_itemSelectionChanged()
{
    drawChart();
}
