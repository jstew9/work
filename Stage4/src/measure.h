/**
  @file measure.h
  @author Alex Clarke
  @version 1.0

  @section DESCRIPTION

  Header of the class representing a service measure

  */

#ifndef MEASURE_H
#define MEASURE_H

#include <QString>
#include <QStringList>
#include <QMap>


#include "municipality.h"


class Measure
{
public:
    Measure();
    Measure(const QString& id);
    Measure(const QString &id, const QString &name);
    Measure(const QString& id, const QMap<QString,Municipality> &citylist);
    Measure(const Measure &param);
    Measure& operator= (const Measure &param);
    virtual ~Measure();
    void setId(const QString &id);
    void setName(const QString &name);
    void setMuniList(const QMap<QString, Municipality> &list);
    bool addMuni(Municipality &muni);
    bool addMuni(const QString& key,const Municipality &muni);
    bool removeMuni(const QString &key);
    const QString& id() const;
    const QString& name() const;
    const QMap<QString,Municipality>& munilist();
    const QStringList &yearRange();
    double measureAverage() const;
    std::pair<double,double> maxmin() const;
    QStringList stringMuniList() const;
    Municipality &findMuni(const QString &key) const;
    void calcYearRange() const;

private:
    void addToYearRange(const QString &year) const;
    QString _id;    //the id of the measure
    QString _name;  //the name of the measure
    QMap<QString,Municipality> *_muniList;  //List of municipalities
    QStringList *_yearList;    //Range of active years


};

#endif // MEASURE_H
