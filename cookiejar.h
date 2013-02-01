#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QSettings>

class CookieJar : public QNetworkCookieJar
{
	Q_OBJECT
public:
	explicit CookieJar(QObject *parent = 0);
	bool setCookiesFromUrl(const QList<QNetworkCookie> & cookieList, const QUrl & url);

signals:
	
public slots:
	
};

#endif // COOKIEJAR_H
