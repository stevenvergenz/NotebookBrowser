#include "cookiejar.h"

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{
	// load cookies from files
	QSettings settings;
	QList<QNetworkCookie> cookieList;

	qDebug() << "Loading cookies" << endl;
	for( int i=0; i<settings.beginReadArray("cookies"); i++ )
	{
		settings.setArrayIndex(i);
		qDebug() << "   " << settings.value("cookie").toByteArray() << endl;
		cookieList += QNetworkCookie::parseCookies( settings.value("cookie").toByteArray() );
	}
	settings.endArray();

	setAllCookies(cookieList);
}

bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> & cookieList, const QUrl & url)
{
	// save cookies to files
	QSettings settings;

	qDebug() << "Saving cookies for " << url.toString() << endl;

	// build list of cookies
	settings.beginWriteArray("cookies");
	int i=0;
	foreach( QNetworkCookie cookie, cookieList ){
		if( !cookie.isSessionCookie() ){
			settings.setArrayIndex(i++);
			settings.setValue("cookie", QVariant(cookie.toRawForm()));
			qDebug() << "   " << cookie.toRawForm() << endl;
		}
	}
	settings.endArray();

	return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

QList<QNetworkCookie> CookieJar::cookiesForUrl(const QUrl & url) const
{
	qDebug() << "Pulling cookie for " << url.toString() << endl;
	QList<QNetworkCookie> ret = QNetworkCookieJar::cookiesForUrl(url);
	foreach( QNetworkCookie cookie, ret ){
		qDebug() << "   " << cookie.toRawForm() << endl;
	}

	return ret;
}
