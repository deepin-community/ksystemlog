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

#include "apacheConfiguration.h"

ApacheConfiguration::ApacheConfiguration()
{
    mConfiguration->setCurrentGroup(QStringLiteral("ApacheLogMode"));

    const QStringList defaultApachePaths{QStringLiteral("/var/log/apache2/error.log")};
    mConfiguration->addItemStringList(QStringLiteral("ApacheLogFilesPaths"), mApachePaths, defaultApachePaths, QStringLiteral("ApacheLogFilesPaths"));

    const QStringList defaultApacheAccessPaths{QStringLiteral("/var/log/apache2/access.log")};
    mConfiguration->addItemStringList(QStringLiteral("ApacheAccessLogFilesPaths"),
                                      mApacheAccessPaths,
                                      defaultApacheAccessPaths,
                                      QStringLiteral("ApacheAccessLogFilesPaths"));
}

ApacheConfiguration::~ApacheConfiguration()
{
}

QStringList ApacheConfiguration::apachePaths() const
{
    return mApachePaths;
}

QStringList ApacheConfiguration::apacheAccessPaths() const
{
    return mApacheAccessPaths;
}

void ApacheConfiguration::setApachePaths(const QStringList &apachePaths)
{
    mApachePaths = apachePaths;
}

void ApacheConfiguration::setApacheAccessPaths(const QStringList &apacheAccessPaths)
{
    mApacheAccessPaths = apacheAccessPaths;
}
