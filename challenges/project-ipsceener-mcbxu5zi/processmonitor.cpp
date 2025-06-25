#include "ProcessMonitor.h"
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QStatusBar>
#include <QRegularExpression>

ProcessMonitor::ProcessMonitor(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();

    // Auto-refresh timer (every 5 seconds)
    autoRefreshTimer = new QTimer(this);
    connect(autoRefreshTimer, &QTimer::timeout, this, &ProcessMonitor::refreshProcessList);
    autoRefreshTimer->start(5000);

    // Initial population
    refreshProcessList();
}

ProcessMonitor::~ProcessMonitor()
{
    if (autoRefreshTimer) {
        autoRefreshTimer->stop();
    }
}

void ProcessMonitor::setupUI()
{
    // Central widget setup
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    mainLayout = new QVBoxLayout(centralWidget);

    // Title
    titleLabel = new QLabel("Prozess Monitor", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Filter layout
    filterLayout = new QHBoxLayout();
    QLabel *filterLabel = new QLabel("Filter:", this);
    filterEdit = new QLineEdit(this);
    filterEdit->setPlaceholderText("Prozessname eingeben...");
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterEdit);

    // Process list
    processListWidget = new QListWidget(this);
    processListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Button layout
    buttonLayout = new QHBoxLayout();
    refreshButton = new QPushButton("Aktualisieren", this);
    killButton = new QPushButton("Prozess beenden", this);
    exitButton = new QPushButton("Beenden", this);

    killButton->setStyleSheet("background-color: #ff6b6b; color: white;");
    refreshButton->setStyleSheet("background-color: #4ecdc4; color: white;");

    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(killButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(exitButton);

    // Add to main layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(processListWidget);
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(refreshButton, &QPushButton::clicked, this, &ProcessMonitor::refreshProcessList);
    connect(killButton, &QPushButton::clicked, this, &ProcessMonitor::killSelectedProcess);
    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
    connect(filterEdit, &QLineEdit::textChanged, this, &ProcessMonitor::filterProcesses);
    connect(processListWidget, &QListWidget::itemDoubleClicked, this, &ProcessMonitor::onProcessDoubleClicked);

    // Window properties
    setWindowTitle("Qt Prozess Monitor");
    setMinimumSize(600, 400);
    resize(800, 600);
}

void ProcessMonitor::refreshProcessList()
{
    allProcesses = getRunningProcesses();
    populateProcessList();
}

void ProcessMonitor::populateProcessList()
{
    processListWidget->clear();

    QString filterText = filterEdit->text().toLower();

    for (const QString &process : allProcesses) {
        if (filterText.isEmpty() || process.toLower().contains(filterText)) {
            processListWidget->addItem(process);
        }
    }

    // Update status
    QString statusText = QString("Prozesse gefunden: %1").arg(processListWidget->count());
    if (!filterEdit->text().isEmpty()) {
        statusText += QString(" (gefiltert von %1)").arg(allProcesses.size());
    }

    // Create status bar if it doesn't exist
    if (!statusBar()) {
        setStatusBar(new QStatusBar(this));
    }
    statusBar()->showMessage(statusText);
}

QStringList ProcessMonitor::getRunningProcesses()
{
    QStringList processes;

#ifdef Q_OS_WIN
    // Windows: Use tasklist command
    QProcess process;
    process.start("tasklist", QStringList() << "/fo" << "csv" << "/nh");
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');

    for (const QString &line : lines) {
        if (!line.isEmpty()) {
            QStringList parts = line.split(',');
            if (parts.size() >= 2) {
                QString processName = parts[0].remove('"');
                QString pid = parts[1].remove('"');
                processes.append(QString("%1 (PID: %2)").arg(processName, pid));
            }
        }
    }
#else
    // Linux/Unix: Use ps command
    QProcess process;
    process.start("ps", QStringList() << "aux");
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');

    for (int i = 1; i < lines.size(); ++i) { // Skip header
        QString line = lines[i].simplified();
        if (!line.isEmpty()) {
            QStringList parts = line.split(' ');
            if (parts.size() >= 11) {
                QString pid = parts[1];
                QString command = parts.mid(10).join(' ');
                processes.append(QString("%1 (PID: %2)").arg(command, pid));
            }
        }
    }
#endif

    return processes;
}

void ProcessMonitor::killSelectedProcess()
{
    QListWidgetItem *currentItem = processListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Warnung", "Bitte wählen Sie einen Prozess aus!");
        return;
    }

    QString processText = currentItem->text();

    // Extract PID from the process text
    QRegularExpression pidRegex("PID: (\\d+)");
    QRegularExpressionMatch match = pidRegex.match(processText);
    if (!match.hasMatch()) {
        QMessageBox::warning(this, "Fehler", "Konnte PID nicht extrahieren!");
        return;
    }

    QString pid = match.captured(1);

    // Confirmation dialog
    int ret = QMessageBox::question(this, "Prozess beenden",
                                    QString("Möchten Sie den Prozess mit PID %1 wirklich beenden?").arg(pid),
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        QProcess killProcess;

#ifdef Q_OS_WIN
        killProcess.start("taskkill", QStringList() << "/PID" << pid << "/F");
#else
        killProcess.start("kill", QStringList() << "-9" << pid);
#endif

        killProcess.waitForFinished();

        if (killProcess.exitCode() == 0) {
            QMessageBox::information(this, "Erfolg", "Prozess wurde beendet!");
            refreshProcessList();
        } else {
            QMessageBox::warning(this, "Fehler", "Prozess konnte nicht beendet werden!");
        }
    }
}

void ProcessMonitor::filterProcesses(const QString &text)
{
    Q_UNUSED(text)
    populateProcessList();
}

void ProcessMonitor::onProcessDoubleClicked()
{
    QListWidgetItem *currentItem = processListWidget->currentItem();
    if (currentItem) {
        QMessageBox::information(this, "Prozess Details",
                                 QString("Prozess: %1").arg(currentItem->text()));
    }
}
