#ifndef WIDGET_H
#define WIDGET_H

#include <QApplication>
#include <QtWebKitWidgets/QWebView>
#include <QAction>
#include <QSettings>
#include <QCloseEvent>

#include <QLocalSocket>
#include <QLocalServer>

#include <QDebug>

#include "cookiejar.h"

class Widget : public QWebView
{
    Q_OBJECT
    
public:
	explicit Widget(QWidget *parent = 0);
    ~Widget();
    
public slots:
	void toggleWindowLock();
	void updateWindowTitle(QString title);
	void acceptSocketUrls();

protected:
	void closeEvent(QCloseEvent* e);

private:
	static const QString defaultPage;

	QAction lockAction;
	QAction backAction;
	QAction forwardAction;

	bool locked;
	QLocalServer* singleLock;
};

#endif // WIDGET_H
