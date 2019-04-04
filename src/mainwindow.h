#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>

#include <memory>

class Config;
class LineProvider;
class LogFormatLoader;
class LogModel;

class QTreeView;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(Config* config, QWidget* parent = nullptr);
    ~MainWindow();

    void loadLogFormat(const QString& filePath);
    void loadLog(const QString& filePath);

private:
    void setupUi();
    void setupActions();
    void onRowsInserted();
    void onSelectionChanged();
    void reloadLogFormat();
    void showOpenLogDialog();
    void showLogFormatDialog();
    void copySelectedLines();
    void addLogToRecentFiles();

    Config* const mConfig;

    const std::unique_ptr<LogFormatLoader> mLogFormatLoader;
    std::unique_ptr<LineProvider> mLineProvider;
    std::unique_ptr<LogModel> mLogModel;

    QAction* const mOpenLogAction;
    QAction* const mSelectLogFormatAction;
    QAction* const mAutoScrollAction;
    QAction* const mCopyLinesAction;

    QToolBar* const mToolBar;
    QTreeView* const mTreeView;

    QString mLogPath;
    QString mLogFormatPath;
};

#endif // MAINWINDOW_H