/**
  @file yeah.h
  @author Alex Clarke
  @version 1.0

  @section DESCRIPTION

  Header of the class representing a year's worth of data about a given measure for a given muinicipality
  */

#ifndef YEAR_H
#define YEAR_H
#include <QString>
class Year
{
public:
    Year();
    Year(const QString &Year, const double &value);
    const QString& name() const;
    const double& value() const;
    void setYear(QString Year);
    void setValue(const double& value);
    QString toString() const;

private:
    QString _name;  //The 'name' of the year, namely what year it is
    double _value;  //The value of the measure
};

#endif // YEAR_H
