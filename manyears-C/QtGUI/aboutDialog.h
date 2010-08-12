#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QTextEdit>
#include <QIcon>
#include <QDesktopServices>
#include <QUrl>

#ifndef DEF_PACKAGE_NAME
#define DEF_PACKAGE_NAME ""
#endif

#ifndef DEF_PACKAGE_VERSION
#define DEF_PACKAGE_VERSION ""
#endif

#ifndef DEF_PACKAGE_VENDOR
#define DEF_PACKAGE_VENDOR ""
#endif

#ifndef DEF_NSIS_URL_INFO_ABOUT
#define DEF_NSIS_URL_INFO_ABOUT ""
#endif

#ifndef DEF_RESOURCE_FILE_LICENSE
#define DEF_RESOURCE_FILE_LICENSE ""
#endif

#ifndef DEF_PACKAGE_DESCRIPTION_FILE
#define DEF_PACKAGE_DESCRIPTION_FILE ""
#endif

class AboutDialog: public QDialog
{

    Q_OBJECT

public:

    AboutDialog();

    ~AboutDialog();

protected slots:

    void launchURL(const QString _url);
    void closeDialog();

private:

    QLabel* logo;
    QLabel* title;
    QLabel* version;
    QLabel* authors;
    QLabel* website;
    QLabel* copyright;
    QTextEdit* readme;
    QPushButton* buttons;

};

#endif
