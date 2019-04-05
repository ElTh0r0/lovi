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
#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QAbstractTableModel>
#include <QColor>

#include <memory>

class LogFormat;
class LineProvider;

struct LogCell {
    QString text;
    QColor bgColor;
    QColor fgColor;
};

struct LogLine {
    QColor bgColor;
    QColor fgColor;
    std::vector<LogCell> cells;

    bool isValid() const {
        return !cells.empty();
    }
};

class LogModel : public QAbstractTableModel {
public:
    LogModel(const LineProvider* lineProvider, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;

    int columnCount(const QModelIndex& parent = {}) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QVariant
    headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QStringList columns() const;

    void setLogFormat(const LogFormat* logFormat);

private:
    std::unique_ptr<LogFormat> mEmptyLogFormat;
    const LogFormat* mLogFormat = nullptr;
    const LineProvider* mLineProvider = nullptr;
    QStringList mColumns;
    mutable QHash<int, LogLine> mLogLineCache;

    LogLine processLine(const QString& line) const;

    void applyHighlights(LogLine* logLine, LogCell* logCell, int column) const;

    void onLineCountChanged(int newCount, int oldCount);
};

#endif // LOGMODEL_H
