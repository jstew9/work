/**
  @file servicetype.cpp
  @author Alex Clarke
  @version 0.1

  @section DESCRIPTION

  Class Storing information about a service type

  */

#include "servicetype.h"
/**
 * Default constructor
 */
ServiceType::ServiceType():_measureList(new QMap<QString,Measure>())
{
}
/**
 * Construct a ServiceType of a given id
 * @param id the chosen id
 */
ServiceType::ServiceType(const QString &id):_measureList(new QMap<QString,Measure>()),_id(QString(id))
{
}
/**
 * @brief ServiceType::ServiceType Construct a ServiceType of a given id and name
 * @param id the chosen id
 * @param name the chosen name
 */
ServiceType::ServiceType(const QString &id, const QString &name):_measureList(new QMap<QString,Measure>()),_id(QString(id)), _name(QString(name))
{
}
/**
 * Construct a ServiceType of a given id, and measurelist
 * @param id the chosen id
 * @param mlist the chosen measure list
 */
ServiceType::ServiceType(const QString &id, const QMap<QString, Measure> &mlist):
    _measureList(new QMap<QString,Measure>(mlist)), _id(QString(id))
{
}
/**
 * Copy constructor
 * @param param the ServiceType to copy from
 */
ServiceType::ServiceType(const ServiceType &param): _measureList(new QMap<QString,Measure>(*param._measureList))
{
    _id = param._id;
}
/**
 * Assignment operator
 * @param param the ServiceType to assign to
 * @return a copy of param
 */
ServiceType &ServiceType::operator =(const ServiceType &param)
{
    if(this != &param) {
        QMap<QString,Measure> *temp = _measureList;
        _measureList = new QMap<QString,Measure>(*param._measureList);
        _id = param._id;
        delete temp;
    }
    return *this;
}
/**
 * Deconstructor
 */
ServiceType::~ServiceType()
{
    delete _measureList;
}
/**
 * Set the id/name of the measure type
 * @param id the new id/name
 */
void ServiceType::setId(const QString &id)
{
    _id = id;
}
/**
 * Manually specifiy a measure list
 * @param list the new measure list
 */
void ServiceType::setMeasureList(const QMap<QString, Measure> &list)
{
    QMap<QString,Measure> *temp = _measureList;
    *_measureList = list;
    delete temp;
}
/**
 * Add a measure to the measure list
 * @param m the measure to add
 * @return true if the key doesn't exist, false otherwise
 */
bool ServiceType::addMeasure(Measure &m)
{
    if(!_measureList->contains(m.id())) {
        _measureList->insert(m.id(),m);
        return true;
    }
    return false;
}
/**
 * Add a measure of a given key to the measure list
 * @param key the desired key for the measure
 * @param m the measure
 * @return true if the key doesn't already exist, false otherwise
 */
bool ServiceType::addMeasure(const QString &key, const Measure &m)
{
    if(!_measureList->contains(key)){
        _measureList->insert(key,m);
        return true;
    }
    return false;
}
/**
 * Remove a measure of a given key
 * @param key the key of the measure to remove
 * @return true if measure exists, false otherwise
 */
bool ServiceType::removeMeasure(const QString &key)
{
    if(_measureList->remove(key) >= 1)return true;
    else return false;
}
/**
 * Get the id/name of the measure category
 * @return the id/name of the measure
 */
const QString &ServiceType::id()
{
    return _id;
}
/**
 * Return a const reference to the list of measures
 * @return the list of measures
 */
const QMap<QString, Measure> &ServiceType::measurelist()
{
    return *_measureList;
}
/**
 * Find a measure of a given key, throw an exception if nothing found
 * @param key the key to search for
 * @return the measure matching key
 */
Measure &ServiceType::findMeasure(const QString &key)
{
    if(_measureList->contains(key))
        return _measureList->operator [](key);
    else throw "Item not found";
}
