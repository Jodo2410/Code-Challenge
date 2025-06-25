#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QTimer>
#include <QtWidgets/QMessageBox>
#include <QtCore/QProcess>
#include <QtCore/QStringList>

class ProcessMonitor : public QMainWindow
{
    Q_OBJECT

public:
    ProcessMonitor(QWidget *parent = nullptr);
    ~ProcessMonitor();

private slots:
    void refreshProcessList();
    void killSelectedProcess();
    void filterProcesses(const QString &text);
    void onProcessDoubleClicked();

private:
    void setupUI();
    void populateProcessList();
    QStringList getRunningProcesses();

    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *filterLayout;

    QLabel *titleLabel;
    QLineEdit *filterEdit;
    QListWidget *processListWidget;
    QPushButton *refreshButton;
    QPushButton *killButton;
    QPushButton *exitButton;

    QTimer *autoRefreshTimer;

    // Data
    QStringList allProcesses;
};

#endif // PROCESSMONITOR_H
