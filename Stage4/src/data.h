/**
  @file data.h
  @author Alex Clarke, Jamie Finnigan
  @version 0.1

  @section DESCRIPTION
  Header for the storage of service measure type data

  */

#ifndef DATA_H
#define DATA_H

#include <QMap>
#include "servicetype.h"

class Data
{
public:
    Data();
    Data(const QMap<QString,ServiceType> &servList);
    Data(const Data &param);
    Data& operator= (const Data &param);
    virtual ~Data();
    void setData(const QMap<QString,ServiceType> &data);
    bool addServType(ServiceType &s);
    bool addServType(const QString &key,const ServiceType &s);
    bool clearServType(const QString &key);
    const QMap<QString,ServiceType>& data();
    const QMap<QString,QString>& names();
    ServiceType &findService(const QString &key);
    const QString serviceName(const QString &key);
    bool addServName(const QString&, const QString&);

private:
    QMap<QString,ServiceType> *_serviceList;
    QMap<QString, QString> *_serviceNameList;
    void generateDefaultServices();
};

#endif // DATA_H
