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
#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QLocale>
#include <QStandardPaths>
#include <QTranslator>
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#include <QStyleHints>
#endif
#include <memory>

#include "BuildConfig.h"
#include "Config.h"
#include "LogFormatStore.h"
#include "MainWindow.h"
#include "Resources.h"

using std::unique_ptr;

unique_ptr<QCommandLineParser> createParser() {
    unique_ptr<QCommandLineParser> parser = std::make_unique<QCommandLineParser>();
    parser->setApplicationDescription(QCoreApplication::translate("main", "Log viewer"));
    parser->addHelpOption();
    parser->addVersionOption();
    parser->addPositionalArgument("log_file", QCoreApplication::translate("main", "Log file."));
    parser->addOption({
        {"f", "format"},
        QCoreApplication::translate("main", "Log format definition."),
        QCoreApplication::translate("main", "log_format_name"),
    });
    return parser;
}

static QString configPath() {
#ifdef USE_PORTABLE_CONFIG
    return QCoreApplication::applicationDirPath() + "/config.json";
#endif

    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if (!QDir(configDir).mkpath(".")) {
        qWarning() << "Could not create dir" << configDir;
    }
    return configDir + "/config.json";
}

static QString logFormatsDirPath() {
#ifdef USE_PORTABLE_CONFIG
    return QCoreApplication::applicationDirPath() + "/logformats";
#endif

    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/logformats";
}

static void loadTranslations(QObject* parent) {
    // Search in current path first, to give translators an easy way to test
    // their translations
    QStringList searchDirs = {QDir::currentPath(), Resources::findDir("translations")};
    auto translator = new QTranslator(parent);
    QLocale locale;
    for (const auto& dir : searchDirs) {
        if (translator->load(locale, APP_NAME, "_", dir)) {
            QCoreApplication::installTranslator(translator);
            return;
        }
    }
}

/**
 * Initialize QIcon so that QIcon::fromTheme() finds our icons on Windows and macOS
 */
static void initFallbackIcons(QMainWindow* win) {
#if defined(Q_OS_WINDOWS) || defined(Q_OS_MACOS)
    QString sIconTheme;
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    if (Qt::ColorScheme::Dark == QGuiApplication::styleHints()->colorScheme()) {
        sIconTheme = QStringLiteral("dark");
    } else if (Qt::ColorScheme::Light == QGuiApplication::styleHints()->colorScheme()) {
        sIconTheme = QStringLiteral("light");
    }
#endif
    // If < Qt 6.5 or if Qt::ColorScheme::Unknown was returned
    if (sIconTheme.isEmpty()) {
        // If window is darker than text
        if (win->window()->palette().window().color().lightnessF()
            < win->window()->palette().windowText().color().lightnessF()) {
            sIconTheme = QStringLiteral("dark");
        } else {
            sIconTheme = QStringLiteral("light");
        }
    }
    QIcon::setThemeName(sIconTheme);
#endif
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(lovi);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    auto iconName = QString(":/appicon/sc-apps-%1.svg").arg(APP_NAME);
    app.setWindowIcon(QIcon(iconName));

    loadTranslations(&app);

    unique_ptr<QCommandLineParser> parser = createParser();
    parser->process(app);

    Config config(configPath());
    LogFormatStore store(logFormatsDirPath());
    MainWindow window(&config, &store);
    initFallbackIcons(&window);
    if (parser->isSet("format")) {
        QString formatName = parser->value("format");
        LogFormat* logFormat = store.byName(formatName);
        if (logFormat) {
            window.setLogFormat(logFormat);
        }
    }
    if (parser->positionalArguments().length() == 1) {
        window.loadLog(parser->positionalArguments().first());
    }
    window.show();

    return app.exec();
}
