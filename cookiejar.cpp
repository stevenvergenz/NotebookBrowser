#include "cookiejar.h"

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{
	// load cookies from files
	QSettings settings;
	QList<QNetworkCookie> cookieList;

	settings.beginGroup("cookies");
	foreach( QString url, settings.allKeys() )
	{
		url = url.replace("|", "/");
		qDebug() << "List type is " << settings.value(url).type() << endl;
		QVariantList urlCookieList = settings.value(url).toList();
		qDebug() << "Loading cookie for " << url << endl;
		foreach( QVariant cookie, urlCookieList){
			qDebug() << "   " << cookie.toByteArray() << endl;
			cookieList += QNetworkCookie::parseCookies(cookie.toByteArray());
		}
	}
	settings.endGroup();

	setAllCookies(cookieList);
}

bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> & cookieList, const QUrl & url)
{
	// save cookies to files
	QSettings settings;
	QVariantList saveList;

	qDebug() << "Saving cookie for " << url.toString() << endl;

	foreach( QNetworkCookie cookie, cookieList ){
		if( !cookie.isSessionCookie() ){
			saveList += QVariant(cookie.toRawForm());
			qDebug() << "   " << cookie.toRawForm() << endl;
		}
	}
	settings.setValue("cookies/"+url.toString().replace("/", "|"), QVariant(saveList));

	return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

QList<QNetworkCookie> CookieJar::cookiesForUrl(const QUrl & url) const
{
	qDebug() << "Pulling cookie for " << url.toString() << endl;
	return QNetworkCookieJar::cookiesForUrl(url);
}
