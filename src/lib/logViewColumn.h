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

#include <QDataStream>
#include <QDebug>
#include <QString>

#include "globals.h"

class LogViewColumn
{
public:
    LogViewColumn(const LogViewColumn &column);

    /*
     * TODO For the moment, filtered and grouped parameters are useless : Use them or remove them
     */
    explicit LogViewColumn(const QString &name, bool filtered = true, bool grouped = true);

    virtual ~LogViewColumn();

    QString columnName() const;
    bool isGrouped() const;
    bool isFiltered() const;

    LogViewColumn &operator=(const LogViewColumn &column);

private:
    QString mColumnName;

    bool mFiltered;
    bool mGrouped;
};

QDataStream &operator<<(QDataStream &out, const LogViewColumn &column);
QDebug &operator<<(QDebug &out, const LogViewColumn &column);

