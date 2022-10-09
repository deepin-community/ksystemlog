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

#include <QIcon>
#include <QList>
#include <QObject>
#include <QString>
#include <QVector>

#include "logFile.h"

class Analyzer;
class LogModeItemBuilder;
class LogModeConfiguration;
class LogModeConfigurationWidget;

class QAction;

struct ActionData {
    QString id;
    bool addToActionCollection = true;
    QVariant analyzerOptions;
};
Q_DECLARE_METATYPE(ActionData)

// TODO Do not let this class visible to other classes (except sub-classes)
class LogModePrivate
{
public:
    QString id;

    QString name;

    QString iconName;

    QIcon icon;

    QAction *action = nullptr;

    LogModeItemBuilder *itemBuilder = nullptr;

    LogModeConfigurationWidget *logModeConfigurationWidget = nullptr;

    QSharedPointer<LogModeConfiguration> logModeConfiguration;

    bool logFilesExist;
};

class LogMode : public QObject
{
    Q_OBJECT

public:
    LogMode(const QString &id, const QString &name, const QString &iconName);

    ~LogMode() override;

    QString id() const;

    QString name() const;

    QIcon icon() const;

    QAction *action() const;

    LogModeItemBuilder *itemBuilder() const;

    /**
     * Returns true if at least one log file exists for this mode.
     */
    bool filesExist() const;

    /**
     * Log mode configuration widget
     */
    LogModeConfigurationWidget *logModeConfigurationWidget() const;

    template<typename T> T logModeConfiguration()
    {
        return static_cast<T>(innerConfiguration());
    }

    /**
     * Create the Analyzer used to parse the log file
     */
    virtual Analyzer *createAnalyzer(const QVariant &options = QVariant()) = 0;

    /**
     * Create the log file list which will be read
     */
    virtual QVector<LogFile> createLogFiles() = 0;

Q_SIGNALS:
    void menuChanged();

protected:
    QAction *createDefaultAction();

    /**
     * Initializes the flag returned by filesExist().
     */
    void checkLogFilesPresence(const QStringList &paths);

    LogModePrivate *const d;

private:
    /**
     * Log Mode Configuration
     */
    LogModeConfiguration *innerConfiguration() const;
};

