/**
  @file municipality.cpp
  @author Alex Clarke
  @version 0.1
  
  @section DESCRIPTION
  
  Class storing information about a city for a given measure
  */

#include "municipality.h"
/**
 * Default constructor; creates an unnamed Municipality, with zeroed attribute values and initalizes the year list
 */
Municipality::Municipality():_years(new QMap<QString,Year>())
{
    _population = _numHouseholds = _geoArea = _popDensity = 0;
}
/**
 * Construct a Municipality of given parameters, with an empty year list
 * @param name the name of the Municipality
 * @param pop the population of the Municipality
 * @param numhouseholds the number of households in the Municipality
 * @param geoarea the geographical area of the Municipality
 * @param popdensity the population density of the Municipality
 */
Municipality::Municipality(const QString &name, const int &pop, const int &numhouseholds, const int &geoarea, const int &popdensity):
    _years(new QMap<QString,Year>()), _name(QString(name))
{
    _population = pop;
    _numHouseholds = numhouseholds;
    _geoArea = geoarea;
    _popDensity = popdensity;
}
/**
 * Construct a Municipality of given parameters, with a copy of a given year list
 * @param name the name of the Municipality
 * @param pop the population of the Municipality
 * @param numhouseholds the number of households in the Municipality
 * @param geoarea the geographical area of the Municipality
 * @param popdensity the population density of the Municipality
 * @param yearlist the given year list to be copied
 */
Municipality::Municipality(const QString &name, const int &pop, const int &numhouseholds, const int &geoarea, const int &popdensity, const QMap<QString,Year> &yearlist):
    _name(QString(name)), _years(new QMap<QString,Year>(yearlist))
{
    _population = pop;
    _numHouseholds = numhouseholds;
    _geoArea = geoarea;
    _popDensity = popdensity;
}
/**
 * Copy constructor performs a deep copy of a given Municipality
 * @param param the Municipality to be copied
 */
Municipality::Municipality(const Municipality &param): _years(new QMap<QString,Year>(*param._years))
{
    _name = param._name;
    _population = param._population;
    _numHouseholds = param._numHouseholds;
    _geoArea = param._geoArea;
    _popDensity = param._popDensity;

}
/**
 * Assignment operator
 * @param param the Municipality to assign to
 * @return the newly assigned Municipality
 */
Municipality &Municipality::operator =(const Municipality &param)
{
    if(this != &param) {
        QMap<QString,Year> *temp = _years;
        _name = param._name;
        _population = param._population;
        _numHouseholds = param._numHouseholds;
        _geoArea = param._geoArea;
        _popDensity = param._popDensity;
        _years =  new QMap<QString,Year>(*param._years);

        delete temp;

    }
    return *this;
}
/**
 * Destructor
 */
Municipality::~Municipality()
{
    delete _years;
}
/**
 * Set a new year list
 * @param list the new year list
 */
void Municipality::setYearList(const QMap<QString,Year> &ylist)
{
    QMap<QString,Year> *temp = _years;
    *_years = ylist;
     delete temp;
}
/**
 * Insert a year into the year list
 * @param the year to insert
 * @return true if a year of that key wasn't already in the list
 */
bool Municipality::addYear(Year &year)
{
    if(!_years->contains(year.name())){
        _years->insert(year.name(),year);
        return true;
    }
    return false;
}
/**
 * Insert a year into the year list with a given key
 * @param key the key
 * @param year the year to insert
 * @return true if the key isn't already in the list, false otherwise
 */
bool Municipality::addYear(const QString &key, const Year &year)
{
    if(!_years->contains(key)){
        _years->insert(key,year);
        return true;
    }
    return false;
}
/**
 * Remove a year of a given key
 * @param key the key of the item to remove
 * @return true if successful, false otherwise
 */
bool Municipality::removeYear(const QString &key)
{
    if(_years->remove(key) >= 1) return true;
    else return false;

}
/**
 * Set the name of the Municipality
 * @param name the name of the Municipality
 */
void Municipality::setName(const QString &name)
{
    _name = name;
}
/**
 * Set the population of the Municipality
 * @param pop the population of the Municipality
 */
void Municipality::setPopulation(const int &pop)
{
    _population = pop;
}
/**
 * Set the number of households of the Municipality
 * @param numhh the number of households in the Municipality
 */
void Municipality::setNumHouseholds(const int &numhh)
{
    _numHouseholds = numhh;
}
/**
 * Set the geographic area of the Municipality
 * @param geoarea the geographic area of the Municipality
 */
void Municipality::setGeoArea(const int &geoarea)
{
    _geoArea = geoarea;
}
/**
 * Set the population density of the Municipality
 * @param popdens the population density of the Municipality
 */
void Municipality::setPopDensity(const int &popdens)
{
    _popDensity = popdens;
}
/**
 * Get a constant reference to the list of years
 * @return the list of years
 */
const QMap<QString,Year> &Municipality::yearlist() const
{
    return *_years;
}
/**
 * Get the name of the Municipality
 * @return the name of the Municipality
 */
const QString &Municipality::name() const
{
    return _name;
}
/**
 * Get the population of the Municipality
 * @return the population of the Municipality
 */
const int &Municipality::population()
{
    return _population;
}
/**
 * Get the number of households in the Municipality
 * @return the number of households in the Municipality
 */
const int &Municipality::numHouseholds()
{
    return _numHouseholds;
}
/**
 * Get the geographic area of the Municipality
 * @return the geographic area of the Municipality
 */
const int &Municipality::geoArea()
{
    return _geoArea;
}
/**
 * Get the population density of the Municipality
 * @return the population density of the Municipality
 */
const int &Municipality::popDensity()
{
    return _popDensity;
}
/**
 * Get a year of a given key, throwing an execption if it is not found
 * @param key the key to search for
 * @return the year matching the key
 */
const Year &Municipality::getYear(const QString &key) const
{
    if(_years->contains(key))
        return _years->operator [](key);
    else throw "Item of key: " + key.toStdString() + " not found";
}
/**
 * Get a formatted QString containing data about the city
 * @return a formatted QString with data about the city
 */
QString Municipality::toString()
{
    QString result = _name + ",Population: " + QString::number(_population) + " Years:";
    for(QMap<QString,Year>::Iterator it = _years->begin(); it != _years->end(); ++it)
        result += " " + it->toString();
    return result;

}
