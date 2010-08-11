#include "configurationLoadSave.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

ConfigurationLoadSave::ConfigurationLoadSave()
{

    this->configVector = new ConfigurationVector();

}

/***********************************************************
* Destructor                                               *
***********************************************************/

ConfigurationLoadSave::~ConfigurationLoadSave()
{

    delete this->configVector;

}

/***********************************************************
* Save to file                                             *
***********************************************************/

void ConfigurationLoadSave::saveToFile(QString _filePath)
{

    QString outputStr;

    QFile outputFile(_filePath);

    outputStr = this->configVector->generateOutput();

    outputFile.open(QFile::WriteOnly | QFile::Text);

    QTextStream outputStream(&outputFile);

    outputStream << outputStr;

    outputFile.close();

}

/***********************************************************
* Open from file                                           *
***********************************************************/

void ConfigurationLoadSave::openFromFile(QString _filePath)
{

    QString inputStr;
    QString currentLine;

    QFile inputFile(_filePath);

    inputFile.open(QFile::ReadOnly | QFile::Text);

    QTextStream inputStream(&inputFile);

    inputStr = "";

    while(!inputStream.atEnd())
    {
        currentLine = inputStream.readLine();
        inputStr.append(currentLine);
    }   

    this->configVector->readInput(inputStr);

    inputFile.close();

}

/***********************************************************
* Accessor                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Configuration vector                                  |
// +-------------------------------------------------------+

ConfigurationVector* ConfigurationLoadSave::getConfigurationVector()
{
    return (this->configVector);
}

