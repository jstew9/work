#include "graphtemplate.h"

GraphTemplate::GraphTemplate()
{
}

GraphTemplate::GraphTemplate(const QVector<QString> &prefs)
{
    parseInPrefs(prefs);
}
QString GraphTemplate::bgImage() const
{
    return _bgImage;
}

void GraphTemplate::setBgImage(const QString &bgImage)
{
    _bgImage = bgImage;
}
QString GraphTemplate::bgAxisImg() const
{
    return _bgAxisImg;
}

void GraphTemplate::setBgAxisImg(const QString &bgAxisImg)
{
    _bgAxisImg = bgAxisImg;
}
QColor GraphTemplate::bgColour() const
{
    return _bgColour;
}

void GraphTemplate::setBgColour(const QColor &bgColour)
{
    _bgColour = bgColour;
}
QColor GraphTemplate::bgAxisColour() const
{
    return _bgAxisColour;
}

void GraphTemplate::setBgAxisColour(const QColor &bgAxisColour)
{
    _bgAxisColour = bgAxisColour;
}
QColor GraphTemplate::bgLegendColour() const
{
    return _bgLegendColour;
}

void GraphTemplate::setBgLegendColour(const QColor &bgLegendColour)
{
    _bgLegendColour = bgLegendColour;
}
QColor GraphTemplate::graphColour() const
{
    return _graphColour;
}

void GraphTemplate::setGraphColour(const QColor &graphColour)
{
    _graphColour = graphColour;
}
int GraphTemplate::legendXPos() const
{
    return _legendXPos;
}

void GraphTemplate::setLegendXPos(int legendXPos)
{
    _legendXPos = legendXPos;
}
int GraphTemplate::legendYPos() const
{
    return _legendYPos;
}

void GraphTemplate::setLegendYPos(int legendYPos)
{
    _legendYPos = legendYPos;
}

QVector<QString> &GraphTemplate::getStringPrefs()
{
    QVector<QString> *results = new QVector<QString>;
    results->append(_bgImage);
    results->append(_bgAxisImg);
    results->append(parseOutColour(_bgColour));
    results->append(parseOutColour(_bgAxisColour));
    results->append(parseOutColour(_bgLegendColour));
    results->append(parseOutColour(_graphColour));
    results->append(QString::number(_legendXPos));
    results->append(QString::number(_legendYPos));

    return *results;
}

void GraphTemplate::parseInPrefs(const QVector<QString> &prefs)
{
    if(prefs.size() == 8) {
        _bgImage = prefs[0];
        _bgAxisImg = prefs[1];
        _bgColour = parseColour(prefs[2]);
        _bgAxisColour = parseColour(prefs[3]);
        _bgLegendColour = parseColour(prefs[4]);
        _graphColour = parseColour(prefs[5]);
        _legendXPos = prefs[6].toInt();
        _legendYPos = prefs[7].toInt();

    }


}

QString &GraphTemplate::parseOutColour(const QColor &colour)
{
    QString *results = new QString();
    *results = QString::number(colour.red()) + "," + QString::number(colour.green()) + "," + QString::number(colour.blue()) + "," + QString::number(colour.alpha());
    return *results;
}

QColor &GraphTemplate::parseColour(const QString &colour)
{
   QStringList temp = colour.split(",");
   QColor *result = new QColor(temp[0].toInt(),temp[1].toInt(),temp[2].toInt(),temp[3].toInt());
   return *result;
}








