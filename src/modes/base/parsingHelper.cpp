/***************************************************************************
 *   KSystemLog, a system log viewer tool                                  *
 *   Copyright (C) 2007 by Nicolas Ternisien                               *
 *   nicolas.ternisien@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "parsingHelper.h"

#include <KFormat>
#include <KLocalizedString>

#include "logging.h"

ParsingHelper *ParsingHelper::self = nullptr;

ParsingHelper *ParsingHelper::instance()
{
    if (!ParsingHelper::self) {
        ParsingHelper::self = new ParsingHelper();
    }

    return ParsingHelper::self;
}

ParsingHelper::ParsingHelper()
{
    // Initialize Existing months
    mMapMonths[QStringLiteral("Jan")] = 1;
    mMapMonths[QStringLiteral("Feb")] = 2;
    mMapMonths[QStringLiteral("Mar")] = 3;
    mMapMonths[QStringLiteral("Apr")] = 4;
    mMapMonths[QStringLiteral("May")] = 5;
    mMapMonths[QStringLiteral("Jun")] = 6;
    mMapMonths[QStringLiteral("Jul")] = 7;
    mMapMonths[QStringLiteral("Aug")] = 8;
    mMapMonths[QStringLiteral("Sep")] = 9;
    mMapMonths[QStringLiteral("Oct")] = 10;
    mMapMonths[QStringLiteral("Nov")] = 11;
    mMapMonths[QStringLiteral("Dec")] = 12;

    // Initialize HTTP Responses
    // 1xx Responses
    mMapHTTPResponse[QStringLiteral("100")] = QStringLiteral("Continue");
    mMapHTTPResponse[QStringLiteral("101")] = QStringLiteral("Switching Protocols");

    // 2xx Responses
    mMapHTTPResponse[QStringLiteral("200")] = QStringLiteral("OK");
    mMapHTTPResponse[QStringLiteral("201")] = QStringLiteral("Created");
    mMapHTTPResponse[QStringLiteral("202")] = QStringLiteral("Accepted");
    mMapHTTPResponse[QStringLiteral("203")] = QStringLiteral("Non-Authoritative Information");
    mMapHTTPResponse[QStringLiteral("204")] = QStringLiteral("No Content");
    mMapHTTPResponse[QStringLiteral("205")] = QStringLiteral("Reset Content");
    mMapHTTPResponse[QStringLiteral("206")] = QStringLiteral("Partial Content");

    // 3xx Responses
    mMapHTTPResponse[QStringLiteral("300")] = QStringLiteral("OK");
    mMapHTTPResponse[QStringLiteral("301")] = QStringLiteral("Moved Permanently");
    mMapHTTPResponse[QStringLiteral("302")] = QStringLiteral("Found");
    mMapHTTPResponse[QStringLiteral("303")] = QStringLiteral("See Other");
    mMapHTTPResponse[QStringLiteral("304")] = QStringLiteral("Not Modified");
    mMapHTTPResponse[QStringLiteral("305")] = QStringLiteral("Use Proxy");
    mMapHTTPResponse[QStringLiteral("306")] = QStringLiteral("(Unused)");
    mMapHTTPResponse[QStringLiteral("307")] = QStringLiteral("Temporary Redirect");

    // 4xx Responses
    mMapHTTPResponse[QStringLiteral("400")] = QStringLiteral("Bad Request");
    mMapHTTPResponse[QStringLiteral("401")] = QStringLiteral("Unauthorized");
    mMapHTTPResponse[QStringLiteral("402")] = QStringLiteral("Payment Required");
    mMapHTTPResponse[QStringLiteral("403")] = QStringLiteral("Forbidden");
    mMapHTTPResponse[QStringLiteral("404")] = QStringLiteral("Not Found");
    mMapHTTPResponse[QStringLiteral("405")] = QStringLiteral("Method Not Allowed");
    mMapHTTPResponse[QStringLiteral("406")] = QStringLiteral("Not Acceptable");
    mMapHTTPResponse[QStringLiteral("407")] = QStringLiteral("Proxy Authentication Required");
    mMapHTTPResponse[QStringLiteral("408")] = QStringLiteral("Request Timeout");
    mMapHTTPResponse[QStringLiteral("409")] = QStringLiteral("Conflict");
    mMapHTTPResponse[QStringLiteral("410")] = QStringLiteral("Gone");
    mMapHTTPResponse[QStringLiteral("411")] = QStringLiteral("Length Required");
    mMapHTTPResponse[QStringLiteral("412")] = QStringLiteral("Precondition Failed");
    mMapHTTPResponse[QStringLiteral("413")] = QStringLiteral("Request Entity Too Large");
    mMapHTTPResponse[QStringLiteral("414")] = QStringLiteral("Request-URI Too Long");
    mMapHTTPResponse[QStringLiteral("415")] = QStringLiteral("Unsupported Media Type");
    mMapHTTPResponse[QStringLiteral("416")] = QStringLiteral("Requested Range Not Satisfiable");
    mMapHTTPResponse[QStringLiteral("417")] = QStringLiteral("Expectation Failed");

    // 5xx Responses
    mMapHTTPResponse[QStringLiteral("500")] = QStringLiteral("Internal Server Error");
    mMapHTTPResponse[QStringLiteral("501")] = QStringLiteral("Not Implemented");
    mMapHTTPResponse[QStringLiteral("502")] = QStringLiteral("Bad Gateway");
    mMapHTTPResponse[QStringLiteral("503")] = QStringLiteral("Service Unavailable");
    mMapHTTPResponse[QStringLiteral("504")] = QStringLiteral("Gateway Timeout");
    mMapHTTPResponse[QStringLiteral("505")] = QStringLiteral("HTTP Version Not Supported");
}

ParsingHelper::~ParsingHelper()
{
}

QDateTime ParsingHelper::parseHttpDateTime(const QString &logLine)
{
    // Format example : 22/May/2005:01:50:34 +0200

    const QString day = logLine.mid(0, 2);
    const QString month = logLine.mid(3, 3);
    const QString year = logLine.mid(7, 4);

    const QString hour = logLine.mid(12, 2);
    const QString min = logLine.mid(15, 2);
    const QString sec = logLine.mid(18, 2);

    // QString zone=logLine.mid(22,5);

    return QDateTime(QDate(year.toInt(), parseSyslogMonth(month), day.toInt()), QTime(hour.toInt(), min.toInt(), sec.toInt()));
}

/**
 * Example : "Oct  1 09:11:45 2005"
 */
QDateTime ParsingHelper::parseSyslogDateTime(const QString &dateTime)
{
    // TODO Create this regexp in constructor
    const static QRegExp regex(QLatin1String(R"((\S*)[ ]+(\d*) (\d*):(\d*):(\d*) (\d*))"));

    const int firstPosition = regex.indexIn(dateTime);
    if (firstPosition == -1) {
        logDebug() << "Unable to parse date " << dateTime;
        return QDateTime::currentDateTime();
    }

    return QDateTime(QDate(regex.cap(6).toInt(), parseSyslogMonth(regex.cap(1)), regex.cap(2).toInt()),
                     QTime(regex.cap(3).toInt(), regex.cap(4).toInt(), regex.cap(5).toInt(), 0));
}

QString ParsingHelper::syslogDateTimeRegexp() const
{
    return QStringLiteral("(\\S*[ ]+\\d* \\d*:\\d*:\\d* \\d*)");
}

int ParsingHelper::parseSyslogMonth(const QString &string)
{
    return mMapMonths.value(string, 1);
}

QString ParsingHelper::parseSize(const QString &stringSize)
{
    const qint64 size = stringSize.toLongLong();

    return KFormat().formatByteSize(size);
}

QString ParsingHelper::parseHttpResponse(const QString &response)
{
    // Search the response string in the map
    QMap<QString, QString>::Iterator it = mMapHTTPResponse.find(response);
    if (it != mMapHTTPResponse.end()) {
        return i18nc("HttpResponseNumber HttpResponseDescription", "%1 %2", response, *it);
    } else {
        return response;
    }
}

QString ParsingHelper::parseAgent(const QString &agent)
{
    return agent;
}
