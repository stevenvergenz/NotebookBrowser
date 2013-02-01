#include "cookiejar.h"

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{
	// load cookies from files
}

bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> & cookieList, const QUrl & url)
{
	// save cookies to files
	QSettings settings;

	foreach( QNetworkCookie cookie, cookieList )
	{

	}

	return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}
