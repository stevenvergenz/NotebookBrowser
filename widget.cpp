#include "widget.h"

const QString Widget::defaultPage = "http://imaginarium.adlnet.org/ImaginariumNotebook.php?id=test";

Widget::Widget(QWidget *parent) : QWebView(parent), lockAction(this), backAction(this),
	forwardAction(this), locked(false), singleLock(NULL)
{
	// set up singleton
	QStringList args = QApplication::arguments();
	QLocalSocket socket;
	socket.connectToServer("NotebookBrowser");
	if( socket.waitForConnected(500) ){
		qDebug() << "Connected to other instance" << endl;
		if( args.count() == 2 ){
			QByteArray bytes;
			bytes.append(args[1].toUtf8());
			socket.write( bytes );
			socket.waitForBytesWritten();
			qDebug() << "URL sent" << endl;
		}
		this->deleteLater();
		return;
	}
	else {
		singleLock = new QLocalServer(this);
		singleLock->listen("NotebookBrowser");
		connect( singleLock, SIGNAL(newConnection()), this, SLOT(acceptSocketUrls()) );
	}

	// load settings from config file
	QCoreApplication::setOrganizationName("ADL");
	QCoreApplication::setOrganizationDomain("adlnet.gov");
	QCoreApplication::setApplicationName("Notebook Browser");
	QSettings::setDefaultFormat( QSettings::IniFormat );
	QSettings settings;
	if( settings.contains("windowGeometry") ){
		restoreGeometry( settings.value("windowGeometry").toByteArray() );
	}

	// set up cookie jar
	page()->networkAccessManager()->setCookieJar( new CookieJar(this) );
	//QWebSettings::globalSettings()->setThirdPartyCookiePolicy( QWebSettings::AlwaysBlockThirdPartyCookies );

	// connect the lock shortcut
	lockAction.setShortcut( QKeySequence("Ctrl+l") );
	addAction(&lockAction);
	connect( &lockAction, SIGNAL(triggered()), this, SLOT(toggleWindowLock()) );

	// connect the back shortcut
	backAction.setShortcut( QKeySequence(Qt::ALT + Qt::Key_Left) );
	addAction( &backAction );
	connect( &backAction, SIGNAL(triggered()), this, SLOT(back()) );

	// connect the forward shortcut
	forwardAction.setShortcut( QKeySequence(Qt::ALT + Qt::Key_Right) );
	addAction( &forwardAction );
	connect( &forwardAction, SIGNAL(triggered()), this,	SLOT(forward()) );

	// set window flags
	Qt::WindowFlags f = Qt::Window | Qt::CustomizeWindowHint
		| Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint;
	setWindowFlags(f);

	connect( this, SIGNAL(titleChanged(QString)), this, SLOT(updateWindowTitle(QString)) );

	// process command-line argument if available
	if( args.count() == 2 ){ // url argument
		QUrl url(args.at(1), QUrl::StrictMode);
		if( url.isValid() ){
			qDebug() << "Valid arg, loading" << endl;
			load( url );
		}
		else {
			qDebug() << "Invalid arg" << endl;
			load( QUrl(defaultPage) );
		}
	}
	else {
		qDebug() << "No arg given" << endl;
		load( QUrl(defaultPage) );
	}
}

Widget::~Widget()
{
	if( singleLock != NULL ){
		singleLock->close();
		delete singleLock;
	}
}

void Widget::acceptSocketUrls()
{
	QLocalSocket* socket = singleLock->nextPendingConnection();
	socket->waitForReadyRead();
	QByteArray data = socket->readAll();
	QString urlString =	QString::fromUtf8(data);
	qDebug() << "Received url " << urlString << endl;

	load(QUrl(urlString));
}

void Widget::updateWindowTitle(QString title){
	if( !locked )
		setWindowTitle(title);
	else
		setWindowTitle(title + " (locked)");
}

void Widget::toggleWindowLock()
{
	qDebug() << "window lock: " << !locked << endl;

	if( !locked ){
		// lock down resizing
		setMinimumSize( width(), height() );
		setMaximumSize( width(), height() );
		locked = true;
	}
	else {
		// open up resizing restrictions
		setMinimumSize(0, 0);
		setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
		locked = false;
	}
	updateWindowTitle(title());
}

void Widget::closeEvent(QCloseEvent* e)
{
	QSettings settings;
	settings.setValue("windowGeometry", saveGeometry());
	e->accept();
}
