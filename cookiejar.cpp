#include "cookiejar.h"

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{
	// load cookies from files
	QSettings settings;

	settings.beginGroup("cookies");

	// get list of stored urls
	QStringList urlList;
	foreach( QString key, settings.allKeys() ){
		QString url = key.split("/")[0];
		if( !urlList.contains(url) )
			urlList.append(url);
	}

	// load cookies for each url
	foreach( QString url, urlList )
	{
		QList<QNetworkCookie> freshCookieList;

		//qDebug() << "Loading cookies for " << url << endl;
		int size = settings.beginReadArray(url);
		for( int i=0; i<size; i++ )
		{
			settings.setArrayIndex(i);
			QList<QNetworkCookie> tempCookieList =
				QNetworkCookie::parseCookies( settings.value("cookie").toByteArray() );
			foreach( QNetworkCookie c, tempCookieList ){
				if( c.expirationDate() > QDateTime::currentDateTime() ){
					freshCookieList += c;
				}
			}

			//qDebug() << "   " << settings.value("cookie").toByteArray() << endl;
		}
		settings.endArray();

		setCookiesFromUrl( freshCookieList, QUrl(url) );
	}

	settings.endGroup();
}

bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> & cookieList, const QUrl & url)
{
	// save cookies to files
	QSettings settings;

	//qDebug() << "Saving cookies for " << url.toString() << endl;

	// build list of cookies
	settings.beginGroup("cookies");
	settings.beginWriteArray(url.host());

	int i=0;
	foreach( QNetworkCookie cookie, cookieList ){
		if( !cookie.isSessionCookie() ){
			settings.setArrayIndex(i++);
			settings.setValue("cookie", QVariant(cookie.toRawForm()));
			qDebug() << "   " << cookie.toRawForm() << endl;
		}
	}
	settings.endArray();
	settings.endGroup();

	return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

QList<QNetworkCookie> CookieJar::cookiesForUrl(const QUrl & url) const
{
	//qDebug() << "Pulling cookie for " << url.toString() << endl;
	QList<QNetworkCookie> ret = QNetworkCookieJar::cookiesForUrl(url);
	/*foreach( QNetworkCookie cookie, ret ){
		qDebug() << "   " << cookie.toRawForm() << endl;
	}*/

	return ret;
}
