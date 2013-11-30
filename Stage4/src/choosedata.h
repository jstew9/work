/**
 * @file  choosedata.h
 * @author  Melanie Imough, Alex Clarke, Jamie Finnigan
 * @version 1.0
 *
 * @section DESCRIPTION
 *
 * This is the header of the class that displays the second screen which comes after the "welcome" screen.
 * It enables the user to start filtering data to get a figures or open a preexisting filter.
 */

#ifndef CHOOSEDATA_H
#define CHOOSEDATA_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "displaywindow.h"
#include "filterdata.h"


namespace Ui {
class ChooseData;
}

class ChooseData : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChooseData(QWidget *parent = 0);
    ~ChooseData();

signals:
    void showOtherWindow();
    void parseService(QString);
    void parseMeasure(QString, QString);
    void serviceReady(QString);
    void measureReady(QString, QString);
public slots:
    void addItem(QString sID, QString measure, QString title);
    void updateMeasure(Measure &m);
protected:
    void closeEvent(QCloseEvent *e);
private slots:
    void on_pushButtonTable_clicked();
    void on_pushButtonBarChart_clicked();
    void on_pushButtonLineChart_clicked();
    void on_pushButtonChooseFilter_clicked();
    void getMeasuresForService(QTreeWidgetItem*);
    void parseMeasures(QTreeWidgetItem*);
    void on_pushButtonActiveGraphs_clicked();
    void on_treeWidget_itemSelectionChanged();
    void on_chkbox_toggled(bool checked);
private:
   void populateTreeWithServices();
   void loadState();
   void saveState();
   Ui::ChooseData *ui;
   DisplayWindow *displayDialog;
   FilterData *_fdata;
   QString _sID;

   Measure* _measure;
   Measure* _measure2; //measure for comparison


};

#endif // CHOOSEDATA_H
