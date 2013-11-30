/**
 * @file  choosedata.cpp
 * @author  Melanie Imough, Alex Clarke, Jamie Finnigan
 * @version 0.1
 *
 * @section DESCRIPTION
 *
 * This is class makes a screen that enables the user to start filtering data. This class makes the screen that
 * comes right after the "welcome" screen. The user can activate dialogbarchart, dialoglinechart, and dialogtable
 * screens from this screen.
 */

#include <QFileDialog>

#include "choosedata.h"
#include "ui_choosedata.h"
#include "widgetbarchart.h"
#include "widgetlinechart.h"
#include "widgetcomplinechart.h"
#include "widgettablechart.h"
#include "filterdata.h"
#include "servicetype.h"
#include <iostream>


/**
 * Constructor that sets the screen that enables the user to choose
 * and filter data.
 *
 * @param *parent Pointer to the QWidget
 *
 */
ChooseData::ChooseData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseData), _measure(new Measure()), _measure2(new Measure())
{
    ui->setupUi(this);
    this->setStyleSheet(
                "QPushButton#pushButtonTable {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonTable:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonTable:hover {"
                "background-color: rgb(130,130,180);}"

                "QPushButton#pushButtonBarChart {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonBarChart:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonBarChart:hover {"
                "background-color: rgb(130,130,180);}"

                "QPushButton#pushButtonLineChart {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonLineChart:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonLineChart:hover {"
                "background-color: rgb(130,130,180);}"

                "QPushButton#pushButtonActiveGraphs {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 18px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonActiveGraphs:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonActiveGraphs:hover {"
                "background-color: rgb(130,130,180);}"

                "QPushButton#pushButtonChooseFilter {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "font: bold 22px;"
                "border-radius: 5px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonChooseFilter:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonChooseFilter:hover {"
                "background-color: rgb(130,130,180);}"
                );

    displayDialog = new DisplayWindow(this);
    _fdata = new FilterData();
    ui->treeWidget->clear();
    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setColumnWidth(1,250);
    ui->treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    populateTreeWithServices();

    QObject::connect(this, SIGNAL(serviceReady(QString)),
                     _fdata, SLOT(parseService(QString)));

    QObject::connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
                     this, SLOT(getMeasuresForService(QTreeWidgetItem*)));

    QObject::connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
                     this, SLOT(parseMeasures(QTreeWidgetItem*)));



    QObject::connect(_fdata, SIGNAL(measureReady(QString,QString,QString)),
                     this, SLOT(addItem(QString,QString,QString)));

    QObject::connect(this, SIGNAL(measureReady(QString,QString)),
                     _fdata, SLOT(parseMeasures(QString,QString)));

    QObject::connect(_fdata, SIGNAL(resultReady(Measure&)),
                     this, SLOT(updateMeasure(Measure&)));


}

/**
 * Destructor that deletes the choosedata screen.
 *
 * @param *parent Pointer to the QWidget
 *
 */
ChooseData::~ChooseData()
{
    delete ui;
    delete _fdata;
    delete displayDialog;

 }

/**
 * Slot that activate the creation of a new dialog screen called dialogtable were the user will
 * be able to see the data in a table and continue to filter the data.
 *
 */
void ChooseData::on_pushButtonTable_clicked()
{


    if(_measure->name() != "") {
      displayDialog->addTab(new WidgetTableChart(displayDialog,*_measure),"Table:" + _measure->id());
      displayDialog->show();
    }
}

/**
 * Slot that activate the creation of a new dialog screen called dialogbarchart were the user will
 * be able to see the data in a bar chart and continue to filter the data.
 *
 */
void ChooseData::on_pushButtonBarChart_clicked()
{

    if(_measure->name() != "") {
        displayDialog->addTab(new widgetBarChart(displayDialog,*_measure,_measure->maxmin()),"Bar:" + _measure->id());
        displayDialog->show();
    }

}

/**
 * Slot that activate the creation of a new dialog screen called dialoglinechart were the user will
 * be able to see the data in a line chart and continue to filter the data.
 *
 */
void ChooseData::on_pushButtonLineChart_clicked()
{
    if(_measure->name() != "" && !ui->chkbox->isChecked()) {
        displayDialog->addTab(new WidgetLineChart(displayDialog, *_measure,_measure->maxmin()),"Line:" + _measure->id());
        displayDialog->show();
    }
    else if(_measure->name() != "" && _measure2->name() != "" && ui->treeWidget->selectedItems().size() == 2){
        displayDialog->addTab(new WidgetCompLineChart(displayDialog, *_measure,*_measure2),"Comp Line:" + _measure->id());
        displayDialog->show();
    }

}

/**
 * Slot that activate the creation of a file dialog where the user can choose a .csv file
 * containing filtered data. The user can then press "get table", "get bar chart", or "get line chart" to
 * get the filtered data in a figure.
 *
 */
void ChooseData::on_pushButtonChooseFilter_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.csv)"));
   // ui->textChoosenFile->setText(fileName); //display the chosen .csv file
}

/**
 * @brief ChooseData::populateTreeWithServices add the services to the service / measure tree
 */
void ChooseData:: populateTreeWithServices(){

    for(QMap<QString,ServiceType>::const_iterator i = _fdata->serviceList().begin();i != _fdata->serviceList().end(); ++i){
        QString name = _fdata->serviceName(i.key());
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, name);
        item->setText(1, i.key());
        ui->treeWidget->addTopLevelItem(item);

    }


}

/**
 * @brief ChooseData::getMeasuresForService slot for deciding the id of the service
 * @param item holding the ServiceType
 */
void ChooseData::getMeasuresForService(QTreeWidgetItem *item){

        //qDebug()<<"In getMeasureForService";
        if(item->childCount() == 0 && !item->parent()){
            QString sID;
            for(QMap<QString,QString>::const_iterator i = _fdata->serviceNameList().begin();i != _fdata->serviceNameList().end(); ++i){
                QString name = _fdata->serviceName(i.key());
                if(name == item->text(0))
                   sID = i.key();

            }
            _sID = sID;
            emit serviceReady(_sID);
        }
    }
/**
 * @brief ChooseData::parseMeasures slot for preparing the sID and mID for parsing the measures,
 * @param item containing the serviceType
 */
void ChooseData::parseMeasures(QTreeWidgetItem *item){
    //qDebug()<<"In in ParseMeasures()";
    if(item->parent()){
        QString sID;
        for(QMap<QString,QString>::const_iterator i = _fdata->serviceNameList().begin();i != _fdata->serviceNameList().end(); ++i){
            QString name = _fdata->serviceName(i.key());
            if(name == item->parent()->text(0))
               sID = i.key();

        }
        _sID = sID;
        emit measureReady( item->text(1),_sID);
    }
}


/**
 * @brief ChooseData::addItem adds measures to the QTreeWidget under their appropriate service type
 * @param sID id for the serviceType
 * @param measure id for the measure of interest
 * @param title description for the measure
 */
void ChooseData::addItem(QString sID, QString measure, QString title){
    QTreeWidgetItem* item = new QTreeWidgetItem();
    //set up new item

    item->setText(0,title);
    item->setText(1,measure);

    //determine who the parent is
    int parentID = 0;
    for(QMap<QString,QString>::const_iterator i = _fdata->serviceNameList().begin();i != _fdata->serviceNameList().end(); ++i){
        if(sID == i.key()){
            break;
        }
        parentID++;
    }

    //qDebug()<<"adding item to tree widget pnum = "<<parentID<< " sID = "<< sID<< "columnCount() "<< item->columnCount();
    ui->treeWidget->topLevelItem(parentID)->addChild(item);

}
/**
 * @brief ChooseData::updateMuni set the last measure parsed to the active measure for the QPlots
 * @param m measure parsed last
 */
void ChooseData::updateMeasure(Measure& m){
    //qDebug()<<"In updateMuni";


    try {
        *_measure = _fdata->loadMeasure(ui->treeWidget->currentItem()->parent()->text(1),ui->treeWidget->currentItem()->text(1));
        if(ui->treeWidget->selectedItems().size() > 1 && _measure->id() == ui->treeWidget->selectedItems()[0]->text(1))
            *_measure2 = _fdata->loadMeasure(ui->treeWidget->selectedItems()[1]->parent()->text(1),ui->treeWidget->selectedItems()[1]->text(1));
        else if(ui->treeWidget->selectedItems().size() > 1)
            *_measure2 = _fdata->loadMeasure(ui->treeWidget->selectedItems()[0]->parent()->text(1),ui->treeWidget->selectedItems()[0]->text(1));

        _measure->calcYearRange();
        _measure2->calcYearRange();
    } catch(std::string s) {
      std::cout << s << std::endl;
    }
}
/**
 * @brief ChooseData::on_pushButtonActiveGraphs_clicked Display active graphs
 */
void ChooseData::on_pushButtonActiveGraphs_clicked()
{
    displayDialog->show();

}
/**
 * @brief ChooseData::on_treeWidget_itemSelectionChanged Handle limiting selections to two when in comparison mode
 */
void ChooseData::on_treeWidget_itemSelectionChanged()
{
    if(ui->treeWidget->currentItem() != NULL){
        if(ui->treeWidget->currentItem()->parent() == NULL)
            ui->treeWidget->currentItem()->setSelected(false);
        if(ui->treeWidget->selectedItems().size() > 2)
            ui->treeWidget->selectedItems()[0]->setSelected(false);
        if(!ui->treeWidget->currentItem()->isSelected() && ui->treeWidget->selectedItems().size() == 1) {
            ui->treeWidget->setCurrentItem(ui->treeWidget->selectedItems()[0]);
            ui->treeWidget->currentItem()->setSelected(true);
        }
        if(ui->treeWidget->selectedItems().size() == 0) {
            _measure = new Measure();
        }
    }
}


/**
 * @brief ChooseData::on_chkbox_toggled Enable/Disable Comparison Mode
 * @param checked
 */
void ChooseData::on_chkbox_toggled(bool checked)
{
    if(checked) {
        ui->treeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
        ui->pushButtonBarChart->hide();
        ui->pushButtonTable->hide();
    } else {
        if(ui->treeWidget->selectedItems().size() == 2) ui->treeWidget->selectedItems()[1]->setSelected(false);
        ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->pushButtonBarChart->show();
        ui->pushButtonTable->show();

    }
}
