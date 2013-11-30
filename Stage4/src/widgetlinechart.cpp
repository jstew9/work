/**
  @file widgetlinechart.cpp
  @author Alex Clarke, Melanie Imough, Jen Stewart
  @version 0.1

  @section DESCRIPTION

  Class for the display of data in a line graph
  */

#include "widgetlinechart.h"
#include "ui_widgetlinechart.h"
#include "measure.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <QMessageBox>
#include <QFileDialog>

#include <string>
#include <QColorDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>

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

    QString title = meas.name() + " Over the Years";
    //insert row for title
    ui->widgetLineChart_2->plotLayout()->insertRow(0);
    //Add default labels to line chart
    ui->widgetLineChart_2->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->widgetLineChart_2, title));
    ui->widgetLineChart_2->xAxis->setLabel("Years");
    ui->widgetLineChart_2->yAxis->setLabel(_meas->name());

    //*********************set interractions*************************
    srand(QDateTime::currentDateTime().toTime_t());
      ui->widgetLineChart_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);

      ui->widgetLineChart_2->axisRect()->setupFullAxesBox();
      ui->widgetLineChart_2->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items


      // connect slot that ties some axis selections together (especially opposite axes):
      connect(ui->widgetLineChart_2, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
      // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
      connect(ui->widgetLineChart_2, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
      connect(ui->widgetLineChart_2, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

      // make bottom and left axes transfer their ranges to top and right axes:
      connect(ui->widgetLineChart_2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widgetLineChart_2->xAxis2, SLOT(setRange(QCPRange)));
      connect(ui->widgetLineChart_2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widgetLineChart_2->yAxis2, SLOT(setRange(QCPRange)));

      // connect some interaction slots:
      connect(ui->widgetLineChart_2, SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)), this, SLOT(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)));
      connect(ui->widgetLineChart_2, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));

      connect(ui->widgetLineChart_2, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

      // setup policy and connect slot for context menu popup:
      ui->widgetLineChart_2->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(ui->widgetLineChart_2, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

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
        ui->widgetLineChart_2->addGraph();
        QPen tempPen(QColor(rand()%255,rand()%255,rand()%255,255));
        tempPen.setWidth(PEN_WIDTH);
        ui->widgetLineChart_2->graph(j)->setPen(tempPen);
        ++j;
    }

    ui->widgetLineChart_2->xAxis->setAutoTickStep(false);
    ui->widgetLineChart_2->xAxis->setTickStep(1.0);
    ui->widgetLineChart_2->xAxis->setTickLabelRotation(-60);
     //ui->widgetLineChart_2->xAxis->setLabel("Years");
    if(_meas->yearRange().size() > 0 )ui->widgetLineChart_2->xAxis->setRange(_meas->yearRange().at(0).toInt(),_meas->yearRange().at(_meas->yearRange().size()-1).toInt());
    //ui->widgetLineChart_2->yAxis->setLabel(_meas->name());
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
 * Draw a line graph using the selected years and Municipalities and plots everything back to default.
 * @param lowrange the low range of the data
 * @param highrange the high range of the data
 */
void WidgetLineChart::drawChartDefault()
{
    ui->widgetLineChart_2->clearPlottables();

    //put back default title
    ui->widgetLineChart_2->plotLayout()->removeAt(0);
    QString title = _meas->name() + " Over the Years";
    ui->widgetLineChart_2->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->widgetLineChart_2,title));

    //Random Number stuff
    srand(time(NULL));

    QList<QListWidgetItem*> tempMuniList = ui->listLineChartMunicipality->selectedItems();
    int j = 0;
    for(QList<QListWidgetItem*>::Iterator it = tempMuniList.begin(); it != tempMuniList.end(); ++it) {

        ui->widgetLineChart_2->addGraph();
        QPen tempPen(QColor(rand()%255,rand()%255,rand()%255,255));
        tempPen.setWidth(PEN_WIDTH);
        ui->widgetLineChart_2->graph(j)->setPen(tempPen);
        ++j;
    }

    ui->widgetLineChart_2->xAxis->setAutoTickStep(false);
    ui->widgetLineChart_2->xAxis->setTickStep(1.0);
    ui->widgetLineChart_2->xAxis->setTickLabelRotation(-60);
    ui->widgetLineChart_2->xAxis->setLabel("Years");
    if(_meas->yearRange().size() > 0 )ui->widgetLineChart_2->xAxis->setRange(_meas->yearRange().at(0).toInt(),_meas->yearRange().at(_meas->yearRange().size()-1).toInt());
    ui->widgetLineChart_2->yAxis->setLabel(_meas->name());
    ui->widgetLineChart_2->yAxis->setRange(_range.first*GRAPH_LOW_RATIO,_range.second*GRAPH_HIGH_RATIO);

    for(int j = 0; j < _meas->yearRange().size(); ++j)
        if(ui->listLineChartYears->item(j)->isSelected()) drawYear(j);

    if(ui->radioButtonMeanAllDataBar->isChecked()) {
        drawAverage();
    }

    ui->widgetLineChart_2->setBackground(QBrush(Qt::white));
    ui->widgetLineChart_2->axisRect()->setBackground(QBrush(Qt::white));

    ui->widgetLineChart_2->legend->setVisible(true);
    ui->widgetLineChart_2->replot();
}

/**
 * Unables the user to edit the title.
 * @param event keep track of the mouse
 * @param title clicked plot title
 */
void WidgetLineChart::titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title)
{
  Q_UNUSED(event)
  // Set the plot title by double clicking on it
  bool ok;
  QString newTitle = QInputDialog::getText(this, "title", "New plot title:", QLineEdit::Normal, title->text(), &ok);
  if (ok)
  {

    title->setText(newTitle);

  }
  ui->widgetLineChart_2->replot();
}

/**
 * Unables the user to edit the titles of the axis.
 * @param axis that was clicked
 * @param part points to the specific part that was clicked
 */
void WidgetLineChart::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;

    QString newLabel = QInputDialog::getText(this, "axis label", "New axis label:", QLineEdit::Normal, axis->label(), &ok);

    if (ok)
    {
      axis->setLabel(newLabel);
    }
  }
  ui->widgetLineChart_2->replot();
}

/**
 * Unables the user to change the name of the item in the legend.
 * @param legend
 * @param item
 */
void WidgetLineChart::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
    }
  }
  ui->widgetLineChart_2->replot();
}

/**
 * Keeps track of selection changes made by user
 */
void WidgetLineChart::selectionChanged()
{
  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object
  if (ui->widgetLineChart_2->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->widgetLineChart_2->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->widgetLineChart_2->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->widgetLineChart_2->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->widgetLineChart_2->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->widgetLineChart_2->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->widgetLineChart_2->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->widgetLineChart_2->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->widgetLineChart_2->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->widgetLineChart_2->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->widgetLineChart_2->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->widgetLineChart_2->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->widgetLineChart_2->graphCount(); ++i)
  {
    QCPGraph *graph = ui->widgetLineChart_2->graph(i);
    QCPPlottableLegendItem *item = ui->widgetLineChart_2->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelected(true);
    }
  }
}

/**
 * Allow user to drag around plots in the chart.
 */
void WidgetLineChart::mousePress()
{
  if (ui->widgetLineChart_2->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->widgetLineChart_2->axisRect()->setRangeDrag(ui->widgetLineChart_2->xAxis->orientation());
  else if (ui->widgetLineChart_2->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->widgetLineChart_2->axisRect()->setRangeDrag(ui->widgetLineChart_2->yAxis->orientation());
  else
    ui->widgetLineChart_2->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

/**
 * Allow axis to be zoomed.
 */
void WidgetLineChart::mouseWheel()
{
  if (ui->widgetLineChart_2->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->widgetLineChart_2->axisRect()->setRangeZoom(ui->widgetLineChart_2->xAxis->orientation());
  else if (ui->widgetLineChart_2->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->widgetLineChart_2->axisRect()->setRangeZoom(ui->widgetLineChart_2->yAxis->orientation());
  else
    ui->widgetLineChart_2->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

/**
 * Keep track of position of mouse to display appropriate menu.
 * @param pos position of mouse
 */
void WidgetLineChart::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);

    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->widgetLineChart_2->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
      //unable the user to change the location of the legend and change its color
     menu->addAction("Change backgroud color", this, SLOT(changeLegendColor()));
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else
  {
    //enable user to change the background of the graph
      menu->addAction("Choose image as graph background", this, SLOT(selectImageBackground()));
      menu->addAction("Choose image as axis background", this, SLOT(selectImageAxisBackground()));
      menu->addAction("Change axis color", this, SLOT(changeAxisColor()));
      menu->addAction("Change graph color", this, SLOT(changeBackgroundColor()));

    if (ui->widgetLineChart_2->selectedGraphs().size() > 0){
        menu->addAction("Change line color", this, SLOT(changeColorLine()));

      }
  }
  menu->popup(ui->widgetLineChart_2->mapToGlobal(pos));
}

/**
 * Set background of graph with an image selected by a user.
 */
void WidgetLineChart::selectImageBackground(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Image Files (*.png *.jpg *.bmp *.gif *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
    QPixmap pixmap1(fileName);
    QPalette p = palette();

    QRect rect = ui->widgetLineChart_2->rect();

    QSize size(rect.width(), rect.height());
    QPixmap pixmap(pixmap1.scaled(size));
    p.setBrush(QPalette::Background, pixmap);
    setPalette(p);
    ui->widgetLineChart_2->setBackground(QBrush(pixmap));

    ui->widgetLineChart_2->replot();
}

/**
 * Unable the user to select an image and set it as axis background of graph.
 */
void WidgetLineChart::selectImageAxisBackground(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Image Files (*.png *.jpg *.bmp *.gif *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
    QPixmap pixmap1(fileName);
    QPalette p = palette();

    p.setBrush(QPalette::Background,  pixmap1);
    setPalette(p);

    QSize size(ui->widgetLineChart_2->axisRect()->width(), ui->widgetLineChart_2->axisRect()->height());
    QPixmap pixmap(pixmap1.scaled(size));

    ui->widgetLineChart_2->axisRect()->setBackground(QBrush(pixmap));

    ui->widgetLineChart_2->replot();
}

/**
 * Unable the user to change the color of the legend.
 */
void WidgetLineChart::changeLegendColor(){
    QColorDialog *colordialog = new QColorDialog(this);
    colordialog->show();
    QColor selectedColor = colordialog->getColor(Qt::transparent, this);
    if (selectedColor.isValid()){
        ui->widgetLineChart_2->legend->setBrush(QBrush(selectedColor));
        ui->widgetLineChart_2->replot();
    }
    colordialog->close();
}

/**
 * change the color of the selected graph or the background of the axis graph if it was seleced.
 */
void WidgetLineChart::changeColorLine(){

    QColorDialog *colordialog = new QColorDialog(this);
    colordialog->show();
    QColor selectedColor = colordialog->getColor(Qt::transparent, this);

    if (selectedColor.isValid()){
        QPen newPenColor;
        newPenColor.setWidth(4);
        newPenColor.setColor(selectedColor);
        ui->widgetLineChart_2->selectedGraphs().first()->setPen(newPenColor); //if graph was selected then change its color
        ui->widgetLineChart_2->replot();
    }
    colordialog->close();
}

/**
 * Let the user change the color of the axis background.
 */
void WidgetLineChart::changeAxisColor(){
    QColorDialog *colordialog = new QColorDialog(this);
    colordialog->show();
    QColor selectedColor = colordialog->getColor(Qt::transparent, this);

    if (selectedColor.isValid()){
        QPen newPenColor;
        newPenColor.setWidth(4);
        newPenColor.setColor(selectedColor);
        ui->widgetLineChart_2->axisRect()->setBackground(QBrush(selectedColor));
        ui->widgetLineChart_2->replot();
    }
    colordialog->close();
}

/**
 * Let the user change the color of the whole plot
 */
void WidgetLineChart::changeBackgroundColor(){
    QColorDialog *colordialog = new QColorDialog(this);
    colordialog->show();
    QColor selectedColor = colordialog->getColor(Qt::transparent, this);
    if (selectedColor.isValid()){
        QPen newPenColor;
        newPenColor.setWidth(4);
        newPenColor.setColor(selectedColor);
        ui->widgetLineChart_2->setBackground(QBrush(selectedColor));
        ui->widgetLineChart_2->replot();
    }
    colordialog->close();
}

/**
 * Let the user change the location of the legend
 */
void WidgetLineChart::moveLegend()
{

  if (QAction* contextAction = qobject_cast<QAction*>(sender()))
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->widgetLineChart_2->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->widgetLineChart_2->replot();
    }
  }
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
 * Redraw the line chart back to default features.
 */
void WidgetLineChart::on_pushButtonRefresh_clicked()
{
    drawChartDefault();
}
/**
 * @brief WidgetLineChart::on_listLineChartYears_itemSelectionChanged Redraw the graph when year selection changes
 */
void WidgetLineChart::on_listLineChartYears_itemSelectionChanged()
{
    drawChart();
}
