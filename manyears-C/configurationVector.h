#ifndef CONFIGURATIONVECTOR_H
#define CONFIGURATIONVECTOR_H

#include <QVector>
#include <QString>
#include <QColor>
#include <QFile>

#include "property.h"

class ConfigurationVector
{

public:


    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    ConfigurationVector();
    ConfigurationVector(const ConfigurationVector& _configurationVector);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~ConfigurationVector();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    ConfigurationVector& operator=(const ConfigurationVector& _configurationVector);

    // +-------------------------------------------------------+
    // | Plus                                                  |
    // +-------------------------------------------------------+

    ConfigurationVector operator+(const ConfigurationVector& _configurationVector);


    /***********************************************************
    * Add element                                              *
    ***********************************************************/

    void addElement(QString _key, float _value);
    void addElement(QString _key, int _value);
    void addElement(QString _key, QColor _value);
    void addElement(Property* _property);
    void addElement(QString _key, QString _value);

    void setValue(QString _key, float _value);
    void setValue(QString _key, int _value);
    void setValue(QString _key, QColor _value);
    void setValue(QString _key, QString _value);


    /***********************************************************
    * Reset the vector                                         *
    ***********************************************************/

    void reset();

    /***********************************************************
    * Get element                                              *
    ***********************************************************/

    float getElementFloat(QString _key) const;
    int getElementInt(QString _key) const;
    QColor getElementColor(QString _key) const;

    QString getKey(int _index) const;
    QString getValue(QString _key) const;
    QString getValueNoType(QString _key) const;
    int getNumberElements() const;
    int getType(QString _key) const;

    /***********************************************************
    * Load from properties                                     *
    ***********************************************************/

    ConfigurationVector loadFromProperties(Property* _property);

    /***********************************************************
    * Generate output                                          *
    ***********************************************************/

    QString generateOutput();

    /***********************************************************
    * Read input                                               *
    ***********************************************************/

    void readInput(QString _inputString);

    /***********************************************************
    * Save to file                                             *
    ***********************************************************/

    void saveToFile(QString _filePath);

    /***********************************************************
    * Open from file                                           *
    ***********************************************************/

    void openFromFile(QString _filePath);

private:

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    QVector<QString> keys;
    QVector<QString> values;
    int numberElements;


};

#endif // CONFIGURATIONVECTOR_H
