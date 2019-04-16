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
#ifndef CONFIG_H
#define CONFIG_H

#include <QHash>
#include <QObject>

class Config : public QObject {
    Q_OBJECT
public:
    explicit Config(const QString& configPath, QObject* parent = nullptr);

    QStringList recentLogFiles() const;
    void setRecentLogFiles(const QStringList& files);

    QHash<QString, QString> logFormatForFile() const;
    void setLogFormatForFile(const QString& file, const QString& format);

private:
    void load();
    void save() const;
    const QString mConfigPath;

    QStringList mRecentLogFiles;
    QHash<QString, QString> mLogFormatForFile;
};

#endif // CONFIG_H