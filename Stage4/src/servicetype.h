/**
  @file servicetype.h
  @author Alex Clarke
  @version 1.0

  @section DESCRIPTION

  Header for class representing a service measure type

  */

#ifndef SERVICETYPE_H
#define SERVICETYPE_H

#include <QString>
#include <QMap>
#include "measure.h"

class ServiceType
{
public:
    ServiceType();
    ServiceType(const QString &id);
    ServiceType(const QString &id, const QString &name);
    ServiceType(const QString &id,const QMap<QString,Measure> &mlist);
    ServiceType(const ServiceType &param);
    ServiceType& operator= (const ServiceType &param);
    virtual ~ServiceType();
    void setId(const QString &id);
    void setMeasureList(const QMap<QString,Measure> &list);
    bool addMeasure(Measure &m);
    bool addMeasure(const QString &key,const Measure &m);
    bool removeMeasure(const QString &key);
    const QString& id();
    const QMap<QString,Measure> &measurelist();
    Measure &findMeasure(const QString &key);

private:
    QMap<QString,Measure> * _measureList;   //List of measures
    QString _id;    //Id of service category
    QString _name;  //name of service category

};

#endif // SERVICETYPE_H
