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

#include "cupsLogMode.h"

#include <QAction>

#include <KLocalizedString>

#include "logging.h"

#include "cupsAnalyzer.h"
#include "cupsConfiguration.h"
#include "cupsConfigurationWidget.h"
#include "cupsItemBuilder.h"

CupsLogMode::CupsLogMode(QSharedPointer<CupsConfiguration> &cupsConfiguration, CupsConfigurationWidget *cupsConfigurationWidget)
    : LogMode(QStringLiteral(CUPS_LOG_MODE_ID), i18n("Cups Log"), QStringLiteral(CUPS_MODE_ICON))
{
    d->logModeConfiguration = cupsConfiguration;
    d->logModeConfigurationWidget = cupsConfigurationWidget;

    d->itemBuilder = new CupsItemBuilder();

    // Cups Log Action
    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the Cups log."));
    d->action->setWhatsThis(
        i18n("Displays the CUPS log in the current tab. CUPS is the program which manages printing on your "
             "computer."));

    checkLogFilesPresence(cupsConfiguration->cupsPaths());
}

CupsLogMode::~CupsLogMode()
{
}

Analyzer *CupsLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new CupsAnalyzer(this);
}

QVector<LogFile> CupsLogMode::createLogFiles()
{
    auto *cupsConfiguration = logModeConfiguration<CupsConfiguration *>();
    return cupsConfiguration->findNoModeLogFiles(cupsConfiguration->cupsPaths());
}
