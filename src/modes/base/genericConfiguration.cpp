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

#include "genericConfiguration.h"

#include <KLocalizedString>

#include "defaults.h"
#include "logging.h"

#include "globals.h"

#include "ksystemlogConfig.h"
GenericLogModeConfiguration::GenericLogModeConfiguration(const QString &configurationGroup,
                                                         const QStringList &defaultLogFilesPaths,
                                                         const QList<int> &defaultLogFilesLevels)
{
    logDebug() << "Using Configuration Group : " << configurationGroup;
    mConfiguration->setCurrentGroup(configurationGroup);

    mConfiguration->addItemStringList(QStringLiteral("LogFilesPaths"), mLogFilesPaths, defaultLogFilesPaths, QStringLiteral("LogFilesPaths"));

    mConfiguration->addItemIntList(QStringLiteral("LogFilesLevels"), mLogFilesLevels, defaultLogFilesLevels, QStringLiteral("LogFilesLevels"));
}

GenericLogModeConfiguration::~GenericLogModeConfiguration()
{
}

QStringList GenericLogModeConfiguration::logFilesPaths() const
{
    return mLogFilesPaths;
}

QList<int> GenericLogModeConfiguration::logFilesLevels() const
{
    return mLogFilesLevels;
}

void GenericLogModeConfiguration::setLogFilesPaths(const QStringList &logFilesPaths)
{
    mLogFilesPaths = logFilesPaths;
}

void GenericLogModeConfiguration::setLogFilesLevels(const QList<int> &logFilesLevels)
{
    mLogFilesLevels = logFilesLevels;
}

QVector<LogFile> GenericLogModeConfiguration::findGenericLogFiles() const
{
    QVector<LogFile> logFiles;

    if (mLogFilesPaths.size() != mLogFilesLevels.size()) {
        logDebug() << i18n("The two arrays size are different, skipping the reading of log files.");
        return logFiles;
    }

    LogLevel *level = nullptr;

    QListIterator<QString> itString(mLogFilesPaths);
    QListIterator<int> itInt(mLogFilesLevels);

    while (itString.hasNext()) {
        const int intValue = itInt.next();
        const QString stringValue = itString.next();

        level = Globals::instance().logLevels().value(Globals::LogLevelIds(intValue), Globals::instance().informationLogLevel());

        const QUrl url = QUrl::fromLocalFile(stringValue);
        if (!url.isValid()) {
            logWarning() << i18n("URL '%1' is not valid, skipping this URL.", url.path());
            continue;
        }

        logFiles.append(LogFile(url, level));
    }

    return logFiles;
}
