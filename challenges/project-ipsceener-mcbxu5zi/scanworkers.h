// Korrigierte scanworkers.h - Header mit Crash-Fix

#ifndef SCANWORKERS_H
#define SCANWORKERS_H

#include <QtCore/QObject>
#include <QtCore/QRunnable>
#include <QtCore/QThread>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QElapsedTimer>
#include <QtCore/QDateTime>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkAddressEntry>
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QRegularExpression>
#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QMutex>

// Enhanced Host information structure
struct HostInfo {
    QString ip;
    QString hostname;
    QString macAddress;
    QString vendor;
    QStringList openPorts;
    QStringList services;
    QString os;
    int responseTime;
    bool isAlive;
    QDateTime lastSeen;
    QString notes;
    QString deviceType;     // New: Desktop, Server, Router, etc.
    QString location;       // New: Physical location if known
    int openPortCount;      // New: Quick reference
    QString lastService;    // New: Most recent service detected
    bool isSecure;         // New: Has SSL/TLS services
};

// Enhanced worker classes for threading
class ScanWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    ScanWorker(const QString &ip, int timeout, QObject *parent = nullptr);
    void run() override;
    void setHostIndex(int index);  // New: for progress tracking

signals:
    void hostFound(const HostInfo &host);
    void hostCompleted();  // New: signal when host scan is complete
    void progress(int percentage);
    void statusUpdate(const QString &message);  // New: for detailed status

private:
    QString targetIp;
    int scanTimeout;
    int hostIndex;

    // Enhanced helper methods
    QString detectOperatingSystem(const QString &ip);
    QString getServiceName(int port);
    bool quickPortCheck(const QString &ip, int port, int timeout = 1000);
};

class PortScanWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    PortScanWorker(const QString &ip, const QList<int> &ports, QObject *parent = nullptr);
    void run() override;

signals:
    // Enhanced signal with more details - this is the main one we'll use
    void portResult(const QString &ip, int port, bool open, const QString &service,
                    const QString &version, const QString &banner, int responseTime);

    void scanStarted(const QString &ip, int totalPorts);
    void scanProgress(int scannedPorts, int totalPorts);
    void scanCompleted(const QString &ip, int portsScanned, qint64 totalTime);

private:
    QString targetIp;
    QList<int> targetPorts;
    int totalPorts;
    int scannedPorts;

    // Enhanced helper methods
    bool isPortOpen(const QString &ip, int port);
    QString getServiceName(int port);
    QPair<QString, QString> grabBanner(const QString &ip, int port);
    QString getProbeForPort(int port);
    QString extractVersionFromBanner(const QString &banner, const QString &service);
};

class PingWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    PingWorker(const QString &ip, QObject *parent = nullptr);
    void run() override;

signals:
    void pingResult(const QString &ip, bool alive, int responseTime);
    void pingCompleted(const QString &ip, bool alive, int responseTime, const QString &method);  // Enhanced

private:
    QString targetIp;

    // Enhanced ping methods
    bool tcpPing(const QString &ip, int port, int timeout);
    bool icmpPing(const QString &ip, int timeout);  // Future implementation
};

// New: Network Discovery Worker
class NetworkDiscoveryWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    NetworkDiscoveryWorker(const QString &interface, QObject *parent = nullptr);
    void run() override;

signals:
    void discoveryStarted(const QString &interface);
    void networkFound(const QString &network, int prefixLength, const QString &interface, const QString &mac);
    void deviceFound(const QString &ip, const QString &hostname, const QString &mac, const QString &type);
    void discoveryProgress(int scannedHosts, int totalHosts);
    void discoveryCompleted(const QString &interface);

private:
    QString networkInterface;

    // Private helper methods
    void performEnhancedNetworkScan(const QString &baseIp, int prefixLength, const QString &interfaceName);
    bool scanHost(const QString &ip);
    bool quickScanHost(const QString &ip);
    QString determineDeviceType(const QString &ip, int hostNumber);
    QString resolveHostname(const QString &ip);
};

// New: ARP Scanner Worker
class ArpScanWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    ArpScanWorker(const QString &interface, QObject *parent = nullptr);
    void run() override;

signals:
    void arpEntryFound(const QString &ip, const QString &mac, const QString &vendor);
    void arpScanCompleted(int entriesFound);

private:
    QString networkInterface;
    QString getMacVendor(const QString &mac);  // MAC address vendor lookup
};

// New: Traceroute Worker
class TracerouteWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    TracerouteWorker(const QString &target, QObject *parent = nullptr);
    void run() override;

signals:
    void hopFound(int hopNumber, const QString &ip, const QString &hostname, int responseTime);
    void tracerouteCompleted(const QString &target, int totalHops);

private:
    QString targetHost;
};

// New: DNS Lookup Worker
class DnsLookupWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    DnsLookupWorker(const QString &target, QObject *parent = nullptr);
    void run() override;

signals:
    void dnsResult(const QString &hostname, const QStringList &addresses);
    void dnsReverse(const QString &ip, const QString &hostname);
    void dnsCompleted(const QString &target);

private:
    QString targetHost;
};

// New: WHOIS Lookup Worker
class WhoisWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    WhoisWorker(const QString &target, QObject *parent = nullptr);
    void run() override;

signals:
    void whoisResult(const QString &target, const QString &whoisData);
    void whoisCompleted(const QString &target);

private:
    QString targetHost;
};

// New: Service Detection Worker
class ServiceDetectionWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    ServiceDetectionWorker(const QString &ip, int port, QObject *parent = nullptr);
    void run() override;

signals:
    void serviceDetected(const QString &ip, int port, const QString &service,
                         const QString &version, const QString &banner);

private:
    QString targetIp;
    int targetPort;

    QString detectHttpService(const QString &ip, int port);
    QString detectSshService(const QString &ip, int port);
    QString detectFtpService(const QString &ip, int port);
    QString detectSmtpService(const QString &ip, int port);
};

// KORRIGIERTE EnhancedArpScanWorker Klasse mit Crash-Fix
class EnhancedArpScanWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    EnhancedArpScanWorker(const QString &interface, QObject *parent = nullptr);
    void run() override;

signals:
    void arpScanStarted(const QString &interface);
    void arpEntryFound(const QString &ip, const QString &mac, const QString &vendor);
    void arpScanProgress(int scannedEntries, int totalEntries);
    void arpScanCompleted(int entriesFound);

private:
    QString networkInterface;

    // Thread-safe helper methods
    QString getMacVendor(const QString &mac);
    QStringList getLocalNetworkIPs();

    // NEUE Methoden für Crash-Fix
    void performAdditionalPingSweep(QStringList &foundIPs, int &entriesFound);
    bool quickTcpPing(const QString &ip);
    QString getMacFromArpTable(const QString &ip);
};

// KORRIGIERTE NetworkHealthWorker Klasse mit Exception Handling
class NetworkHealthWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    NetworkHealthWorker(const QStringList &targets, QObject *parent = nullptr);
    void run() override;

signals:
    void healthCheckStarted(int totalTargets);
    void hostHealthResult(const QString &ip, bool reachable, int responseTime,
                          int packetLoss, const QString &status);
    void healthCheckCompleted(int totalHosts, int reachableHosts, int unreachableHosts);

private:
    QStringList targetHosts;
    bool performPingTest(const QString &ip, int &responseTime, int &packetLoss);
    QString getHealthStatus(bool reachable, int responseTime, int packetLoss);
};

#endif // SCANWORKERS_H
