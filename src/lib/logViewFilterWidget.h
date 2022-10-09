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

#include <QStandardItem>
#include <QWidget>

#include <KTreeWidgetSearchLine>

#include "globals.h"
#include "logViewColumns.h"

class LogViewWidget;
class LogViewWidgetSearchLine;
class QComboBox;

class LogViewFilterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogViewFilterWidget(QWidget *parent = nullptr);

    ~LogViewFilterWidget() override;

    QComboBox *filterList() const;
    LogViewWidgetSearchLine *filterLine() const;

public Q_SLOTS:
    void updateFilterColumns(const LogViewColumns &list);

private Q_SLOTS:
    void changeColumnFilter(int column);
    void prioritiesChanged(QStandardItem *item);

Q_SIGNALS:
    void treeWidgetUpdated();

private:
    void initSearchListFilter();
    LogViewWidgetSearchLine *mFilterLine = nullptr;

    /**
     * Filter of the column list
     */
    QComboBox *mFilterList = nullptr;

    QComboBox *mPrioritiesComboBox = nullptr;

    QStandardItemModel *mPrioritiesModel = nullptr;
};

class LogViewWidgetSearchLine : public KTreeWidgetSearchLine
{
    Q_OBJECT

public:
    explicit LogViewWidgetSearchLine(QWidget *parent = nullptr);

    ~LogViewWidgetSearchLine() override;

    // Silence compiler warning
    using KTreeWidgetSearchLine::updateSearch;

    // Reimplemented just to send a signal _AFTER_ the tree updating
    void updateSearch(const QString &pattern = QString()) override;

    void setPriorityEnabled(int priority, bool enabled);

protected:
    bool itemMatches(const QTreeWidgetItem *item, const QString &pattern) const override;

Q_SIGNALS:
    void treeWidgetUpdated();

private:
    bool mPriorities[Globals::LOG_LEVEL_NUM];
};

