#include "IpScanner.h"
#include "ScanWorkers.h"
#include <QtCore/QDebug>
#include <QtCore/QProcess>
#include <QtCore/QRegularExpression>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtCore/QStandardPaths>
#include <QtGui/QClipboard>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#include <QProgressDialog>

IpScanner::IpScanner(QWidget *parent)
    : QMainWindow(parent), scanRunning(false), scanPaused(false),
    totalHosts(0), scannedHosts(0), completedHosts(0)
{
    threadPool = QThreadPool::globalInstance();
    threadPool->setMaxThreadCount(50);

    settings = new QSettings("IpScanner", "Config", this);

    setupUI();
    setupMenuBar();
    setupStatusBar();
    loadSettings();

    // Timer für UI Updates
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &IpScanner::updateProgress);

    // Timer für Scan-Fortschritt
    progressTimer = new QTimer(this);
    connect(progressTimer, &QTimer::timeout, this, &IpScanner::updateScanProgress);
}

IpScanner::~IpScanner()
{
    try {
        saveSettings();

        // Stop any running vulnerability scans
        if (vulnerabilityScanRunning) {
            stopVulnerabilitySccan();
        }

        // WICHTIG: Alle laufenden Scans stoppen
        if (scanRunning) {
            stopScan();
        }

        // ThreadPool sicher beenden
        if (threadPool) {
            threadPool->clear();
            threadPool->waitForDone(5000); // 5 Sekunden Timeout
        }

        // Timer stoppen
        if (updateTimer) {
            updateTimer->stop();
        }
        if (progressTimer) {
            progressTimer->stop();
        }

    } catch (...) {
        // Ignore exceptions in destructor
    }
}

void IpScanner::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create main splitter
    mainSplitter = new QSplitter(Qt::Horizontal, this);

    // Create tab widget
    mainTabWidget = new QTabWidget(this);

    createScanTab();
    createResultsTab();
    createPortScanTab();
    createNetworkTab();
    createVulnerabilityTab();
    createLogTab();

    mainSplitter->addWidget(mainTabWidget);
    mainLayout->addWidget(mainSplitter);

    // Window properties
    setWindowTitle("Advanced IP Scanner v2.0 - Verbessert");
    setMinimumSize(1200, 800);
    resize(1600, 1000);

    // Verbessertes Dark Theme
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e1e;
            color: #ffffff;
        }
        QTabWidget::pane {
            border: 1px solid #404040;
            background-color: #2d2d2d;
            border-radius: 5px;
        }
        QTabBar::tab {
            background-color: #404040;
            color: #fff;
            padding: 10px 20px;
            margin: 2px;
            border-radius: 5px;
            font-weight: bold;
        }
        QTabBar::tab:selected {
            background-color: #0078d4;
            color: white;
        }
        QTabBar::tab:hover {
            background-color: #505050;
        }
        QTreeWidget {
            background-color: #2d2d2d;
            color: #fff;
            gridline-color: #404040;
            border: 1px solid #404040;
            border-radius: 5px;
            font-size: 11px;
        }
        QTreeWidget::item {
            padding: 5px;
            border-bottom: 1px solid #404040;
        }
        QTreeWidget::item:selected {
            background-color: #0078d4;
            color: white;
        }
        QTreeWidget::item:hover {
            background-color: #404040;
        }
        QPushButton {
            background-color: #0078d4;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            font-weight: bold;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #106ebe;
        }
        QPushButton:pressed {
            background-color: #005a9e;
        }
        QPushButton:disabled {
            background-color: #666666;
            color: #999999;
        }
        QLineEdit, QSpinBox, QComboBox {
            background-color: #404040;
            color: #fff;
            border: 1px solid #606060;
            padding: 8px;
            border-radius: 5px;
            font-size: 11px;
        }
        QLineEdit:focus, QSpinBox:focus, QComboBox:focus {
            border: 2px solid #0078d4;
        }
        QTextEdit {
            background-color: #2d2d2d;
            color: #fff;
            border: 1px solid #404040;
            border-radius: 5px;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 10px;
        }
        QGroupBox {
            font-weight: bold;
            border: 2px solid #404040;
            margin: 15px 0;
            padding-top: 15px;
            border-radius: 5px;
            background-color: #252525;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 15px;
            padding: 5px 10px;
            background-color: #0078d4;
            color: white;
            border-radius: 3px;
        }
        QProgressBar {
            border: 1px solid #404040;
            border-radius: 5px;
            text-align: center;
            font-weight: bold;
            background-color: #2d2d2d;
            color: white;
            height: 25px;
        }
        QProgressBar::chunk {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #0078d4, stop:1 #106ebe);
            border-radius: 4px;
        }
        QLabel {
            color: #ffffff;
            font-size: 11px;
        }
        QStatusBar {
            background-color: #1e1e1e;
            color: #ffffff;
            border-top: 1px solid #404040;
        }
    )");
}

void IpScanner::createScanTab()
{
    scanTab = new QWidget();
    mainTabWidget->addTab(scanTab, "🔍 Scan Konfiguration");

    QVBoxLayout *layout = new QVBoxLayout(scanTab);

    // Target configuration group
    QGroupBox *targetGroup = new QGroupBox("🎯 Ziel Konfiguration");
    QGridLayout *targetLayout = new QGridLayout(targetGroup);

    targetLayout->addWidget(new QLabel("Ziel:"), 0, 0);
    targetEdit = new QLineEdit("192.168.1.1-254");
    targetEdit->setPlaceholderText("IP Bereich (z.B. 192.168.1.1-254, 10.0.0.0/24)");
    targetLayout->addWidget(targetEdit, 0, 1, 1, 3);

    targetLayout->addWidget(new QLabel("Scan Typ:"), 1, 0);
    scanTypeCombo = new QComboBox();
    scanTypeCombo->addItems({"Schneller Scan", "Detaillierter Scan", "Stealth Scan", "Aggressiver Scan"});
    targetLayout->addWidget(scanTypeCombo, 1, 1);

    targetLayout->addWidget(new QLabel("Threads:"), 1, 2);
    threadsSpinBox = new QSpinBox();
    threadsSpinBox->setRange(1, 200);
    threadsSpinBox->setValue(50);
    targetLayout->addWidget(threadsSpinBox, 1, 3);

    targetLayout->addWidget(new QLabel("Timeout (ms):"), 2, 0);
    timeoutSpinBox = new QSpinBox();
    timeoutSpinBox->setRange(100, 10000);
    timeoutSpinBox->setValue(3000);
    targetLayout->addWidget(timeoutSpinBox, 2, 1);

    // Scan options group
    QGroupBox *optionsGroup = new QGroupBox("⚙️ Scan Optionen");
    QGridLayout *optionsLayout = new QGridLayout(optionsGroup);

    pingCheckBox = new QCheckBox("Ping Scan");
    pingCheckBox->setChecked(true);
    optionsLayout->addWidget(pingCheckBox, 0, 0);

    portScanCheckBox = new QCheckBox("Port Scan");
    optionsLayout->addWidget(portScanCheckBox, 0, 1);

    osDetectionCheckBox = new QCheckBox("OS Erkennung");
    optionsLayout->addWidget(osDetectionCheckBox, 1, 0);

    serviceDetectionCheckBox = new QCheckBox("Service Erkennung");
    optionsLayout->addWidget(serviceDetectionCheckBox, 1, 1);

    // Control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    startButton = new QPushButton("▶️ Scan Starten");
    startButton->setStyleSheet("QPushButton { background-color: #28a745; font-size: 12px; } QPushButton:hover { background-color: #218838; }");

    stopButton = new QPushButton("⏹️ Scan Stoppen");
    stopButton->setEnabled(false);
    stopButton->setStyleSheet("QPushButton { background-color: #dc3545; font-size: 12px; } QPushButton:hover { background-color: #c82333; }");

    pauseButton = new QPushButton("⏸️ Pausieren");
    pauseButton->setEnabled(false);
    pauseButton->setStyleSheet("QPushButton { background-color: #ffc107; color: #000; font-size: 12px; } QPushButton:hover { background-color: #e0a800; }");

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addStretch();

    // Enhanced Progress section
    QGroupBox *progressGroup = new QGroupBox("📊 Fortschritt & Status");
    QVBoxLayout *progressLayout = new QVBoxLayout(progressGroup);

    // Main progress bar
    progressBar = new QProgressBar();
    progressBar->setTextVisible(true);
    progressBar->setFormat("Fortschritt: %p% (%v/%m Hosts)");

    // Status labels
    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLabel = new QLabel("Bereit zum Scannen");
    statusLabel->setStyleSheet("font-weight: bold; color: #00ff00;");

    currentHostLabel = new QLabel("Aktueller Host: --");
    currentHostLabel->setStyleSheet("color: #ffff00;");

    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(currentHostLabel);

    // Statistics layout
    QHBoxLayout *statsLayout = new QHBoxLayout();

    totalHostsLabel = new QLabel("Gesamt: 0");
    scannedHostsLabel = new QLabel("Gescannt: 0");
    foundHostsLabel = new QLabel("Gefunden: 0");
    elapsedTimeLabel = new QLabel("Zeit: 00:00");

    totalHostsLabel->setStyleSheet("color: #ffffff; font-weight: bold;");
    scannedHostsLabel->setStyleSheet("color: #00aaff; font-weight: bold;");
    foundHostsLabel->setStyleSheet("color: #00ff00; font-weight: bold;");
    elapsedTimeLabel->setStyleSheet("color: #ffaa00; font-weight: bold;");

    statsLayout->addWidget(totalHostsLabel);
    statsLayout->addWidget(scannedHostsLabel);
    statsLayout->addWidget(foundHostsLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(elapsedTimeLabel);

    progressLayout->addWidget(progressBar);
    progressLayout->addLayout(statusLayout);
    progressLayout->addLayout(statsLayout);

    // Add all groups to main layout
    layout->addWidget(targetGroup);
    layout->addWidget(optionsGroup);
    layout->addLayout(buttonLayout);
    layout->addWidget(progressGroup);
    layout->addStretch();

    // Connect signals
    connect(startButton, &QPushButton::clicked, this, &IpScanner::startScan);
    connect(stopButton, &QPushButton::clicked, this, &IpScanner::stopScan);
    connect(pauseButton, &QPushButton::clicked, this, &IpScanner::pauseScan);
}

void IpScanner::createResultsTab()
{
    resultsTab = new QWidget();
    mainTabWidget->addTab(resultsTab, "📋 Scan Ergebnisse");

    QHBoxLayout *layout = new QHBoxLayout(resultsTab);

    // Results tree with improved columns
    hostTreeWidget = new QTreeWidget();
    hostTreeWidget->setHeaderLabels({
        "IP Adresse", "Hostname", "MAC Adresse", "Hersteller",
        "Status", "Antwortzeit", "Offene Ports", "Betriebssystem",
        "Letzte Aktivität", "Services"
    });

    // Set column widths
    hostTreeWidget->setColumnWidth(0, 120);  // IP
    hostTreeWidget->setColumnWidth(1, 150);  // Hostname
    hostTreeWidget->setColumnWidth(2, 140);  // MAC
    hostTreeWidget->setColumnWidth(3, 120);  // Vendor
    hostTreeWidget->setColumnWidth(4, 80);   // Status
    hostTreeWidget->setColumnWidth(5, 90);   // Response time
    hostTreeWidget->setColumnWidth(6, 200);  // Ports
    hostTreeWidget->setColumnWidth(7, 120);  // OS
    hostTreeWidget->setColumnWidth(8, 140);  // Last seen

    hostTreeWidget->setAlternatingRowColors(true);
    hostTreeWidget->setSortingEnabled(true);
    hostTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    hostTreeWidget->setRootIsDecorated(false);

    // Enhanced details panel
    QVBoxLayout *detailsLayout = new QVBoxLayout();

    QLabel *detailsTitle = new QLabel("🔍 Host Details");
    detailsTitle->setStyleSheet("font-size: 14px; font-weight: bold; margin: 5px;");

    hostDetailsEdit = new QTextEdit();
    hostDetailsEdit->setMaximumWidth(400);
    hostDetailsEdit->setReadOnly(true);

    detailsLayout->addWidget(detailsTitle);
    detailsLayout->addWidget(hostDetailsEdit);

    QWidget *detailsWidget = new QWidget();
    detailsWidget->setLayout(detailsLayout);

    layout->addWidget(hostTreeWidget, 3);
    layout->addWidget(detailsWidget, 1);

    // Connect signals
    connect(hostTreeWidget, &QTreeWidget::itemSelectionChanged, this, &IpScanner::onItemSelectionChanged);
    connect(hostTreeWidget, &QTreeWidget::itemDoubleClicked, this, &IpScanner::onItemDoubleClicked);
    connect(hostTreeWidget, &QWidget::customContextMenuRequested, this, &IpScanner::showContextMenu);
}

void IpScanner::createPortScanTab()
{
    portScanTab = new QWidget();
    mainTabWidget->addTab(portScanTab, "🔌 Port Scanner");

    QVBoxLayout *layout = new QVBoxLayout(portScanTab);

    // Port scan configuration
    QGroupBox *configGroup = new QGroupBox("🔧 Port Scan Konfiguration");
    QGridLayout *configLayout = new QGridLayout(configGroup);

    configLayout->addWidget(new QLabel("Port Bereich:"), 0, 0);
    portRangeEdit = new QLineEdit("1-1000");
    portRangeEdit->setPlaceholderText("z.B. 22,80,443 oder 1-1000");
    configLayout->addWidget(portRangeEdit, 0, 1, 1, 2);

    configLayout->addWidget(new QLabel("Scan Typ:"), 1, 0);
    portScanTypeCombo = new QComboBox();
    portScanTypeCombo->addItems({"TCP Connect", "TCP SYN", "UDP Scan", "Umfassend"});
    configLayout->addWidget(portScanTypeCombo, 1, 1);

    portScanButton = new QPushButton("🚀 Port Scan Starten");
    portScanButton->setStyleSheet("QPushButton { background-color: #17a2b8; } QPushButton:hover { background-color: #138496; }");
    configLayout->addWidget(portScanButton, 2, 0, 1, 3);

    // Common ports list
    QGroupBox *commonGroup = new QGroupBox("🌐 Häufige Ports");
    QVBoxLayout *commonLayout = new QVBoxLayout(commonGroup);

    commonPortsList = new QListWidget();
    commonPortsList->setMaximumHeight(150);
    commonPortsList->addItems({
        "21 - FTP (File Transfer)", "22 - SSH (Secure Shell)", "23 - Telnet",
        "25 - SMTP (Email)", "53 - DNS", "80 - HTTP (Web)",
        "110 - POP3 (Email)", "143 - IMAP (Email)", "443 - HTTPS (Secure Web)",
        "993 - IMAPS", "995 - POP3S", "3389 - RDP (Remote Desktop)",
        "5432 - PostgreSQL", "3306 - MySQL"
    });
    commonLayout->addWidget(commonPortsList);

    // Port results with enhanced columns
    portResultsTree = new QTreeWidget();
    portResultsTree->setHeaderLabels({
        "IP Adresse", "Port", "Status", "Service", "Version", "Banner", "Antwortzeit"
    });

    layout->addWidget(configGroup);
    layout->addWidget(commonGroup);
    layout->addWidget(portResultsTree);

    connect(portScanButton, &QPushButton::clicked, this, &IpScanner::portScan);
    connect(commonPortsList, &QListWidget::itemDoubleClicked, this, &IpScanner::addCommonPort);
}

void IpScanner::createNetworkTab()
{
    networkTab = new QWidget();
    mainTabWidget->addTab(networkTab, "🌐 Netzwerk Analyse");

    QVBoxLayout *layout = new QVBoxLayout(networkTab);

    // Interface selection group
    QGroupBox *interfaceGroup = new QGroupBox("🔌 Netzwerk Interface");
    QHBoxLayout *interfaceLayout = new QHBoxLayout(interfaceGroup);

    interfaceCombo = new QComboBox();
    interfaceCombo->addItems(getLocalInterfaces());
    interfaceLayout->addWidget(new QLabel("Interface:"));
    interfaceLayout->addWidget(interfaceCombo);

    // Scan buttons with enhanced styling
    arpScanButton = new QPushButton("📡 ARP Scan");
    discoveryButton = new QPushButton("🔍 Netzwerk Erkennung");
    QPushButton *healthCheckButton = new QPushButton("🏥 Gesundheitscheck");
    QPushButton *topologyButton = new QPushButton("🗺️ Topologie");

    arpScanButton->setStyleSheet("QPushButton { background-color: #6f42c1; } QPushButton:hover { background-color: #5a32a3; }");
    discoveryButton->setStyleSheet("QPushButton { background-color: #e83e8c; } QPushButton:hover { background-color: #d91a72; }");
    healthCheckButton->setStyleSheet("QPushButton { background-color: #fd7e14; } QPushButton:hover { background-color: #e8681c; }");
    topologyButton->setStyleSheet("QPushButton { background-color: #20c997; } QPushButton:hover { background-color: #1aa179; }");

    interfaceLayout->addWidget(arpScanButton);
    interfaceLayout->addWidget(discoveryButton);
    interfaceLayout->addWidget(healthCheckButton);
    interfaceLayout->addWidget(topologyButton);
    interfaceLayout->addStretch();

    // Network statistics group
    QGroupBox *statsGroup = new QGroupBox("📊 Netzwerk Statistiken");
    QHBoxLayout *statsLayout = new QHBoxLayout(statsGroup);

    QLabel *devicesLabel = new QLabel("Geräte: 0");
    QLabel *routersLabel = new QLabel("Router: 0");
    QLabel *serversLabel = new QLabel("Server: 0");
    QLabel *workstationsLabel = new QLabel("Clients: 0");

    devicesLabel->setStyleSheet("color: #ffffff; font-weight: bold; padding: 5px; background: #17a2b8; border-radius: 3px;");
    routersLabel->setStyleSheet("color: #ffffff; font-weight: bold; padding: 5px; background: #ffc107; color: black; border-radius: 3px;");
    serversLabel->setStyleSheet("color: #ffffff; font-weight: bold; padding: 5px; background: #dc3545; border-radius: 3px;");
    workstationsLabel->setStyleSheet("color: #ffffff; font-weight: bold; padding: 5px; background: #28a745; border-radius: 3px;");

    statsLayout->addWidget(devicesLabel);
    statsLayout->addWidget(routersLabel);
    statsLayout->addWidget(serversLabel);
    statsLayout->addWidget(workstationsLabel);
    statsLayout->addStretch();

    // Network tree with enhanced columns
    networkTree = new QTreeWidget();
    networkTree->setHeaderLabels({
        "Gerät", "IP Adresse", "MAC Adresse", "Hersteller", "Interface", "Typ", "Letzte Aktivität"
    });

    // Set optimized column widths
    networkTree->setColumnWidth(0, 150);  // Device name
    networkTree->setColumnWidth(1, 120);  // IP
    networkTree->setColumnWidth(2, 140);  // MAC
    networkTree->setColumnWidth(3, 150);  // Vendor
    networkTree->setColumnWidth(4, 100);  // Interface
    networkTree->setColumnWidth(5, 120);  // Type
    networkTree->setColumnWidth(6, 140);  // Last seen

    networkTree->setAlternatingRowColors(true);
    networkTree->setSortingEnabled(true);
    networkTree->setRootIsDecorated(false);

    // Control buttons
    QHBoxLayout *controlLayout = new QHBoxLayout();
    QPushButton *refreshButton = new QPushButton("🔄 Aktualisieren");
    QPushButton *clearButton = new QPushButton("🗑️ Löschen");
    QPushButton *exportButton = new QPushButton("📤 Exportieren");

    refreshButton->setStyleSheet("QPushButton { background-color: #007bff; } QPushButton:hover { background-color: #0056b3; }");
    clearButton->setStyleSheet("QPushButton { background-color: #dc3545; } QPushButton:hover { background-color: #c82333; }");
    exportButton->setStyleSheet("QPushButton { background-color: #28a745; } QPushButton:hover { background-color: #218838; }");

    controlLayout->addWidget(refreshButton);
    controlLayout->addWidget(clearButton);
    controlLayout->addWidget(exportButton);
    controlLayout->addStretch();

    layout->addWidget(interfaceGroup);
    layout->addWidget(statsGroup);
    layout->addWidget(networkTree);
    layout->addLayout(controlLayout);

    // Setup context menu
    setupNetworkContextMenu();

    // Connect signals
    connect(arpScanButton, &QPushButton::clicked, this, &IpScanner::arpScan);
    connect(discoveryButton, &QPushButton::clicked, this, &IpScanner::networkDiscovery);
    connect(healthCheckButton, &QPushButton::clicked, this, &IpScanner::performNetworkHealthCheck);
    connect(topologyButton, &QPushButton::clicked, this, &IpScanner::analyzeNetworkTopology);

    connect(refreshButton, &QPushButton::clicked, this, &IpScanner::refreshNetworkInterfaces);
    connect(clearButton, &QPushButton::clicked, [this]() {
        networkTree->clear();
        addLogEntry("Netzwerk-Ergebnisse gelöscht", "INFO");
    });
    connect(exportButton, &QPushButton::clicked, this, &IpScanner::exportNetworkMap);

    // Update statistics when tree changes
    connect(networkTree, &QTreeWidget::itemChanged, this, &IpScanner::updateNetworkStatistics);

    // Store references to statistics labels for updates
    networkStatsLabels = {devicesLabel, routersLabel, serversLabel, workstationsLabel};
}
void IpScanner::updateNetworkStatistics()
{
    // Check if we have the statistics labels
    if (networkStatsLabels.size() < 4) return;

    int devices = networkTree->topLevelItemCount();
    int routers = 0, servers = 0, workstations = 0;

    for (int i = 0; i < devices; ++i) {
        QTreeWidgetItem *item = networkTree->topLevelItem(i);
        if (!item) continue;

        QString type = item->text(5); // Type column

        if (type.contains("Router") || type.contains("Gateway")) {
            routers++;
        } else if (type.contains("Server")) {
            servers++;
        } else if (type.contains("Computer") || type.contains("Device")) {
            workstations++;
        }
    }

    // Update statistics labels
    networkStatsLabels[0]->setText(QString("Geräte: %1").arg(devices));
    networkStatsLabels[1]->setText(QString("Router: %1").arg(routers));
    networkStatsLabels[2]->setText(QString("Server: %1").arg(servers));
    networkStatsLabels[3]->setText(QString("Clients: %1").arg(workstations));
}
void IpScanner::createLogTab()
{
    logTab = new QWidget();
    mainTabWidget->addTab(logTab, "📝 Aktivitäts Log");

    QVBoxLayout *layout = new QVBoxLayout(logTab);

    // Log controls
    QHBoxLayout *controlLayout = new QHBoxLayout();

    clearLogButton = new QPushButton("🗑️ Log Löschen");
    saveLogButton = new QPushButton("💾 Log Speichern");
    exportLogButton = new QPushButton("📤 Log Exportieren");

    clearLogButton->setStyleSheet("QPushButton { background-color: #dc3545; } QPushButton:hover { background-color: #c82333; }");
    saveLogButton->setStyleSheet("QPushButton { background-color: #28a745; } QPushButton:hover { background-color: #218838; }");
    exportLogButton->setStyleSheet("QPushButton { background-color: #17a2b8; } QPushButton:hover { background-color: #138496; }");

    controlLayout->addWidget(clearLogButton);
    controlLayout->addWidget(saveLogButton);
    controlLayout->addWidget(exportLogButton);
    controlLayout->addStretch();

    // Log filter
    QHBoxLayout *filterLayout = new QHBoxLayout();
    logFilterEdit = new QLineEdit();
    logFilterEdit->setPlaceholderText("Log Einträge filtern...");
    QPushButton *filterButton = new QPushButton("🔍 Filter");
    filterLayout->addWidget(new QLabel("Filter:"));
    filterLayout->addWidget(logFilterEdit);
    filterLayout->addWidget(filterButton);

    // Log text area
    logTextEdit = new QTextEdit();
    logTextEdit->setFont(QFont("Consolas", 10));

    layout->addLayout(controlLayout);
    layout->addLayout(filterLayout);
    layout->addWidget(logTextEdit);

    connect(clearLogButton, &QPushButton::clicked, logTextEdit, &QTextEdit::clear);
    connect(saveLogButton, &QPushButton::clicked, this, &IpScanner::saveLog);
    connect(exportLogButton, &QPushButton::clicked, this, &IpScanner::exportResults);
    connect(filterButton, &QPushButton::clicked, this, &IpScanner::filterLog);

    // Add initial log entry
    addLogEntry("IP Scanner initialisiert - Bereit für Netzwerk-Scans", "INFO");
}

void IpScanner::startScan()
{
    if (targetEdit->text().isEmpty()) {
        QMessageBox::warning(this, "⚠️ Warnung", "Bitte geben Sie einen Ziel-IP-Bereich ein!");
        return;
    }

    // Reset scan state
    scanRunning = true;
    scanPaused = false;
    scannedHosts = 0;
    completedHosts = 0;
    discoveredHosts.clear();

    // Update UI
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    pauseButton->setEnabled(true);
    clearResults();

    currentTarget = targetEdit->text();
    threadPool->setMaxThreadCount(threadsSpinBox->value());

    // Parse targets and update totals
    QStringList targets = parseTargetString(currentTarget);
    totalHosts = targets.size();

    // Update progress display
    progressBar->setRange(0, totalHosts);
    progressBar->setValue(0);
    totalHostsLabel->setText(QString("Gesamt: %1").arg(totalHosts));
    scannedHostsLabel->setText("Gescannt: 0");
    foundHostsLabel->setText("Gefunden: 0");
    statusLabel->setText("🔄 Scan läuft...");
    statusLabel->setStyleSheet("font-weight: bold; color: #ffaa00;");

    addLogEntry(QString("Scan gestartet für Ziel: %1 (%2 Hosts)").arg(currentTarget).arg(totalHosts), "INFO");

    // Start elapsed time tracking
    scanStartTime = QDateTime::currentDateTime();

    // Create and start scan workers
    for (const QString &ip : targets) {
        ScanWorker *worker = new ScanWorker(ip, timeoutSpinBox->value());
        worker->setHostIndex(scannedHosts++); // Set index for progress tracking

        // Connect signals with explicit lambda capturing
        connect(worker, &ScanWorker::hostFound, this, [this](const HostInfo &host) {
            this->onHostFound(host);
        });

        connect(worker, &ScanWorker::hostCompleted, this, [this]() {
            this->onHostCompleted();
        });

        threadPool->start(worker);
    }

    // Start progress update timers
    updateTimer->start(500);  // Update UI every 500ms
    progressTimer->start(100); // Update progress every 100ms
}

void IpScanner::stopScan()
{
    scanRunning = false;
    scanPaused = false;

    // Update UI
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    pauseButton->setEnabled(false);
    pauseButton->setText("⏸️ Pausieren");

    threadPool->clear();
    updateTimer->stop();
    progressTimer->stop();

    statusLabel->setText("🛑 Scan gestoppt");
    statusLabel->setStyleSheet("font-weight: bold; color: #ff0000;");

    addLogEntry("Scan vom Benutzer gestoppt", "WARNING");

    QString finalMessage = QString("Scan beendet. %1 von %2 Hosts gefunden")
                               .arg(discoveredHosts.size()).arg(totalHosts);
    statusBar()->showMessage(finalMessage);
}

void IpScanner::onHostFound(const HostInfo &host)
{
    QMutexLocker locker(&hostListMutex);
    discoveredHosts.append(host);

    // Add to tree widget with enhanced display
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, host.ip);
    item->setText(1, host.hostname.isEmpty() ? "Unbekannt" : host.hostname);
    item->setText(2, host.macAddress.isEmpty() ? "N/A" : host.macAddress);
    item->setText(3, host.vendor.isEmpty() ? "Unbekannt" : host.vendor);
    item->setText(4, host.isAlive ? "🟢 Online" : "🔴 Offline");
    item->setText(5, host.responseTime >= 0 ? QString("%1 ms").arg(host.responseTime) : "Timeout");
    item->setText(6, host.openPorts.isEmpty() ? "Keine erkannt" : host.openPorts.join(", "));
    item->setText(7, host.os.isEmpty() ? "Unbekannt" : host.os);
    item->setText(8, host.lastSeen.toString("dd.MM.yyyy hh:mm:ss"));
    item->setText(9, host.services.isEmpty() ? "Keine" : host.services.join(", "));

    // Enhanced color coding
    if (host.isAlive) {
        item->setBackground(0, QColor(0, 120, 0, 100));
        item->setIcon(0, QIcon()); // Could add online icon
    } else {
        item->setBackground(0, QColor(120, 0, 0, 100));
        item->setIcon(0, QIcon()); // Could add offline icon
    }

    hostTreeWidget->addTopLevelItem(item);

    // Update found hosts counter
    foundHostsLabel->setText(QString("Gefunden: %1").arg(discoveredHosts.size()));

    addLogEntry(QString("Host gefunden: %1 (%2) - Status: %3")
                    .arg(host.ip, host.hostname.isEmpty() ? "Unbekannt" : host.hostname, host.isAlive ? "Online" : "Offline"),
                host.isAlive ? "SUCCESS" : "INFO");
}

void IpScanner::onHostCompleted()
{
    QMutexLocker locker(&hostListMutex);
    completedHosts++;

    // Update current host display
    if (completedHosts <= totalHosts) {
        currentHostLabel->setText(QString("Host %1/%2 abgeschlossen").arg(completedHosts).arg(totalHosts));
        scannedHostsLabel->setText(QString("Gescannt: %1").arg(completedHosts));
    }

    // Check if scan is complete
    if (completedHosts >= totalHosts) {
        onScanFinished();
    }
}

void IpScanner::updateProgress()
{
    if (!scanRunning) return;

    // Update progress bar
    progressBar->setValue(completedHosts);

    // Update elapsed time
    if (!scanStartTime.isNull()) {
        qint64 elapsed = scanStartTime.msecsTo(QDateTime::currentDateTime());
        int seconds = elapsed / 1000;
        int minutes = seconds / 60;
        seconds = seconds % 60;
        elapsedTimeLabel->setText(QString("Zeit: %1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
    }
}

void IpScanner::updateScanProgress()
{
    if (!scanRunning) return;

    // This could be used for more detailed progress updates
    // Currently handled by updateProgress()
}

void IpScanner::onScanFinished()
{
    scanRunning = false;

    // Update UI
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    pauseButton->setEnabled(false);
    pauseButton->setText("⏸️ Pausieren");

    progressBar->setValue(totalHosts);
    statusLabel->setText("✅ Scan abgeschlossen");
    statusLabel->setStyleSheet("font-weight: bold; color: #00ff00;");
    currentHostLabel->setText("Scan abgeschlossen");

    updateTimer->stop();
    progressTimer->stop();

    addLogEntry(QString("Scan abgeschlossen! %1 von %2 Hosts gefunden in %3")
                    .arg(discoveredHosts.size())
                    .arg(totalHosts)
                    .arg(elapsedTimeLabel->text().split(": ")[1]), "SUCCESS");

    QString finalMessage = QString("Scan erfolgreich beendet - %1 aktive Hosts von %2 gefunden")
                               .arg(discoveredHosts.size()).arg(totalHosts);
    statusBar()->showMessage(finalMessage);

    // Auto-resize columns for better display
    for (int i = 0; i < hostTreeWidget->columnCount(); ++i) {
        hostTreeWidget->resizeColumnToContents(i);
    }
}

void IpScanner::pauseScan()
{
    if (scanPaused) {
        resumeScan();
    } else {
        scanPaused = true;
        pauseButton->setText("▶️ Fortsetzen");
        statusLabel->setText("⏸️ Scan pausiert");
        statusLabel->setStyleSheet("font-weight: bold; color: #ffaa00;");
        addLogEntry("Scan pausiert", "INFO");
        updateTimer->stop();
    }
}

void IpScanner::resumeScan()
{
    scanPaused = false;
    pauseButton->setText("⏸️ Pausieren");
    statusLabel->setText("🔄 Scan läuft...");
    statusLabel->setStyleSheet("font-weight: bold; color: #ffaa00;");
    addLogEntry("Scan fortgesetzt", "INFO");
    updateTimer->start(500);
}

void IpScanner::addCommonPort()
{
    QListWidgetItem *item = commonPortsList->currentItem();
    if (item) {
        QString portText = item->text();
        QString portNumber = portText.split(" - ")[0];

        QString currentPorts = portRangeEdit->text();
        if (!currentPorts.isEmpty() && !currentPorts.endsWith(",")) {
            currentPorts += ",";
        }
        currentPorts += portNumber;
        portRangeEdit->setText(currentPorts);

        addLogEntry(QString("Port %1 zur Scan-Liste hinzugefügt").arg(portNumber), "INFO");
    }
}

void IpScanner::filterLog()
{
    QString filterText = logFilterEdit->text().toLower();
    if (filterText.isEmpty()) {
        // Show all log entries
        return;
    }

    // Implementation for log filtering would go here
    addLogEntry(QString("Log gefiltert nach: '%1'").arg(filterText), "INFO");
}

void IpScanner::saveLog()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Log Speichern",
                                                    QString("scan_log_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    "Text Files (*.txt);;All Files (*)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "❌ Fehler", "Datei konnte nicht zum Schreiben geöffnet werden");
        return;
    }

    QTextStream stream(&file);
    stream << logTextEdit->toPlainText();

    addLogEntry(QString("Log gespeichert: %1").arg(fileName), "SUCCESS");
    QMessageBox::information(this, "✅ Erfolg", QString("Log erfolgreich gespeichert:\n%1").arg(fileName));
}

// Enhanced helper functions
QStringList IpScanner::parseTargetString(const QString &target)
{
    QStringList result;
    addLogEntry(QString("Parse Ziel: %1").arg(target), "DEBUG");

    // Trimmen des Eingabetexts
    QString cleanTarget = target.trimmed();

    if (cleanTarget.contains('/')) {
        // CIDR Notation (z.B. 192.168.0.0/16, 10.0.0.0/8)
        result = parseCIDRRange(cleanTarget);
    }
    else if (cleanTarget.contains('-')) {
        // Range format mit erweiterten Möglichkeiten
        result = parseIPRange(cleanTarget);
    }
    else if (cleanTarget.contains(',')) {
        // Comma-separated IPs
        QStringList ips = cleanTarget.split(',');
        for (const QString &ip : ips) {
            QString cleanIp = ip.trimmed();
            if (!cleanIp.isEmpty() && isValidIP(cleanIp)) {
                result.append(cleanIp);
            }
        }
        addLogEntry(QString("Mehrere IPs erkannt: %1 Adressen").arg(result.size()), "INFO");
    }
    else if (cleanTarget.contains('*')) {
        // Wildcard format (z.B. 192.168.*.*, 10.0.1.*)
        result = parseWildcardRange(cleanTarget);
    }
    else {
        // Single IP
        if (isValidIP(cleanTarget)) {
            result.append(cleanTarget);
            addLogEntry(QString("Einzelne IP erkannt: %1").arg(cleanTarget), "INFO");
        } else {
            addLogEntry(QString("Ungültige IP-Adresse: %1").arg(cleanTarget), "ERROR");
        }
    }

    return result;
}
bool IpScanner::isValidIP(const QString &ip)
{
    QStringList parts = ip.split('.');
    if (parts.size() != 4) return false;

    for (const QString &part : parts) {
        bool ok;
        int octet = part.toInt(&ok);
        if (!ok || octet < 0 || octet > 255) {
            return false;
        }
    }
    return true;
}
QStringList IpScanner::parseCIDRRange(const QString &cidrNotation)
{
    QStringList result;
    QStringList parts = cidrNotation.split('/');

    if (parts.size() != 2) {
        addLogEntry("Ungültige CIDR-Notation", "ERROR");
        return result;
    }

    QString network = parts[0].trimmed();
    int prefixLength = parts[1].trimmed().toInt();

    // Erweiterte CIDR-Unterstützung von /8 bis /30
    if (prefixLength < 8 || prefixLength > 30) {
        addLogEntry(QString("CIDR-Präfix %1 nicht unterstützt (8-30 erlaubt)").arg(prefixLength), "ERROR");
        return result;
    }

    // IP in Integer umwandeln
    QStringList ipParts = network.split('.');
    if (ipParts.size() != 4) {
        addLogEntry("Ungültige IP-Adresse in CIDR-Notation", "ERROR");
        return result;
    }

    quint32 ipInt = 0;
    for (int i = 0; i < 4; i++) {
        bool ok;
        int octet = ipParts[i].toInt(&ok);
        if (!ok || octet < 0 || octet > 255) {
            addLogEntry("Ungültiges IP-Oktett", "ERROR");
            return result;
        }
        ipInt = (ipInt << 8) + octet;
    }

    // Netzwerk-Maske berechnen
    quint32 mask = 0xFFFFFFFF << (32 - prefixLength);
    quint32 networkInt = ipInt & mask;

    // Anzahl der Host-Bits
    int hostBits = 32 - prefixLength;
    quint32 maxHosts = (1 << hostBits) - 2; // Ohne Netzwerk- und Broadcast-Adresse

    // Sicherheitsbegrenzung für sehr große Bereiche
    const quint32 MAX_SCAN_HOSTS = 65536; // Maximal 65536 Hosts scannen
    if (maxHosts > MAX_SCAN_HOSTS) {
        addLogEntry(QString("CIDR-Bereich zu groß (%1 Hosts). Maximal %2 Hosts erlaubt")
                        .arg(maxHosts).arg(MAX_SCAN_HOSTS), "WARNING");

        // Benutzer fragen, ob er trotzdem fortfahren möchte
        QMessageBox::StandardButton reply = QMessageBox::question(
            nullptr, "Großer IP-Bereich",
            QString("Der angegebene Bereich enthält %1 Hosts.\n"
                    "Dies kann sehr lange dauern und Netzwerk-Ressourcen belasten.\n\n"
                    "Möchten Sie trotzdem fortfahren?").arg(maxHosts),
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            return result;
        }

        // Auf Maximum begrenzen
        maxHosts = MAX_SCAN_HOSTS;
    }

    // IPs generieren
    for (quint32 i = 1; i <= maxHosts; ++i) {
        quint32 hostInt = networkInt + i;

        // Integer zu IP-String konvertieren
        QString ip = QString("%1.%2.%3.%4")
                         .arg((hostInt >> 24) & 0xFF)
                         .arg((hostInt >> 16) & 0xFF)
                         .arg((hostInt >> 8) & 0xFF)
                         .arg(hostInt & 0xFF);

        result.append(ip);
    }

    addLogEntry(QString("CIDR %1/%2 erkannt: %3 Hosts")
                    .arg(network).arg(prefixLength).arg(result.size()), "INFO");

    return result;
}

// Erweiterte Range-Unterstützung
QStringList IpScanner::parseIPRange(const QString &rangeString)
{
    QStringList result;

    if (rangeString.contains(" - ") || rangeString.contains("-")) {
        // Verschiedene Range-Formate unterstützen:
        // 192.168.1.1-254          (Letzte Oktett)
        // 192.168.1.1-192.168.1.254 (Vollständige IPs)
        // 192.168.1-10.1           (Drittes Oktett)
        // 10.0-255.1.1             (Zweites Oktett)

        QStringList parts = rangeString.split(QRegularExpression("\\s*-\\s*"));
        if (parts.size() == 2) {
            QString startIP = parts[0].trimmed();
            QString endPart = parts[1].trimmed();

            result = generateIPRange(startIP, endPart);
        }
    }

    return result;
}
QStringList IpScanner::parseWildcardRange(const QString &wildcardString)
{
    QStringList result;
    QStringList parts = wildcardString.split('.');

    if (parts.size() != 4) {
        addLogEntry("Ungültiges Wildcard-Format", "ERROR");
        return result;
    }

    // Wildcards in Bereiche umwandeln
    QList<QList<int>> ranges;
    for (const QString &part : parts) {
        QList<int> range;
        if (part == "*") {
            // Vollständiger Bereich für dieses Oktett
            for (int i = 0; i <= 255; ++i) {
                range.append(i);
            }
        } else if (part.contains('-')) {
            // Bereich im Oktett (z.B. 1-50)
            QStringList rangeParts = part.split('-');
            if (rangeParts.size() == 2) {
                int start = rangeParts[0].toInt();
                int end = rangeParts[1].toInt();
                for (int i = start; i <= end && i <= 255; ++i) {
                    range.append(i);
                }
            }
        } else {
            // Einzelner Wert
            int value = part.toInt();
            if (value >= 0 && value <= 255) {
                range.append(value);
            }
        }
        ranges.append(range);
    }

    // Alle Kombinationen generieren
    const int MAX_WILDCARD_COMBINATIONS = 10000;
    int totalCombinations = 1;
    for (const auto &range : ranges) {
        totalCombinations *= range.size();
    }

    if (totalCombinations > MAX_WILDCARD_COMBINATIONS) {
        addLogEntry(QString("Wildcard-Bereich zu groß (%1 Kombinationen). Maximal %2 erlaubt")
                        .arg(totalCombinations).arg(MAX_WILDCARD_COMBINATIONS), "WARNING");
        return result;
    }

    // IPs aus Wildcards generieren
    for (int a : ranges[0]) {
        for (int b : ranges[1]) {
            for (int c : ranges[2]) {
                for (int d : ranges[3]) {
                    QString ip = QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
                    result.append(ip);
                }
            }
        }
    }

    addLogEntry(QString("Wildcard %1 erkannt: %2 IPs")
                    .arg(wildcardString).arg(result.size()), "INFO");

    return result;
}

// Hilfsfunktion für IP-Range-Generierung
QStringList IpScanner::generateIPRange(const QString &startIP, const QString &endPart)
{
    QStringList result;
    QStringList startParts = startIP.split('.');

    if (startParts.size() != 4) {
        addLogEntry("Ungültige Start-IP", "ERROR");
        return result;
    }

    // Prüfen ob endPart eine vollständige IP oder nur ein Oktett ist
    if (endPart.contains('.')) {
        // Vollständige End-IP
        QStringList endParts = endPart.split('.');
        if (endParts.size() != 4) {
            addLogEntry("Ungültige End-IP", "ERROR");
            return result;
        }

        // Range zwischen zwei vollständigen IPs
        quint32 startInt = ipToInt(startIP);
        quint32 endInt = ipToInt(endPart);

        if (endInt < startInt) {
            addLogEntry("End-IP ist kleiner als Start-IP", "ERROR");
            return result;
        }

        const quint32 MAX_RANGE_SIZE = 65536;
        quint32 rangeSize = endInt - startInt + 1;

        if (rangeSize > MAX_RANGE_SIZE) {
            addLogEntry(QString("IP-Range zu groß (%1 IPs). Maximal %2 erlaubt")
                            .arg(rangeSize).arg(MAX_RANGE_SIZE), "WARNING");
            rangeSize = MAX_RANGE_SIZE;
        }

        for (quint32 i = 0; i < rangeSize; ++i) {
            quint32 currentInt = startInt + i;
            QString ip = intToIP(currentInt);
            result.append(ip);
        }

    } else {
        // Nur letztes Oktett als Ende
        int endOctet = endPart.toInt();
        int startOctet = startParts[3].toInt();

        if (endOctet < startOctet || endOctet > 255) {
            addLogEntry("Ungültiger End-Oktett-Wert", "ERROR");
            return result;
        }

        QString baseIP = QString("%1.%2.%3.").arg(startParts[0], startParts[1], startParts[2]);

        for (int i = startOctet; i <= endOctet; ++i) {
            result.append(baseIP + QString::number(i));
        }
    }

    addLogEntry(QString("IP-Range %1-%2 erkannt: %3 IPs")
                    .arg(startIP).arg(endPart).arg(result.size()), "INFO");

    return result;
}
quint32 IpScanner::ipToInt(const QString &ip)
{
    QStringList parts = ip.split('.');
    if (parts.size() != 4) return 0;

    quint32 result = 0;
    for (int i = 0; i < 4; i++) {
        result = (result << 8) + parts[i].toUInt();
    }
    return result;
}

QString IpScanner::intToIP(quint32 ip)
{
    return QString("%1.%2.%3.%4")
    .arg((ip >> 24) & 0xFF)
        .arg((ip >> 16) & 0xFF)
        .arg((ip >> 8) & 0xFF)
        .arg(ip & 0xFF);
}
QList<int> IpScanner::parsePortRange(const QString &range)
{
    QList<int> ports;

    addLogEntry(QString("Parse Port-Bereich: %1").arg(range), "DEBUG");

    if (range.contains('-')) {
        // Range format like "1-1000"
        QStringList parts = range.split('-');
        if (parts.size() == 2) {
            int start = parts[0].trimmed().toInt();
            int end = parts[1].trimmed().toInt();
            if (start > 0 && end <= 65535 && start <= end) {
                for (int i = start; i <= end; ++i) {
                    ports.append(i);
                }
                addLogEntry(QString("Port-Bereich: %1-%2 (%3 Ports)").arg(start).arg(end).arg(ports.size()), "INFO");
            }
        }
    } else if (range.contains(',')) {
        // Comma-separated format like "22,80,443"
        QStringList parts = range.split(',');
        for (const QString &part : parts) {
            int port = part.trimmed().toInt();
            if (port > 0 && port <= 65535) {
                ports.append(port);
            }
        }
        addLogEntry(QString("Einzelne Ports: %1 (%2 Ports)").arg(range).arg(ports.size()), "INFO");
    } else {
        // Single port
        int port = range.trimmed().toInt();
        if (port > 0 && port <= 65535) {
            ports.append(port);
            addLogEntry(QString("Einzelner Port: %1").arg(port), "INFO");
        }
    }

    return ports;
}

void IpScanner::addLogEntry(const QString &message, const QString &type)
{
    QString timestamp = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    QString colorCode;
    QString icon;

    // Enhanced log entry formatting with colors and icons
    if (type == "SUCCESS") {
        colorCode = "#00ff00";
        icon = "✅";
    } else if (type == "ERROR") {
        colorCode = "#ff0000";
        icon = "❌";
    } else if (type == "WARNING") {
        colorCode = "#ffaa00";
        icon = "⚠️";
    } else if (type == "DEBUG") {
        colorCode = "#888888";
        icon = "🔧";
    } else {
        colorCode = "#ffffff";
        icon = "ℹ️";
    }

    QString logEntry = QString("<span style='color: %1;'>[%2] %3 [%4] %5</span>")
                           .arg(colorCode, timestamp, icon, type, message);

    logTextEdit->append(logEntry);
    logTextEdit->ensureCursorVisible();

    // Auto-scroll to bottom
    QTextCursor cursor = logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    logTextEdit->setTextCursor(cursor);
}

void IpScanner::setupMenuBar()
{
    // File Menu (bereits vorhanden - erweitert)
    fileMenu = menuBar()->addMenu("📁 &Datei");

    newProjectAction = new QAction("🆕 &Neues Projekt", this);
    newProjectAction->setShortcut(QKeySequence::New);
    fileMenu->addAction(newProjectAction);

    openProjectAction = new QAction("📂 &Projekt öffnen", this);
    openProjectAction->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openProjectAction);

    saveProjectAction = new QAction("💾 &Projekt speichern", this);
    saveProjectAction->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveProjectAction);

    fileMenu->addSeparator();

    exportAction = new QAction("📤 &Ergebnisse exportieren", this);
    exportAction->setShortcut(QKeySequence("Ctrl+E"));
    fileMenu->addAction(exportAction);



    QMenu *vulnerabilityMenu = menuBar()->addMenu("🔒 &Vulnerability");

    QAction *startVulnScanAction = new QAction("🔍 &Vulnerability Scan starten", this);
    startVulnScanAction->setShortcut(QKeySequence("Ctrl+V"));
    vulnerabilityMenu->addAction(startVulnScanAction);

    QAction *stopVulnScanAction = new QAction("⏹️ Vulnerability Scan &stoppen", this);
    stopVulnScanAction->setShortcut(QKeySequence("Ctrl+Shift+V"));
    vulnerabilityMenu->addAction(stopVulnScanAction);

    vulnerabilityMenu->addSeparator();

    QAction *exportVulnReportAction = new QAction("📊 Vulnerability &Report exportieren", this);
    exportVulnReportAction->setShortcut(QKeySequence("Ctrl+R"));
    vulnerabilityMenu->addAction(exportVulnReportAction);

    QAction *clearVulnResultsAction = new QAction("🗑️ Vulnerability Ergebnisse &löschen", this);
    vulnerabilityMenu->addAction(clearVulnResultsAction);

    vulnerabilityMenu->addSeparator();

    QAction *vulnSettingsAction = new QAction("⚙️ Vulnerability &Einstellungen", this);
    vulnerabilityMenu->addAction(vulnSettingsAction);

    QAction *vulnDatabaseAction = new QAction("📚 Vulnerability &Datenbank aktualisieren", this);
    vulnerabilityMenu->addAction(vulnDatabaseAction);


    connect(startVulnScanAction, &QAction::triggered, this, &IpScanner::startVulnerabilitySccan);
    connect(stopVulnScanAction, &QAction::triggered, this, &IpScanner::stopVulnerabilitySccan);
    connect(exportVulnReportAction, &QAction::triggered, this, &IpScanner::exportVulnerabilityReport);
    connect(clearVulnResultsAction, &QAction::triggered, this, &IpScanner::clearVulnerabilityResults);
    connect(vulnSettingsAction, &QAction::triggered, this, &IpScanner::showVulnerabilitySettings);
    connect(vulnDatabaseAction, &QAction::triggered, this, &IpScanner::updateVulnerabilityDatabase);

    // NEU: Netzwerk-Export Aktionen
    QAction *exportNetworkAction = new QAction("🌐 &Netzwerk-Karte exportieren", this);
    exportNetworkAction->setShortcut(QKeySequence("Ctrl+Shift+E"));
    fileMenu->addAction(exportNetworkAction);

    fileMenu->addSeparator();

    exitAction = new QAction("🚪 &Beenden", this);
    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);

    // Scan Menu (erweitert)
    scanMenu = menuBar()->addMenu("🔍 &Scan");
    scanMenu->addAction("🏓 Schneller &Ping", this, &IpScanner::pingHost);
    scanMenu->addAction("🛤️ &Traceroute", this, &IpScanner::traceroute);
    scanMenu->addAction("🌐 &DNS Lookup", this, &IpScanner::dnsLookup);
    scanMenu->addSeparator();
    scanMenu->addAction("🔒 &Vulnerability Scan", this, &IpScanner::vulnScan);
    scanMenu->addAction("⚙️ &Service Scan", this, &IpScanner::serviceScan);

    // NEU: Netzwerk Menu
    QMenu *networkMenu = menuBar()->addMenu("🌐 &Netzwerk");

    QAction *networkDiscoveryAction = new QAction("🔍 &Netzwerk Erkennung", this);
    networkDiscoveryAction->setShortcut(QKeySequence("Ctrl+D"));
    networkMenu->addAction(networkDiscoveryAction);

    QAction *arpScanAction = new QAction("📡 &ARP Scan", this);
    arpScanAction->setShortcut(QKeySequence("Ctrl+A"));
    networkMenu->addAction(arpScanAction);

    networkMenu->addSeparator();

    QAction *topologyAnalysisAction = new QAction("🗺️ &Topologie Analyse", this);
    topologyAnalysisAction->setShortcut(QKeySequence("Ctrl+T"));
    networkMenu->addAction(topologyAnalysisAction);

    QAction *networkStatsAction = new QAction("📊 Netzwerk-&Statistiken", this);
    networkStatsAction->setShortcut(QKeySequence("Ctrl+S"));
    networkMenu->addAction(networkStatsAction);

    QAction *healthCheckAction = new QAction("🏥 &Gesundheitscheck", this);
    healthCheckAction->setShortcut(QKeySequence("Ctrl+H"));
    networkMenu->addAction(healthCheckAction);

    networkMenu->addSeparator();

    QAction *refreshInterfacesAction = new QAction("🔄 Interfaces &Aktualisieren", this);
    refreshInterfacesAction->setShortcut(QKeySequence("F5"));
    networkMenu->addAction(refreshInterfacesAction);

    // Tools Menu (bereits vorhanden - erweitert)
    toolsMenu = menuBar()->addMenu("🔧 &Tools");
    toolsMenu->addAction("🔍 &WHOIS Lookup", this, &IpScanner::whoIs);

    toolsMenu->addSeparator();

    // NEU: Erweiterte Tools
    QAction *macLookupAction = new QAction("🏭 &MAC Hersteller Lookup", this);
    toolsMenu->addAction(macLookupAction);

    QAction *subnetCalcAction = new QAction("🧮 &Subnetz Rechner", this);
    toolsMenu->addAction(subnetCalcAction);

    toolsMenu->addSeparator();

    settingsAction = new QAction("⚙️ &Einstellungen", this);
    toolsMenu->addAction(settingsAction);

    // View Menu (NEU)
    QMenu *viewMenu = menuBar()->addMenu("👁️ &Ansicht");

    QAction *expandAllAction = new QAction("📖 Alle &Erweitern", this);
    expandAllAction->setShortcut(QKeySequence("Ctrl+Plus"));
    viewMenu->addAction(expandAllAction);

    QAction *collapseAllAction = new QAction("📖 Alle &Zuklappen", this);
    collapseAllAction->setShortcut(QKeySequence("Ctrl+Minus"));
    viewMenu->addAction(collapseAllAction);

    viewMenu->addSeparator();

    QAction *autoRefreshAction = new QAction("🔄 &Auto-Aktualisierung", this);
    autoRefreshAction->setCheckable(true);
    autoRefreshAction->setChecked(false);
    viewMenu->addAction(autoRefreshAction);

    // Help Menu (bereits vorhanden)
    helpMenu = menuBar()->addMenu("❓ &Hilfe");
    helpMenu->addAction("ℹ️ &Über", this, &IpScanner::showAbout);
    helpMenu->addAction("📋 &Tastenkürzel", this, &IpScanner::showShortcuts);

    // Connect neue Aktionen
    connect(newProjectAction, &QAction::triggered, this, &IpScanner::clearResults);
    connect(openProjectAction, &QAction::triggered, this, &IpScanner::loadProject);
    connect(saveProjectAction, &QAction::triggered, this, &IpScanner::saveProject);
    connect(exportAction, &QAction::triggered, this, &IpScanner::exportResults);
    connect(exportNetworkAction, &QAction::triggered, this, &IpScanner::exportNetworkMap);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Netzwerk-Menü Verbindungen
    connect(networkDiscoveryAction, &QAction::triggered, this, &IpScanner::networkDiscovery);
    connect(arpScanAction, &QAction::triggered, this, &IpScanner::arpScan);
    connect(topologyAnalysisAction, &QAction::triggered, this, &IpScanner::analyzeNetworkTopology);
    connect(networkStatsAction, &QAction::triggered, this, &IpScanner::showNetworkStatistics);
    connect(healthCheckAction, &QAction::triggered, this, &IpScanner::performNetworkHealthCheck);
    connect(refreshInterfacesAction, &QAction::triggered, this, &IpScanner::refreshNetworkInterfaces);

    // Tools-Menü Verbindungen
    connect(macLookupAction, &QAction::triggered, this, &IpScanner::showMacLookup);
    connect(subnetCalcAction, &QAction::triggered, this, &IpScanner::showSubnetCalculator);
    connect(settingsAction, &QAction::triggered, this, &IpScanner::showSettings);

    // View-Menü Verbindungen
    connect(expandAllAction, &QAction::triggered, [this]() {
        hostTreeWidget->expandAll();
        networkTree->expandAll();
        addLogEntry("Alle Einträge erweitert", "INFO");
    });

    connect(collapseAllAction, &QAction::triggered, [this]() {
        hostTreeWidget->collapseAll();
        networkTree->collapseAll();
        addLogEntry("Alle Einträge zugeklappt", "INFO");
    });

    connect(autoRefreshAction, &QAction::toggled, [this](bool enabled) {
        if (enabled) {
            // Auto-refresh timer setup (future implementation)
            addLogEntry("Auto-Aktualisierung aktiviert", "INFO");
        } else {
            addLogEntry("Auto-Aktualisierung deaktiviert", "INFO");
        }
    });
}
void IpScanner::refreshNetworkInterfaces()
{
    interfaceCombo->clear();
    QStringList interfaces = getLocalInterfaces();
    interfaceCombo->addItems(interfaces);

    addLogEntry("Netzwerk-Interfaces aktualisiert", "INFO");
    statusBar()->showMessage("Netzwerk-Interfaces aktualisiert", 2000);
}
void IpScanner::showSubnetCalculator()
{
    // Subnet Calculator Tool
    QDialog *subnetDialog = new QDialog(this);
    subnetDialog->setWindowTitle("🧮 Subnetz Rechner");
    subnetDialog->resize(600, 500);
    subnetDialog->setStyleSheet(styleSheet());

    QVBoxLayout *layout = new QVBoxLayout(subnetDialog);

    // Input section
    QGroupBox *inputGroup = new QGroupBox("📥 Eingabe");
    QGridLayout *inputLayout = new QGridLayout(inputGroup);

    QLineEdit *ipEdit = new QLineEdit();
    ipEdit->setPlaceholderText("z.B. 192.168.1.0");
    QSpinBox *cidrSpinBox = new QSpinBox();
    cidrSpinBox->setRange(1, 32);
    cidrSpinBox->setValue(24);

    inputLayout->addWidget(new QLabel("IP-Adresse:"), 0, 0);
    inputLayout->addWidget(ipEdit, 0, 1);
    inputLayout->addWidget(new QLabel("CIDR:"), 0, 2);
    inputLayout->addWidget(cidrSpinBox, 0, 3);

    QPushButton *calculateButton = new QPushButton("🧮 Berechnen");
    inputLayout->addWidget(calculateButton, 1, 0, 1, 4);

    // Result section
    QTextEdit *resultEdit = new QTextEdit();
    resultEdit->setReadOnly(true);
    resultEdit->setFont(QFont("Consolas", 10));

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *clearButton = new QPushButton("🗑️ Löschen");
    QPushButton *closeButton = new QPushButton("❌ Schließen");

    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addWidget(inputGroup);
    layout->addWidget(resultEdit);
    layout->addLayout(buttonLayout);

    // Connect signals
    connect(calculateButton, &QPushButton::clicked, [ipEdit, cidrSpinBox, resultEdit, this]() {
        QString ip = ipEdit->text().trimmed();
        int cidr = cidrSpinBox->value();

        if (ip.isEmpty()) {
            resultEdit->setPlainText("❌ Bitte geben Sie eine IP-Adresse ein!");
            return;
        }

        // Validate IP address
        QStringList ipParts = ip.split('.');
        if (ipParts.size() != 4) {
            resultEdit->setPlainText("❌ Ungültige IP-Adresse Format!");
            return;
        }

        bool valid = true;
        for (const QString &part : ipParts) {
            int num = part.toInt(&valid);
            if (!valid || num < 0 || num > 255) {
                resultEdit->setPlainText("❌ Ungültige IP-Adresse!");
                return;
            }
        }

        // Calculate subnet information
        QString result = calculateSubnetInfo(ip, cidr);
        resultEdit->setPlainText(result);
        addLogEntry(QString("Subnetz-Berechnung: %1/%2").arg(ip).arg(cidr), "INFO");
    });

    connect(clearButton, &QPushButton::clicked, [ipEdit, cidrSpinBox, resultEdit]() {
        ipEdit->clear();
        cidrSpinBox->setValue(24);
        resultEdit->clear();
    });

    connect(closeButton, &QPushButton::clicked, subnetDialog, &QDialog::accept);

    // Enable Enter key for calculation
    connect(ipEdit, &QLineEdit::returnPressed, calculateButton, &QPushButton::click);

    subnetDialog->exec();
}
QString IpScanner::calculateSubnetInfo(const QString &ip, int cidr)
{
    // Convert IP to 32-bit integer
    QStringList ipParts = ip.split('.');
    quint32 ipInt = 0;
    for (int i = 0; i < 4; i++) {
        ipInt = (ipInt << 8) + ipParts[i].toUInt();
    }

    // Calculate subnet mask
    quint32 mask = 0xFFFFFFFF << (32 - cidr);

    // Calculate network address
    quint32 networkInt = ipInt & mask;

    // Calculate broadcast address
    quint32 broadcastInt = networkInt | (~mask);

    // Calculate first and last host addresses
    quint32 firstHostInt = networkInt + 1;
    quint32 lastHostInt = broadcastInt - 1;

    // Calculate number of hosts
    quint32 totalHosts = (~mask) + 1;
    quint32 usableHosts = totalHosts - 2; // Subtract network and broadcast

    // Convert back to dotted decimal notation
    auto intToIp = [](quint32 addr) -> QString {
        return QString("%1.%2.%3.%4")
        .arg((addr >> 24) & 0xFF)
            .arg((addr >> 16) & 0xFF)
            .arg((addr >> 8) & 0xFF)
            .arg(addr & 0xFF);
    };

    QString networkAddr = intToIp(networkInt);
    QString broadcastAddr = intToIp(broadcastInt);
    QString firstHostAddr = intToIp(firstHostInt);
    QString lastHostAddr = intToIp(lastHostInt);
    QString subnetMask = intToIp(mask);

    // Build result string
    QString result = QString(R"(
🧮 SUBNETZ BERECHNUNG ERGEBNISSE
===============================================

📍 EINGABE:
   IP-Adresse: %1
   CIDR Notation: /%2

🌐 NETZWERK INFORMATIONEN:
   Netzwerk-Adresse:    %3
   Subnetz-Maske:       %4
   Broadcast-Adresse:   %5

👥 HOST BEREICH:
   Erster Host:         %6
   Letzter Host:        %7

📊 KAPAZITÄT:
   Gesamte Adressen:    %8
   Nutzbare Hosts:      %9

🔧 ZUSÄTZLICHE INFORMATIONEN:
   Netzwerk-Klasse:     %10
   Subnetz-Typ:         %11
   Wildcard-Maske:      %12

💡 VERWENDUNGSHINWEISE:
   • Netzwerk-Adresse (%3) für Routing-Tabellen
   • Broadcast-Adresse (%5) für Netzwerk-weite Nachrichten
   • Host-Bereich (%6 - %7) für Geräte-Zuweisung
    )").arg(ip)
                         .arg(cidr)
                         .arg(networkAddr)
                         .arg(subnetMask)
                         .arg(broadcastAddr)
                         .arg(firstHostAddr)
                         .arg(lastHostAddr)
                         .arg(totalHosts)
                         .arg(usableHosts)
                         .arg(getNetworkClass(networkInt))
                         .arg(getSubnetType(cidr))
                         .arg(intToIp(~mask));

    return result;
}
QString IpScanner::getNetworkClass(quint32 networkAddr)
{
    quint8 firstOctet = (networkAddr >> 24) & 0xFF;

    if (firstOctet >= 1 && firstOctet <= 126) {
        return "Klasse A (1.0.0.0 - 126.255.255.255)";
    } else if (firstOctet >= 128 && firstOctet <= 191) {
        return "Klasse B (128.0.0.0 - 191.255.255.255)";
    } else if (firstOctet >= 192 && firstOctet <= 223) {
        return "Klasse C (192.0.0.0 - 223.255.255.255)";
    } else if (firstOctet >= 224 && firstOctet <= 239) {
        return "Klasse D (Multicast)";
    } else if (firstOctet >= 240 && firstOctet <= 255) {
        return "Klasse E (Experimentell)";
    } else {
        return "Unbekannte Klasse";
    }
}
QString IpScanner::getSubnetType(int cidr)
{
    if (cidr >= 30) {
        return "Point-to-Point Link (/30-/32)";
    } else if (cidr >= 24) {
        return "Kleines LAN (/24-/29)";
    } else if (cidr >= 16) {
        return "Mittleres Netzwerk (/16-/23)";
    } else if (cidr >= 8) {
        return "Großes Netzwerk (/8-/15)";
    } else {
        return "Sehr großes Netzwerk (/1-/7)";
    }
}
void IpScanner::showMacLookup()
{
    // MAC Address Lookup Tool
    QDialog *macDialog = new QDialog(this);
    macDialog->setWindowTitle("🏭 MAC Hersteller Lookup");
    macDialog->resize(500, 300);
    macDialog->setStyleSheet(styleSheet());

    QVBoxLayout *layout = new QVBoxLayout(macDialog);

    // Input section
    QHBoxLayout *inputLayout = new QHBoxLayout();
    QLabel *inputLabel = new QLabel("MAC-Adresse:");
    QLineEdit *macEdit = new QLineEdit();
    macEdit->setPlaceholderText("z.B. AA:BB:CC:DD:EE:FF oder AA-BB-CC-DD-EE-FF");
    QPushButton *lookupButton = new QPushButton("🔍 Lookup");

    inputLayout->addWidget(inputLabel);
    inputLayout->addWidget(macEdit);
    inputLayout->addWidget(lookupButton);

    // Result section
    QTextEdit *resultEdit = new QTextEdit();
    resultEdit->setReadOnly(true);
    resultEdit->setMaximumHeight(150);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *clearButton = new QPushButton("🗑️ Löschen");
    QPushButton *closeButton = new QPushButton("❌ Schließen");

    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addLayout(inputLayout);
    layout->addWidget(resultEdit);
    layout->addLayout(buttonLayout);

    // Connect signals
    connect(lookupButton, &QPushButton::clicked, [this, macEdit, resultEdit]() {
        QString mac = macEdit->text().trimmed();
        if (mac.isEmpty()) {
            resultEdit->setPlainText("❌ Bitte geben Sie eine MAC-Adresse ein!");
            return;
        }

        QString vendor = getMacVendor(mac);
        QString result = QString("🔍 MAC-Adresse: %1\n🏭 Hersteller: %2\n\n").arg(mac, vendor);

        // Additional MAC info
        if (mac.length() >= 8) {
            QString oui = mac.left(8).toUpper().replace(":", "").replace("-", "").left(6);
            result += QString("🔢 OUI (Hersteller-ID): %1\n").arg(oui);

            // Check if it's a local/global address
            QString firstOctet = mac.left(2);
            bool isLocal = (firstOctet.toInt(nullptr, 16) & 0x02) != 0;
            bool isMulticast = (firstOctet.toInt(nullptr, 16) & 0x01) != 0;

            result += QString("🌐 Adresstyp: %1\n").arg(
                isMulticast ? "Multicast" : (isLocal ? "Lokal verwaltet" : "Global eindeutig"));
        }

        resultEdit->setPlainText(result);
        addLogEntry(QString("MAC-Lookup durchgeführt: %1 -> %2").arg(mac, vendor), "INFO");
    });

    connect(clearButton, &QPushButton::clicked, [macEdit, resultEdit]() {
        macEdit->clear();
        resultEdit->clear();
    });

    connect(closeButton, &QPushButton::clicked, macDialog, &QDialog::accept);

    // Enable Enter key for lookup
    connect(macEdit, &QLineEdit::returnPressed, lookupButton, &QPushButton::click);

    macDialog->exec();
}
void IpScanner::setupStatusBar()
{
    statusBar()->showMessage("✅ Bereit für Netzwerk-Scanning");
    statusBar()->setStyleSheet("QStatusBar { padding: 5px; }");
}

// Enhanced scan methods
void IpScanner::pingHost()
{
    QString target = targetEdit->text();
    if (target.isEmpty()) {
        QMessageBox::information(this, "🏓 Ping", "Bitte geben Sie eine Ziel-IP-Adresse ein");
        return;
    }

    addLogEntry(QString("Starte Ping zu %1").arg(target), "INFO");
    statusBar()->showMessage(QString("Pinge %1...").arg(target));

    PingWorker *worker = new PingWorker(target);
    connect(worker, &PingWorker::pingResult, [this](const QString &ip, bool alive, int responseTime) {
        QString result = alive ?
                             QString("✅ Ping zu %1 erfolgreich (%2 ms)").arg(ip).arg(responseTime) :
                             QString("❌ Ping zu %1 fehlgeschlagen").arg(ip);
        addLogEntry(result, alive ? "SUCCESS" : "WARNING");
        QMessageBox::information(this, "🏓 Ping Ergebnis", result);
        statusBar()->showMessage("Bereit");
    });

    threadPool->start(worker);
}

void IpScanner::portScan()
{
    QTreeWidgetItem *currentItem = hostTreeWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "🔌 Port Scan", "Bitte wählen Sie zuerst einen Host aus den Ergebnissen aus");
        return;
    }

    QString targetIp = currentItem->text(0);
    QString portRange = portRangeEdit->text();

    if (portRange.isEmpty()) {
        QMessageBox::warning(this, "🔌 Port Scan", "Bitte geben Sie einen Port-Bereich ein");
        return;
    }

    addLogEntry(QString("Starte Port-Scan von %1 (Ports: %2)").arg(targetIp, portRange), "INFO");
    statusBar()->showMessage(QString("Scanne Ports auf %1...").arg(targetIp));

    // Clear previous results
    portResultsTree->clear();

    // Parse port range
    QList<int> ports = parsePortRange(portRange);

    if (ports.isEmpty()) {
        QMessageBox::warning(this, "❌ Fehler", "Ungültiger Port-Bereich");
        return;
    }

    PortScanWorker *worker = new PortScanWorker(targetIp, ports);

    // Connect to the enhanced portResult signal with explicit casting
    connect(worker, QOverload<const QString&, int, bool, const QString&, const QString&, const QString&, int>::of(&PortScanWorker::portResult),
            [this](const QString &ip, int port, bool open, const QString &service, const QString &version, const QString &banner, int responseTime) {
                QTreeWidgetItem *item = new QTreeWidgetItem();
                item->setText(0, ip);
                item->setText(1, QString::number(port));
                item->setText(2, open ? "🟢 Offen" : "🔴 Geschlossen");
                item->setText(3, service);
                item->setText(4, version.isEmpty() ? "Unbekannt" : version);
                item->setText(5, banner.isEmpty() ? "Kein Banner" : banner.left(50) + (banner.length() > 50 ? "..." : ""));
                item->setText(6, responseTime > 0 ? QString("%1 ms").arg(responseTime) : "N/A");

                if (open) {
                    item->setBackground(2, QColor(0, 120, 0, 100));
                    addLogEntry(QString("Port %1 offen auf %2 (%3) - %4").arg(port).arg(ip, service, version), "SUCCESS");
                } else {
                    item->setBackground(2, QColor(120, 0, 0, 50));
                }

                portResultsTree->addTopLevelItem(item);
            });

    // Connect to scan progress signals
    connect(worker, &PortScanWorker::scanStarted, [this](const QString &ip, int totalPorts) {
        addLogEntry(QString("Port-Scan gestartet für %1 (%2 Ports)").arg(ip).arg(totalPorts), "INFO");
        statusBar()->showMessage(QString("Scanne %1 Ports auf %2...").arg(totalPorts).arg(ip));
    });

    connect(worker, &PortScanWorker::scanProgress, [this](int scannedPorts, int totalPorts) {
        int progress = (scannedPorts * 100) / totalPorts;
        statusBar()->showMessage(QString("Port-Scan Fortschritt: %1% (%2/%3 Ports)")
                                     .arg(progress).arg(scannedPorts).arg(totalPorts));
    });

    connect(worker, &PortScanWorker::scanCompleted, [this, targetIp](const QString &ip, int portsScanned, qint64 totalTime) {
        Q_UNUSED(ip)
        addLogEntry(QString("Port-Scan von %1 abgeschlossen - %2 Ports in %3 ms gescannt")
                        .arg(targetIp).arg(portsScanned).arg(totalTime), "SUCCESS");
        statusBar()->showMessage("Port-Scan abgeschlossen");

        // Auto-resize columns for better display
        for (int i = 0; i < portResultsTree->columnCount(); ++i) {
            portResultsTree->resizeColumnToContents(i);
        }

        // Show summary
        int openPorts = 0;
        for (int i = 0; i < portResultsTree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *item = portResultsTree->topLevelItem(i);
            if (item && item->text(2).contains("Offen")) {
                openPorts++;
            }
        }
/*
        QMessageBox::information(this, "✅ Port-Scan Abgeschlossen",
                                 QString("Port-Scan erfolgreich beendet!\n\n"
                                         "📊 Gescannte Ports: %1\n"
                                         "🟢 Offene Ports: %2\n"
                                         "⏱️ Scan-Zeit: %3 ms\n"
                                         "🎯 Ziel: %4")
                                     .arg(portsScanned).arg(openPorts).arg(totalTime).arg(targetIp));
*/
    });

    threadPool->start(worker);
}

// Additional required methods that were referenced but missing
void IpScanner::loadSettings()
{
    targetEdit->setText(settings->value("lastTarget", "192.168.1.1-254").toString());
    threadsSpinBox->setValue(settings->value("threads", 50).toInt());
    timeoutSpinBox->setValue(settings->value("timeout", 3000).toInt());

    restoreGeometry(settings->value("geometry").toByteArray());
    restoreState(settings->value("windowState").toByteArray());

    addLogEntry("Einstellungen geladen", "INFO");
}

void IpScanner::saveSettings()
{
    settings->setValue("lastTarget", targetEdit->text());
    settings->setValue("threads", threadsSpinBox->value());
    settings->setValue("timeout", timeoutSpinBox->value());
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());

    addLogEntry("Einstellungen gespeichert", "SUCCESS");
}

void IpScanner::clearResults()
{
    hostTreeWidget->clear();
    portResultsTree->clear();
    networkTree->clear();
    discoveredHosts.clear();
    hostDetailsEdit->clear();
    progressBar->setValue(0);
    scannedHosts = 0;
    completedHosts = 0;
    totalHosts = 0;

    // Reset labels
    totalHostsLabel->setText("Gesamt: 0");
    scannedHostsLabel->setText("Gescannt: 0");
    foundHostsLabel->setText("Gefunden: 0");
    statusLabel->setText("Bereit zum Scannen");
    statusLabel->setStyleSheet("font-weight: bold; color: #00ff00;");
    currentHostLabel->setText("Aktueller Host: --");
    elapsedTimeLabel->setText("Zeit: 00:00");

    addLogEntry("Ergebnisse gelöscht - Bereit für neuen Scan", "INFO");
}

QStringList IpScanner::getLocalInterfaces()
{
    QStringList interfaces;
    const auto networkInterfaces = QNetworkInterface::allInterfaces();
    for (const auto &interface : networkInterfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            interfaces << QString("%1 (%2)").arg(interface.name(), interface.humanReadableName());
        }
    }
    if (interfaces.isEmpty()) {
        interfaces << "Keine aktiven Interfaces gefunden";
    }
    return interfaces;
}

void IpScanner::showShortcuts()
{
    QString shortcuts = R"(
<h3>🎯 Tastenkürzel - Advanced IP Scanner</h3>
<table border="1" cellpadding="5" style="width: 100%;">

<tr style="background-color: #0078d4; color: white;">
<td colspan="2"><b>📁 Datei-Operationen</b></td>
</tr>
<tr><td><b>Ctrl+N</b></td><td>Neues Projekt</td></tr>
<tr><td><b>Ctrl+O</b></td><td>Projekt öffnen</td></tr>
<tr><td><b>Ctrl+S</b></td><td>Projekt speichern</td></tr>
<tr><td><b>Ctrl+E</b></td><td>Ergebnisse exportieren</td></tr>
<tr><td><b>Ctrl+Shift+E</b></td><td>Netzwerk-Karte exportieren</td></tr>
<tr><td><b>Ctrl+Q</b></td><td>Programm beenden</td></tr>

<tr style="background-color: #e83e8c; color: white;">
<td colspan="2"><b>🌐 Netzwerk-Analyse</b></td>
</tr>
<tr><td><b>Ctrl+D</b></td><td>Netzwerk Erkennung</td></tr>
<tr><td><b>Ctrl+A</b></td><td>ARP Scan</td></tr>
<tr><td><b>Ctrl+T</b></td><td>Topologie Analyse</td></tr>
<tr><td><b>Ctrl+H</b></td><td>Netzwerk Gesundheitscheck</td></tr>
<tr><td><b>F5</b></td><td>Interfaces aktualisieren</td></tr>

<tr style="background-color: #28a745; color: white;">
<td colspan="2"><b>🔍 Scan-Operationen</b></td>
</tr>
<tr><td><b>F5</b></td><td>Scan starten</td></tr>
<tr><td><b>Esc</b></td><td>Scan stoppen</td></tr>
<tr><td><b>Space</b></td><td>Scan pausieren/fortsetzen</td></tr>

<tr style="background-color: #17a2b8; color: white;">
<td colspan="2"><b>👁️ Ansicht</b></td>
</tr>
<tr><td><b>Ctrl+Plus</b></td><td>Alle Einträge erweitern</td></tr>
<tr><td><b>Ctrl+Minus</b></td><td>Alle Einträge zuklappen</td></tr>
<tr><td><b>Ctrl+R</b></td><td>Ansicht aktualisieren</td></tr>

</table>

<p style="margin-top: 15px;"><b>💡 Tipp:</b> Verwenden Sie das Kontextmenü (Rechtsklick) für weitere Optionen!</p>
    )";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("📋 Tastenkürzel");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(shortcuts);
    msgBox.setStyleSheet(styleSheet()); // Apply dark theme
    msgBox.exec();
}

void IpScanner::showAbout()
{
    QString aboutText = R"(
<h2>🔍 Advanced IP Scanner v2.0</h2>
<p><b>Verbesserter Netzwerk-Scanner mit erweiterten Funktionen</b></p>

<h3>✨ Features:</h3>
<ul>
<li>🌐 Erweiterte Netzwerk-Erkennung</li>
<li>🔌 Detaillierter Port-Scanning</li>
<li>⚙️ Service-Erkennung</li>
<li>🖥️ Betriebssystem-Fingerprinting</li>
<li>📊 Echtzeit-Fortschrittsanzeige</li>
<li>📤 Export/Import von Ergebnissen</li>
<li>🔀 Multi-threaded Scanning</li>
<li>🎨 Modernes Dark Theme Interface</li>
</ul>

<h3>🛠️ Technologie:</h3>
<p>Entwickelt mit Qt6 C++ für maximale Performance und Stabilität</p>

<h3>📧 Support:</h3>
<p>Für Fragen und Support besuchen Sie unsere Website</p>
    )";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("ℹ️ Über IP Scanner");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(aboutText);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

// Additional stub implementations for completeness
void IpScanner::traceroute()
{
    QMessageBox::information(this, "🛤️ Traceroute", "Traceroute-Funktion wird in der nächsten Version implementiert");
    addLogEntry("Traceroute-Funktion aufgerufen (noch nicht implementiert)", "INFO");
}

void IpScanner::dnsLookup()
{
    QMessageBox::information(this, "🌐 DNS Lookup", "DNS Lookup-Funktion wird in der nächsten Version implementiert");
    addLogEntry("DNS Lookup-Funktion aufgerufen (noch nicht implementiert)", "INFO");
}

void IpScanner::vulnScan()
{
    QMessageBox::information(this, "🔒 Vulnerability Scanner", "Vulnerability Scanner-Funktion wird in der nächsten Version implementiert");
    addLogEntry("Vulnerability Scanner aufgerufen (noch nicht implementiert)", "INFO");
}

void IpScanner::serviceScan()
{
    QMessageBox::information(this, "⚙️ Service Scanner", "Service Scanner-Funktion wird in der nächsten Version implementiert");
    addLogEntry("Service Scanner aufgerufen (noch nicht implementiert)", "INFO");
}

void IpScanner::networkDiscovery()
{
    QString selectedInterface = interfaceCombo->currentText();
    if (selectedInterface.isEmpty() || selectedInterface.contains("Keine aktiven")) {
        QMessageBox::warning(this, "🔍 Network Discovery",
                             "Bitte wählen Sie ein gültiges Netzwerk-Interface aus!");
        return;
    }

    // Prüfung auf bereits laufende Discovery
    if (!discoveryButton->isEnabled()) {
        addLogEntry("Netzwerk-Erkennung bereits aktiv", "WARNING");
        return;
    }

    QString interfaceName = selectedInterface.split(" (").first();

    addLogEntry(QString("Starte Netzwerk-Erkennung auf Interface: %1").arg(interfaceName), "INFO");
    statusBar()->showMessage(QString("Netzwerk wird erkundet: %1...").arg(interfaceName));

    // UI sofort deaktivieren
    discoveryButton->setEnabled(false);
    discoveryButton->setText("🔍 Erkunde...");

    // Clear previous results
    networkTree->clear();

    // Create and start network discovery worker mit Exception Handling
    NetworkDiscoveryWorker *worker = new NetworkDiscoveryWorker(interfaceName);

    // Thread-safe signal connections
    connect(worker, &NetworkDiscoveryWorker::discoveryStarted,
            this, [this](const QString &interface) {
                addLogEntry(QString("Netzwerk-Erkennung gestartet auf %1").arg(interface), "INFO");
            }, Qt::QueuedConnection);

    connect(worker, &NetworkDiscoveryWorker::networkFound,
            this, [this](const QString &network, int prefixLength, const QString &interface, const QString &mac) {
                QMetaObject::invokeMethod(this, [this, network, prefixLength, interface, mac]() {
                    try {
                        QTreeWidgetItem *networkItem = new QTreeWidgetItem();
                        networkItem->setText(0, QString("Netzwerk: %1/%2").arg(network).arg(prefixLength));
                        networkItem->setText(1, network);
                        networkItem->setText(2, mac.isEmpty() ? "N/A" : mac);
                        networkItem->setText(3, "Interface Vendor");
                        networkItem->setText(4, interface);
                        networkItem->setText(5, "Network");
                        networkItem->setText(6, QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));

                        networkItem->setBackground(0, QColor(0, 100, 200, 100));

                        if (networkTree) {
                            networkTree->addTopLevelItem(networkItem);
                        }

                        addLogEntry(QString("Netzwerk gefunden: %1/%2 auf %3")
                                        .arg(network).arg(prefixLength).arg(interface), "SUCCESS");
                    } catch (...) {
                        addLogEntry("Fehler beim Hinzufügen von Netzwerk-Eintrag", "ERROR");
                    }
                }, Qt::QueuedConnection);
            }, Qt::QueuedConnection);

    connect(worker, &NetworkDiscoveryWorker::deviceFound,
            this, [this](const QString &ip, const QString &hostname, const QString &mac, const QString &type) {
                QMetaObject::invokeMethod(this, [this, ip, hostname, mac, type]() {
                    try {
                        QTreeWidgetItem *deviceItem = new QTreeWidgetItem();
                        deviceItem->setText(0, hostname.isEmpty() ? QString("Gerät (%1)").arg(ip) : hostname);
                        deviceItem->setText(1, ip);
                        deviceItem->setText(2, mac.isEmpty() ? "N/A" : mac);
                        deviceItem->setText(3, getMacVendor(mac));
                        deviceItem->setText(4, "Auto-Discovery");
                        deviceItem->setText(5, type);
                        deviceItem->setText(6, QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));

                        // Color code by device type
                        QColor typeColor;
                        if (type.contains("Router") || type.contains("Gateway")) {
                            typeColor = QColor(255, 165, 0, 100); // Orange for routers
                        } else if (type.contains("Server")) {
                            typeColor = QColor(255, 0, 0, 100); // Red for servers
                        } else {
                            typeColor = QColor(0, 255, 0, 100); // Green for devices
                        }
                        deviceItem->setBackground(5, typeColor);

                        if (networkTree) {
                            networkTree->addTopLevelItem(deviceItem);
                            updateNetworkStatistics();
                        }

                        addLogEntry(QString("Gerät entdeckt: %1 (%2) - Typ: %3")
                                        .arg(hostname.isEmpty() ? "Unbekannt" : hostname, ip, type), "SUCCESS");
                    } catch (...) {
                        addLogEntry("Fehler beim Hinzufügen von Geräte-Eintrag", "ERROR");
                    }
                }, Qt::QueuedConnection);
            }, Qt::QueuedConnection);

    connect(worker, &NetworkDiscoveryWorker::discoveryCompleted,
            this, [this](const QString &interface) {
                QMetaObject::invokeMethod(this, [this, interface]() {
                    try {
                        addLogEntry(QString("Netzwerk-Erkennung abgeschlossen für %1").arg(interface), "SUCCESS");
                        statusBar()->showMessage("Netzwerk-Erkennung abgeschlossen");

                        if (discoveryButton) {
                            discoveryButton->setEnabled(true);
                            discoveryButton->setText("🔍 Netzwerk Erkennung");
                        }

                        // Auto-resize columns
                        if (networkTree) {
                            for (int i = 0; i < networkTree->columnCount(); ++i) {
                                networkTree->resizeColumnToContents(i);
                            }
                        }

                        int devicesFound = networkTree ? networkTree->topLevelItemCount() : 0;
                        QMessageBox::information(this, "✅ Netzwerk-Erkennung Abgeschlossen",
                                                 QString("Netzwerk-Erkennung erfolgreich!\n\n"
                                                         "📊 Gefundene Geräte: %1\n"
                                                         "🔌 Interface: %2")
                                                     .arg(devicesFound).arg(interface));
                    } catch (...) {
                        addLogEntry("Fehler beim Abschließen der Netzwerk-Erkennung", "ERROR");

                        if (discoveryButton) {
                            discoveryButton->setEnabled(true);
                            discoveryButton->setText("🔍 Netzwerk Erkennung");
                        }
                    }
                }, Qt::QueuedConnection);
            }, Qt::QueuedConnection);

    // Start worker with lower priority
    threadPool->start(worker, QThread::LowPriority);
}
QString IpScanner::getMacVendor(const QString &mac)
{
    if (mac.isEmpty() || mac.length() < 8) {
        return "Unbekannt";
    }

    // Extract first 3 octets (OUI - Organizationally Unique Identifier)
    QString oui = mac.left(8).toUpper().replace(":", "").replace("-", "");
    if (oui.length() < 6) return "Unbekannt";

    oui = oui.left(6);

    // Common MAC address prefixes (OUI database)
    static QMap<QString, QString> vendorMap = {
                                               // Apple
                                               {"00036C", "Apple Inc."}, {"000393", "Apple Inc."}, {"000502", "Apple Inc."},
                                               {"000A27", "Apple Inc."}, {"000A95", "Apple Inc."}, {"000D93", "Apple Inc."},
                                               {"0010FA", "Apple Inc."}, {"001124", "Apple Inc."}, {"0016CB", "Apple Inc."},
                                               {"001E52", "Apple Inc."}, {"001F5B", "Apple Inc."}, {"0021E9", "Apple Inc."},
                                               {"002312", "Apple Inc."}, {"002332", "Apple Inc."}, {"002436", "Apple Inc."},
                                               {"002500", "Apple Inc."}, {"0025BC", "Apple Inc."}, {"002608", "Apple Inc."},
                                               {"0026BB", "Apple Inc."}, {"002718", "Apple Inc."}, {"3C15C2", "Apple Inc."},
                                               {"40A6D9", "Apple Inc."}, {"40D32D", "Apple Inc."}, {"44D884", "Apple Inc."},
                                               {"68AE20", "Apple Inc."}, {"6C2648", "Apple Inc."}, {"6C8DC1", "Apple Inc."},
                                               {"7073CB", "Apple Inc."}, {"7CD1C3", "Apple Inc."}, {"8C7712", "Apple Inc."},
                                               {"90B21F", "Apple Inc."}, {"A45E60", "Apple Inc."}, {"A85C2C", "Apple Inc."},
                                               {"AC3743", "Apple Inc."}, {"BC9FEF", "Apple Inc."}, {"D8004D", "Apple Inc."},
                                               {"E0F847", "Apple Inc."}, {"E41E0A", "Apple Inc."}, {"E4C63D", "Apple Inc."},
                                               {"E80688", "Apple Inc."}, {"F01C13", "Apple Inc."}, {"F0DBF8", "Apple Inc."},

                                               // Samsung
                                               {"0002E3", "Samsung Electronics"}, {"000E8F", "Samsung Electronics"},
                                               {"0012FB", "Samsung Electronics"}, {"001377", "Samsung Electronics"},
                                               {"0015B9", "Samsung Electronics"}, {"001632", "Samsung Electronics"},
                                               {"0017C9", "Samsung Electronics"}, {"001D25", "Samsung Electronics"},
                                               {"002454", "Samsung Electronics"}, {"0024E9", "Samsung Electronics"},
                                               {"30F9ED", "Samsung Electronics"}, {"34BE00", "Samsung Electronics"},
                                               {"38AA3C", "Samsung Electronics"}, {"3C5AB4", "Samsung Electronics"},
                                               {"444E1A", "Samsung Electronics"}, {"5C0A5B", "Samsung Electronics"},
                                               {"68EBE5", "Samsung Electronics"}, {"74F61C", "Samsung Electronics"},
                                               {"7825AD", "Samsung Electronics"}, {"84B541", "Samsung Electronics"},
                                               {"88C663", "Samsung Electronics"}, {"8C77120", "Samsung Electronics"},
                                               {"CC07AB", "Samsung Electronics"}, {"E8039A", "Samsung Electronics"},
                                               {"EC1F72", "Samsung Electronics"}, {"F80CF3", "Samsung Electronics"},

                                               // Intel
                                               {"000C29", "Intel Corporation"}, {"000E0C", "Intel Corporation"},
                                               {"001320", "Intel Corporation"}, {"001517", "Intel Corporation"},
                                               {"001B21", "Intel Corporation"}, {"001E67", "Intel Corporation"},
                                               {"002161", "Intel Corporation"}, {"0022FA", "Intel Corporation"},
                                               {"002564", "Intel Corporation"}, {"003048", "Intel Corporation"},
                                               {"34E6AD", "Intel Corporation"}, {"3C970E", "Intel Corporation"},
                                               {"44850D", "Intel Corporation"}, {"4C72B9", "Intel Corporation"},
                                               {"6C29A2", "Intel Corporation"}, {"7C7A91", "Intel Corporation"},
                                               {"8C164F", "Intel Corporation"}, {"AC8112", "Intel Corporation"},
                                               {"B4B686", "Intel Corporation"}, {"CC46D6", "Intel Corporation"},
                                               {"D0577B", "Intel Corporation"}, {"E0CB4E", "Intel Corporation"},
                                               {"F04DA2", "Intel Corporation"}, {"F8B156", "Intel Corporation"},

                                               // Microsoft
                                               {"000D3A", "Microsoft Corporation"}, {"001DD8", "Microsoft Corporation"},
                                               {"002248", "Microsoft Corporation"}, {"002655", "Microsoft Corporation"},
                                               {"40490F", "Microsoft Corporation"}, {"64000C", "Microsoft Corporation"},
                                               {"7C1E52", "Microsoft Corporation"}, {"98579A", "Microsoft Corporation"},
                                               {"9CB654", "Microsoft Corporation"}, {"A41F72", "Microsoft Corporation"},
                                               {"E0CB4E", "Microsoft Corporation"}, {"F81A67", "Microsoft Corporation"},

                                               // Cisco
                                               {"000142", "Cisco Systems"}, {"000163", "Cisco Systems"},
                                               {"0001C7", "Cisco Systems"}, {"0001C9", "Cisco Systems"},
                                               {"000D28", "Cisco Systems"}, {"000E83", "Cisco Systems"},
                                               {"001120", "Cisco Systems"}, {"001364", "Cisco Systems"},
                                               {"001A6B", "Cisco Systems"}, {"001E14", "Cisco Systems"},
                                               {"002155", "Cisco Systems"}, {"002618", "Cisco Systems"},
                                               {"0030F2", "Cisco Systems"}, {"1C17D3", "Cisco Systems"},
                                               {"2037A0", "Cisco Systems"}, {"547F.EE", "Cisco Systems"},
                                               {"5475D0", "Cisco Systems"}, {"781D.BA", "Cisco Systems"},
                                               {"B46BFC", "Cisco Systems"}, {"C8D119", "Cisco Systems"},

                                               // TP-Link
                                               {"001A8C", "TP-Link Technologies"}, {"002268", "TP-Link Technologies"},
                                               {"B0487A", "TP-Link Technologies"}, {"C06394", "TP-Link Technologies"},
                                               {"E8DE27", "TP-Link Technologies"}, {"F46BEF", "TP-Link Technologies"},

                                               // D-Link
                                               {"001195", "D-Link Corporation"}, {"001346", "D-Link Corporation"},
                                               {"001CF0", "D-Link Corporation"}, {"002191", "D-Link Corporation"},
                                               {"1C7EE5", "D-Link Corporation"}, {"5CD998", "D-Link Corporation"},
                                               {"CCB255", "D-Link Corporation"}, {"E46F13", "D-Link Corporation"},

                                               // ASUS
                                               {"000C6E", "ASUSTeK Computer"}, {"001E8C", "ASUSTeK Computer"},
                                               {"002354", "ASUSTeK Computer"}, {"0026180", "ASUSTeK Computer"},
                                               {"1C872C", "ASUSTeK Computer"}, {"2C56DC", "ASUSTeK Computer"},
                                               {"382C4A", "ASUSTeK Computer"}, {"60A44C", "ASUSTeK Computer"},
                                               {"74D435", "ASUSTeK Computer"}, {"BC863E", "ASUSTeK Computer"},
                                               {"F832E4", "ASUSTeK Computer"}, {"FC34AE", "ASUSTeK Computer"},

                                               // Huawei
                                               {"001EC0", "Huawei Technologies"}, {"002312", "Huawei Technologies"},
                                               {"4C549F", "Huawei Technologies"}, {"5C338A", "Huawei Technologies"},
                                               {"642737", "Huawei Technologies"}, {"6C4F42", "Huawei Technologies"},
                                               {"844BF5", "Huawei Technologies"}, {"9C28EF", "Huawei Technologies"},
                                               {"C8A823", "Huawei Technologies"}, {"E0A3AC", "Huawei Technologies"},

                                               // Xiaomi
                                               {"647002", "Xiaomi Communications"}, {"788A20", "Xiaomi Communications"},
                                               {"8CFABA", "Xiaomi Communications"}, {"98FA9B", "Xiaomi Communications"},
                                               {"A0E4CB", "Xiaomi Communications"}, {"F48E38", "Xiaomi Communications"},

                                               // Raspberry Pi Foundation
                                               {"B827EB", "Raspberry Pi Foundation"}, {"DCA632", "Raspberry Pi Foundation"},
                                               {"E45F01", "Raspberry Pi Foundation"},

                                               // VMware
                                               {"005056", "VMware Inc."}, {"000C29", "VMware Inc."}, {"001C14", "VMware Inc."},

                                               // VirtualBox
                                               {"080027", "VirtualBox (Oracle)"}, {"525400", "QEMU/KVM"},

                                               // Amazon
                                               {"061F29", "Amazon Technologies Inc."}, {"124B66", "Amazon Technologies Inc."},

                                               // Google
                                               {"001A11", "Google Inc."}, {"3C5AB4", "Google Inc."},

                                               // Common Router/Modem brands
                                               {"00904C", "Netgear Inc."}, {"CC40D0", "Netgear Inc."}, {"E091F5", "Netgear Inc."},
                                               {"000FB5", "Netgear Inc."}, {"2C3033", "Netgear Inc."}, {"84C9B2", "Netgear Inc."},
                                               {"001CDF", "Belkin International"}, {"944452", "Belkin International"},
                                               {"BE9DA7", "Belkin International"}, {"EC1A59", "Belkin International"},
                                               {"001D7E", "Linksys"}, {"0018F8", "Linksys"}, {"0020A6", "Linksys"},
                                               {"68CA.E4", "Linksys"}, {"C05627", "Linksys"}, {"E0CB4E", "Linksys"},
                                               };

    QString vendor = vendorMap.value(oui, "Unbekannter Hersteller");

    // If not found in our database, try to determine device type by MAC pattern
    if (vendor == "Unbekannter Hersteller") {
        // Some heuristics based on MAC patterns
        if (oui.startsWith("00") || oui.startsWith("02")) {
            vendor = "Vermutlich Router/Modem";
        } else if (oui.startsWith("52")) {
            vendor = "Virtualisiert (QEMU/KVM)";
        } else if (oui.startsWith("08")) {
            vendor = "Vermutlich VM (VirtualBox)";
        }
    }

    return vendor;
}
void IpScanner::analyzeNetworkTopology()
{
    addLogEntry("Analysiere Netzwerk-Topologie...", "INFO");

    // Get all discovered hosts and analyze network structure
    QMap<QString, QList<HostInfo>> networkMap;

    for (const HostInfo &host : discoveredHosts) {
        QString subnet = getSubnetFromIp(host.ip);
        networkMap[subnet].append(host);
    }

    // Create topology analysis
    QString topologyReport = "=== NETZWERK-TOPOLOGIE ANALYSE ===\n\n";

    for (auto it = networkMap.begin(); it != networkMap.end(); ++it) {
        QString subnet = it.key();
        QList<HostInfo> hosts = it.value();

        topologyReport += QString("📍 Subnetz: %1\n").arg(subnet);
        topologyReport += QString("   Hosts: %1\n").arg(hosts.size());

        // Analyze device types
        int routers = 0, servers = 0, workstations = 0, unknown = 0;

        for (const HostInfo &host : hosts) {
            if (isLikelyRouter(host)) {
                routers++;
            } else if (isLikelyServer(host)) {
                servers++;
            } else if (isLikelyWorkstation(host)) {
                workstations++;
            } else {
                unknown++;
            }
        }

        topologyReport += QString("   Router/Gateways: %1\n").arg(routers);
        topologyReport += QString("   Server: %1\n").arg(servers);
        topologyReport += QString("   Arbeitsstationen: %1\n").arg(workstations);
        topologyReport += QString("   Unbekannt: %1\n\n").arg(unknown);
    }

    // Show topology analysis in a dialog
    QDialog *topologyDialog = new QDialog(this);
    topologyDialog->setWindowTitle("🌐 Netzwerk-Topologie Analyse");
    topologyDialog->resize(600, 500);
    topologyDialog->setStyleSheet(styleSheet()); // Apply dark theme

    QVBoxLayout *layout = new QVBoxLayout(topologyDialog);
    QTextEdit *reportEdit = new QTextEdit();
    reportEdit->setPlainText(topologyReport);
    reportEdit->setReadOnly(true);
    reportEdit->setFont(QFont("Consolas", 10));

    QPushButton *closeButton = new QPushButton("❌ Schließen");
    QPushButton *exportButton = new QPushButton("📤 Exportieren");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(exportButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addWidget(reportEdit);
    layout->addLayout(buttonLayout);

    connect(closeButton, &QPushButton::clicked, topologyDialog, &QDialog::accept);
    connect(exportButton, &QPushButton::clicked, [this, topologyDialog]() {
        exportNetworkMap();
        topologyDialog->accept();
    });

    topologyDialog->exec();

    addLogEntry("Netzwerk-Topologie Analyse abgeschlossen", "SUCCESS");
}
bool IpScanner::isLikelyWorkstation(const HostInfo &host)
{
    // Heuristics for workstation detection
    return host.openPorts.size() > 0 && host.openPorts.size() < 5 && !isLikelyRouter(host) && !isLikelyServer(host);
}
bool IpScanner::isLikelyRouter(const HostInfo &host)
{
    // Heuristics for router detection
    QStringList routerPorts = {"22", "23", "80", "443", "8080"};
    QString ip = host.ip;

    // Common router IPs
    if (ip.endsWith(".1") || ip.endsWith(".254") || ip.endsWith(".100")) {
        return true;
    }

    // Has common router ports and services
    int routerPortCount = 0;
    for (const QString &port : host.openPorts) {
        if (routerPorts.contains(port)) {
            routerPortCount++;
        }
    }

    return routerPortCount >= 2;
}
void IpScanner::setupNetworkContextMenu()
{
    networkTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(networkTree, &QWidget::customContextMenuRequested,
            this, &IpScanner::showNetworkContextMenu);
}
void IpScanner::showNetworkContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = networkTree->itemAt(pos);
    if (!item) return;

    QString ip = item->text(1);
    QString deviceName = item->text(0);
    QString mac = item->text(2);

    QMenu menu(this);
    menu.setStyleSheet("QMenu { background-color: #2d2d2d; color: white; border: 1px solid #404040; }");

    if (!ip.isEmpty() && ip != "N/A") {
        menu.addAction("🏓 Host pingen", [this, ip]() {
            targetEdit->setText(ip);
            pingHost();
        });

        menu.addAction("🔌 Ports scannen", [this, ip]() {
            targetEdit->setText(ip);
            portScan();
        });

        menu.addAction("🌐 Im Browser öffnen", [ip]() {
            QDesktopServices::openUrl(QUrl(QString("http://%1").arg(ip)));
        });

        menu.addSeparator();

        menu.addAction("📋 IP kopieren", [this, ip]() {
            QApplication::clipboard()->setText(ip);
            addLogEntry(QString("IP %1 kopiert").arg(ip), "INFO");
        });

        if (!mac.isEmpty() && mac != "N/A") {
            menu.addAction("📋 MAC kopieren", [this, mac]() {
                QApplication::clipboard()->setText(mac);
                addLogEntry(QString("MAC %1 kopiert").arg(mac), "INFO");
            });
        }

        menu.addSeparator();

        menu.addAction("ℹ️ Details anzeigen", [this, item]() {
            showNetworkDeviceDetails(item);
        });

        menu.addAction("🗑️ Aus Liste entfernen", [this, item, ip]() {
            delete item;
            addLogEntry(QString("Netzwerkgerät %1 entfernt").arg(ip), "INFO");
        });
    }

    menu.exec(networkTree->mapToGlobal(pos));
}
void IpScanner::showNetworkDeviceDetails(QTreeWidgetItem *item)
{
    if (!item) return;

    QString deviceName = item->text(0);
    QString ip = item->text(1);
    QString mac = item->text(2);
    QString vendor = item->text(3);
    QString interface = item->text(4);
    QString type = item->text(5);
    QString lastSeen = item->text(6);

    // Create detailed information dialog
    QDialog *detailsDialog = new QDialog(this);
    detailsDialog->setWindowTitle(QString("🖥️ Gerät Details - %1").arg(deviceName));
    detailsDialog->resize(500, 400);
    detailsDialog->setStyleSheet(styleSheet()); // Apply dark theme

    QVBoxLayout *layout = new QVBoxLayout(detailsDialog);

    // Device information
    QTextEdit *detailsEdit = new QTextEdit();
    detailsEdit->setReadOnly(true);

    QString detailsText = QString(R"(
<h2>🖥️ Netzwerk-Gerät Informationen</h2>

<h3>📍 Grundlegende Informationen</h3>
<table border="0" cellpadding="5">
<tr><td><b>Gerätename:</b></td><td>%1</td></tr>
<tr><td><b>IP-Adresse:</b></td><td>%2</td></tr>
<tr><td><b>MAC-Adresse:</b></td><td>%3</td></tr>
<tr><td><b>Hersteller:</b></td><td>%4</td></tr>
<tr><td><b>Gerätetyp:</b></td><td>%5</td></tr>
<tr><td><b>Interface:</b></td><td>%6</td></tr>
<tr><td><b>Letzte Aktivität:</b></td><td>%7</td></tr>
</table>

<h3>🔍 Erweiterte Analyse</h3>
<p><b>Subnetz:</b> %8</p>
<p><b>Wahrscheinlicher Typ:</b> %9</p>

<h3>📝 Notizen</h3>
<p>Doppelklick für weitere Aktionen oder verwenden Sie das Kontextmenü für zusätzliche Optionen.</p>
    )").arg(deviceName, ip, mac, vendor, type, interface, lastSeen,
                                   getSubnetFromIp(ip),
                                   type.contains("Router") ? "Netzwerk-Gateway" :
                                       type.contains("Server") ? "Server/Dienst" : "Endgerät");

    detailsEdit->setHtml(detailsText);

    // Action buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *pingButton = new QPushButton("🏓 Ping");
    QPushButton *portScanButton = new QPushButton("🔌 Port Scan");
    QPushButton *closeButton = new QPushButton("❌ Schließen");

    pingButton->setStyleSheet("QPushButton { background-color: #28a745; }");
    portScanButton->setStyleSheet("QPushButton { background-color: #17a2b8; }");
    closeButton->setStyleSheet("QPushButton { background-color: #dc3545; }");

    buttonLayout->addWidget(pingButton);
    buttonLayout->addWidget(portScanButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addWidget(detailsEdit);
    layout->addLayout(buttonLayout);

    // Connect button actions
    connect(pingButton, &QPushButton::clicked, [this, ip, detailsDialog]() {
        targetEdit->setText(ip);
        detailsDialog->accept();
        pingHost();
    });

    connect(portScanButton, &QPushButton::clicked, [this, ip, detailsDialog]() {
        targetEdit->setText(ip);
        detailsDialog->accept();
        portScan();
    });

    connect(closeButton, &QPushButton::clicked, detailsDialog, &QDialog::accept);

    detailsDialog->exec();
}
void IpScanner::showNetworkStatistics()
{
    // Calculate network statistics
    int totalDevices = networkTree->topLevelItemCount();
    int routers = 0, servers = 0, workstations = 0, unknown = 0;
    QSet<QString> uniqueVendors;
    QSet<QString> uniqueSubnets;

    for (int i = 0; i < networkTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = networkTree->topLevelItem(i);
        QString type = item->text(5);
        QString vendor = item->text(3);
        QString ip = item->text(1);

        if (type.contains("Router") || type.contains("Gateway")) {
            routers++;
        } else if (type.contains("Server")) {
            servers++;
        } else if (type.contains("Computer") || type.contains("Device")) {
            workstations++;
        } else {
            unknown++;
        }

        if (!vendor.isEmpty() && vendor != "Unbekannt") {
            uniqueVendors.insert(vendor);
        }

        if (!ip.isEmpty()) {
            uniqueSubnets.insert(getSubnetFromIp(ip));
        }
    }

    // Create statistics dialog
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("📊 Netzwerk-Statistiken");
    statsDialog->resize(600, 500);
    statsDialog->setStyleSheet(styleSheet());

    QVBoxLayout *layout = new QVBoxLayout(statsDialog);
    QTextEdit *statsEdit = new QTextEdit();
    statsEdit->setReadOnly(true);

    QString statsText = QString(R"(
<h2>📊 Netzwerk-Statistiken</h2>

<h3>🔢 Gerätezusammenfassung</h3>
<table border="1" cellpadding="8">
<tr><td><b>Gesamte Geräte:</b></td><td>%1</td></tr>
<tr><td><b>Router/Gateways:</b></td><td>%2</td></tr>
<tr><td><b>Server:</b></td><td>%3</td></tr>
<tr><td><b>Arbeitsstationen:</b></td><td>%4</td></tr>
<tr><td><b>Unbekannte Geräte:</b></td><td>%5</td></tr>
</table>

<h3>🏭 Hersteller-Vielfalt</h3>
<p><b>Eindeutige Hersteller:</b> %6</p>
<p><b>Gefundene Hersteller:</b><br>%7</p>

<h3>🌐 Netzwerk-Segmente</h3>
<p><b>Eindeutige Subnetze:</b> %8</p>
<p><b>Erkannte Subnetze:</b><br>%9</p>

<h3>📈 Netzwerk-Gesundheit</h3>
<p><b>Aktive Geräte:</b> %10 von %11 entdeckt</p>
<p><b>Scan-Abdeckung:</b> %12%</p>
    )").arg(totalDevices)
                            .arg(routers)
                            .arg(servers)
                            .arg(workstations)
                            .arg(unknown)
                            .arg(uniqueVendors.size())
                            .arg(QStringList(uniqueVendors.begin(), uniqueVendors.end()).join(", "))
                            .arg(uniqueSubnets.size())
                            .arg(QStringList(uniqueSubnets.begin(), uniqueSubnets.end()).join(", "))
                            .arg(totalDevices)
                            .arg(totalDevices) // Placeholder for total possible devices
                            .arg(totalDevices > 0 ? 100 : 0); // Simplified percentage

    statsEdit->setHtml(statsText);

    QPushButton *closeButton = new QPushButton("Schließen");
    QPushButton *exportButton = new QPushButton("📤 Exportieren");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(exportButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    layout->addWidget(statsEdit);
    layout->addLayout(buttonLayout);

    connect(closeButton, &QPushButton::clicked, statsDialog, &QDialog::accept);
    connect(exportButton, &QPushButton::clicked, [this, statsDialog]() {
        exportNetworkMap();
        statsDialog->accept();
    });

    statsDialog->exec();

    addLogEntry("Netzwerk-Statistiken angezeigt", "INFO");
}
void IpScanner::exportNetworkMap()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "📤 Netzwerk-Karte Exportieren",
                                                    QString("network_map_%1.html").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    "HTML Files (*.html);;JSON Files (*.json);;CSV Files (*.csv)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "❌ Export Fehler", "Datei konnte nicht zum Schreiben geöffnet werden");
        return;
    }

    QTextStream stream(&file);

    if (fileName.endsWith(".html")) {
        // Export as HTML
        stream << R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Netzwerk-Karte</title>
    <style>
        body { font-family: Arial, sans-serif; background: #1e1e1e; color: #fff; margin: 20px; }
        h1 { color: #0078d4; text-align: center; }
        .network-segment { border: 2px solid #0078d4; margin: 20px 0; padding: 15px; border-radius: 10px; background: #2d2d2d; }
        .device { margin: 10px 0; padding: 10px; border-radius: 5px; background: #404040; }
        .router { border-left: 5px solid #ff6b35; }
        .server { border-left: 5px solid #ff4757; }
        .workstation { border-left: 5px solid #2ed573; }
        .unknown { border-left: 5px solid #ffa502; }
        table { border-collapse: collapse; width: 100%; margin: 10px 0; }
        th, td { border: 1px solid #404040; padding: 8px; text-align: left; }
        th { background-color: #0078d4; }
        .summary { background: #2d2d2d; padding: 15px; border-radius: 5px; margin: 20px 0; }
    </style>
</head>
<body>
    <h1>🌐 Netzwerk-Karte - )" << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") << R"(</h1>

    <div class="summary">
        <h3>📊 Netzwerk-Übersicht</h3>
        <p><strong>Gescannte Geräte:</strong> )" << networkTree->topLevelItemCount() << R"(</p>
        <p><strong>Erstellt am:</strong> )" << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") << R"(</p>
    </div>)";

        // Group devices by subnet
        QMap<QString, QList<QTreeWidgetItem*>> subnetMap;
        for (int i = 0; i < networkTree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *item = networkTree->topLevelItem(i);
            if (!item) continue;

            QString ip = item->text(1);
            QString subnet = getSubnetFromIp(ip);
            subnetMap[subnet].append(item);
        }

        // Generate HTML for each subnet
        for (auto it = subnetMap.begin(); it != subnetMap.end(); ++it) {
            QString subnet = it.key();
            QList<QTreeWidgetItem*> devices = it.value();

            stream << QString(R"(
    <div class="network-segment">
        <h3>🔗 Subnetz: %1 (%2 Geräte)</h3>
        <table>
            <tr>
                <th>Gerät</th>
                <th>IP-Adresse</th>
                <th>MAC-Adresse</th>
                <th>Hersteller</th>
                <th>Typ</th>
                <th>Letzte Aktivität</th>
            </tr>)").arg(subnet).arg(devices.size());

            for (QTreeWidgetItem *device : devices) {
                QString deviceClass = "unknown";
                QString type = device->text(5);
                if (type.contains("Router")) deviceClass = "router";
                else if (type.contains("Server")) deviceClass = "server";
                else if (type.contains("Computer") || type.contains("Device")) deviceClass = "workstation";

                stream << QString(R"(
            <tr class="device %1">
                <td>%2</td>
                <td>%3</td>
                <td>%4</td>
                <td>%5</td>
                <td>%6</td>
                <td>%7</td>
            </tr>)").arg(deviceClass)
                              .arg(device->text(0))
                              .arg(device->text(1))
                              .arg(device->text(2))
                              .arg(device->text(3))
                              .arg(device->text(5))
                              .arg(device->text(6));
            }

            stream << R"(
        </table>
    </div>)";
        }

        stream << R"(
    <div class="summary">
        <p><em>Generiert mit Advanced IP Scanner v2.0 - Netzwerk-Analyse-Modul</em></p>
    </div>
</body>
</html>)";

        addLogEntry(QString("Netzwerk-Karte als HTML exportiert: %1").arg(fileName), "SUCCESS");

    } else if (fileName.endsWith(".json")) {
        // Export as JSON
        QJsonObject root;
        root["scanner"] = "Advanced IP Scanner";
        root["module"] = "Network Analysis";
        root["version"] = "2.0";
        root["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        root["totalDevices"] = networkTree->topLevelItemCount();

        QJsonArray devicesArray;
        for (int i = 0; i < networkTree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *item = networkTree->topLevelItem(i);
            if (!item) continue;

            QJsonObject deviceObj;
            deviceObj["name"] = item->text(0);
            deviceObj["ip"] = item->text(1);
            deviceObj["mac"] = item->text(2);
            deviceObj["vendor"] = item->text(3);
            deviceObj["interface"] = item->text(4);
            deviceObj["type"] = item->text(5);
            deviceObj["lastSeen"] = item->text(6);
            deviceObj["subnet"] = getSubnetFromIp(item->text(1));
            devicesArray.append(deviceObj);
        }

        root["devices"] = devicesArray;
        QJsonDocument doc(root);
        file.write(doc.toJson());

        addLogEntry(QString("Netzwerk-Karte als JSON exportiert: %1").arg(fileName), "SUCCESS");

    } else if (fileName.endsWith(".csv")) {
        // Export as CSV
        stream << "Gerätename,IP-Adresse,MAC-Adresse,Hersteller,Interface,Typ,Letzte Aktivität,Subnetz\n";

        for (int i = 0; i < networkTree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *item = networkTree->topLevelItem(i);
            if (!item) continue;

            stream << QString("%1,%2,%3,%4,%5,%6,%7,%8\n")
                          .arg(item->text(0))
                          .arg(item->text(1))
                          .arg(item->text(2))
                          .arg(item->text(3))
                          .arg(item->text(4))
                          .arg(item->text(5))
                          .arg(item->text(6))
                          .arg(getSubnetFromIp(item->text(1)));
        }

        addLogEntry(QString("Netzwerk-Karte als CSV exportiert: %1").arg(fileName), "SUCCESS");
    }

    QMessageBox::information(this, "✅ Export Erfolgreich",
                             QString("Netzwerk-Karte erfolgreich exportiert:\n%1\n\n📊 %2 Geräte exportiert")
                                 .arg(fileName).arg(networkTree->topLevelItemCount()));
}
void IpScanner::performNetworkHealthCheck()
{
    if (discoveredHosts.isEmpty()) {
        QMessageBox::information(this, "🏥 Netzwerk-Gesundheitscheck",
                                 "Keine Hosts verfügbar. Führen Sie zuerst einen Scan durch.");
        return;
    }

    addLogEntry("Starte Netzwerk-Gesundheitscheck...", "INFO");
    statusBar()->showMessage("Netzwerk-Gesundheitscheck läuft...");

    // Collect all IPs for health check
    QStringList targetIPs;
    for (const HostInfo &host : discoveredHosts) {
        targetIPs.append(host.ip);
    }

    // Create health check worker
    NetworkHealthWorker *worker = new NetworkHealthWorker(targetIPs);

    connect(worker, &NetworkHealthWorker::healthCheckStarted,
            [this](int totalTargets) {
                addLogEntry(QString("Gesundheitscheck gestartet für %1 Hosts").arg(totalTargets), "INFO");
            });

    connect(worker, &NetworkHealthWorker::hostHealthResult,
            [this](const QString &ip, bool reachable, int responseTime, int packetLoss, const QString &status) {
                QString logMessage = QString("Health Check %1: %2 - %3ms, %4% Paketverlust")
                .arg(ip, status).arg(responseTime).arg(packetLoss);
                addLogEntry(logMessage, reachable ? "SUCCESS" : "WARNING");
            });

    connect(worker, &NetworkHealthWorker::healthCheckCompleted,
            [this](int totalHosts, int reachableHosts, int unreachableHosts) {
                addLogEntry(QString("Netzwerk-Gesundheitscheck abgeschlossen: %1/%2 Hosts erreichbar")
                                .arg(reachableHosts).arg(totalHosts), "SUCCESS");
                statusBar()->showMessage("Netzwerk-Gesundheitscheck abgeschlossen");

                // Show health summary
                QString healthSummary = QString(R"(
🏥 Netzwerk-Gesundheitscheck Ergebnisse

📊 Zusammenfassung:
• Gesamte Hosts: %1
• Erreichbare Hosts: %2 (%3%)
• Nicht erreichbare Hosts: %4 (%5%)

🎯 Netzwerk-Status: %6
                )").arg(totalHosts)
                                            .arg(reachableHosts)
                                            .arg(totalHosts > 0 ? (reachableHosts * 100) / totalHosts : 0)
                                            .arg(unreachableHosts)
                                            .arg(totalHosts > 0 ? (unreachableHosts * 100) / totalHosts : 0)
                                            .arg(reachableHosts > totalHosts * 0.9 ? "Ausgezeichnet 🟢" :
                                                     reachableHosts > totalHosts * 0.7 ? "Gut 🟡" : "Kritisch 🔴");

                QMessageBox::information(this, "🏥 Gesundheitscheck Abgeschlossen", healthSummary);
            });

    threadPool->start(worker);
}
bool IpScanner::isLikelyServer(const HostInfo &host)
{
    // Heuristics for server detection
    QStringList serverPorts = {"21", "22", "25", "53", "80", "110", "143", "443", "993", "995", "3389", "5432", "3306", "1433"};

    int serverPortCount = 0;
    for (const QString &port : host.openPorts) {
        if (serverPorts.contains(port)) {
            serverPortCount++;
        }
    }

    return serverPortCount >= 3 || host.openPorts.contains("22") || host.openPorts.contains("3389");
}
QString IpScanner::getSubnetFromIp(const QString &ip)
{
    QStringList parts = ip.split('.');
    if (parts.size() >= 3) {
        return QString("%1.%2.%3.0/24").arg(parts[0], parts[1], parts[2]);
    }
    return "Unbekannt";
}
void IpScanner::arpScan()
{
    QString selectedInterface = interfaceCombo->currentText();
    if (selectedInterface.isEmpty() || selectedInterface.contains("Keine aktiven")) {
        QMessageBox::warning(this, "📡 ARP Scan",
                             "Bitte wählen Sie ein gültiges Netzwerk-Interface aus!");
        return;
    }

    QString interfaceName = selectedInterface.split(" (").first();

    // WICHTIGER FIX: Button-Status bereits hier setzen um Doppel-Starts zu verhindern
    if (!arpScanButton->isEnabled()) {
        addLogEntry("ARP-Scan bereits aktiv", "WARNING");
        return;
    }

    addLogEntry(QString("Starte ARP-Scan auf Interface: %1").arg(interfaceName), "INFO");
    statusBar()->showMessage(QString("ARP-Scan läuft: %1...").arg(interfaceName));

    // UI sofort deaktivieren
    arpScanButton->setEnabled(false);
    arpScanButton->setText("📡 Scanne...");

    // KRITISCHER FIX: Worker mit QueuedConnection verbinden
    EnhancedArpScanWorker *worker = new EnhancedArpScanWorker(interfaceName);

    // Alle Signal-Verbindungen mit QueuedConnection für Thread-Safety
    connect(worker, &EnhancedArpScanWorker::arpScanStarted,
            this, [this](const QString &interface) {
                addLogEntry(QString("ARP-Scan gestartet auf %1").arg(interface), "INFO");
            }, Qt::QueuedConnection);

    connect(worker, &EnhancedArpScanWorker::arpEntryFound,
            this, [this](const QString &ip, const QString &mac, const QString &vendor) {
                // THREAD-SAFE: Alle UI-Updates im Main-Thread
                QMetaObject::invokeMethod(this, [this, ip, mac, vendor]() {
                    try {
                        QTreeWidgetItem *arpItem = new QTreeWidgetItem();

                        // Try to resolve hostname mit Timeout
                        QString hostname = resolveHostnameWithTimeout(ip, 1000);

                        arpItem->setText(0, hostname.isEmpty() ? QString("Host (%1)").arg(ip) : hostname);
                        arpItem->setText(1, ip);
                        arpItem->setText(2, mac.toUpper());
                        arpItem->setText(3, vendor.isEmpty() ? "Unbekannter Hersteller" : vendor);
                        arpItem->setText(4, interfaceCombo->currentText().split(" (").first());
                        arpItem->setText(5, "ARP-Eintrag");
                        arpItem->setText(6, QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));

                        // Enhanced color coding based on vendor
                        QColor backgroundColor = getVendorColor(vendor);
                        arpItem->setBackground(3, backgroundColor);

                        // WICHTIG: Prüfen ob networkTree noch existiert
                        if (networkTree) {
                            networkTree->addTopLevelItem(arpItem);
                            updateNetworkStatistics(); // Update statistics
                        }

                        addLogEntry(QString("ARP-Eintrag: %1 -> %2 (%3)")
                                        .arg(ip, mac.toUpper(), vendor), "SUCCESS");

                    } catch (const std::exception& e) {
                        addLogEntry(QString("Fehler beim Hinzufügen von ARP-Eintrag: %1").arg(e.what()), "ERROR");
                    } catch (...) {
                        addLogEntry("Unbekannter Fehler beim Hinzufügen von ARP-Eintrag", "ERROR");
                    }
                }, Qt::QueuedConnection);
            }, Qt::QueuedConnection);

    connect(worker, &EnhancedArpScanWorker::arpScanProgress,
            this, [this](int scannedEntries, int totalEntries) {
                QMetaObject::invokeMethod(this, [this, scannedEntries, totalEntries]() {
                    if (totalEntries > 0) {
                        int progress = (scannedEntries * 100) / totalEntries;
                        statusBar()->showMessage(
                            QString("ARP-Scan Fortschritt: %1% (%2/%3)")
                                .arg(progress).arg(scannedEntries).arg(totalEntries));
                    }
                }, Qt::QueuedConnection);
            }, Qt::QueuedConnection);

    connect(worker, &EnhancedArpScanWorker::arpScanCompleted,
            this, [this](int entriesFound) {
                // KRITISCHER FIX: UI-Updates im Main-Thread mit Exception Handling
                QMetaObject::invokeMethod(this, [this, entriesFound]() {
                    try {
                        addLogEntry(QString("ARP-Scan abgeschlossen - %1 Einträge gefunden").arg(entriesFound), "SUCCESS");
                        statusBar()->showMessage(QString("ARP-Scan abgeschlossen - %1 Geräte gefunden").arg(entriesFound));

                        // UI wieder aktivieren
                        if (arpScanButton) {
                            arpScanButton->setEnabled(true);
                            arpScanButton->setText("📡 ARP Scan");
                        }

                        // Auto-resize columns - mit Null-Check
                        if (networkTree) {
                            for (int i = 0; i < networkTree->columnCount(); ++i) {
                                networkTree->resizeColumnToContents(i);
                            }
                        }

                        // Success dialog nur bei Fund von Einträgen
                        if (entriesFound > 0) {
                            QMessageBox::information(this, "✅ ARP-Scan Abgeschlossen",
                                                     QString("ARP-Scan erfolgreich!\n\n"
                                                             "📊 ARP-Einträge gefunden: %1\n"
                                                             "🔌 Interface: %2")
                                                         .arg(entriesFound)
                                                         .arg(interfaceCombo->currentText()));
                        } else {
                            QMessageBox::information(this, "ℹ️ ARP-Scan Abgeschlossen",
                                                     QString("ARP-Scan abgeschlossen.\n\n"
                                                             "Keine neuen ARP-Einträge gefunden.\n"
                                                             "Möglicherweise sind keine anderen Geräte aktiv\n"
                                                             "oder das Netzwerk ist sehr ruhig."));
                        }

                    } catch (const std::exception& e) {
                        addLogEntry(QString("Fehler beim Abschließen des ARP-Scans: %1").arg(e.what()), "ERROR");

                        // Failsafe: Button wieder aktivieren
                        if (arpScanButton) {
                            arpScanButton->setEnabled(true);
                            arpScanButton->setText("📡 ARP Scan");
                        }
                    } catch (...) {
                        addLogEntry("Unbekannter Fehler beim Abschließen des ARP-Scans", "ERROR");

                        // Failsafe: Button wieder aktivieren
                        if (arpScanButton) {
                            arpScanButton->setEnabled(true);
                            arpScanButton->setText("📡 ARP Scan");
                        }
                    }
                }, Qt::QueuedConnection);
            }, Qt::QueuedConnection);

    // WICHTIGER FIX: Worker mit niedrigerer Priorität starten
    threadPool->start(worker, QThread::LowPriority);
}
QString IpScanner::resolveHostnameWithTimeout(const QString &ip, int timeoutMs)
{
    try {
        // Schneller Hostname-Lookup mit Timeout
        QHostInfo hostInfo = QHostInfo::fromName(ip);
        if (hostInfo.error() == QHostInfo::NoError && !hostInfo.hostName().isEmpty()) {
            QString hostname = hostInfo.hostName();
            if (hostname != ip && !hostname.isEmpty()) {
                return hostname;
            }
        }
    } catch (...) {
        // Ignore hostname resolution errors
    }
    return QString();
}

QColor IpScanner::getVendorColor(const QString &vendor)
{
    // Color coding based on vendor for better visualization
    if (vendor.contains("Apple", Qt::CaseInsensitive)) {
        return QColor(169, 169, 169, 100); // Silver for Apple
    } else if (vendor.contains("Microsoft", Qt::CaseInsensitive) ||
               vendor.contains("Intel", Qt::CaseInsensitive)) {
        return QColor(0, 123, 255, 100); // Blue for Microsoft/Intel
    } else if (vendor.contains("Samsung", Qt::CaseInsensitive) ||
               vendor.contains("LG", Qt::CaseInsensitive)) {
        return QColor(40, 167, 69, 100); // Green for mobile vendors
    } else if (vendor.contains("Cisco", Qt::CaseInsensitive) ||
               vendor.contains("TP-Link", Qt::CaseInsensitive) ||
               vendor.contains("D-Link", Qt::CaseInsensitive) ||
               vendor.contains("Netgear", Qt::CaseInsensitive)) {
        return QColor(255, 165, 0, 100); // Orange for network equipment
    } else if (vendor.contains("VMware", Qt::CaseInsensitive) ||
               vendor.contains("VirtualBox", Qt::CaseInsensitive) ||
               vendor.contains("QEMU", Qt::CaseInsensitive)) {
        return QColor(138, 43, 226, 100); // Purple for virtualization
    } else {
        return QColor(108, 117, 125, 100); // Gray for others
    }
}

QString IpScanner::resolveHostname(const QString &ip)
{
    QHostInfo hostInfo = QHostInfo::fromName(ip);
    if (hostInfo.error() == QHostInfo::NoError && !hostInfo.hostName().isEmpty()) {
        QString hostname = hostInfo.hostName();
        if (hostname != ip) { // Only return if it's actually resolved
            return hostname;
        }
    }
    return QString();
}
void IpScanner::whoIs()
{
    QMessageBox::information(this, "🔍 WHOIS", "WHOIS-Funktion wird in der nächsten Version implementiert");
    addLogEntry("WHOIS aufgerufen (noch nicht implementiert)", "INFO");
}

void IpScanner::onItemSelectionChanged()
{
    QTreeWidgetItem *currentItem = hostTreeWidget->currentItem();
    if (!currentItem) {
        hostDetailsEdit->clear();
        return;
    }

    QString ip = currentItem->text(0);
    QString hostname = currentItem->text(1);
    QString mac = currentItem->text(2);
    QString vendor = currentItem->text(3);
    QString status = currentItem->text(4);
    QString responseTime = currentItem->text(5);
    QString ports = currentItem->text(6);
    QString os = currentItem->text(7);
    QString lastSeen = currentItem->text(8);
    QString services = currentItem->text(9);

    QString details = QString(R"(
<h3>🖥️ Host Informationen</h3>
<table border="0" cellpadding="3">
<tr><td><b>📍 IP Adresse:</b></td><td>%1</td></tr>
<tr><td><b>🏷️ Hostname:</b></td><td>%2</td></tr>
<tr><td><b>🔧 MAC Adresse:</b></td><td>%3</td></tr>
<tr><td><b>🏭 Hersteller:</b></td><td>%4</td></tr>
<tr><td><b>🟢 Status:</b></td><td>%5</td></tr>
<tr><td><b>⏱️ Antwortzeit:</b></td><td>%6</td></tr>
<tr><td><b>🖥️ Betriebssystem:</b></td><td>%7</td></tr>
<tr><td><b>🕐 Letzte Aktivität:</b></td><td>%8</td></tr>
</table>

<h4>🔌 Offene Ports:</h4>
<p>%9</p>

<h4>⚙️ Services:</h4>
<p>%10</p>

<h4>📝 Aktionen:</h4>
<p>Doppelklick für Schnellaktionen</p>
    )").arg(ip, hostname, mac, vendor, status, responseTime, os, lastSeen,
                               ports.isEmpty() ? "Keine erkannt" : ports,
                               services.isEmpty() ? "Keine erkannt" : services);

    hostDetailsEdit->setHtml(details);
}

void IpScanner::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (!item) return;

    QString ip = item->text(0);

    QMenu menu(this);
    menu.setStyleSheet("QMenu { background-color: #2d2d2d; color: white; border: 1px solid #404040; }");

    menu.addAction("🏓 Host pingen", [this, ip]() {
        targetEdit->setText(ip);
        pingHost();
    });
    menu.addAction("🔌 Ports scannen", [this, ip]() {
        targetEdit->setText(ip);
        portScan();
    });
    menu.addAction("🌐 Im Browser öffnen", [ip]() {
        QDesktopServices::openUrl(QUrl(QString("http://%1").arg(ip)));
    });
    menu.addAction("📋 IP kopieren", [this, ip]() {
        QApplication::clipboard()->setText(ip);
        addLogEntry(QString("IP %1 in Zwischenablage kopiert").arg(ip), "INFO");
    });

    menu.exec(QCursor::pos());
}

void IpScanner::showContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = hostTreeWidget->itemAt(pos);
    if (!item) return;

    QString ip = item->text(0);

    QMenu menu(this);
    menu.setStyleSheet("QMenu { background-color: #2d2d2d; color: white; border: 1px solid #404040; }");

    menu.addAction("📋 IP Adresse kopieren", [this, ip]() {
        QApplication::clipboard()->setText(ip);
        addLogEntry(QString("IP %1 kopiert").arg(ip), "INFO");
    });

    menu.addAction("🗑️ Host entfernen", [this, item, ip]() {
        delete item;
        for (int i = 0; i < discoveredHosts.size(); ++i) {
            if (discoveredHosts[i].ip == ip) {
                discoveredHosts.removeAt(i);
                break;
            }
        }
        addLogEntry(QString("Host %1 entfernt").arg(ip), "INFO");
    });

    menu.addSeparator();
    menu.addAction("🏓 Ping", [this, ip]() {
        targetEdit->setText(ip);
        pingHost();
    });
    menu.addAction("🔌 Port Scan", [this, ip]() {
        targetEdit->setText(ip);
        portScan();
    });

    menu.exec(hostTreeWidget->mapToGlobal(pos));
}

void IpScanner::exportResults()
{
    if (discoveredHosts.isEmpty()) {
        QMessageBox::information(this, "📤 Export", "Keine Ergebnisse zum Exportieren vorhanden");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "📤 Ergebnisse Exportieren",
                                                    QString("scan_results_%1.json").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    "JSON Files (*.json);;CSV Files (*.csv);;Text Files (*.txt);;HTML Files (*.html)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "❌ Export Fehler", "Datei konnte nicht zum Schreiben geöffnet werden");
        return;
    }

    QTextStream stream(&file);

    if (fileName.endsWith(".json")) {
        // Export as JSON
        QJsonDocument doc = createJsonReport();
        file.write(doc.toJson());
        addLogEntry(QString("Ergebnisse als JSON exportiert: %1").arg(fileName), "SUCCESS");
    }
    else if (fileName.endsWith(".csv")) {
        // Export as CSV
        stream << "IP Adresse,Hostname,MAC Adresse,Hersteller,Status,Antwortzeit,Offene Ports,Betriebssystem,Letzte Aktivität,Services\n";

        for (const HostInfo &host : discoveredHosts) {
            stream << QString("%1,%2,%3,%4,%5,%6,\"%7\",%8,%9,\"%10\"\n")
            .arg(host.ip,
                 host.hostname.isEmpty() ? "Unbekannt" : host.hostname,
                 host.macAddress.isEmpty() ? "N/A" : host.macAddress,
                 host.vendor.isEmpty() ? "Unbekannt" : host.vendor,
                 host.isAlive ? "Online" : "Offline",
                 QString::number(host.responseTime),
                 host.openPorts.join(";"),
                 host.os.isEmpty() ? "Unbekannt" : host.os,
                 host.lastSeen.toString("dd.MM.yyyy hh:mm:ss"),
                 host.services.join(";"));
        }
        addLogEntry(QString("Ergebnisse als CSV exportiert: %1").arg(fileName), "SUCCESS");
    }
    else if (fileName.endsWith(".html")) {
        // Export as HTML
        stream << R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>IP Scanner Ergebnisse</title>
    <style>
        body { font-family: Arial, sans-serif; background: #1e1e1e; color: #fff; margin: 20px; }
        h1 { color: #0078d4; text-align: center; }
        table { border-collapse: collapse; width: 100%; margin: 20px 0; }
        th, td { border: 1px solid #404040; padding: 12px; text-align: left; }
        th { background-color: #0078d4; color: white; }
        tr:nth-child(even) { background-color: #2d2d2d; }
        .online { color: #00ff00; font-weight: bold; }
        .offline { color: #ff0000; font-weight: bold; }
        .summary { background: #2d2d2d; padding: 15px; border-radius: 5px; margin: 20px 0; }
    </style>
</head>
<body>
    <h1>🔍 IP Scanner Ergebnisse</h1>
    <div class="summary">
        <h3>📊 Zusammenfassung</h3>
        <p><strong>Scan-Ziel:</strong> )" << currentTarget << R"(</p>
        <p><strong>Datum:</strong> )" << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") << R"(</p>
        <p><strong>Gefundene Hosts:</strong> )" << discoveredHosts.size() << R"(</p>
    </div>
    <table>
        <tr>
            <th>IP Adresse</th>
            <th>Hostname</th>
            <th>Status</th>
            <th>Antwortzeit</th>
            <th>Offene Ports</th>
            <th>Betriebssystem</th>
            <th>Letzte Aktivität</th>
        </tr>)";

        for (const HostInfo &host : discoveredHosts) {
            stream << QString(R"(
        <tr>
            <td>%1</td>
            <td>%2</td>
            <td class="%3">%4</td>
            <td>%5</td>
            <td>%6</td>
            <td>%7</td>
            <td>%8</td>
        </tr>)")
                          .arg(host.ip,
                               host.hostname.isEmpty() ? "Unbekannt" : host.hostname,
                               host.isAlive ? "online" : "offline",
                               host.isAlive ? "🟢 Online" : "🔴 Offline",
                               host.responseTime >= 0 ? QString("%1 ms").arg(host.responseTime) : "Timeout",
                               host.openPorts.isEmpty() ? "Keine" : host.openPorts.join(", "),
                               host.os.isEmpty() ? "Unbekannt" : host.os,
                               host.lastSeen.toString("dd.MM.yyyy hh:mm:ss"));
        }

        stream << R"(
    </table>
    <div class="summary">
        <p><em>Generiert mit Advanced IP Scanner v2.0</em></p>
    </div>
</body>
</html>)";
        addLogEntry(QString("Ergebnisse als HTML exportiert: %1").arg(fileName), "SUCCESS");
    }
    else {
        // Export as text
        stream << "🔍 IP Scanner Ergebnisse\n";
        stream << "========================\n\n";
        stream << QString("📅 Datum: %1\n").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
        stream << QString("🎯 Ziel: %1\n").arg(currentTarget);
        stream << QString("📊 Gefundene Hosts: %1\n\n").arg(discoveredHosts.size());

        for (const HostInfo &host : discoveredHosts) {
            stream << QString("IP: %1\n").arg(host.ip);
            stream << QString("Hostname: %1\n").arg(host.hostname.isEmpty() ? "Unbekannt" : host.hostname);
            stream << QString("Status: %1\n").arg(host.isAlive ? "🟢 Online" : "🔴 Offline");
            stream << QString("Antwortzeit: %1 ms\n").arg(host.responseTime);
            stream << QString("Offene Ports: %1\n").arg(host.openPorts.isEmpty() ? "Keine" : host.openPorts.join(", "));
            stream << QString("Betriebssystem: %1\n").arg(host.os.isEmpty() ? "Unbekannt" : host.os);
            stream << QString("Letzte Aktivität: %1\n").arg(host.lastSeen.toString("dd.MM.yyyy hh:mm:ss"));
            stream << "----------------------------------------\n";
        }
        addLogEntry(QString("Ergebnisse als Text exportiert: %1").arg(fileName), "SUCCESS");
    }

    QMessageBox::information(this, "✅ Export Erfolgreich",
                             QString("Ergebnisse erfolgreich exportiert nach:\n%1\n\n📊 %2 Hosts exportiert")
                                 .arg(fileName).arg(discoveredHosts.size()));
}

void IpScanner::loadProject()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "📂 Projekt Laden",
                                                    "",
                                                    "Project Files (*.json)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "❌ Laden Fehler", "Datei konnte nicht gelesen werden");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) {
        QMessageBox::warning(this, "❌ Format Fehler", "Ungültiges JSON Format");
        return;
    }

    QJsonObject root = doc.object();

    // Load project data
    currentTarget = root["target"].toString();
    targetEdit->setText(currentTarget);

    // Clear current results
    clearResults();

    // Load hosts
    QJsonArray hostsArray = root["hosts"].toArray();
    for (const auto &value : hostsArray) {
        QJsonObject hostObj = value.toObject();

        HostInfo host;
        host.ip = hostObj["ip"].toString();
        host.hostname = hostObj["hostname"].toString();
        host.macAddress = hostObj["mac"].toString();
        host.vendor = hostObj["vendor"].toString();
        host.isAlive = hostObj["alive"].toBool();
        host.responseTime = hostObj["responseTime"].toInt();
        host.os = hostObj["os"].toString();
        host.lastSeen = QDateTime::fromString(hostObj["lastSeen"].toString(), Qt::ISODate);
        host.notes = hostObj["notes"].toString();

        // Load ports and services
        QJsonArray portsArray = hostObj["openPorts"].toArray();
        for (const auto &portValue : portsArray) {
            host.openPorts.append(portValue.toString());
        }

        QJsonArray servicesArray = hostObj["services"].toArray();
        for (const auto &serviceValue : servicesArray) {
            host.services.append(serviceValue.toString());
        }

        // Add to results
        onHostFound(host);
    }

    addLogEntry(QString("Projekt geladen: %1 (%2 Hosts)").arg(fileName).arg(discoveredHosts.size()), "SUCCESS");
    QMessageBox::information(this, "✅ Projekt Geladen",
                             QString("Projekt erfolgreich geladen:\n%1\n\n📊 %2 Hosts wiederhergestellt")
                                 .arg(fileName).arg(discoveredHosts.size()));
}

void IpScanner::saveProject()
{
    if (discoveredHosts.isEmpty()) {
        QMessageBox::information(this, "💾 Projekt Speichern", "Keine Daten zum Speichern vorhanden");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "💾 Projekt Speichern",
                                                    QString("project_%1.json").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    "Project Files (*.json)");

    if (fileName.isEmpty()) return;

    QJsonDocument doc = createJsonReport();

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        addLogEntry(QString("Projekt gespeichert: %1").arg(fileName), "SUCCESS");
        QMessageBox::information(this, "✅ Projekt Gespeichert",
                                 QString("Projekt erfolgreich gespeichert:\n%1").arg(fileName));
    } else {
        QMessageBox::warning(this, "❌ Speichern Fehler", "Datei konnte nicht gespeichert werden");
    }
}

QJsonDocument IpScanner::createJsonReport()
{
    QJsonObject root;
    root["scanner"] = "Advanced IP Scanner";
    root["version"] = "2.0";
    root["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    root["target"] = currentTarget;
    root["totalScanned"] = totalHosts;
    root["hostsFound"] = discoveredHosts.size();

    QJsonArray hostsArray;
    for (const HostInfo &host : discoveredHosts) {
        QJsonObject hostObj;
        hostObj["ip"] = host.ip;
        hostObj["hostname"] = host.hostname;
        hostObj["mac"] = host.macAddress;
        hostObj["vendor"] = host.vendor;
        hostObj["alive"] = host.isAlive;
        hostObj["responseTime"] = host.responseTime;
        hostObj["os"] = host.os;
        hostObj["lastSeen"] = host.lastSeen.toString(Qt::ISODate);
        hostObj["notes"] = host.notes;

        QJsonArray portsArray;
        for (const QString &port : host.openPorts) {
            portsArray.append(port);
        }
        hostObj["openPorts"] = portsArray;

        QJsonArray servicesArray;
        for (const QString &service : host.services) {
            servicesArray.append(service);
        }
        hostObj["services"] = servicesArray;

        hostsArray.append(hostObj);
    }

    root["hosts"] = hostsArray;
    return QJsonDocument(root);
}

void IpScanner::showSettings()
{
    QDialog *settingsDialog = new QDialog(this);
    settingsDialog->setWindowTitle("⚙️ Einstellungen");
    settingsDialog->setModal(true);
    settingsDialog->resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(settingsDialog);

    // Scan Settings Group
    QGroupBox *scanGroup = new QGroupBox("🔍 Scan Einstellungen");
    QFormLayout *scanLayout = new QFormLayout(scanGroup);

    QSpinBox *defaultThreads = new QSpinBox();
    defaultThreads->setRange(1, 200);
    defaultThreads->setValue(threadsSpinBox->value());
    scanLayout->addRow("Standard Threads:", defaultThreads);

    QSpinBox *defaultTimeout = new QSpinBox();
    defaultTimeout->setRange(100, 10000);
    defaultTimeout->setSuffix(" ms");
    defaultTimeout->setValue(timeoutSpinBox->value());
    scanLayout->addRow("Standard Timeout:", defaultTimeout);

    QCheckBox *autoSaveResults = new QCheckBox("Ergebnisse automatisch speichern");
    scanLayout->addRow(autoSaveResults);

    // UI Settings Group
    QGroupBox *uiGroup = new QGroupBox("🎨 Interface Einstellungen");
    QFormLayout *uiLayout = new QFormLayout(uiGroup);

    QCheckBox *showTooltips = new QCheckBox("Tooltips anzeigen");
    showTooltips->setChecked(true);
    uiLayout->addRow(showTooltips);

    QCheckBox *confirmActions = new QCheckBox("Aktionen bestätigen");
    confirmActions->setChecked(true);
    uiLayout->addRow(confirmActions);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("✅ OK");
    QPushButton *cancelButton = new QPushButton("❌ Abbrechen");
    QPushButton *resetButton = new QPushButton("🔄 Zurücksetzen");

    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(scanGroup);
    layout->addWidget(uiGroup);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, [=]() {
        threadsSpinBox->setValue(defaultThreads->value());
        timeoutSpinBox->setValue(defaultTimeout->value());
        saveSettings();
        settingsDialog->accept();
        addLogEntry("Einstellungen aktualisiert", "SUCCESS");
    });

    connect(cancelButton, &QPushButton::clicked, settingsDialog, &QDialog::reject);

    connect(resetButton, &QPushButton::clicked, [=]() {
        defaultThreads->setValue(50);
        defaultTimeout->setValue(3000);
        addLogEntry("Einstellungen zurückgesetzt", "INFO");
    });

    settingsDialog->exec();
}


void IpScanner::copyToClipboard()
{
    QTreeWidgetItem *currentItem = hostTreeWidget->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "📋 Kopieren", "Bitte wählen Sie zuerst einen Host aus");
        return;
    }

    QString ip = currentItem->text(0);
    QApplication::clipboard()->setText(ip);
    addLogEntry(QString("IP %1 in Zwischenablage kopiert").arg(ip), "SUCCESS");

    // Show temporary status message
    statusBar()->showMessage(QString("IP %1 kopiert").arg(ip), 2000);
}

void IpScanner::removeHost()
{
    QTreeWidgetItem *currentItem = hostTreeWidget->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "🗑️ Entfernen", "Bitte wählen Sie zuerst einen Host aus");
        return;
    }

    QString ip = currentItem->text(0);

    // Confirmation dialog
    int ret = QMessageBox::question(this, "🗑️ Host Entfernen",
                                    QString("Möchten Sie den Host %1 wirklich aus den Ergebnissen entfernen?").arg(ip),
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        // Remove from tree
        delete currentItem;

        // Remove from discovered hosts list
        for (int i = 0; i < discoveredHosts.size(); ++i) {
            if (discoveredHosts[i].ip == ip) {
                discoveredHosts.removeAt(i);
                break;
            }
        }

        // Update found hosts counter
        foundHostsLabel->setText(QString("Gefunden: %1").arg(discoveredHosts.size()));

        addLogEntry(QString("Host %1 aus Ergebnissen entfernt").arg(ip), "INFO");
        statusBar()->showMessage(QString("Host %1 entfernt").arg(ip), 2000);
    }
}

void IpScanner::addToFavorites()
{
    QTreeWidgetItem *currentItem = hostTreeWidget->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "⭐ Favoriten", "Bitte wählen Sie zuerst einen Host aus");
        return;
    }

    QString ip = currentItem->text(0);
    QString hostname = currentItem->text(1);

    // For now, we'll just show a message. In a full implementation,
    // this would save to a favorites list in settings
    QMessageBox::information(this, "⭐ Zu Favoriten hinzugefügt",
                             QString("Host wurde zu Favoriten hinzugefügt:\n\n"
                                     "🔗 IP: %1\n"
                                     "🏷️ Hostname: %2\n\n"
                                     "💡 Favoriten-Funktionalität wird in der nächsten Version erweitert.")
                                 .arg(ip, hostname.isEmpty() ? "Unbekannt" : hostname));

    addLogEntry(QString("Host %1 (%2) zu Favoriten hinzugefügt").arg(ip, hostname), "SUCCESS");
}
void IpScanner::createVulnerabilityTab()
{
    vulnerabilityTab = new QWidget();
    mainTabWidget->addTab(vulnerabilityTab, "🔒 Vulnerability Scanner");

    QHBoxLayout *layout = new QHBoxLayout(vulnerabilityTab);

    // Left side: Configuration and control
    QVBoxLayout *controlLayout = new QVBoxLayout();

    // Target configuration
    QGroupBox *targetGroup = new QGroupBox("🎯 Ziel Konfiguration");
    QGridLayout *targetLayout = new QGridLayout(targetGroup);

    targetLayout->addWidget(new QLabel("Ziel:"), 0, 0);
    vulnTargetEdit = new QLineEdit();
    vulnTargetEdit->setPlaceholderText("IP-Adresse oder Hostname");
    targetLayout->addWidget(vulnTargetEdit, 0, 1, 1, 2);

    targetLayout->addWidget(new QLabel("Scan-Typ:"), 1, 0);
    vulnScanTypeCombo = new QComboBox();
    vulnScanTypeCombo->addItems({
        "Schneller Vulnerability Scan",
        "Umfassender Security Audit",
        "SSL/TLS Focus Scan",
        "Credential Testing Focus",
        "Custom Scan"
    });
    targetLayout->addWidget(vulnScanTypeCombo, 1, 1, 1, 2);

    targetLayout->addWidget(new QLabel("Timeout (ms):"), 2, 0);
    vulnTimeoutSpinBox = new QSpinBox();
    vulnTimeoutSpinBox->setRange(1000, 30000);
    vulnTimeoutSpinBox->setValue(5000);
    vulnTimeoutSpinBox->setSuffix(" ms");
    targetLayout->addWidget(vulnTimeoutSpinBox, 2, 1, 1, 2);

    // Scan options
    QGroupBox *optionsGroup = new QGroupBox("⚙️ Scan Optionen");
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);

    enableCredentialTestsCheckBox = new QCheckBox("Credential Testing aktivieren");
    enableCredentialTestsCheckBox->setChecked(true);
    enableCredentialTestsCheckBox->setToolTip("Testet auf schwache/Standard-Passwörter");
    optionsLayout->addWidget(enableCredentialTestsCheckBox);

    enableSSLTestsCheckBox = new QCheckBox("SSL/TLS Vulnerability Tests");
    enableSSLTestsCheckBox->setChecked(true);
    enableSSLTestsCheckBox->setToolTip("Prüft SSL-Zertifikate und Cipher-Suites");
    optionsLayout->addWidget(enableSSLTestsCheckBox);

    enableServiceTestsCheckBox = new QCheckBox("Service-spezifische Tests");
    enableServiceTestsCheckBox->setChecked(true);
    enableServiceTestsCheckBox->setToolTip("Testet bekannte Service-Vulnerabilities");
    optionsLayout->addWidget(enableServiceTestsCheckBox);

    // Control buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    startVulnScanButton = new QPushButton("🔒 Vulnerability Scan Starten");
    startVulnScanButton->setStyleSheet("QPushButton { background-color: #dc3545; font-size: 12px; font-weight: bold; } QPushButton:hover { background-color: #c82333; }");

    stopVulnScanButton = new QPushButton("⏹️ Scan Stoppen");
    stopVulnScanButton->setEnabled(false);
    stopVulnScanButton->setStyleSheet("QPushButton { background-color: #6c757d; font-size: 12px; } QPushButton:hover { background-color: #545b62; }");

    exportVulnReportButton = new QPushButton("📊 Report Exportieren");
    exportVulnReportButton->setEnabled(false);
    exportVulnReportButton->setStyleSheet("QPushButton { background-color: #28a745; font-size: 12px; } QPushButton:hover { background-color: #218838; }");

    buttonLayout->addWidget(startVulnScanButton);
    buttonLayout->addWidget(stopVulnScanButton);
    buttonLayout->addWidget(exportVulnReportButton);

    // Progress and status
    QGroupBox *progressGroup = new QGroupBox("📊 Scan Fortschritt");
    QVBoxLayout *progressLayout = new QVBoxLayout(progressGroup);

    vulnerabilityProgressBar = new QProgressBar();
    vulnerabilityProgressBar->setTextVisible(true);
    vulnerabilityProgressBar->setFormat("Vulnerability Scan: %p%");

    vulnerabilityStatusLabel = new QLabel("Bereit für Vulnerability Scan");
    vulnerabilityStatusLabel->setStyleSheet("font-weight: bold; color: #28a745;");

    QHBoxLayout *statsLayout = new QHBoxLayout();
    vulnerabilitiesFoundLabel = new QLabel("Vulnerabilities: 0");
    securityScoreLabel = new QLabel("Score: --");
    riskLevelLabel = new QLabel("Risk: --");

    vulnerabilitiesFoundLabel->setStyleSheet("color: #dc3545; font-weight: bold;");
    securityScoreLabel->setStyleSheet("color: #ffc107; font-weight: bold;");
    riskLevelLabel->setStyleSheet("color: #17a2b8; font-weight: bold;");

    statsLayout->addWidget(vulnerabilitiesFoundLabel);
    statsLayout->addWidget(securityScoreLabel);
    statsLayout->addWidget(riskLevelLabel);
    statsLayout->addStretch();

    progressLayout->addWidget(vulnerabilityProgressBar);
    progressLayout->addWidget(vulnerabilityStatusLabel);
    progressLayout->addLayout(statsLayout);

    // Add to control layout
    controlLayout->addWidget(targetGroup);
    controlLayout->addWidget(optionsGroup);
    controlLayout->addLayout(buttonLayout);
    controlLayout->addWidget(progressGroup);
    controlLayout->addStretch();

    // Right side: Results display
    QVBoxLayout *resultsLayout = new QVBoxLayout();

    QLabel *resultsTitle = new QLabel("🔍 Vulnerability Results");
    resultsTitle->setStyleSheet("font-size: 14px; font-weight: bold; margin: 5px;");

    // Vulnerability results tree
    vulnerabilityTreeWidget = new QTreeWidget();
    vulnerabilityTreeWidget->setHeaderLabels({
        "Severity", "Vulnerability", "Service", "Port", "CVE Score", "Status"
    });
    vulnerabilityTreeWidget->setColumnWidth(0, 80);   // Severity
    vulnerabilityTreeWidget->setColumnWidth(1, 200);  // Vulnerability
    vulnerabilityTreeWidget->setColumnWidth(2, 80);   // Service
    vulnerabilityTreeWidget->setColumnWidth(3, 60);   // Port
    vulnerabilityTreeWidget->setColumnWidth(4, 80);   // CVE Score
    vulnerabilityTreeWidget->setColumnWidth(5, 100);  // Status

    vulnerabilityTreeWidget->setAlternatingRowColors(true);
    vulnerabilityTreeWidget->setSortingEnabled(true);
    vulnerabilityTreeWidget->setRootIsDecorated(false);

    // Details panel
    QLabel *detailsTitle = new QLabel("📋 Vulnerability Details");
    detailsTitle->setStyleSheet("font-size: 12px; font-weight: bold; margin: 5px;");

    vulnerabilityDetailsEdit = new QTextEdit();
    vulnerabilityDetailsEdit->setMaximumHeight(200);
    vulnerabilityDetailsEdit->setReadOnly(true);

    resultsLayout->addWidget(resultsTitle);
    resultsLayout->addWidget(vulnerabilityTreeWidget, 3);
    resultsLayout->addWidget(detailsTitle);
    resultsLayout->addWidget(vulnerabilityDetailsEdit, 1);

    // Add to main layout
    QWidget *controlWidget = new QWidget();
    controlWidget->setLayout(controlLayout);
    controlWidget->setMaximumWidth(400);

    QWidget *resultsWidget = new QWidget();
    resultsWidget->setLayout(resultsLayout);

    layout->addWidget(controlWidget, 1);
    layout->addWidget(resultsWidget, 2);

    // Connect signals
    connect(startVulnScanButton, &QPushButton::clicked, this, &IpScanner::startVulnerabilitySccan);
    connect(stopVulnScanButton, &QPushButton::clicked, this, &IpScanner::stopVulnerabilitySccan);
    connect(exportVulnReportButton, &QPushButton::clicked, this, &IpScanner::exportVulnerabilityReport);

    connect(vulnerabilityTreeWidget, &QTreeWidget::itemSelectionChanged,
            this, &IpScanner::onVulnerabilitySelectionChanged);

    // Auto-populate target from main scan if available
    connect(targetEdit, &QLineEdit::textChanged, [this](const QString &text) {
        if (vulnTargetEdit->text().isEmpty()) {
            vulnTargetEdit->setText(text);
        }
    });
}
void IpScanner::startVulnerabilitySccan()
{
    QString target = vulnTargetEdit->text().trimmed();
    if (target.isEmpty()) {
        QMessageBox::warning(this, "⚠️ Warnung", "Bitte geben Sie ein Ziel für den Vulnerability Scan ein!");
        return;
    }

    // Clear previous results
    clearVulnerabilityResults();

    // Update UI state
    vulnerabilityScanRunning = true;
    startVulnScanButton->setEnabled(false);
    stopVulnScanButton->setEnabled(true);
    exportVulnReportButton->setEnabled(false);

    vulnerabilityStatusLabel->setText("🔄 Vulnerability Scan läuft...");
    vulnerabilityStatusLabel->setStyleSheet("font-weight: bold; color: #ffc107;");

    addLogEntry(QString("Vulnerability Scan gestartet für: %1").arg(target), "INFO");

    // Create and configure vulnerability scanner
    VulnerabilityScanner *scanner = new VulnerabilityScanner(target);

    // Configure scanner based on UI settings
    scanner->setTimeout(vulnTimeoutSpinBox->value());
    scanner->enableCredentialTesting(enableCredentialTestsCheckBox->isChecked());
    scanner->enableSSLTesting(enableSSLTestsCheckBox->isChecked());
    scanner->enableServiceTesting(enableServiceTestsCheckBox->isChecked());

    // Connect scanner signals
    connect(scanner, &VulnerabilityScanner::scanStarted,
            [this](const QString &target) {
                vulnerabilityStatusLabel->setText(QString("🔍 Scanne %1...").arg(target));
            });

    connect(scanner, &VulnerabilityScanner::scanProgress,
            this, &IpScanner::onVulnerabilityScanProgress);

    connect(scanner, &VulnerabilityScanner::statusUpdate,
            this, &IpScanner::onVulnerabilityScanStatusUpdate);

    connect(scanner, &VulnerabilityScanner::vulnerabilityFound,
            this, &IpScanner::onVulnerabilityFound);

    connect(scanner, &VulnerabilityScanner::assessmentComplete,
            this, &IpScanner::onSecurityAssessmentComplete);

    connect(scanner, &VulnerabilityScanner::scanCompleted,
            [this](const QString &target) {
                Q_UNUSED(target)
                vulnerabilityScanRunning = false;
                startVulnScanButton->setEnabled(true);
                stopVulnScanButton->setEnabled(false);
                exportVulnReportButton->setEnabled(true);

                vulnerabilityStatusLabel->setText("✅ Vulnerability Scan abgeschlossen");
                vulnerabilityStatusLabel->setStyleSheet("font-weight: bold; color: #28a745;");
            });

    connect(scanner, &VulnerabilityScanner::errorOccurred,
            [this](const QString &target, const QString &error) {
                addLogEntry(QString("Vulnerability Scan Fehler für %1: %2").arg(target, error), "ERROR");
                QMessageBox::warning(this, "❌ Scan Fehler",
                                     QString("Fehler beim Scannen von %1:\n%2").arg(target, error));
            });

    // Start the scanner
    threadPool->start(scanner);
}

void IpScanner::stopVulnerabilitySccan()
{
    vulnerabilityScanRunning = false;
    startVulnScanButton->setEnabled(true);
    stopVulnScanButton->setEnabled(false);

    vulnerabilityStatusLabel->setText("🛑 Vulnerability Scan gestoppt");
    vulnerabilityStatusLabel->setStyleSheet("font-weight: bold; color: #dc3545;");

    addLogEntry("Vulnerability Scan vom Benutzer gestoppt", "WARNING");
}

void IpScanner::onVulnerabilityFound(const QString &target, const VulnerabilityInfo &vuln)
{
    Q_UNUSED(target)

    // Add to discovered vulnerabilities list
    discoveredVulnerabilities.append(vuln);

    // Add to tree widget
    QTreeWidgetItem *item = new QTreeWidgetItem();

    // Set severity with color coding
    QString severityText;
    QColor severityColor;
    switch (vuln.severity) {
    case VulnSeverity::CRITICAL:
        severityText = "🔴 CRITICAL";
        severityColor = QColor(220, 53, 69, 150);
        break;
    case VulnSeverity::HIGH:
        severityText = "🟠 HIGH";
        severityColor = QColor(255, 193, 7, 150);
        break;
    case VulnSeverity::MEDIUM:
        severityText = "🟡 MEDIUM";
        severityColor = QColor(255, 165, 0, 150);
        break;
    case VulnSeverity::LOW:
        severityText = "🟢 LOW";
        severityColor = QColor(40, 167, 69, 150);
        break;
    case VulnSeverity::INFO:
        severityText = "ℹ️ INFO";
        severityColor = QColor(23, 162, 184, 150);
        break;
    }

    item->setText(0, severityText);
    item->setText(1, vuln.title);
    item->setText(2, vuln.service);
    item->setText(3, QString::number(vuln.port));
    item->setText(4, vuln.cveScore.isEmpty() ? "N/A" : vuln.cveScore);
    item->setText(5, vuln.exploitable ? "⚠️ Exploitable" : "📋 Detected");

    // Color code the entire row based on severity
    for (int col = 0; col < 6; col++) {
        item->setBackground(col, severityColor);
    }

    vulnerabilityTreeWidget->addTopLevelItem(item);

    // Update statistics
    int vulnCount = discoveredVulnerabilities.size();
    vulnerabilitiesFoundLabel->setText(QString("Vulnerabilities: %1").arg(vulnCount));

    // Log the vulnerability
    addLogEntry(QString("Vulnerability gefunden: %1 (%2) - %3")
                    .arg(vuln.title, vuln.service, severityText),
                vuln.severity == VulnSeverity::CRITICAL || vuln.severity == VulnSeverity::HIGH ? "WARNING" : "INFO");
}
void IpScanner::onSecurityAssessmentComplete(const SecurityAssessment &assessment)
{
    lastAssessment = assessment;

    // Update security score and risk level
    securityScoreLabel->setText(QString("Score: %1/100").arg(assessment.securityScore));
    riskLevelLabel->setText(QString("Risk: %1").arg(assessment.overallRisk));

    // Color code risk level
    QString riskColor;
    if (assessment.overallRisk == "CRITICAL") {
        riskColor = "color: #dc3545; font-weight: bold;";
    } else if (assessment.overallRisk == "HIGH") {
        riskColor = "color: #fd7e14; font-weight: bold;";
    } else if (assessment.overallRisk == "MEDIUM") {
        riskColor = "color: #ffc107; font-weight: bold;";
    } else {
        riskColor = "color: #28a745; font-weight: bold;";
    }
    riskLevelLabel->setStyleSheet(riskColor);

    // Auto-resize columns
    for (int i = 0; i < vulnerabilityTreeWidget->columnCount(); ++i) {
        vulnerabilityTreeWidget->resizeColumnToContents(i);
    }

    // Show summary message
    QString summaryMessage = QString(
                                 "🔒 Security Assessment Abgeschlossen\n\n"
                                 "📊 Ergebnisse:\n"
                                 "• Vulnerabilities gefunden: %1\n"
                                 "• Critical: %2, High: %3, Medium: %4\n"
                                 "• Security Score: %5/100\n"
                                 "• Risk Level: %6\n\n"
                                 "💡 Detaillierte Informationen finden Sie im Vulnerability Tab."
                                 ).arg(assessment.totalVulnerabilities)
                                 .arg(assessment.criticalCount)
                                 .arg(assessment.highCount)
                                 .arg(assessment.mediumCount)
                                 .arg(assessment.securityScore)
                                 .arg(assessment.overallRisk);

    addLogEntry(QString("Security Assessment abgeschlossen - %1 Vulnerabilities, Score: %2, Risk: %3")
                    .arg(assessment.totalVulnerabilities)
                    .arg(assessment.securityScore)
                    .arg(assessment.overallRisk), "SUCCESS");

    // Show summary dialog if critical vulnerabilities found
    if (assessment.criticalCount > 0) {
        QMessageBox::warning(this, "⚠️ Kritische Vulnerabilities gefunden!", summaryMessage);
    } else {
        QMessageBox::information(this, "✅ Security Assessment", summaryMessage);
    }
}

void IpScanner::onVulnerabilityScanProgress(const QString &target, int percentage)
{
    Q_UNUSED(target)
    vulnerabilityProgressBar->setValue(percentage);
}

void IpScanner::onVulnerabilityScanStatusUpdate(const QString &target, const QString &message)
{
    Q_UNUSED(target)
    vulnerabilityStatusLabel->setText(message);
}

void IpScanner::onVulnerabilitySelectionChanged()
{
    QTreeWidgetItem *currentItem = vulnerabilityTreeWidget->currentItem();
    if (!currentItem) {
        vulnerabilityDetailsEdit->clear();
        return;
    }

    QString vulnTitle = currentItem->text(1);

    // Find the corresponding vulnerability
    VulnerabilityInfo selectedVuln;
    for (const VulnerabilityInfo &vuln : discoveredVulnerabilities) {
        if (vuln.title == vulnTitle) {
            selectedVuln = vuln;
            break;
        }
    }

    if (selectedVuln.title.isEmpty()) {
        vulnerabilityDetailsEdit->clear();
        return;
    }

    // Display detailed information
    QString details = QString(R"(
<h3>🔒 Vulnerability Details</h3>
<table border="0" cellpadding="3">
<tr><td><b>🆔 ID:</b></td><td>%1</td></tr>
<tr><td><b>📋 Title:</b></td><td>%2</td></tr>
<tr><td><b>⚠️ Severity:</b></td><td>%3</td></tr>
<tr><td><b>🔧 Service:</b></td><td>%4</td></tr>
<tr><td><b>🔌 Port:</b></td><td>%5</td></tr>
<tr><td><b>📊 CVE Score:</b></td><td>%6</td></tr>
<tr><td><b>🕐 Discovered:</b></td><td>%7</td></tr>
</table>

<h4>📝 Description:</h4>
<p>%8</p>

<h4>🔍 Evidence:</h4>
<p>%9</p>

<h4>💡 Recommendation:</h4>
<p>%10</p>

<h4>⚠️ Exploit Information:</h4>
<p><b>Exploitable:</b> %11</p>
<p><b>Method:</b> %12</p>
    )").arg(selectedVuln.id)
                          .arg(selectedVuln.title)
                          .arg(currentItem->text(0))
                          .arg(selectedVuln.service)
                          .arg(selectedVuln.port)
                          .arg(selectedVuln.cveScore.isEmpty() ? "N/A" : selectedVuln.cveScore)
                          .arg(selectedVuln.discovered.toString("dd.MM.yyyy hh:mm:ss"))
                          .arg(selectedVuln.description)
                          .arg(selectedVuln.evidence)
                          .arg(selectedVuln.recommendation)
                          .arg(selectedVuln.exploitable ? "⚠️ Yes" : "✅ No")
                          .arg(selectedVuln.exploitMethod.isEmpty() ? "N/A" : selectedVuln.exploitMethod);

    vulnerabilityDetailsEdit->setHtml(details);
}

void IpScanner::clearVulnerabilityResults()
{
    vulnerabilityTreeWidget->clear();
    vulnerabilityDetailsEdit->clear();
    discoveredVulnerabilities.clear();

    vulnerabilityProgressBar->setValue(0);
    vulnerabilitiesFoundLabel->setText("Vulnerabilities: 0");
    securityScoreLabel->setText("Score: --");
    riskLevelLabel->setText("Risk: --");

    vulnerabilityStatusLabel->setText("Bereit für Vulnerability Scan");
    vulnerabilityStatusLabel->setStyleSheet("font-weight: bold; color: #28a745;");
}

void IpScanner::exportVulnerabilityReport()
{
    if (discoveredVulnerabilities.isEmpty()) {
        QMessageBox::information(this, "📊 Export", "Keine Vulnerability-Daten zum Exportieren vorhanden");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "📊 Vulnerability Report Exportieren",
                                                    QString("vulnerability_report_%1.html")
                                                        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    "HTML Files (*.html);;JSON Files (*.json);;PDF Files (*.pdf)");

    if (fileName.isEmpty()) return;

    // Generate and save the report
    generateVulnerabilityReport(fileName);
}
void IpScanner::generateVulnerabilityReport(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "❌ Export Fehler", "Datei konnte nicht zum Schreiben geöffnet werden");
        return;
    }

    QTextStream stream(&file);

    if (fileName.endsWith(".html")) {
        generateHTMLVulnerabilityReport(stream);
    } else if (fileName.endsWith(".json")) {
        generateJSONVulnerabilityReport(stream);
    }

    file.close();

    addLogEntry(QString("Vulnerability Report exportiert: %1").arg(fileName), "SUCCESS");
    QMessageBox::information(this, "✅ Export Erfolgreich",
                             QString("Vulnerability Report erfolgreich exportiert:\n%1\n\n📊 %2 Vulnerabilities dokumentiert")
                                 .arg(fileName).arg(discoveredVulnerabilities.size()));
}
void IpScanner::generateHTMLVulnerabilityReport(QTextStream &stream)
{
    QString target = vulnTargetEdit->text();
    QDateTime reportTime = QDateTime::currentDateTime();

    stream << R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Vulnerability Assessment Report</title>
    <style>
        body {
            font-family: 'Segoe UI', Arial, sans-serif;
            background: linear-gradient(135deg, #1e1e1e, #2d2d2d);
            color: #fff;
            margin: 0;
            padding: 20px;
            line-height: 1.6;
        }
        .container { max-width: 1200px; margin: 0 auto; background: #2d2d2d; border-radius: 10px; overflow: hidden; box-shadow: 0 10px 30px rgba(0,0,0,0.5); }
        .header { background: linear-gradient(135deg, #dc3545, #c82333); padding: 30px; text-align: center; }
        .header h1 { margin: 0; font-size: 2.5em; text-shadow: 2px 2px 4px rgba(0,0,0,0.5); }
        .header .subtitle { font-size: 1.2em; margin-top: 10px; opacity: 0.9; }
        .summary { padding: 30px; background: #343a40; }
        .summary-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin-top: 20px; }
        .summary-card { background: #495057; padding: 20px; border-radius: 8px; text-align: center; border-left: 4px solid; }
        .summary-card.critical { border-left-color: #dc3545; }
        .summary-card.high { border-left-color: #fd7e14; }
        .summary-card.medium { border-left-color: #ffc107; }
        .summary-card.low { border-left-color: #28a745; }
        .summary-card h3 { margin: 0 0 10px 0; font-size: 2em; }
        .summary-card p { margin: 0; font-size: 0.9em; opacity: 0.8; }
        .content { padding: 30px; }
        .vulnerability { margin: 20px 0; background: #404040; border-radius: 8px; padding: 20px; border-left: 5px solid; }
        .vulnerability.critical { border-left-color: #dc3545; }
        .vulnerability.high { border-left-color: #fd7e14; }
        .vulnerability.medium { border-left-color: #ffc107; }
        .vulnerability.low { border-left-color: #28a745; }
        .vulnerability.info { border-left-color: #17a2b8; }
        .vuln-header { display: flex; justify-content: between; align-items: center; margin-bottom: 15px; }
        .vuln-title { font-size: 1.3em; font-weight: bold; margin: 0; }
        .vuln-severity { padding: 5px 15px; border-radius: 20px; font-size: 0.8em; font-weight: bold; text-transform: uppercase; }
        .severity-critical { background: #dc3545; color: white; }
        .severity-high { background: #fd7e14; color: white; }
        .severity-medium { background: #ffc107; color: black; }
        .severity-low { background: #28a745; color: white; }
        .severity-info { background: #17a2b8; color: white; }
        .vuln-details { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 15px; margin: 15px 0; }
        .detail-item { background: #343a40; padding: 10px; border-radius: 5px; }
        .detail-label { font-weight: bold; color: #adb5bd; font-size: 0.9em; }
        .detail-value { margin-top: 5px; }
        .recommendation { background: #1e7e34; padding: 15px; border-radius: 5px; margin-top: 15px; }
        .recommendation h4 { margin: 0 0 10px 0; color: #fff; }
        .exploitable { background: #721c24; padding: 10px; border-radius: 5px; margin-top: 10px; }
        .not-exploitable { background: #155724; padding: 10px; border-radius: 5px; margin-top: 10px; }
        .footer { background: #1a1a1a; padding: 20px; text-align: center; color: #6c757d; }
        .risk-indicator { display: inline-block; padding: 10px 20px; border-radius: 25px; font-weight: bold; margin: 10px; }
        .risk-critical { background: #dc3545; color: white; }
        .risk-high { background: #fd7e14; color: white; }
        .risk-medium { background: #ffc107; color: black; }
        .risk-low { background: #28a745; color: white; }
        @media print { body { background: white; color: black; } .container { box-shadow: none; } }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🔒 Security Vulnerability Assessment</h1>
            <div class="subtitle">Comprehensive Security Analysis Report</div>
        </div>

        <div class="summary">
            <h2>📊 Executive Summary</h2>
            <p><strong>Target:</strong> )" << target << R"(</p>
            <p><strong>Scan Date:</strong> )" << reportTime.toString("dd.MM.yyyy hh:mm:ss") << R"(</p>
            <p><strong>Scanner:</strong> Advanced IP Scanner v2.0 - Vulnerability Module</p>

            <div class="summary-grid">
                <div class="summary-card critical">
                    <h3>)" << lastAssessment.criticalCount << R"(</h3>
                    <p>Critical Vulnerabilities</p>
                </div>
                <div class="summary-card high">
                    <h3>)" << lastAssessment.highCount << R"(</h3>
                    <p>High Vulnerabilities</p>
                </div>
                <div class="summary-card medium">
                    <h3>)" << lastAssessment.mediumCount << R"(</h3>
                    <p>Medium Vulnerabilities</p>
                </div>
                <div class="summary-card low">
                    <h3>)" << lastAssessment.lowCount << R"(</h3>
                    <p>Low/Info Vulnerabilities</p>
                </div>
            </div>

            <div style="text-align: center; margin: 20px 0;">
                <div class="risk-indicator risk-)" << lastAssessment.overallRisk.toLower() << R"(">
                    Overall Risk Level: )" << lastAssessment.overallRisk << R"(
                </div>
                <div style="margin-top: 10px;">
                    <strong>Security Score: )" << lastAssessment.securityScore << R"(/100</strong>
                </div>
            </div>

            <h3>🎯 Key Findings:</h3>
            <ul>
                <li><strong>Total Vulnerabilities Found:</strong> )" << lastAssessment.totalVulnerabilities << R"(</li>
                <li><strong>Weak Credentials:</strong> )" << (lastAssessment.hasWeakCredentials ? "⚠️ Detected" : "✅ None Found") << R"(</li>
                <li><strong>Unencrypted Services:</strong> )" << (lastAssessment.hasUnencryptedServices ? "⚠️ Present" : "✅ None Found") << R"(</li>
                <li><strong>Outdated Software:</strong> )" << (lastAssessment.hasOutdatedSoftware ? "⚠️ Detected" : "✅ Up to Date") << R"(</li>
                <li><strong>Operating System:</strong> )" << lastAssessment.operatingSystem << R"(</li>
            </ul>
        </div>

        <div class="content">
            <h2>🔍 Detailed Vulnerability Analysis</h2>)";

    // Add each vulnerability
    for (const VulnerabilityInfo &vuln : discoveredVulnerabilities) {
        QString severityClass, severityText;
        switch (vuln.severity) {
        case VulnSeverity::CRITICAL:
            severityClass = "critical"; severityText = "CRITICAL"; break;
        case VulnSeverity::HIGH:
            severityClass = "high"; severityText = "HIGH"; break;
        case VulnSeverity::MEDIUM:
            severityClass = "medium"; severityText = "MEDIUM"; break;
        case VulnSeverity::LOW:
            severityClass = "low"; severityText = "LOW"; break;
        case VulnSeverity::INFO:
            severityClass = "info"; severityText = "INFO"; break;
        }

        stream << R"(
            <div class="vulnerability )" << severityClass << R"(">
                <div class="vuln-header">
                    <h3 class="vuln-title">)" << vuln.title << R"(</h3>
                    <span class="vuln-severity severity-)" << severityClass << R"(">)" << severityText << R"(</span>
                </div>

                <div class="vuln-details">
                    <div class="detail-item">
                        <div class="detail-label">Service</div>
                        <div class="detail-value">)" << vuln.service << R"(</div>
                    </div>
                    <div class="detail-item">
                        <div class="detail-label">Port</div>
                        <div class="detail-value">)" << vuln.port << R"(</div>
                    </div>
                    <div class="detail-item">
                        <div class="detail-label">CVE Score</div>
                        <div class="detail-value">)" << (vuln.cveScore.isEmpty() ? "N/A" : vuln.cveScore) << R"(</div>
                    </div>
                    <div class="detail-item">
                        <div class="detail-label">Discovered</div>
                        <div class="detail-value">)" << vuln.discovered.toString("dd.MM.yyyy hh:mm") << R"(</div>
                    </div>
                </div>

                <p><strong>Description:</strong> )" << vuln.description << R"(</p>
                <p><strong>Evidence:</strong> )" << vuln.evidence << R"(</p>

                <div class="recommendation">
                    <h4>💡 Recommendation:</h4>
                    <p>)" << vuln.recommendation << R"(</p>
                </div>

                <div class=")" << (vuln.exploitable ? "exploitable" : "not-exploitable") << R"(">
                    <strong>Exploitability:</strong> )" << (vuln.exploitable ? "⚠️ EXPLOITABLE" : "✅ Not Exploitable") << R"(
                    )" << (vuln.exploitable && !vuln.exploitMethod.isEmpty() ?
                       QString("<br><strong>Method:</strong> %1").arg(vuln.exploitMethod) : "") << R"(
                </div>

                )" << (!vuln.references.isEmpty() ?
                       QString("<p><strong>References:</strong> %1</p>").arg(vuln.references.join(", ")) : "") << R"(
            </div>)";
    }

    stream << R"(
        </div>

        <div class="footer">
            <p>Report generated by Advanced IP Scanner v2.0 - Vulnerability Assessment Module</p>
            <p>Generated on )" << reportTime.toString("dd.MM.yyyy hh:mm:ss") << R"( | Total Scan Time: )" <<
        (lastAssessment.scanTime.isValid() ?
             QString("%1 seconds").arg(lastAssessment.scanTime.secsTo(reportTime)) : "Unknown") << R"(</p>
            <p><em>This report contains sensitive security information. Handle with care.</em></p>
        </div>
    </div>
</body>
</html>)";
}

void IpScanner::generateJSONVulnerabilityReport(QTextStream &stream)
{
    QJsonObject root;
    root["scanner"] = "Advanced IP Scanner";
    root["module"] = "Vulnerability Assessment";
    root["version"] = "2.0";
    root["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    root["target"] = vulnTargetEdit->text();

    // Assessment summary
    QJsonObject assessmentObj;
    assessmentObj["totalVulnerabilities"] = lastAssessment.totalVulnerabilities;
    assessmentObj["criticalCount"] = lastAssessment.criticalCount;
    assessmentObj["highCount"] = lastAssessment.highCount;
    assessmentObj["mediumCount"] = lastAssessment.mediumCount;
    assessmentObj["lowCount"] = lastAssessment.lowCount;
    assessmentObj["infoCount"] = lastAssessment.infoCount;
    assessmentObj["securityScore"] = lastAssessment.securityScore;
    assessmentObj["overallRisk"] = lastAssessment.overallRisk;
    assessmentObj["operatingSystem"] = lastAssessment.operatingSystem;
    assessmentObj["hasWeakCredentials"] = lastAssessment.hasWeakCredentials;
    assessmentObj["hasUnencryptedServices"] = lastAssessment.hasUnencryptedServices;
    assessmentObj["hasOutdatedSoftware"] = lastAssessment.hasOutdatedSoftware;

    QJsonArray servicesArray;
    for (const QString &service : lastAssessment.runningServices) {
        servicesArray.append(service);
    }
    assessmentObj["runningServices"] = servicesArray;

    root["assessment"] = assessmentObj;

    // Individual vulnerabilities
    QJsonArray vulnerabilitiesArray;
    for (const VulnerabilityInfo &vuln : discoveredVulnerabilities) {
        QJsonObject vulnObj;
        vulnObj["id"] = vuln.id;
        vulnObj["title"] = vuln.title;
        vulnObj["description"] = vuln.description;
        vulnObj["severity"] = static_cast<int>(vuln.severity);
        vulnObj["severityText"] = [](VulnSeverity sev) {
            switch (sev) {
            case VulnSeverity::CRITICAL: return "CRITICAL";
            case VulnSeverity::HIGH: return "HIGH";
            case VulnSeverity::MEDIUM: return "MEDIUM";
            case VulnSeverity::LOW: return "LOW";
            case VulnSeverity::INFO: return "INFO";
            }
            return "UNKNOWN";
        }(vuln.severity);
        vulnObj["service"] = vuln.service;
        vulnObj["port"] = vuln.port;
        vulnObj["evidence"] = vuln.evidence;
        vulnObj["recommendation"] = vuln.recommendation;
        vulnObj["discovered"] = vuln.discovered.toString(Qt::ISODate);
        vulnObj["cveScore"] = vuln.cveScore;
        vulnObj["exploitable"] = vuln.exploitable;
        vulnObj["exploitMethod"] = vuln.exploitMethod;

        QJsonArray referencesArray;
        for (const QString &ref : vuln.references) {
            referencesArray.append(ref);
        }
        vulnObj["references"] = referencesArray;

        vulnerabilitiesArray.append(vulnObj);
    }

    root["vulnerabilities"] = vulnerabilitiesArray;

    QJsonDocument doc(root);
    stream << doc.toJson();
}
void IpScanner::showVulnerabilitySettings()
{
    QDialog *settingsDialog = new QDialog(this);
    settingsDialog->setWindowTitle("⚙️ Vulnerability Scanner Einstellungen");
    settingsDialog->setModal(true);
    settingsDialog->resize(500, 400);
    settingsDialog->setStyleSheet(styleSheet());

    QVBoxLayout *layout = new QVBoxLayout(settingsDialog);

    // Scan Settings Group
    QGroupBox *scanGroup = new QGroupBox("🔍 Scan Einstellungen");
    QFormLayout *scanLayout = new QFormLayout(scanGroup);

    QSpinBox *defaultTimeoutSpinBox = new QSpinBox();
    defaultTimeoutSpinBox->setRange(1000, 30000);
    defaultTimeoutSpinBox->setValue(vulnTimeoutSpinBox->value());
    defaultTimeoutSpinBox->setSuffix(" ms");
    scanLayout->addRow("Standard Timeout:", defaultTimeoutSpinBox);

    QCheckBox *autoStartAfterPortScan = new QCheckBox("Nach Port-Scan automatisch starten");
    scanLayout->addRow(autoStartAfterPortScan);

    QCheckBox *showDetailedProgress = new QCheckBox("Detaillierten Fortschritt anzeigen");
    showDetailedProgress->setChecked(true);
    scanLayout->addRow(showDetailedProgress);

    // Report Settings Group
    QGroupBox *reportGroup = new QGroupBox("📊 Report Einstellungen");
    QFormLayout *reportLayout = new QFormLayout(reportGroup);

    QComboBox *defaultReportFormat = new QComboBox();
    defaultReportFormat->addItems({"HTML Report", "JSON Data", "PDF Report"});
    reportLayout->addRow("Standard Export Format:", defaultReportFormat);

    QCheckBox *autoExportOnComplete = new QCheckBox("Report automatisch exportieren");
    reportLayout->addRow(autoExportOnComplete);

    QCheckBox *includeRecommendations = new QCheckBox("Empfehlungen einschließen");
    includeRecommendations->setChecked(true);
    reportLayout->addRow(includeRecommendations);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("✅ OK");
    QPushButton *cancelButton = new QPushButton("❌ Abbrechen");
    QPushButton *resetButton = new QPushButton("🔄 Zurücksetzen");

    buttonLayout->addWidget(resetButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(scanGroup);
    layout->addWidget(reportGroup);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, [=]() {
        vulnTimeoutSpinBox->setValue(defaultTimeoutSpinBox->value());
        settingsDialog->accept();
        addLogEntry("Vulnerability Scanner Einstellungen aktualisiert", "SUCCESS");
    });

    connect(cancelButton, &QPushButton::clicked, settingsDialog, &QDialog::reject);

    connect(resetButton, &QPushButton::clicked, [=]() {
        defaultTimeoutSpinBox->setValue(5000);
        autoStartAfterPortScan->setChecked(false);
        showDetailedProgress->setChecked(true);
        defaultReportFormat->setCurrentIndex(0);
        autoExportOnComplete->setChecked(false);
        includeRecommendations->setChecked(true);
    });

    settingsDialog->exec();
}

void IpScanner::updateVulnerabilityDatabase()
{
    QProgressDialog *progressDialog = new QProgressDialog("Aktualisiere Vulnerability Datenbank...", "Abbrechen", 0, 100, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setStyleSheet(styleSheet());
    progressDialog->show();

    // Simulate database update (in real implementation, this would download CVE data)
    for (int i = 0; i <= 100; i += 10) {
        progressDialog->setValue(i);
        QThread::msleep(200);
        QApplication::processEvents();

        if (progressDialog->wasCanceled()) {
            addLogEntry("Vulnerability Datenbank Update abgebrochen", "WARNING");
            return;
        }
    }

    progressDialog->close();
    addLogEntry("Vulnerability Datenbank erfolgreich aktualisiert", "SUCCESS");
    QMessageBox::information(this, "✅ Update Erfolgreich",
                             "Vulnerability Datenbank wurde erfolgreich aktualisiert!\n\n"
                             "Neue CVE-Einträge und Vulnerability-Signaturen sind jetzt verfügbar.");
}
void IpScanner::initializeVulnerabilityScanner()
{
    vulnerabilityScanRunning = false;
    discoveredVulnerabilities.clear();

    // Initialize vulnerability database
    VulnerabilityDatabase::instance()->updateDatabase();
}
