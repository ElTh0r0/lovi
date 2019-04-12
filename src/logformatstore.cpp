/*
 * Copyright 2019 Aurélien Gâteau <mail@agateau.com>
 *
 * This file is part of Lovi.
 *
 * Lovi is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "logformatstore.h"

#include "logformat.h"
#include "logformatio.h"

#include <QDir>
#include <QFileInfo>

using std::unique_ptr;

LogFormatStore::LogFormatStore(const QString& dirPath, QObject* parent)
        : QObject(parent), mDirPath(dirPath) {
    load();
}

LogFormatStore::~LogFormatStore() {
}

LogFormat* LogFormatStore::byName(const QString& name) const {
    auto it = std::find(mLogFormatNames.begin(), mLogFormatNames.end(), name);
    if (it == mLogFormatNames.end()) {
        return {};
    }
    return at(it - mLogFormatNames.begin());
}

QString LogFormatStore::nameAt(int idx) const {
    return mLogFormatNames.at(idx);
}

LogFormat* LogFormatStore::at(int idx) const {
    LogFormat* logFormat = mLogFormats.at(idx).get();
    if (!logFormat) {
        auto path = QString("%1/%2.json").arg(mDirPath, mLogFormatNames.at(idx));
        unique_ptr<LogFormat> newLogFormat = LogFormatIO::loadFromPath(path);
        logFormat = newLogFormat.get();
        mLogFormats[idx] = std::move(newLogFormat);
    }
    return logFormat;
}

int LogFormatStore::count() const {
    return mLogFormatNames.size();
}

void LogFormatStore::load() {
    QDir dir(mDirPath);
    mLogFormatNames.clear();
    for (const QFileInfo& info : dir.entryInfoList({"*.json"}, QDir::Files, QDir::Name)) {
        mLogFormatNames.push_back(info.baseName());
    }
    mLogFormats.clear();
    mLogFormats.resize(mLogFormatNames.size());
}
