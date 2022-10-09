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

#include "logModePluginsLoader.h"

#include "globals.h"

#include "defaults.h"
#include "logging.h"

// Includes existing log modes
#include "openFactory.h"

#include "systemFactory.h"

#include "acpidFactory.h"
#include "apacheFactory.h"
#include "authenticationFactory.h"
#include "cronFactory.h"
#include "cupsFactory.h"
#include "daemonFactory.h"
#include "kernelFactory.h"
#include "postfixFactory.h"
#include "sambaFactory.h"
#include "xorgFactory.h"
#include "xsessionFactory.h"
#ifdef HAVE_JOURNALD
#include "journaldFactory.h"
#endif
#ifdef HAVE_AUDIT
#include "auditFactory.h"
#endif

LogModePluginsLoader::LogModePluginsLoader(QWidget *parent)
    : mParent(parent)
{
}

LogModePluginsLoader::~LogModePluginsLoader()
{
}

void LogModePluginsLoader::loadPlugins()
{
    // Open Log mode needs the current window to display error messages and open dialogs
    Globals::instance().registerLogModeFactory(new OpenLogModeFactory(mParent));

    // System
    Globals::instance().registerLogModeFactory(new SystemLogModeFactory());

    // Kernel
    Globals::instance().registerLogModeFactory(new KernelLogModeFactory());

    // Authentication
    Globals::instance().registerLogModeFactory(new AuthenticationLogModeFactory());

    // Daemon
    Globals::instance().registerLogModeFactory(new DaemonLogModeFactory());

    // Xorg
    Globals::instance().registerLogModeFactory(new XorgLogModeFactory());

    // Cron
    Globals::instance().registerLogModeFactory(new CronLogModeFactory());

    // Apache
    Globals::instance().registerLogModeFactory(new ApacheLogModeFactory());

    // Postfix
    Globals::instance().registerLogModeFactory(new PostfixLogModeFactory());

    // Cups
    Globals::instance().registerLogModeFactory(new CupsLogModeFactory());

    // Samba
    Globals::instance().registerLogModeFactory(new SambaLogModeFactory());

    // Acpid
    Globals::instance().registerLogModeFactory(new AcpidLogModeFactory());

    // XSession
    Globals::instance().registerLogModeFactory(new XSessionLogModeFactory());

    // Audit
#ifdef HAVE_AUDIT
    Globals::instance().registerLogModeFactory(new AuditLogModeFactory());
#endif

    // Journald
#ifdef HAVE_JOURNALD
    Globals::instance().registerLogModeFactory(new JournaldModeFactory());
#endif
}
