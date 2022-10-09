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

#include "logModeItemBuilder.h"

#include <KLocalizedString>

#include "logging.h"

#include "logLine.h"
#include "logMode.h"
#include "logViewWidgetItem.h"

#include "ksystemlogConfig.h"

LogModeItemBuilder::LogModeItemBuilder()
{
}

LogModeItemBuilder::~LogModeItemBuilder()
{
}

QString LogModeItemBuilder::formatDate(const QDateTime &dateTime) const
{
    return Globals::instance().formatDate((Globals::DateFormat)KSystemLogConfig::dateFormat(), dateTime);
}

void LogModeItemBuilder::prepareItem(LogViewWidgetItem *item) const
{
    LogLine *line = item->logLine();

    item->setText(0, formatDate(line->time()));
    item->setData(0, Qt::UserRole, line->logLevel()->id());

    int i = 1;
    const QStringList logItems = line->logItems();
    for (const QString &label : logItems) {
        item->setText(i, label);
        i++;
    }

    item->setIcon(0, line->logLevel()->icon());
}

QString LogModeItemBuilder::createFormattedText(LogLine *line) const
{
    QString result;

    QListIterator<QString> it(line->logItems());

    result.append(QLatin1String("<table>"));

    result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
    result.append(labelMessageFormat(i18n("Hostname:"), it.next()));
    result.append(labelMessageFormat(i18n("Process:"), it.next()));
    result.append(labelMessageFormat(i18n("Level:"), line->logLevel()->name()));
    result.append(labelMessageFormat(i18n("Original file:"), line->sourceFileName()));

    result.append(QLatin1String("</table>"));

    return result;
}

QString LogModeItemBuilder::createToolTipText(LogLine *line) const
{
    QString result;

    result.append(QLatin1String("<table>"));

    result.append(labelMessageFormat(i18n("Date:"), formatDate(line->time())));
    result.append(labelMessageFormat(i18n("Level:"), line->logLevel()->name()));
    result.append(labelMessageFormat(i18n("Original file:"), line->sourceFileName()));

    result.append(QLatin1String("</table>"));

    return result;
}

QString LogModeItemBuilder::labelMessageFormat(const QString &label, const QString &value) const
{
    return QLatin1String("<tr><td align='right'><b><nobr>") + label + QLatin1String("</nobr></b></td><td>") + messageFormat(value)
        + QLatin1String("</td></tr>");
}

QString LogModeItemBuilder::messageFormat(const QString &message) const
{
    QString transformation(message);
    transformation.replace(QStringLiteral("&"), QStringLiteral("&amp;"));
    transformation.replace(QStringLiteral("<"), QStringLiteral("&lt;"));
    transformation.replace(QStringLiteral(">"), QStringLiteral("&gt;"));
    return transformation;
}
