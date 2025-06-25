#ifndef IPSCANNER_H
#define IPSCANNER_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtGui/QAction>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QSet>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDialog>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QThreadPool>
#include <QtCore/QRunnable>
#include <QtCore/QMutex>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QUdpSocket>
#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QSettings>
#include <QtCore/QElapsedTimer>
#include <QtGui/QClipboard>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#include "ScanWorkers.h"
#include "VulnerabilityScanner.h"

// Forward declarations
class ScanWorker;
class PortScanWorker;
class PingWorker;
class EnhancedArpScanWorker;
class NetworkHealthWorker;
class VulnerabilityScanner;

class IpScanner : public QMainWindow
{
    Q_OBJECT

public:
    IpScanner(QWidget *parent = nullptr);
    ~IpScanner();

private slots:
    // Main scanning functions
    void startScan();
    void stopScan();
    void pauseScan();
    void resumeScan();

    // Host discovery
    void pingHost();
    void traceroute();
    void dnsLookup();

    // Port scanning
    void portScan();
    void vulnScan();
    void serviceScan();

    // Network analysis
    void networkDiscovery();
    void arpScan();
    void whoIs();

    // Results handling
    void onHostFound(const HostInfo &host);
    void onHostCompleted();  // New: for progress tracking
    void updateProgress();   // New: for UI updates
    void updateScanProgress(); // New: for scan progress
    void onScanFinished();
    void onItemSelectionChanged();
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);

    void analyzeNetworkTopology();
    void exportNetworkMap();
    void refreshNetworkInterfaces();
    void showNetworkStatistics();
    void performNetworkHealthCheck();
    void updateNetworkStatistics();

    void showMacLookup();
    void showSubnetCalculator();

    // Export/Import
    void exportResults();
    void saveProject();
    void loadProject();

    // Settings and UI
    void showSettings();
    void showAbout();
    void showShortcuts();  // New: show keyboard shortcuts

    // Context menu and actions
    void showContextMenu(const QPoint &pos);
    void copyToClipboard();
    void removeHost();
    void addToFavorites();

    void showNetworkContextMenu(const QPoint &pos);
    void showNetworkDeviceDetails(QTreeWidgetItem *item);

    // New utility functions
    void addCommonPort();
    void filterLog();
    void saveLog();

    // Vulnerability Scanner functions
    void startVulnerabilitySccan();
    void stopVulnerabilitySccan();
    void exportVulnerabilityReport();
    void clearVulnerabilityResults();
    void showVulnerabilitySettings();
    void updateVulnerabilityDatabase();
    void onVulnerabilitySelectionChanged();
    void onVulnerabilityScanProgress(const QString &target, int percentage);
    void onVulnerabilityScanStatusUpdate(const QString &target, const QString &message);
    void onVulnerabilityFound(const QString &target, const VulnerabilityInfo &vuln);
    void onSecurityAssessmentComplete(const SecurityAssessment &assessment);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void createScanTab();
    void createResultsTab();
    void createPortScanTab();
    void createNetworkTab();
    void createLogTab();
    void createVulnerabilityTab();

    QString resolveHostnameWithTimeout(const QString &ip, int timeoutMs);
    QColor getVendorColor(const QString &vendor);

    QStringList parseCIDRRange(const QString &cidrNotation);
    QStringList parseIPRange(const QString &rangeString);
    QStringList parseWildcardRange(const QString &wildcardString);
    QStringList generateIPRange(const QString &startIP, const QString &endPart);
    quint32 ipToInt(const QString &ip);
    QString intToIP(quint32 ip);
    bool isValidIP(const QString &ip);

    // Helper functions
    void loadSettings();
    void saveSettings();
    void addLogEntry(const QString &message, const QString &type = "INFO");
    QString getCurrentNetwork();
    QStringList getLocalInterfaces();
    void populateHostTree();
    void clearResults();

    QString resolveHostname(const QString &ip);
    QString getMacVendor(const QString &mac);
    QString getSubnetFromIp(const QString &ip);
    bool isLikelyRouter(const HostInfo &host);
    bool isLikelyServer(const HostInfo &host);
    bool isLikelyWorkstation(const HostInfo &host);
    void setupNetworkContextMenu();

    // Enhanced parsing functions
    QStringList parseTargetString(const QString &target);
    QList<int> parsePortRange(const QString &range);
    QJsonDocument createJsonReport();

    QString calculateSubnetInfo(const QString &ip, int cidr);
    QString getNetworkClass(quint32 networkAddr);
    QString getSubnetType(int cidr);

    // Vulnerability report generation
    void generateVulnerabilityReport(const QString &fileName);
    void generateHTMLVulnerabilityReport(QTextStream &stream);
    void generateJSONVulnerabilityReport(QTextStream &stream);
    void initializeVulnerabilityScanner();

    // UI Components
    QWidget *centralWidget;
    QTabWidget *mainTabWidget;
    QSplitter *mainSplitter;

    QList<QLabel*> networkStatsLabels;

    // Scan Tab - Enhanced
    QWidget *scanTab;
    QLineEdit *targetEdit;
    QSpinBox *threadsSpinBox;
    QSpinBox *timeoutSpinBox;
    QComboBox *scanTypeCombo;
    QCheckBox *pingCheckBox;
    QCheckBox *portScanCheckBox;
    QCheckBox *osDetectionCheckBox;
    QCheckBox *serviceDetectionCheckBox;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *pauseButton;
    QProgressBar *progressBar;
    QLabel *statusLabel;

    // New progress indicators
    QLabel *currentHostLabel;
    QLabel *totalHostsLabel;
    QLabel *scannedHostsLabel;
    QLabel *foundHostsLabel;
    QLabel *elapsedTimeLabel;

    // Results Tab
    QWidget *resultsTab;
    QTreeWidget *hostTreeWidget;
    QTextEdit *hostDetailsEdit;

    // Port Scan Tab
    QWidget *portScanTab;
    QLineEdit *portRangeEdit;
    QComboBox *portScanTypeCombo;
    QListWidget *commonPortsList;
    QPushButton *portScanButton;
    QTreeWidget *portResultsTree;

    // Network Tab
    QWidget *networkTab;
    QComboBox *interfaceCombo;
    QTreeWidget *networkTree;
    QPushButton *arpScanButton;
    QPushButton *discoveryButton;

    // Log Tab - Enhanced
    QWidget *logTab;
    QTextEdit *logTextEdit;
    QPushButton *clearLogButton;
    QPushButton *saveLogButton;
    QPushButton *exportLogButton;  // New
    QLineEdit *logFilterEdit;      // New

    // Vulnerability Scanner Tab
    QWidget *vulnerabilityTab;
    QLineEdit *vulnTargetEdit;
    QComboBox *vulnScanTypeCombo;
    QSpinBox *vulnTimeoutSpinBox;
    QCheckBox *enableCredentialTestsCheckBox;
    QCheckBox *enableSSLTestsCheckBox;
    QCheckBox *enableServiceTestsCheckBox;
    QPushButton *startVulnScanButton;
    QPushButton *stopVulnScanButton;
    QPushButton *exportVulnReportButton;
    QProgressBar *vulnerabilityProgressBar;
    QLabel *vulnerabilityStatusLabel;
    QLabel *vulnerabilitiesFoundLabel;
    QLabel *securityScoreLabel;
    QLabel *riskLevelLabel;
    QTreeWidget *vulnerabilityTreeWidget;
    QTextEdit *vulnerabilityDetailsEdit;

    // Menu and toolbar
    QMenu *fileMenu;
    QMenu *scanMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    // Actions
    QAction *newProjectAction;
    QAction *openProjectAction;
    QAction *saveProjectAction;
    QAction *exportAction;
    QAction *exitAction;
    QAction *settingsAction;
    QAction *aboutAction;

    // Data members
    QList<HostInfo> discoveredHosts;
    QThreadPool *threadPool;
    QTimer *updateTimer;      // New: for UI updates
    QTimer *progressTimer;    // New: for progress updates
    QSettings *settings;
    QString currentTarget;
    bool scanRunning;
    bool scanPaused;
    int totalHosts;
    int scannedHosts;
    int completedHosts;       // New: for accurate progress tracking
    QMutex hostListMutex;
    QDateTime scanStartTime;  // New: for elapsed time tracking

    // Vulnerability scanner data
    bool vulnerabilityScanRunning;
    QList<VulnerabilityInfo> discoveredVulnerabilities;
    SecurityAssessment lastAssessment;

    // Scan workers
    QList<QThread*> scanThreads;
};

#endif // IPSCANNER_H
