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

#include "fileListHelper.h"

#include <QAction>
#include <QFileDialog>
#include <QObject>
#include <QPushButton>

#include <KLocalizedString>
#include <KMessageBox>

#include "defaults.h"

#include "logging.h"

FileListHelper::FileListHelper(QWidget *p)
    : mParent(p)
{
}

FileListHelper::~FileListHelper()
{
}

QAction *FileListHelper::prepareButtonAndAction(QPushButton *button, const QIcon &icon)
{
    // Initialize action
    auto action = new QAction(button);
    action->setIcon(icon);
    action->setText(button->text());

    // Initialize button
    button->setIcon(icon);
    // Allow the disabling of the matching action when disabling this button
    button->addAction(action);

    // Assert that when an action is triggered, the related button sends a clicked() event
    //(the button is the QObject which is connected to custom slots)
    connect(action, &QAction::triggered, button, &QAbstractButton::click);

    return action;
}

QAction *FileListHelper::prepareButtonAndAction(QPushButton *button, const QIcon &icon, const QObject *receiver, const char *member)
{
    QAction *action = prepareButtonAndAction(button, icon);
    connect(button, SIGNAL(clicked(bool)), receiver, member);

    return action;
}

void FileListHelper::prepareButton(QPushButton *button, const QIcon &icon, const QObject *receiver, const char *member, QWidget *fileList)
{
    // Initialize action
    QAction *action = prepareButtonAndAction(button, icon, receiver, member);
    fileList->addAction(action);
}

QStringList FileListHelper::findPaths(const QList<QUrl> &urls)
{
    QStringList paths;

    for (QList<QUrl>::ConstIterator it = urls.constBegin(), total = urls.constEnd(); it != total; ++it) {
        QUrl url(*it);

        if (isValidFile(url)) {
            // If this Url uses a joker (i.e. : "/var/log/apache2/*")
            if (url.fileName().contains(QLatin1Char('*'))) {
                const QStringList foundPaths = expandJoker(url);
                logDebug() << "Found paths of " << url.path() << ":" << foundPaths;
                for (const QString &foundPath : foundPaths) {
                    paths.append(foundPath);
                }
            } else {
                paths.append(url.toLocalFile());
            }
        }
    }

    return paths;
}

bool FileListHelper::isValidFile(const QUrl &url)
{
    QString message;

    // If it is not valid
    if (!url.isValid()) {
        return false;
    }

    // If it is not a local file
    if (!url.isLocalFile()) {
        message = i18n("'%1' is not a local file.", url.path());
        KMessageBox::error(mParent, message, i18n("File selection failed"), KMessageBox::Notify);
        return false;
    }

    // If it's a directory, it's not valid
    if (QDir(url.toLocalFile()).exists()) {
        return false;
    }

    return true;
}

QList<QUrl> FileListHelper::openUrls() const
{
    QFileDialog fileDialog(mParent, QString(), QStringLiteral(DEFAULT_LOG_FOLDER), i18n("All Files (*)") + QLatin1String(";;") + i18n("Log Files (*.log)"));
    fileDialog.setWindowTitle(i18n("Choose Log File"));
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    fileDialog.exec();
    return fileDialog.selectedUrls();
}

QUrl FileListHelper::openUrl(const QString &originPath) const
{
    QFileDialog fileDialog(mParent, QString(), originPath, i18n("All Files (*)") + QLatin1String(";;") + i18n("Log Files (*.log)"));
    fileDialog.setWindowTitle(i18n("Choose Log File"));
    fileDialog.setFileMode(QFileDialog::AnyFile);

    fileDialog.exec();
    const QList<QUrl> urls = fileDialog.selectedUrls();
    if (!urls.isEmpty()) {
        return fileDialog.selectedUrls().at(0);
    } else {
        return QUrl();
    }
}

QStringList FileListHelper::expandJoker(const QUrl &url)
{
    const QFileInfo info(url.toLocalFile());

    logDebug() << "Dir " << info.dir().path();
    const QString filename = info.fileName();

    if (filename.isEmpty()) {
        return QStringList();
    }

    QStringList foundPaths;
    const QStringList files = info.dir().entryList(QStringList(filename), QDir::Files | QDir::NoSymLinks);
    foundPaths.reserve(files.count());
    for (const QString &file : files) {
        foundPaths.append(info.dir().absoluteFilePath(file));
    }

    return foundPaths;
}

void FileListHelper::setEnabledAction(QPushButton *button, bool enabled)
{
    button->setEnabled(enabled);

    const QList<QAction *> actions = button->actions();
    for (QAction *action : actions) {
        action->setEnabled(enabled);
    }
}
