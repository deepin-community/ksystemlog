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

#include "xorgAnalyzer.h"
#include <KLocalizedString>

XorgAnalyzer::XorgAnalyzer(LogMode *logMode)
    : FileAnalyzer(logMode)
    , mCurrentDateTime(QDateTime::currentDateTime())
{
    initializeTypeName();
}

XorgAnalyzer::~XorgAnalyzer()
{
}

LogViewColumns XorgAnalyzer::initColumns()
{
    LogViewColumns columns;

    columns.addColumn(LogViewColumn(i18n("Line"), false, false));
    columns.addColumn(LogViewColumn(i18n("Type"), false, false));
    columns.addColumn(LogViewColumn(i18n("Message"), false, false));

    columns.setGroupByDay(false);
    columns.setGroupByHour(false);

    return columns;
}

LogFileReader *XorgAnalyzer::createLogFileReader(const LogFile &logFile)
{
    return new LocalLogFileReader(logFile);
}

Analyzer::LogFileSortMode XorgAnalyzer::logFileSortMode()
{
    return Analyzer::AscendingSortedLogFile;
}

LogLine *XorgAnalyzer::parseMessage(const QString &logLine, const LogFile &originalFile)
{
    QString string(logLine);

    const QString type = string.left(4);

    LogLevel *logLineType = findTypeName(type);

    // If the type is not empty, the log message has a type, so we can delete it
    if (logLineType) {
        string.remove(0, 5);
    } else {
        logLineType = Globals::instance().informationLogLevel();
    }

    QStringList list;
    list.append(logLineType->name());
    list.append(string);

    return new LogLine(mLogLineInternalIdGenerator++, mCurrentDateTime, list, originalFile.url().toLocalFile(), logLineType, mLogMode);
}

void XorgAnalyzer::initializeTypeName()
{
    mXorgLevels[QStringLiteral("(--)")] = new LogLevel(1001, i18n("Probed"), QStringLiteral(PROBED_LOG_LEVEL_ICON), QColor(246, 206, 30), this);

    mXorgLevels[QStringLiteral("(**)")] = new LogLevel(1002, i18n("From config file"), QStringLiteral(CONFIG_FILE_LOG_LEVEL_ICON), QColor(161, 133, 240), this);

    mXorgLevels[QStringLiteral("(==)")] =
        new LogLevel(1003, i18n("Default setting"), QStringLiteral(DEFAULT_SETTING_LOG_LEVEL_ICON), QColor(169, 189, 165), this);

    mXorgLevels[QStringLiteral("(++)")] =
        new LogLevel(1004, i18n("From command Line"), QStringLiteral(COMMAND_LINE_LOG_LEVEL_ICON), QColor(179, 181, 214), this);

    mXorgLevels[QStringLiteral("(!!)")] = Globals::instance().noticeLogLevel();
    mXorgLevels[QStringLiteral("(II)")] = Globals::instance().informationLogLevel();
    mXorgLevels[QStringLiteral("(WW)")] = Globals::instance().warningLogLevel();
    mXorgLevels[QStringLiteral("(EE)")] = Globals::instance().errorLogLevel();

    mXorgLevels[QStringLiteral("(NI)")] =
        new LogLevel(1005, i18n("Not implemented"), QStringLiteral(NOT_IMPLEMENTED_LOG_LEVEL_ICON), QColor(136, 146, 240), this);

    mXorgLevels[QStringLiteral("(\?\?)")] = Globals::instance().noLogLevel();
}

LogLevel *XorgAnalyzer::findTypeName(const QString &type)
{
    QMap<QString, LogLevel *>::iterator it;

    it = mXorgLevels.find(type);
    if (it != mXorgLevels.end()) {
        return *it;
    } else {
        return nullptr;
    }
}
