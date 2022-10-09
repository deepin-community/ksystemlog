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

#include "systemLogMode.h"

#include <KLocalizedString>

#include "logging.h"

#include "systemAnalyzer.h"
#include "systemConfiguration.h"
#include "systemConfigurationWidget.h"

#include "logModeItemBuilder.h"

SystemLogMode::SystemLogMode()
    : LogMode(QStringLiteral(SYSTEM_LOG_MODE_ID), i18n("System Log"), QStringLiteral(SYSTEM_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<SystemConfiguration>(new SystemConfiguration());

    d->logModeConfigurationWidget = new SystemConfigurationWidget();

    d->itemBuilder = new LogModeItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the system log."));
    d->action->setWhatsThis(
        i18n("Displays the system log in the current tab. This log is generally used by non-specialized processes "
             "(like \"sudo\" or \"fsck\" commands)"));

    checkLogFilesPresence(logModeConfiguration<SystemConfiguration *>()->logFilesPaths());
}

SystemLogMode::~SystemLogMode()
{
}

Analyzer *SystemLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new SystemAnalyzer(this);
}

QVector<LogFile> SystemLogMode::createLogFiles()
{
    return logModeConfiguration<SystemConfiguration *>()->findGenericLogFiles();
}
