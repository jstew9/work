/**
  @file year.cpp
  @author Alex Clarke
  @version 0.1

  @section DESCRIPTION

  Class allowing for the storage and manipulation of measure data petraining to a given city

  */

#include "year.h"
#include <float.h>

/**
 * Constructs an unnamed year with a deafault value
 */
Year::Year() {
    _name = "";
    _value = -DBL_MAX;
}
/**
 * More useful constructor, allows the setting of a name and value
 * @param year the year of the year
 * @param value the value of the measure
 */
Year::Year(const QString &year, const double &value) {
    _name = year;
    _value = value;
}
/**
 * Get the 'name' of the year
 * @return the 'name' of the year
 */
const QString& Year::name() const {
    return _name;
}
/**
 * Get the value of the measure
 * @return the value of the measure
 */
const double& Year::value() const {
    return _value;
}
/**
 * Set the value of the measure
 * @param the new value of the measure
 */
void Year::setValue(const double &value) {
    _value = value;
}
/**
 * Set the 'name'(year) of the year
 * @param year the new year
 */
void Year::setYear(QString year) {
    _name = year;
}
/**
 * Return a formatted QString about the year
 * @return a formatted QString about the year
 */
QString Year::toString() const
{
    QString result = _name + ":" + QString::number(_value);
    return result;
}
