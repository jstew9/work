/**
  @file filterdata.h
  @author Alex Clarke, Jamie Finnigan
  @version 1.0

  @class FilterData
  @brief Data and database handler



  */
#include "filterdata.h"
#include <QFile>
#include <QTextStream>

/**
 * @brief FilterData::FilterData Default constructor that takes data from the database and creates the rest of the data objects.
 * Uses default data.
 */
FilterData::FilterData(): _data(new Data()), _db(new QSqlDatabase())
{
    initDatabase();
    generateServices();
}

/**
 * @brief FilterData::FilterData Constructor utelizing an existing data structure.
 * No database connection needed.
 * @param data An existing data object
 */
FilterData::FilterData(const Data &data): _data(new Data(data)), _db(new QSqlDatabase())
{
}

/**
 * @brief FilterData::FilterData Copy Constructor
 * @param param The FilterData object to be constructed from.
 */
FilterData::FilterData(const FilterData &param): _data(new Data(*param._data)),_db(new QSqlDatabase(*param._db))
{
}

/**
 * @brief FilterData::operator = Assignment Operator
 * @param param existing FilterData to be copied from
 * @return A new, identical FilterData object.
 */
FilterData &FilterData::operator =(const FilterData &param)
{
    if(this != &param) {
        Data *temp = _data;
        QSqlDatabase *tempdb = _db;
        _db = new QSqlDatabase(*param._db);
        _data = new Data(*param._data);
        delete temp;
        delete tempdb;
    }
    return *this;
}
/**
 * @brief FilterData destructor.
 */

FilterData::~FilterData()
{
    delete _data;
    _db->close();
    delete _db;

}
/**
 * @brief FilterData::loadTemplate Load a template in from file
 * @param file the file to load
 * @return the filers in string format
 */
QVector<QString> &FilterData::loadTemplate(QFile &file)
{
    //if(!file.open(QIODevice::ReadOnly | QIODevice::Text))return;

    QVector<QString> results;

    QTextStream in(&file);

    while(!in.atEnd()) {
        results.append(in.readLine());
    }

    return results;

}
/**
 * @brief FilterData::saveTemplate save a template (see saveTable)
 * @param prefs the list of preferences
 * @param file the file to save to
 */
void FilterData::saveTemplate(const QVector<QString> &prefs, QFile &file)
{
    saveTable(prefs,file);
}

void FilterData::addYear(const QString &skey, const QString &mkey, const QString &mukey,Year &year)
{

    _data->findService(skey).findMeasure(mkey).findMuni(mukey).addYear(year);
}

/**
 * @brief Obtain a measure from the database.
 *
 * @param servType - a string containing the service type name
 * @param measure - a string with the measure name
 * @return The measure if it was found.
 */
Measure &FilterData::loadMeasure(const QString &servType, const QString &measure)
{
    return _data->findService(servType).findMeasure(measure);

}
/**
 * Save a table to disk
 * @param table the table to save
 * @param file the file to save to
 */
void FilterData::saveTable(const QVector<QString> &table,QFile &file)
{
    QTextStream out(&file);
    for(QVector<QString>::ConstIterator it = table.begin(); it != table.end(); ++it) {
        out << *it << "\n";
    }
}

/**
 * @brief FilterData::initDatabase  opens the database up for reading and writing
 * @return true if the database successfuly opens, otherwise false
 */
bool FilterData::initDatabase(){

    _db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    _db->setDatabaseName("OMBI.db");
    return _db->open();
}

/**
 * Serves two purposes, adds measures to serviceTypes, and emits vectors containing everything in the measures table
 * @brief FilterData::parseService given a service ID, returns all the measures belonging to that sID
 * @param sID ServiceID (the first 4 characters of the measures belonging to the service)
 */
void FilterData::parseService(QString sID){
   // qDebug()<<"In parseService() sID = "<<sID;

    //get all the records from the measures table that begin with the serviceID
    QString cmd = "SELECT * FROM measures WHERE MeasureID LIKE \'"+sID+"%\'";


    QSqlQuery query(*_db);
    //vectors for storing results from the query
    QVector<QString> measures;
    QVector<QString> title;
    QVector<QString> type;
    query.exec(cmd);
    //qDebug()<<"query.exec() = "<<query.exec(cmd);
    //while there are still records to parse
    while (query.next()) {
        //push the data on to the back of the vectors to be emited
        measures.push_back(query.value(0).toString());
        title.push_back(query.value(1).toString());
        type.push_back(query.value(2).toString());
        Measure *m = new Measure(measures.back(),title.back());
        _data->findService(sID).addMeasure(measures.back(), *m);
        emit measureReady(sID, measures.back(), title.back());
    }
 }

/**
 * Serves two purposes, fills in the data for the selected measure, and emits string representations of the entries fields
 * @brief FilterData::parseMeasure
 * @param mID measureID tag that needs parsing,
 * @param sID
 */
void FilterData::parseMeasures(QString mID, QString sID){
    QString cmd = "SELECT * FROM measuredata WHERE MeasureID = \""+mID+"\"";
    QSqlQuery query(*_db);
    query.exec(cmd);
    while(query.next()){
        _numberOfEntries++;
        //prepare the municipality and add it
        Municipality* m = new Municipality();
        m->setName(query.value(1).toString());
        _data->findService(sID).findMeasure(mID).addMuni(*m);
        //prepare the year and add it
        Year* y = new Year();
        if(query.value(3).toString() != "") {
            y->setYear(query.value(2).toString());
            y->setValue(query.value(3).toDouble());
            _data->findService(sID).findMeasure(mID).findMuni(m->name()).addYear(*y);

        }
    }
    emit resultReady(_data->findService(sID).findMeasure(mID));
}

/**
 * @brief FilterData::serviceList pass the serviceList through
 * @return the serviceList belonging to data
 */
const QMap<QString,ServiceType>& FilterData::serviceList(){
    return _data->data();
}


const QMap<QString,QString>& FilterData::serviceNameList(){
    return _data->names();
}
/**
 * @brief FilterData::serviceName pass the name of the service item belonging to key
 * @param key serviceType key
 * @return the name of the service paired with key
 */
const QString& FilterData::serviceName(const QString &key){
    return _data->serviceName(key);
}

/**
 * @brief FilterData::generateServices reads the services table from the database and adds them to the _data member
 */
void FilterData::generateServices(){

    QString cmd = "SELECT * FROM services";
    QSqlQuery query(*_db);
    query.exec(cmd);

    while(query.next()){
     //   qDebug()<<query.value(0).toString()<< "= id "<< query.value(1).toString() << " = name";
        ServiceType* s = new ServiceType(query.value(0).toString(),query.value(1).toString());
        _data->addServType(query.value(0).toString(),*s);
        _data->addServName(query.value(0).toString(),query.value(1).toString() );
    }
}
