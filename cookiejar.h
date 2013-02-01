#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QSettings>
#include <QStringList>
#include <QVariantList>
#include <QList>
#include <QDebug>

class CookieJar : public QNetworkCookieJar
{
	Q_OBJECT
public:
	explicit CookieJar(QObject *parent = 0);
	bool setCookiesFromUrl(const QList<QNetworkCookie> & cookieList, const QUrl & url);
	QList<QNetworkCookie> cookiesForUrl(const QUrl & url) const;

signals:
	
public slots:
	
};

#endif // COOKIEJAR_H
