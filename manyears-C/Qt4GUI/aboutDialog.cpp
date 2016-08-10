#include "aboutDialog.h"

AboutDialog::AboutDialog()
{

    int xImage = 20;
    int yImage = 20;
    int sideMargin = 10;
    int widthImage = 75;
    int heightImage = 75;

    int titleHeight = 40;
    int versionHeight = 20;
    int authorsHeight = 20;
    int websiteHeight = 20;
    int copyrightHeight = 20;
    int readmeHeight = 100;
    int buttonHeight = 25;

    int titleSpace = 5;
    int versionSpace = 2;
    int authorsSpace = 2;
    int websiteSpace = 2;
    int copyrightSpace = 10;
    int readmeSpace = 5;

    int widthButton = 100;

    int xText = widthImage + 20 + sideMargin;
    int yText = 10;
    int widthText = 500;

    // Read text
    QString resCopyright;
    QString resReadme;
    QString pathProject;
    QFile* pathCopyright;
    QFile* pathReadme;
    QString strCopyright;
    QString strReadme;

    QTextStream* stream;

    pathProject = QString(DEF_PROJECT_SOURCE_DIR);

    resCopyright = QString(DEF_RESOURCE_FILE_LICENSE);
    resCopyright = resCopyright.right(resCopyright.length() - pathProject.length());
    if (resCopyright.left(1) == "/")
    {
        resCopyright = ":" + resCopyright;
    }
    else
    {
        resCopyright = ":/" + resCopyright;
    }

    pathCopyright = new QFile(resCopyright);
    if (pathCopyright->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        stream = new QTextStream(pathCopyright);
        strCopyright = stream->readAll();
        pathCopyright->close();
    }
    else
    {
        strCopyright = "";
    }

    resReadme = QString(DEF_PACKAGE_DESCRIPTION_FILE);
    resReadme = resReadme.right(resReadme.length() - pathProject.length());
    if (resReadme.left(1) == "/")
    {
        resReadme = ":" + resReadme;
    }
    else
    {
        resReadme = ":/" + resReadme;
    }

    pathReadme = new QFile(resReadme);
    if (pathReadme->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        stream = new QTextStream(pathReadme);
        strReadme = stream->readAll();
        pathReadme->close();
    }
    else
    {
        strReadme = "";
    }

    // Create elements
    this->title = new QLabel(this);
    this->version = new QLabel(this);
    this->authors = new QLabel(this);
    this->website = new QLabel(this);
    this->copyright = new QLabel(this);
    this->readme = new QTextEdit(this);
    this->buttons = new QPushButton(this);
    this->logo = new QLabel(this);

    // Set information
    this->title->setText(QString("<FONT face = 'Tahoma' size = '6'><B>") + QString(DEF_PACKAGE_NAME) + QString("</B></FONT>"));
    this->version->setText(QString("<FONT face = 'Tahoma' size = '3'><B>Version: </B>") + QString(DEF_PACKAGE_VERSION) + QString("</FONT>"));
    this->authors->setText(QString("<FONT face = 'Tahoma' size = '3'><B>Authors: </B>") + QString(DEF_PACKAGE_VENDOR) + QString("</FONT>"));
    this->website->setText(QString("<FONT face = 'Tahoma' size = '3'><B>Website: </B><A href = '") + QString(DEF_NSIS_URL_INFO_ABOUT) + QString("'>") + QString(DEF_NSIS_URL_INFO_ABOUT) + QString("</A></FONT>"));
    this->copyright->setText(QString("<FONT face = 'Tahoma' size = '3'><B>Copyright: </B>") + strCopyright + QString("</FONT>"));
    this->readme->setText(strReadme);

    QIcon tmpImage(":/Images/logoonly.png");
    this->logo->setPixmap(tmpImage.pixmap(widthImage, heightImage));

    // Set position
    this->title->setGeometry(xText, yText, widthText, titleHeight);
    this->version->setGeometry(xText, yText + titleHeight + titleSpace, widthText, versionHeight);
    this->authors->setGeometry(xText, yText + titleHeight + titleSpace + versionHeight + versionSpace, widthText, authorsHeight);
    this->website->setGeometry(xText, yText + titleHeight + titleSpace + versionHeight + versionSpace + authorsHeight + authorsSpace, widthText, websiteHeight);
    this->copyright->setGeometry(xText, yText + titleHeight + titleSpace + versionHeight + versionSpace + authorsHeight + authorsSpace + websiteHeight + websiteSpace, widthText, copyrightHeight);
    this->readme->setGeometry(xText, yText + titleHeight + titleSpace + versionHeight + versionSpace + authorsHeight + authorsSpace + websiteHeight + websiteSpace + copyrightHeight + copyrightSpace, widthText, readmeHeight);
    this->buttons->setGeometry(xText + widthText - widthButton, yText + titleHeight + titleSpace + versionHeight + versionSpace + authorsHeight + authorsSpace + websiteHeight + websiteSpace + copyrightHeight + copyrightSpace + readmeHeight + readmeSpace, widthButton, buttonHeight);
    this->logo->setGeometry(xImage, yImage, widthImage, heightImage);

    // Others

    this->readme->setReadOnly(true);
    this->buttons->setText("Close");

    // Window
    this->setFixedSize(xText + widthText + sideMargin,
                      yText + titleHeight + titleSpace + versionHeight + versionSpace + authorsHeight + authorsSpace + websiteHeight + websiteSpace + copyrightHeight + copyrightSpace + readmeHeight + readmeSpace + buttonHeight + yText);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);



    // Event
    QObject::connect(this->website, SIGNAL(linkActivated(QString)), this, SLOT(launchURL(QString)));
    QObject::connect(this->buttons, SIGNAL(clicked()), this, SLOT(closeDialog()));

}

AboutDialog::~AboutDialog()
{

}

void AboutDialog::launchURL(const QString _url)
{

    QDesktopServices::openUrl(QUrl(_url));

}

void AboutDialog::closeDialog()
{
    this->close();
}
