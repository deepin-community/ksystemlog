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

#include "tabLogManager.h"

#include <KLocalizedString>

#include "logging.h"

#include "view.h"

#include "defaults.h"
#include "logManager.h"
#include "logMode.h"

TabLogManager::TabLogManager(LogManager *logManager)
    : mLogManager(logManager)
{
}

TabLogManager::~TabLogManager()
{
    // delete mLogManager->usedView();
}

LogManager *TabLogManager::logManager() const
{
    return mLogManager;
}

void TabLogManager::addNewLinesCount(int newLines)
{
    mNewLinesCount += newLines;
}

void TabLogManager::initNewLinesCount()
{
    mNewLinesCount = 0;
}

QString TabLogManager::title() const
{
    if (mNewLinesCount == 0) {
        return logModeName();
    } else {
        return i18nc("Log mode name (added lines count)", "%1 (%2)", mLogManager->title(), mNewLinesCount);
    }
}

QString TabLogManager::logModeName() const
{
    if (!mLogManager->logMode()) {
        return i18nc("Newly created tab", "Empty Log");
    } else {
        return mLogManager->title();
    }
}
