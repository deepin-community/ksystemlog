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

#include "localLogFileReader.h"

#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMutex>

#include <KDirWatch>
#include <KLocalizedString>
#include <karchive_version.h>
#if KARCHIVE_VERSION >= QT_VERSION_CHECK(5, 85, 0)
#include <KCompressionDevice>
#else
#include <KFilterDev>
#endif

#include "logFileReaderPrivate.h"

#include "logging.h"

class LocalLogFileReaderPrivate : public LogFileReaderPrivate
{
public:
    KDirWatch *mWatch = nullptr;

    long mPreviousFilePosition;

    /**
     * Mutex avoiding multiple logFileModified() calls
     */
    QMutex mInsertionLocking;
};

LocalLogFileReader::LocalLogFileReader(const LogFile &logFile)
    : LogFileReader(*new LocalLogFileReaderPrivate(), logFile)
{
    init();
}

LocalLogFileReader::LocalLogFileReader(LocalLogFileReaderPrivate &dd, const LogFile &logFile)
    : LogFileReader(dd, logFile)
{
    init();
}

LocalLogFileReader::~LocalLogFileReader()
{
    Q_D(LocalLogFileReader);

    // Delete the watching object
    delete d->mWatch;

    // d pointer is deleted by the parent class
}

void LocalLogFileReader::init()
{
    Q_D(LocalLogFileReader);

    d->mWatch = new KDirWatch();
    connect(d->mWatch, &KDirWatch::dirty, this, &LocalLogFileReader::logFileModified);

    // Init current file position
    d->mPreviousFilePosition = 0;

    logDebug() << "Reading local file " << d->logFile.url().toLocalFile();
}

void LocalLogFileReader::watchFile(bool enable)
{
    Q_D(LocalLogFileReader);
    const QString filePath = d->logFile.url().toLocalFile();

    if (enable) {
        logDebug() << "Monitoring file : " << filePath;

        if (!d->mWatch->contains(filePath)) {
            d->mWatch->addFile(filePath);
        }

        // Reinit current file position
        d->mPreviousFilePosition = 0;

        // If we enable the watching, then we first try to see if new lines have appeared
        logFileModified();
    } else {
        d->mWatch->removeFile(filePath);
    }
}

QIODevice *LocalLogFileReader::open()
{
    Q_D(LocalLogFileReader);
    const QString filePath = d->logFile.url().toLocalFile();

    if (!d->logFile.url().isValid()) {
        const QString message(i18n("This file is not valid. Please adjust it in the settings of KSystemLog."));
        Q_EMIT errorOccured(i18n("File Does Not Exist"), message);
        Q_EMIT statusBarChanged(message);
    }

    QMimeDatabase db;
    const QString mimeType = db.mimeTypeForFile(filePath, QMimeDatabase::MatchContent).name();

    logDebug() << filePath << " : " << mimeType;
    QScopedPointer<QIODevice> inputDevice;

    // Try to see if this file exists
    const QFileInfo info(filePath);
    // If the file does not exist
    if (!info.exists()) {
        const QString message(i18n("The file '%1' does not exist.", filePath));
        Q_EMIT errorOccured(i18n("File Does Not Exist"), message);
        Q_EMIT statusBarChanged(message);
        return nullptr;
    }

    // Plain text file : we use a QFile object
    if (mimeType == QLatin1String("text/plain") || mimeType == QLatin1String("application/octet-stream")) {
        logDebug() << "Using QFile input device";

        inputDevice.reset(new QFile(filePath));
    }
    // Compressed file : we use the KFilterDev helper
    else {
        logDebug() << "Using KFilterDev input device";

        // inputDevice = KFilterDev::deviceForFile(filePath, mimeType);

#if KARCHIVE_VERSION >= QT_VERSION_CHECK(5, 85, 0)
        inputDevice.reset(new KCompressionDevice(filePath, KCompressionDevice::compressionTypeForMimeType(mimeType)));
#else
        inputDevice.reset(new KCompressionDevice(filePath, KFilterDev::compressionTypeForMimeType(mimeType)));
#endif

        if (!inputDevice) {
            const QString message(i18n("Unable to uncompress the '%2' format of '%1'.", filePath, mimeType));
            Q_EMIT errorOccured(i18n("Unable to Uncompress File"), message);
            Q_EMIT statusBarChanged(message);
            return nullptr;
        }
    }

    if (!inputDevice->open(QIODevice::ReadOnly)) {
        const QString message(i18n("You do not have sufficient permissions to read '%1'.", filePath));
        Q_EMIT errorOccured(i18n("Insufficient Permissions"), message);
        Q_EMIT statusBarChanged(message);
        return nullptr;
    }

    return inputDevice.take();
}

void LocalLogFileReader::close(QIODevice *inputDevice)
{
    inputDevice->close();
    delete inputDevice;
}

QStringList LocalLogFileReader::readContent(QIODevice *inputDevice)
{
    logDebug() << "Retrieving raw buffer...";

    Q_D(LocalLogFileReader);

    QStringList rawBuffer;

    QTextStream inputStream(inputDevice);
    while (!inputStream.atEnd()) {
        rawBuffer.append(inputStream.readLine());
    }

    logDebug() << "Raw buffer retrieved.";

    // Get the size file for the next calculation
    d->mPreviousFilePosition = inputDevice->size();
    logDebug() << "New file position : " << d->mPreviousFilePosition << " (" << d->logFile.url().toLocalFile() << ")";

    return rawBuffer;
}

void LocalLogFileReader::logFileModified()
{
    Q_D(LocalLogFileReader);

    logDebug() << "Locking log file modification...";
    if (!d->mInsertionLocking.tryLock()) {
        logDebug() << "Log file modification already detected.";
        return;
    }

    QIODevice *inputDevice = open();
    if (!inputDevice) {
        logCritical() << "Could not open file " << d->logFile.url().toLocalFile();
        return;
    }

    // If there are new lines in the file, insert only them or this is the first time we read this file
    if (d->mPreviousFilePosition != 0 && d->mPreviousFilePosition <= inputDevice->size()) {
        logDebug() << "Reading from position " << d->mPreviousFilePosition << " (" << d->logFile.url().toLocalFile() << ")";

        if (inputDevice->isSequential()) {
            logCritical() << "The file current position could not be modified";
        } else {
            // Place the cursor to the last line opened
            inputDevice->seek(d->mPreviousFilePosition);
        }

        logDebug() << "Retrieving a part of the file...";

        Q_EMIT contentChanged(this, Analyzer::UpdatingRead, readContent(inputDevice));
    }
    // Else reread all lines, clear log list
    else {
        logDebug() << "New file or file truncated. (Re-)Loading log file";

        Q_EMIT contentChanged(this, Analyzer::FullRead, readContent(inputDevice));
    }

    close(inputDevice);

    logDebug() << "Unlocking log file modification...";
    d->mInsertionLocking.unlock();
}
