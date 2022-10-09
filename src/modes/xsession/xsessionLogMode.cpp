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

#include "xsessionLogMode.h"

#include <KLocalizedString>

#include "logging.h"

#include "xsessionAnalyzer.h"
#include "xsessionConfiguration.h"
#include "xsessionConfigurationWidget.h"
#include "xsessionItemBuilder.h"

XSessionLogMode::XSessionLogMode()
    : LogMode(QStringLiteral(X_SESSION_LOG_MODE_ID), i18n("X Session Log"), QStringLiteral(X_SESSION_MODE_ICON))
{
    d->logModeConfiguration = QSharedPointer<XSessionConfiguration>(new XSessionConfiguration());

    d->logModeConfigurationWidget = new XSessionConfigurationWidget();

    d->itemBuilder = new XSessionItemBuilder();

    d->action = createDefaultAction();
    d->action->setToolTip(i18n("Display the X Session log."));
    d->action->setWhatsThis(
        i18n("Displays the X Session log in the current tab. X Session log is the place where graphical programs "
             "write their output. See this log if you want to know why a program has crashed, or why your desktop "
             "environment (Plasma, Gnome, Unity, Xfce, LXQt, ...) has not started."));

    auto *configuration = logModeConfiguration<XSessionConfiguration *>();
    checkLogFilesPresence(QStringList() << configuration->xsessionPath());
}

XSessionLogMode::~XSessionLogMode()
{
}

Analyzer *XSessionLogMode::createAnalyzer(const QVariant &options)
{
    Q_UNUSED(options)
    return new XSessionAnalyzer(this);
}

QVector<LogFile> XSessionLogMode::createLogFiles()
{
    auto *configuration = Globals::instance().findLogMode(QStringLiteral(X_SESSION_LOG_MODE_ID))->logModeConfiguration<XSessionConfiguration *>();

    const QVector<LogFile> logFiles{configuration->findGenericLogFile(configuration->xsessionPath())};
    return logFiles;
}
