/**
  @file widgetBarChart.cpp
  @author Alex Clarke, Melanie Imough, Jen Stewart
  @version 0.1

  @section DESCRIPTION

  Class for the display of data in a bar chart
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

#include <string>
#include <QColorDialog>
#include <QInputDialog>
#include <QMenu>

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

    //Add title to bar chart
    ui->widgetBarChart_2->plotLayout()->insertRow(0);
    QString title = meas.name() + " Over the Years";
    ui->widgetBarChart_2->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->widgetBarChart_2, title));
    ui->widgetBarChart_2->yAxis->setLabel(_meas->name());


    //*********************set interractions*************************
    srand(QDateTime::currentDateTime().toTime_t());
      ui->widgetBarChart_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);

      ui->widgetBarChart_2->axisRect()->setupFullAxesBox();
      ui->widgetBarChart_2->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items

      // connect slot that ties some axis selections together (especially opposite axes):
      connect(ui->widgetBarChart_2, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
      // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
      connect(ui->widgetBarChart_2, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
      connect(ui->widgetBarChart_2, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

      // make bottom and left axes transfer their ranges to top and right axes:
      connect(ui->widgetBarChart_2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->widgetBarChart_2->xAxis2, SLOT(setRange(QCPRange)));
      connect(ui->widgetBarChart_2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->widgetBarChart_2->yAxis2, SLOT(setRange(QCPRange)));

      // connect some interaction slots:
      connect(ui->widgetBarChart_2, SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)), this, SLOT(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)));
      connect(ui->widgetBarChart_2, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
      connect(ui->widgetBarChart_2, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

      // setup policy and connect slot for context menu popup:
      ui->widgetBarChart_2->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(ui->widgetBarChart_2, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));


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
 * Draw a bar chart using the selected years and Municipalities back with default features such as
 *labels and background color.
 */
void widgetBarChart::drawChartDefault()
{
    ui->widgetBarChart_2->clearPlottables();

    QString title = _meas->name();
    ui->widgetBarChart_2->plotLayout()->removeAt(0);
    ui->widgetBarChart_2->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->widgetBarChart_2, title));

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
    ui->widgetBarChart_2->axisRect()->setBackground(QBrush(Qt::white));
    ui->widgetBarChart_2->setBackground(QBrush(Qt::white));

    ui->widgetBarChart_2->replot();
}


/**
 * Unables the user to edit the title.
 * @param event keep track of the mouse
 * @param title clicked plot title
 */
void widgetBarChart::titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title)
{

  Q_UNUSED(event)
  // Set the plot title by double clicking on it
  bool ok;
  QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
  if (ok)
  {

    title->setText(newTitle);

  }
  ui->widgetBarChart_2->replot();
}

/**
 * Unables the user to edit the titles of the axis.
 * @param axis that was clicked
 * @param part points to the specific part that was clicked
 */
void widgetBarChart::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
    }
  }
  ui->widgetBarChart_2->replot();
}

/**
 * Unables the user to change the name of the item in the legend.
 * @param legend
 * @param item
 */
void widgetBarChart::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
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
      ui->widgetBarChart_2->replot();
    }
  }
}

/**
 * Keeps track of selection changes made by user
 */
void widgetBarChart::selectionChanged()
{
  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object
  if (ui->widgetBarChart_2->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->widgetBarChart_2->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->widgetBarChart_2->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->widgetBarChart_2->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->widgetBarChart_2->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->widgetBarChart_2->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->widgetBarChart_2->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->widgetBarChart_2->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->widgetBarChart_2->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->widgetBarChart_2->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->widgetBarChart_2->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->widgetBarChart_2->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:

  for (int i=0; i<ui->widgetBarChart_2->plottableCount(); ++i)
  {
    QCPAbstractPlottable *plottable = ui->widgetBarChart_2->plottable(i);
    QCPPlottableLegendItem *item = ui->widgetBarChart_2->legend->itemWithPlottable(plottable);

    if (item->selected() || plottable->selected())
    {
      item->setSelected(true);

      plottable->setSelected(true);
    }
  }
}

/**
 * Allow user to drag around plots in the chart.
 */
void widgetBarChart::mousePress()
{
  if (ui->widgetBarChart_2->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->widgetBarChart_2->axisRect()->setRangeDrag(ui->widgetBarChart_2->xAxis->orientation());
  else if (ui->widgetBarChart_2->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->widgetBarChart_2->axisRect()->setRangeDrag(ui->widgetBarChart_2->yAxis->orientation());
  else
    ui->widgetBarChart_2->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

/**
 * Allow axis to be zoomed.
 */
void widgetBarChart::mouseWheel()
{
  if (ui->widgetBarChart_2->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->widgetBarChart_2->axisRect()->setRangeZoom(ui->widgetBarChart_2->xAxis->orientation());
  else if (ui->widgetBarChart_2->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->widgetBarChart_2->axisRect()->setRangeZoom(ui->widgetBarChart_2->yAxis->orientation());
  else
    ui->widgetBarChart_2->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}


/**
 * Keep track of position of mouse to display appropriate menu.
 * @param pos position of mouse
 */
void widgetBarChart::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);

    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->widgetBarChart_2->legend->selectTest(pos, false) >= 0) // context menu on legend requested
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

      if (ui->widgetBarChart_2->selectedPlottables().count() > 0){
        menu->addAction("Change plot color", this, SLOT(changeColorLine()));

      }

  menu->popup(ui->widgetBarChart_2->mapToGlobal(pos));
}
}
/**
 * Set background of graph with an image selected by a user.
 */
void widgetBarChart::selectImageBackground(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Image Files (*.png *.jpg *.bmp *.gif *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
    QPixmap pixmap1(fileName);
    QPalette p = palette();

    QRect rect = ui->widgetBarChart_2->rect();

    QSize size(rect.width() , rect.height());

    QPixmap pixmap(pixmap1.scaled(size));

    p.setBrush(QPalette::Background,  pixmap);
    setPalette(p);

    ui->widgetBarChart_2->setBackground(QBrush(pixmap));

    ui->widgetBarChart_2->replot();
}

/**
 * Unable the user to select an image and set it as axis background of graph.
 */
void widgetBarChart::selectImageAxisBackground(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Image Files (*.png *.jpg *.bmp *.gif *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)"));
    QPixmap pixmap1(fileName);
    QPalette p = palette();

    p.setBrush(QPalette::Background,  pixmap1);
    setPalette(p);

    QSize size(ui->widgetBarChart_2->axisRect()->width(), ui->widgetBarChart_2->axisRect()->height());
    QPixmap pixmap(pixmap1.scaled(size));

    ui->widgetBarChart_2->axisRect()->setBackground(QBrush(pixmap));
    ui->widgetBarChart_2->replot();
}

/**
 * Unable the user to change the color of the legend.
 */
void widgetBarChart::changeLegendColor(){
    QColorDialog *colordialog = new QColorDialog(this);
    colordialog->show();
    QColor selectedColor = colordialog->getColor(Qt::transparent, this);
    if (selectedColor.isValid()){
        ui->widgetBarChart_2->legend->setBrush(QBrush(selectedColor));
        ui->widgetBarChart_2->replot();
    }
    colordialog->close();
}

/**
 * change the color of the selected graph or the background of the axis graph if it was seleced.
 */
void widgetBarChart::changeColorLine(){

    QColorDialog *colordialog = new QColorDialog(this);
    colordialog->show();
    QColor selectedColor = colordialog->getColor(Qt::transparent, this);

    if (selectedColor.isValid()){
        ui->widgetBarChart_2->selectedPlottables().first()->setBrush(QBrush(selectedColor));
        ui->widgetBarChart_2->replot();
    }
    colordialog->close();
}

/**
 * Let the user change the color of the axis background.
 */
void widgetBarChart::changeAxisColor(){
    QColorDialog *colordialog = new QColorDialog(this);
    colordialog->show();
    QColor selectedColor = colordialog->getColor(Qt::transparent, this);

    if (selectedColor.isValid()){
        QPen newPenColor;
        newPenColor.setWidth(4);
        newPenColor.setColor(selectedColor);
        ui->widgetBarChart_2->axisRect()->setBackground(QBrush(selectedColor));
        ui->widgetBarChart_2->replot();
    }
    colordialog->close();
}

/**
 * Let the user change the color of the whole plot
 */
void widgetBarChart::changeBackgroundColor(){
    QColorDialog *colordialog = new QColorDialog(this);
    colordialog->show();
    QColor selectedColor = colordialog->getColor(Qt::transparent, this);
    if (selectedColor.isValid()){
        QPen newPenColor;
        newPenColor.setWidth(4);
        newPenColor.setColor(selectedColor);
        ui->widgetBarChart_2->setBackground(QBrush(selectedColor));
        ui->widgetBarChart_2->replot();
    }
    colordialog->close();
}

/**
 * Let the user change the location of the legend
 */
void widgetBarChart::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender()))
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->widgetBarChart_2->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->widgetBarChart_2->replot();
    }
  }
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
 * Draw a line representing the average for the measure.
 * @param checked
 */
void widgetBarChart::on_radioButtonMeanAllDataBar_toggled(bool checked)
{
   drawChart();

}
/**
 * Redraw the graph back to default features.
 */
void widgetBarChart::on_pushButtonRefresh_clicked()
{
   drawChartDefault();

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
