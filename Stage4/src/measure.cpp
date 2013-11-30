/**
  @file measure.cpp
  @author Alex Clarke
  @version 0.1

  @section DESCRIPTION

  Class storing information about a service measure

  */

#include "measure.h"
#include "float.h"
#include <iostream>
/**
 * Default constructor, initalize an empty Municipality list
 */
Measure::Measure(): _muniList(new QMap<QString,Municipality>()), _yearList(new QStringList())
{
}
/**
 * Construct a service measure of given id, with an initalized empty Municipality list
 * @param id the given id
 */
Measure::Measure(const QString &id): _muniList(new QMap<QString,Municipality>()), _id(QString(id)),_yearList(new QStringList())
{
}
/**
 * Construct a service measure of given id and name, with an initalized empty Municipality list
 * @param id the given id
 * @param name the given name
 */
Measure::Measure(const QString &id, const QString &name): _muniList(new QMap<QString,Municipality>()), _id(QString(id)),_name(QString(name)),_yearList(new QStringList())
{
}
/**
 * Construct a service measure of a given id/name and copy a given Municipality list into citylist
 * @param id the given id/name
 * @param citylist the list of cities to copy
 */
Measure::Measure(const QString &id, const QMap<QString, Municipality> &munilist):
    _muniList(new QMap<QString,Municipality>(munilist)), _id(QString(id)),_yearList(new QStringList())
{
}
/**
 * Copy constructor, perform a deep copy of another service measure
 * @param param the service measure to copy
 */
Measure::Measure(const Measure &param): _muniList(new QMap<QString,Municipality>(*param._muniList)),_yearList(new QStringList(*param._yearList))
{
    _id = param._id;
    _name = param._name;
}
/**
 * Assignment operator
 * @param param the Measure to assign to
 * @return
 */
Measure &Measure::operator =(const Measure &param)
{
    if(this != &param) {
        QMap<QString,Municipality> *temp = _muniList;
        QStringList *tyearlist = _yearList;
        _muniList = new QMap<QString,Municipality>(*param._muniList);
        _yearList = new QStringList(*param._yearList);
        _name = param._name;
        _id = param._id;
        delete temp;
        delete tyearlist;
    }
    return *this;
}
/**
 * Destructor
 */
Measure::~Measure()
{
    delete _muniList;
    delete _yearList;
}
/**
 * Set the id/name of the measure
 * @param id the id/name to set
 */
void Measure::setId(const QString &id)
{
    _id = id;
}
/**
 * set the name of the measure
 * @param name the desired name
 */
void Measure::setName(const QString &name)
{
    _name = name;
}
/**
 * Set the list of Municipalities
 * @param list
 */
void Measure::setMuniList(const QMap<QString,Municipality> &list)
{
    delete _muniList;
    *_muniList = list;
}

/**
 * Add a Municipality to the measure
 * @param muni the Municipality to add
 * @return  true if the key doesn't exist, false otherwise
 */
bool Measure::addMuni(Municipality &muni)
{
    if(!_muniList->contains(muni.name())) {
        _muniList->insert(muni.name(),muni);
        return true;
    }
    return false;

}
/**
 * Add a Municipality with a specified key
 * @param key the desired key
 * @param muni the Municipality to add
 * @return true if the key doesn't exist, false otherwise
 */
bool Measure::addMuni(const QString &key, const Municipality &muni)
{
    if(!_muniList->contains(key)) {
        _muniList->insert(key,muni);
        return true;
    }
    return false;
}
/**
 * Remove a Municipality of a given key
 * @param key the key of the Municipality to remove
 * @return true if the key was found, false otherwise
 */
bool Measure::removeMuni(const QString &key)
{
    if(_muniList->remove(key) >= 1) return true;
    else return false;
}
/**
 * Get the id/name of the Measure
 * @return the id/name of the Measure
 */
const QString &Measure::id() const
{
    return _id;
}
/**
 * Get the name of the measure
 * @return the name of the measure
 */
const QString &Measure::name() const
{
    return _name;
}
/**
 * Get a constant reference to the list of Municipalities
 * @return the list of Municipalities
 */
const QMap<QString, Municipality> &Measure::munilist()
{
    return *_muniList;
}
/**
 * @brief Measure::yearRange get the range of years with data in this measure
 * @return A list of active years
 */
const QStringList &Measure::yearRange()
{
    return *_yearList;
}
/**
 * Calculate the average value for all Municipalities and Years
 * @return the calculated average
 */
double Measure::measureAverage() const
{
    double tempMuniAvg = 0.0;
    int muniCount = 0;
    int count = 0;
    for(QMap<QString,Municipality>::const_iterator it = _muniList->begin();it != _muniList->end(); ++it) {
        double tempAvg = 0.0;
        if(it->yearlist().size() != 0) {
            count = 0;
            for(QMap<QString,Year>::const_iterator itt = it->yearlist().begin(); itt != it->yearlist().end(); ++itt) {
                      tempAvg += itt->value();
                   ++count;
            }
            tempAvg /= count;
            tempMuniAvg += tempAvg;
            ++muniCount;
        }
    }
    tempMuniAvg /= muniCount;
    return tempMuniAvg;
}
/**
 * find the maximum and minimum values of any year in the muni list
 * @return the min,max in the muni list
 */
std::pair<double, double> Measure::maxmin() const
{
    std::pair<double,double> result;
    result.first = DBL_MAX;
    result.second = -DBL_MAX;
    for(QMap<QString,Municipality>::const_iterator it = _muniList->begin(); it != _muniList->end(); ++it) {
        for(QMap<QString,Year>::const_iterator itt = it->yearlist().begin(); itt != it->yearlist().end(); ++itt) {
            if(itt->value() < result.first) result.first = itt->value();
            if(itt->value() > result.second) result.second = itt->value();
        }
    }

    return result;

}
/**
 * Get a QString list of Municipality names
 * @return the List of Municipality names
 */
QStringList Measure::stringMuniList() const
{
    QStringList result = _muniList->keys();
    return result;
}
/**
 * Find a Municipality of a given key, throw an error if nothing found
 * @param key the key to search for
 * @return the Municipality of a given key
 */
Municipality &Measure::findMuni(const QString &key) const
{
    if(_muniList->contains(key))
        return _muniList->operator [](key);
    else throw "Item of key: " + key.toStdString() + " not found";
}
/**
 * @brief Measure::calcYearRange find all data containg years for the measure
 */
void Measure::calcYearRange() const
{
    for(QMap<QString,Municipality>::const_iterator it = _muniList->begin(); it != _muniList->end(); ++it) {
        for(QMap<QString,Year>::const_iterator itt = it->yearlist().begin(); itt != it->yearlist().end(); ++itt){
            addToYearRange(itt->name());
        }
    }
}
/**
 * @brief Measure::addToYearRange add a year to the range of years, ignoring duplicates
 * @param year the year to add
 */
void Measure::addToYearRange(const QString &year) const
{
    if(!_yearList->contains(year)) _yearList->append(year);
    else return;
    _yearList->sort();
}

