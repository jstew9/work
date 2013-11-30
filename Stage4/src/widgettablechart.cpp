/**
  @file widgettablechart.cpp
  @author Alex Clarke, Melanie Imough
  @version 1.0

  @class WidgetTableChart
  @brief Class for the display of data in a table
  */

#include "widgettablechart.h"
#include "ui_widgettablechart.h"
#include <float.h>
#include <QStandardItemModel>
#include <iostream>
#include <QException>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
/**
 * Default constructor
 * @param parent
 */
WidgetTableChart::WidgetTableChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTableChart)
{
    ui->setupUi(this);

    this->setStyleSheet(
                "QPushButton#pushButtonSaveTableData {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "border-radius: 5px;"
                "font: bold 22px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonSaveTableData:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonSaveTableData:hover {"
                "background-color: rgb(130,130,180);}"

                "QPushButton#pushButtonLoadTable {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "border-radius: 5px;"
                "font: bold 20px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonLoadTable:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonLoadTable:hover {"
                "background-color: rgb(130,130,180);}"
                );

    ui->listBarChartMunicipality->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listBarChartYears->setSelectionMode(QAbstractItemView::MultiSelection);

    _dirname = "";
}
/**
 * Generate a table chart with a given parent and measure
 * @param parent the parent of the widget
 * @param meas the active measure
 */
WidgetTableChart::WidgetTableChart(QWidget *parent, Measure &meas):QWidget(parent),ui(new Ui::WidgetTableChart),_meas(new Measure(meas))
{
    ui->setupUi(this);

    this->setStyleSheet(
                "QPushButton#pushButtonSaveTableData {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "border-radius: 5px;"
                "font: bold 22px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonSaveTableData:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonSaveTableData:hover {"
                "background-color: rgb(130,130,180);}"

                "QPushButton#pushButtonLoadTable {"
                "background-color: rgb(125,125,175);"
                "border-style: outset;"
                "border-width: 2px;"
                "border-color: rgb(50,50,75);"
                "border-radius: 5px;"
                "font: bold 22px;"
                "min-width: 7em;}"

                "QPushButton#pushButtonLoadTable:hover:pressed {"
                "background-color: rgb(100,100,145);"
                "border-style:inset;}"

                "QPushButton#pushButtonLoadTable:hover {"
                "background-color: rgb(130,130,180);}"
                );
    ui->listBarChartMunicipality->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listBarChartYears->setSelectionMode(QAbstractItemView::MultiSelection);
    //List Stuff
    QStringList mlist = meas.stringMuniList();
    ui->listBarChartMunicipality->addItems(mlist);
    //ui->listBarChartYears->addItem("2008"); ui->listBarChartYears->addItem("2009"); ui->listBarChartYears->addItem("2010"); ui->listBarChartYears->addItem("2011"); ui->listBarChartYears->addItem("2012");
    ui->listBarChartYears->addItems(_meas->yearRange());
    ui->measurename->setText(_meas->name());
    drawTable();

    _dirname = "";
}
/**
 * Deconstructor
 */
WidgetTableChart::~WidgetTableChart()
{
    delete ui;
    delete _meas;
    delete _model;
}
/**
 * @brief WidgetTableChart::drawYear Draw a given year's table value(s)
 * @param sel the index of selected year
 * @param column the column to write to
 */
void WidgetTableChart::drawYear(const int &sel,const int &column) {

    QList<QListWidgetItem*> tempList = ui->listBarChartMunicipality->selectedItems();
    int row = 0;
    for(QList<QListWidgetItem*>::Iterator it = tempList.begin(); it != tempList.end(); ++it) {
        QListWidgetItem *tempItem = *it;
        QString strData;
        try {
            strData = QString::number(_meas->findMuni(tempItem->text()).getYear(_meas->yearRange().at(sel)).value());
        } catch (std::string s) {
            strData = "NA";
        }
        _model->setItem(row, column, new QStandardItem(strData));
        _model->setHorizontalHeaderItem(column, new QStandardItem(_meas->yearRange().at(sel)));
        ++row;
    }

}

/**
 * Draw the table using the chosen data
 */
void WidgetTableChart::drawTable()
{
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVector<QString> selectedMuniList;
    QVector<double> valueData2012, valueData2011, valueData2010, valueData2009, valueData2008;
    QVector<QString> yearLabels;
    int numColumn = ui->listBarChartYears->selectedItems().count();
   // std::cout << "num col: " << numColumn << std::endl;
    QList<QListWidgetItem*> tempMuniList = ui->listBarChartMunicipality->selectedItems();

    for(QList<QListWidgetItem*>::Iterator it = tempMuniList.begin(); it != tempMuniList.end(); ++it) {
        QListWidgetItem *tempItem = *it;
        selectedMuniList.append(_meas->findMuni(tempItem->text()).name());
    }
    int numRow = selectedMuniList.count();
    _model = new QStandardItemModel(numRow,numColumn,this);
    //std::cout << "num row: " << numRow << std::endl;
    int currentCol = 0;
    for(int j = 0; j < _meas->yearRange().size(); ++j)
        if(ui->listBarChartYears->item(j)->isSelected()) drawYear(j,currentCol++);


    //set the row titles
    for(int j = 0; j < numRow; j++){
        _model->setVerticalHeaderItem(j, new QStandardItem(selectedMuniList.at(j)));
        std::cout << selectedMuniList[j].toStdString() << std::endl;
    }

    ui->tableView->setModel(_model);

}
/**
 * Redraw the table
 */
void WidgetTableChart::on_pushButtonLoadTable_clicked()
{
    drawTable();
}
/**
 * Format the table data to a CSV format and save the file
 * @brief WidgetTableChart::on_pushButtonSaveTableData_clicked
 * @author Jen Stewart
 */

void WidgetTableChart::on_pushButtonSaveTableData_clicked()
{
    //make sure that the table is not empty
    if (ui->listBarChartMunicipality->selectedItems().isEmpty()
            || ui->listBarChartYears->selectedItems().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("Please select data before saving.");
        msgBox.exec();
        return;
    }

    //first, obtain the names of the municipalities that are selected.

    QVector<QString> selectedMuniList;
    QList<QListWidgetItem*> tempMuniList = ui->listBarChartMunicipality->selectedItems();

    for(QList<QListWidgetItem*>::Iterator it = tempMuniList.begin(); it != tempMuniList.end(); ++it) {
        QListWidgetItem *tempItem = *it;
        selectedMuniList.append(_meas->findMuni(tempItem->text()).name());
    }

    //next, obtain the years that are selected
    QVector<QString> selectedYearList;
    QList<QListWidgetItem*> tempYearList = ui->listBarChartYears->selectedItems();

    int i = 0;
    for(QList<QListWidgetItem*>::Iterator it = tempYearList.begin(); it != tempYearList.end(); ++it) {
        QListWidgetItem *tempItem = *it;
        if (i == 0)
            selectedYearList.append(tempItem->text());
        else
        {
            int j;
            for (j = 0; j < i; ++j){
                if((tempItem->text().compare(selectedYearList.at(j))) < 0)
                {
                     selectedYearList.insert(j, tempItem->text());
                     j=i+1;
                }
            }
            if (j == i)
            {
                selectedYearList.append(tempItem->text());
            }
        }
        ++i;
    }

    //Add all the text necessary to a string in csv format
    QString csvformat;

    //Add the column headers
    csvformat.append(",");
    for (QVector<QString>::Iterator it = selectedYearList.begin(); it != selectedYearList.end(); ++it )
    {
        csvformat.append(*it);
        csvformat.append(",");
    }
    csvformat.append("\n");

    //add each row starting with the municipality names
    for (QVector<QString>::Iterator it = selectedMuniList.begin(); it != selectedMuniList.end(); ++it)
    {
        QString muniName = *it;
        csvformat.append(muniName);
        csvformat.append(",");
        for (QVector<QString>::Iterator jt = selectedYearList.begin(); jt != selectedYearList.end(); ++jt )
        {
            try {
                csvformat.append(_meas->findMuni(muniName).getYear(*jt).toString().remove(0,5));
            }
            catch (std::string s)
            {
                csvformat.append("NA");
            }

            csvformat.append(",");
        }
        csvformat.append("\n");
    }

    //open a qfile dialog to let the user enter a file name
    QString fileName = QFileDialog::getSaveFileName(this,"Save File", _dirname,"*.csv");
    //open the file to write to
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    //using a text stream, write the string out to the file.
    QTextStream out(&file);
    out << csvformat;
    out.flush();

    //save the directory the file was saved in so that the file dialog will open there next time
    fileName.chop(fileName.length()-fileName.lastIndexOf("/"));
    _dirname = fileName;

}
