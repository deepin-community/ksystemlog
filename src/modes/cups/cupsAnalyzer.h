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

#pragma once

#include "fileAnalyzer.h"

#include "logging.h"

#include "cupsLogMode.h"
#include "localLogFileReader.h"
#include "parsingHelper.h"

#define DEBUG2_LOG_LEVEL_ICON "source"

class CupsAnalyzer : public FileAnalyzer
{
    Q_OBJECT

public:
    explicit CupsAnalyzer(LogMode *logMode);

    ~CupsAnalyzer() override;

    LogViewColumns initColumns() override;

protected:
    LogFileReader *createLogFileReader(const LogFile &logFile) override;

    Analyzer::LogFileSortMode logFileSortMode() override;

    /*
     * Also sees :
     * https://www.cups.org/doc/man-cupsd-logs.html
     * level date-time message
     *
     * Levels :
     * A - Alert message (LogLevel alert)
     * C - Critical error message (LogLevel crit)
     * D - Debugging message (LogLevel debug)
     * d - Detailed debugging message (LogLevel debug2)
     * E - Normal error message (LogLevel error)
     * I - Informational message (LogLevel info)
     * N - Notice message (LogLevel notice)
     * W - Warning message (LogLevel warn)
     * X - Emergency error message (LogLevel emerg)
     *
     * Log line examples :
     * I [15/Feb/2004:01:29:32 +0100] LoadPPDs: No new or changed PPDs...
     * E [15/Feb/2004:01:43:15 +0100] Scheduler shutting down due to SIGTERM.
     *
     */
    LogLine *parseMessage(const QString &logLine, const LogFile &originalLogFile) override;

private:
    QMap<QChar, LogLevel *> mMapTypeLevels;

    void initializeTypeLevels();

    LogLevel *findLogLevel(QChar type);
};

