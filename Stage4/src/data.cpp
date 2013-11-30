/**
  @file data.cpp
  @author Alex Clarke, Jamie Finnigan
  @version 1.0

  @class Data
  @brief Class for the storage of service measure type data

  Records both the sevice types and the name of each service type.
  */

#include "data.h"
#include <iostream>
#include <QDebug>
/**
 * Default constructor
 */
Data::Data(): _serviceList(new QMap<QString,ServiceType>()), _serviceNameList(new QMap<QString, QString>())
{
    //generateDefaultServices();
}
/**
 * Construct a data set of a given service list
 * @param servList the given service list
 */
Data::Data(const QMap<QString,ServiceType> &servList): _serviceList(new QMap<QString,ServiceType>(servList)), _serviceNameList(new QMap<QString, QString>())
{
    //generateDefaultServices();

}
/**
 * Copy Constructor
 * @param param
 */
Data::Data(const Data &param):_serviceList(new QMap<QString,ServiceType>(*param._serviceList)), _serviceNameList(new QMap<QString, QString>(*param._serviceNameList))
{
}
/**
 * Assignment Operator
 * @param param
 * @return
 */
Data &Data::operator =(const Data &param)
{
    if(this != &param) {
        QMap<QString, ServiceType> *temp = _serviceList;
        QMap<QString, QString> *tempsnl = _serviceNameList;
        _serviceList = new QMap<QString,ServiceType>(*param._serviceList);
        _serviceNameList = new QMap<QString,QString>(*param._serviceNameList);
        delete temp;
        delete tempsnl;

    }
    return *this;
}
/**
 * Deconstructor
 */
Data::~Data()
{
    delete _serviceList;
    delete _serviceNameList;
}
/**
 * Set the data list
 * @param data the new list of data
 */
void Data::setData(const QMap<QString, ServiceType> &data)
{
    QMap<QString, ServiceType> *temp = _serviceList;
    *_serviceList = data;
    delete temp;
}
/**
 * Add a ServiceType to the ServiceType list
 * @param s the ServiceType to add
 * @return true if the ServiceType doesn't exist, false otherwise
 */
bool Data::addServType(ServiceType &s)
{
    if(!_serviceList->contains(s.id())) {
        _serviceList->insert(s.id(),s);
        return true;
    }
    return false;

}
/**
 * Add a ServiceType of a given key and servicetype
 * @param key the key to associate with the ServiceType
 * @param s the ServiceType to add
 * @return true if the key doesn't exist, false otherwise
 */
bool Data::addServType(const QString &key, const ServiceType &s)
{
    if(!_serviceList->contains(key)) {
        _serviceList->insert(key,s);
        return true;
    }
    return false;
}
/**
 * Clear the data of a given ServiceType and replace with an empty ServiceType of the same name
 * @param key
 * @return true if successful, false otherwise
 */
bool Data::clearServType(const QString &key)
{
    ServiceType *temp = new ServiceType(key);
    if(_serviceList->remove(key) == 1) {
        addServType(*temp);
        return true;
    }
    return false;

}
/**
 * Get the list of ServiceTypes
 * @return the list of ServiceType
 */
const QMap<QString, ServiceType> &Data::data()
{
    return *_serviceList;
}
/**
 * Get the list of the names of the ServiceTypes
 * @return the list of the names of the ServiceType
 */
const QMap<QString, QString> &Data::names()
{
    return *_serviceNameList;
}
/**
 * Find a servicetype of a given key
 * @param key
 * @return the desired servicetype
 */
ServiceType &Data::findService(const QString &key)
{
    if(_serviceList->contains(key))
        return _serviceList->operator [](key);
    else throw "Item of key: " + key.toStdString() + " not found";
}

/**
 * @brief Get a service name given a key
 * @param key the key
 * @return service name matching the key
 */
const QString Data::serviceName(const QString &key){
    if(_serviceNameList->contains(key))
    return _serviceNameList->operator [](key);
}
/**
 * @brief Data::addServName - Add a new service type name to the data.
 * @param key - the service type's key
 * @param name - the name of the service type
 * @return true if the insertion worked, otherwise false
 */

bool Data::addServName(const QString &key, const QString &name)
{
    //qDebug()<< key<<"    "<<name;
    if(!_serviceNameList->contains(key)) {
        _serviceNameList->insert(key,name);
        return true;
    }
    return false;

}
