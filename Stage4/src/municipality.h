/**
  @file municiaplity.h
  @author Alex Clarke
  @version 0.1
  
  @section DESCRIPTION
  
  Header of the class representing a municipality for a given measure
  
  */

#ifndef MUNICIPALITY_H
#define MUNICIPALITY_H

#include "year.h"
#include <QMap>
#include <QString>

class Municipality
{
public:
    Municipality();
    Municipality(const QString &name,const int &pop,const int &numhouseholds,const int &geoarea, const int &popdensity);
    Municipality(const QString &name,const int &pop,const int &numhouseholds,const int &geoarea, const int &popdensity, const QMap<QString,Year> &yearlist);
    Municipality(const Municipality& param);
    Municipality& operator= (const Municipality &param);
    virtual ~Municipality();
    void setYearList(const QMap<QString, Year> &ylist);
    bool addYear(Year &year);
    bool addYear(const QString &key,const Year &year);
    bool removeYear(const QString &key);
    void setName(const QString &name);
    void setPopulation(const int &pop);
    void setNumHouseholds(const int &numhh);
    void setGeoArea(const int &geoarea);
    void setPopDensity(const int &popdens);
    const QMap<QString, Year> &yearlist() const;
    const QString &name() const;
    const int &population();
    const int &numHouseholds();
    const int &geoArea();
    const int &popDensity();
    const Year &getYear(const QString &key) const;
    QString toString();

private:
    QMap<QString,Year> *_years; //List of years with their year as key
    QString _name;
    int _population;
    int _numHouseholds;
    int _geoArea;
    int _popDensity;



};

#endif // MUNICIPALITY_H
