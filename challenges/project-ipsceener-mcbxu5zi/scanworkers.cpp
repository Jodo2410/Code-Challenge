#include "ScanWorkers.h"
#include <QtCore/QMap>
#include <QtCore/QIODevice>
#include <QtCore/QDebug>
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtCore/QRegularExpression>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkAddressEntry>

static QMutex arpMutex;

// ============================================================================
// ScanWorker - KORRIGIERTE IMPLEMENTIERUNG
// ============================================================================
ScanWorker::ScanWorker(const QString &ip, int timeout, QObject *parent)
    : QObject(parent), targetIp(ip), scanTimeout(timeout), hostIndex(-1)
{
    setAutoDelete(true);
}

void ScanWorker::setHostIndex(int index)
{
    hostIndex = index;
}

void ScanWorker::run()
{
    try {
        HostInfo host;
        host.ip = targetIp;
        host.isAlive = false;
        host.responseTime = -1;
        host.lastSeen = QDateTime::currentDateTime();

        // Enhanced ping test with multiple methods
        QElapsedTimer timer;
        timer.start();

        // Method 1: Try common web ports first
        QList<int> quickPorts = {80, 443, 22, 23, 25, 53, 135, 139, 445, 993, 995, 3389, 5900};
        bool foundOpenPort = false;

        for (int port : quickPorts) {
            if (quickPortCheck(targetIp, port, 1000)) {
                host.isAlive = true;
                host.responseTime = timer.elapsed();
                host.openPorts.append(QString::number(port));
                host.services.append(getServiceName(port));
                foundOpenPort = true;
                break; // Found one open port, that's enough for basic scan
            }
        }

        if (host.isAlive) {
            // Try to resolve hostname
            QHostInfo hostInfo = QHostInfo::fromName(targetIp);
            if (hostInfo.error() == QHostInfo::NoError && !hostInfo.hostName().isEmpty()) {
                QString hostname = hostInfo.hostName();
                if (hostname != targetIp) {
                    host.hostname = hostname;
                }
            }

            // Basic OS detection
            host.os = detectOperatingSystem(targetIp);

            // Set device type based on open ports
            if (host.openPorts.contains("22") || host.openPorts.contains("3389")) {
                host.deviceType = "Server";
            } else if (host.openPorts.contains("80") || host.openPorts.contains("443")) {
                host.deviceType = "Web Server";
            } else {
                host.deviceType = "Computer";
            }

            emit hostFound(host);
        }

        emit hostCompleted();

    } catch (const std::exception& e) {
        qDebug() << "Exception in ScanWorker::run():" << e.what();
        emit hostCompleted();
    } catch (...) {
        qDebug() << "Unknown exception in ScanWorker::run()";
        emit hostCompleted();
    }
}

bool ScanWorker::quickPortCheck(const QString &ip, int port, int timeout)
{
    QTcpSocket socket;
    socket.connectToHost(ip, port, QIODevice::ReadOnly);
    bool connected = socket.waitForConnected(timeout);

    if (connected) {
        socket.disconnectFromHost();
        socket.waitForDisconnected(1000);
    }

    return connected;
}

QString ScanWorker::detectOperatingSystem(const QString &ip)
{
    // Simple OS detection based on common service patterns
    QMap<int, QString> serviceOsMap = {
        {22, "Linux/Unix"}, // SSH typically Linux
        {3389, "Windows"}, // RDP is Windows
        {445, "Windows"}, // SMB typically Windows
        {139, "Windows"}, // NetBIOS typically Windows
        {80, "Web Server"}, // HTTP could be anything
        {443, "Web Server"} // HTTPS could be anything
    };

    // Check which services are available and make educated guess
    for (auto it = serviceOsMap.begin(); it != serviceOsMap.end(); ++it) {
        if (quickPortCheck(ip, it.key(), 1000)) {
            return it.value();
        }
    }

    return "Unknown";
}

QString ScanWorker::getServiceName(int port)
{
    static QMap<int, QString> services = {
        {20, "FTP-Data"}, {21, "FTP"}, {22, "SSH"}, {23, "Telnet"},
        {25, "SMTP"}, {53, "DNS"}, {67, "DHCP"}, {68, "DHCP"},
        {69, "TFTP"}, {80, "HTTP"}, {110, "POP3"}, {123, "NTP"},
        {143, "IMAP"}, {161, "SNMP"}, {162, "SNMP-Trap"}, {443, "HTTPS"},
        {993, "IMAPS"}, {995, "POP3S"}, {1433, "MSSQL"}, {1521, "Oracle"},
        {3306, "MySQL"}, {3389, "RDP"}, {5432, "PostgreSQL"}, {5900, "VNC"},
        {6379, "Redis"}, {8080, "HTTP-Proxy"}, {8443, "HTTPS-Alt"},
        {27017, "MongoDB"}
    };

    return services.value(port, "Unknown");
}

// ============================================================================
// EnhancedArpScanWorker - KORRIGIERTE IMPLEMENTIERUNG
// ============================================================================
EnhancedArpScanWorker::EnhancedArpScanWorker(const QString &interface, QObject *parent)
    : QObject(parent), networkInterface(interface)
{
    setAutoDelete(true);
}

void EnhancedArpScanWorker::run()
{
    try {
        emit arpScanStarted(networkInterface);

        QStringList foundIPs;
        int entriesFound = 0;

        // Method 1: Parse system ARP table
        QProcess *arpProcess = new QProcess();

#ifdef Q_OS_WIN
        arpProcess->setProgram("arp");
        arpProcess->setArguments(QStringList() << "-a");
#else
        arpProcess->setProgram("arp");
        arpProcess->setArguments(QStringList() << "-a");
#endif

        arpProcess->start();

        if (arpProcess->waitForFinished(10000)) {
            QString output = QString::fromLocal8Bit(arpProcess->readAllStandardOutput());
            QStringList lines = output.split('\n');

            for (const QString &line : lines) {
                if (line.trimmed().isEmpty()) continue;

                QString ip, mac;

#ifdef Q_OS_WIN \
    // Windows ARP format: "  192.168.1.1      00-11-22-33-44-55     dynamic"
                QRegularExpression winArpRegex(R"(\s*(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\s+([a-fA-F0-9\-]{17})\s+)");
                QRegularExpressionMatch winMatch = winArpRegex.match(line);
                if (winMatch.hasMatch()) {
                    ip = winMatch.captured(1);
                    mac = winMatch.captured(2).replace("-", ":").toUpper();
                }
#else \
    // Linux ARP format: "192.168.1.1 ether 00:11:22:33:44:55 C eth0"
                QRegularExpression linuxArpRegex(R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\s+\w+\s+([a-fA-F0-9:]{17}))");
                QRegularExpressionMatch linuxMatch = linuxArpRegex.match(line);
                if (linuxMatch.hasMatch()) {
                    ip = linuxMatch.captured(1);
                    mac = linuxMatch.captured(2).toUpper();
                }
#endif

                if (!ip.isEmpty() && !mac.isEmpty() &&
                    !ip.startsWith("224.") && !ip.startsWith("239.") && // Skip multicast
                    mac != "FF:FF:FF:FF:FF:FF") { // Skip broadcast

                    if (!foundIPs.contains(ip)) {
                        QString vendor = getMacVendor(mac);

                        emit arpEntryFound(ip, mac, vendor);
                        foundIPs.append(ip);
                        entriesFound++;

                        emit arpScanProgress(entriesFound, entriesFound + 10); // Estimate progress
                    }
                }
            }
        }

        delete arpProcess;

        // Method 2: Additional ping sweep for discovery
        performAdditionalPingSweep(foundIPs, entriesFound);

        emit arpScanCompleted(entriesFound);

    } catch (const std::exception& e) {
        qDebug() << "Exception in EnhancedArpScanWorker::run():" << e.what();
        emit arpScanCompleted(0);
    } catch (...) {
        qDebug() << "Unknown exception in EnhancedArpScanWorker::run()";
        emit arpScanCompleted(0);
    }
}

void EnhancedArpScanWorker::performAdditionalPingSweep(QStringList &foundIPs, int &entriesFound)
{
    try {
        QStringList localNetworks = getLocalNetworkIPs();

        for (const QString &baseNetwork : localNetworks) {
            QStringList ipParts = baseNetwork.split('.');
            if (ipParts.size() == 4) {
                QString networkBase = QString("%1.%2.%3.").arg(ipParts[0], ipParts[1], ipParts[2]);

                // Ping nur häufige Host-IPs
                QList<int> commonHosts = {1, 2, 5, 10, 20, 50, 100, 150, 200, 254};

                for (int host : commonHosts) {
                    QString targetIP = networkBase + QString::number(host);

                    if (foundIPs.contains(targetIP)) continue;

                    // Schneller TCP-Ping statt System-Ping
                    if (quickTcpPing(targetIP)) {
                        // Versuche MAC-Adresse aus ARP-Tabelle zu holen
                        QString mac = getMacFromArpTable(targetIP);
                        if (!mac.isEmpty()) {
                            QString vendor = getMacVendor(mac);

                            emit arpEntryFound(targetIP, mac, vendor);
                            entriesFound++;
                            foundIPs.append(targetIP);
                        }
                    }

                    // Rate limiting
                    QThread::msleep(50);
                }
            }
        }
    } catch (...) {
        qDebug() << "Exception im Additional Ping Sweep";
    }
}

bool EnhancedArpScanWorker::quickTcpPing(const QString &ip)
{
    QTcpSocket socket;
    socket.connectToHost(ip, 80, QIODevice::ReadOnly);
    bool connected = socket.waitForConnected(1000);

    if (connected) {
        socket.disconnectFromHost();
        socket.waitForDisconnected(1000);
    }

    return connected;
}

QString EnhancedArpScanWorker::getMacFromArpTable(const QString &ip)
{
    try {
        QProcess *arpLookup = new QProcess();

#ifdef Q_OS_WIN
        arpLookup->setProgram("arp");
        arpLookup->setArguments(QStringList() << "-a" << ip);
#else
        arpLookup->setProgram("arp");
        arpLookup->setArguments(QStringList() << ip);
#endif

        arpLookup->start();

        if (!arpLookup->waitForFinished(3000)) {
            arpLookup->kill();
            delete arpLookup;
            return QString();
        }

        QString output = QString::fromLocal8Bit(arpLookup->readAllStandardOutput());
        delete arpLookup;

#ifdef Q_OS_WIN
        QRegularExpression macRegex(R"(([a-fA-F0-9]{2}[-:]?[a-fA-F0-9]{2}[-:]?[a-fA-F0-9]{2}[-:]?[a-fA-F0-9]{2}[-:]?[a-fA-F0-9]{2}[-:]?[a-fA-F0-9]{2}))");
#else
        QRegularExpression macRegex(R"(([a-fA-F0-9:]{17}))");
#endif

        QRegularExpressionMatch match = macRegex.match(output);
        if (match.hasMatch()) {
            QString mac = match.captured(1).replace("-", ":").toUpper();
            return mac;
        }
    } catch (...) {
        qDebug() << "Exception beim MAC-Lookup für IP:" << ip;
    }

    return QString();
}

QStringList EnhancedArpScanWorker::getLocalNetworkIPs()
{
    QStringList networks;

    try {
        const auto interfaces = QNetworkInterface::allInterfaces();
        for (const QNetworkInterface &interface : interfaces) {
            if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
                !interface.flags().testFlag(QNetworkInterface::IsLoopBack) &&
                interface.flags().testFlag(QNetworkInterface::IsRunning)) {

                const auto addressEntries = interface.addressEntries();
                for (const QNetworkAddressEntry &entry : addressEntries) {
                    if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                        QString ip = entry.ip().toString();
                        if (!ip.startsWith("169.254.")) { // Skip APIPA addresses
                            networks.append(ip);
                        }
                    }
                }
            }
        }
    } catch (...) {
        qDebug() << "Exception beim Ermitteln lokaler IPs";
    }

    return networks;
}

QString EnhancedArpScanWorker::getMacVendor(const QString &mac)
{
    if (mac.isEmpty() || mac.length() < 8) {
        return "Unbekannt";
    }

    try {
        // Extract first 3 octets (OUI)
        QString oui = mac.left(8).toUpper().replace(":", "").replace("-", "");
        if (oui.length() < 6) return "Unbekannt";

        oui = oui.left(6);

        // Thread-safe vendor lookup mit static map
        static const QMap<QString, QString> vendorMap = {
                                                         // Apple
                                                         {"000393", "Apple Inc."}, {"001124", "Apple Inc."}, {"0016CB", "Apple Inc."},
                                                         {"001E52", "Apple Inc."}, {"001F5B", "Apple Inc."}, {"0021E9", "Apple Inc."},
                                                         {"002312", "Apple Inc."}, {"002332", "Apple Inc."}, {"002436", "Apple Inc."},
                                                         {"3C15C2", "Apple Inc."}, {"40A6D9", "Apple Inc."}, {"44D884", "Apple Inc."},

                                                         // Samsung
                                                         {"000E8F", "Samsung Electronics"}, {"0012FB", "Samsung Electronics"},
                                                         {"001377", "Samsung Electronics"}, {"0015B9", "Samsung Electronics"},
                                                         {"30F9ED", "Samsung Electronics"}, {"38AA3C", "Samsung Electronics"},
                                                         {"74F61C", "Samsung Electronics"}, {"E8039A", "Samsung Electronics"},

                                                         // Intel
                                                         {"000C29", "Intel Corporation"}, {"001517", "Intel Corporation"},
                                                         {"001B21", "Intel Corporation"}, {"001E67", "Intel Corporation"},
                                                         {"3C970E", "Intel Corporation"}, {"7C7A91", "Intel Corporation"},

                                                         // Microsoft
                                                         {"000D3A", "Microsoft Corporation"}, {"001DD8", "Microsoft Corporation"},
                                                         {"40490F", "Microsoft Corporation"}, {"7C1E52", "Microsoft Corporation"},

                                                         // Cisco
                                                         {"000142", "Cisco Systems"}, {"000163", "Cisco Systems"},
                                                         {"000D28", "Cisco Systems"}, {"001364", "Cisco Systems"},
                                                         {"547FEE", "Cisco Systems"}, {"C8D119", "Cisco Systems"},

                                                         // TP-Link
                                                         {"001A8C", "TP-Link Technologies"}, {"002268", "TP-Link Technologies"},
                                                         {"B0487A", "TP-Link Technologies"}, {"E8DE27", "TP-Link Technologies"},

                                                         // D-Link
                                                         {"001195", "D-Link Corporation"}, {"001346", "D-Link Corporation"},
                                                         {"1C7EE5", "D-Link Corporation"}, {"E46F13", "D-Link Corporation"},

                                                         // Raspberry Pi
                                                         {"B827EB", "Raspberry Pi Foundation"}, {"DCA632", "Raspberry Pi Foundation"},

                                                         // Virtualization
                                                         {"005056", "VMware Inc."}, {"000C29", "VMware Inc."},
                                                         {"080027", "Oracle VirtualBox"}, {"525400", "QEMU/KVM"},

                                                         // Network equipment
                                                         {"00904C", "Netgear Inc."}, {"CC40D0", "Netgear Inc."},
                                                         {"001CDF", "Belkin International"}, {"944452", "Belkin International"},
                                                         {"001D7E", "Linksys"}, {"68CAE4", "Linksys"},
                                                         };

        return vendorMap.value(oui, "Unbekannter Hersteller");

    } catch (...) {
        return "Unbekannt";
    }
}

// ============================================================================
// Enhanced PortScanWorker implementation
// ============================================================================
PortScanWorker::PortScanWorker(const QString &ip, const QList<int> &ports, QObject *parent)
    : QObject(parent), targetIp(ip), targetPorts(ports), totalPorts(ports.size()), scannedPorts(0)
{
    setAutoDelete(true);
}

void PortScanWorker::run()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    emit scanStarted(targetIp, totalPorts);

    for (int port : targetPorts) {
        QElapsedTimer portTimer;
        portTimer.start();

        bool open = isPortOpen(targetIp, port);
        int responseTime = portTimer.elapsed();

        QString service = getServiceName(port);
        QString banner = "";
        QString version = "";

        if (open) {
            // Try to grab banner for service identification
            QPair<QString, QString> bannerInfo = grabBanner(targetIp, port);
            banner = bannerInfo.first;
            version = bannerInfo.second;

            if (version.isEmpty() && !banner.isEmpty()) {
                version = extractVersionFromBanner(banner, service);
            }
        }

        emit portResult(targetIp, port, open, service, version, banner, responseTime);

        scannedPorts++;
        emit scanProgress(scannedPorts, totalPorts);

        // Small delay to avoid overwhelming the target
        QThread::msleep(10);
    }

    qint64 totalTime = scanTimer.elapsed();
    emit scanCompleted(targetIp, scannedPorts, totalTime);
}

bool PortScanWorker::isPortOpen(const QString &ip, int port)
{
    QTcpSocket socket;
    socket.connectToHost(ip, port, QIODevice::ReadWrite);
    bool connected = socket.waitForConnected(3000);

    if (connected) {
        socket.disconnectFromHost();
        socket.waitForDisconnected(1000);
    }

    return connected;
}

QString PortScanWorker::getServiceName(int port)
{
    static QMap<int, QString> services = {
        {20, "FTP-Data"}, {21, "FTP"}, {22, "SSH"}, {23, "Telnet"},
        {25, "SMTP"}, {37, "Time"}, {42, "nameserver"}, {43, "WHOIS"},
        {49, "TACACS"}, {53, "DNS"}, {67, "DHCP-Server"}, {68, "DHCP-Client"},
        {69, "TFTP"}, {70, "Gopher"}, {79, "Finger"}, {80, "HTTP"},
        {88, "Kerberos"}, {102, "MS Exchange"}, {110, "POP3"}, {113, "Ident"},
        {119, "NNTP"}, {123, "NTP"}, {135, "RPC"}, {137, "NetBIOS-ns"},
        {138, "NetBIOS-dgm"}, {139, "NetBIOS-ssn"}, {143, "IMAP"},
        {161, "SNMP"}, {162, "SNMP-Trap"}, {179, "BGP"}, {194, "IRC"},
        {389, "LDAP"}, {396, "Novell"}, {443, "HTTPS"}, {444, "SNPP"},
        {445, "SMB"}, {458, "Apple QuickTime"}, {546, "DHCPv6-Client"},
        {547, "DHCPv6-Server"}, {563, "NNTP+SSL"}, {569, "MSN"},
        {636, "LDAP+SSL"}, {691, "MS Exchange"}, {902, "VMware"},
        {993, "IMAPS"}, {995, "POP3S"}, {1025, "Microsoft RPC"},
        {1194, "OpenVPN"}, {1337, "WASTE"}, {1433, "MSSQL"},
        {1434, "MSSQL-Monitor"}, {1521, "Oracle"}, {1723, "PPTP"},
        {1741, "CiscoWorks"}, {1755, "MS-Streaming"}, {1863, "MSN"},
        {2049, "NFS"}, {2082, "cPanel"}, {2083, "cPanel-SSL"},
        {2086, "WHM"}, {2087, "WHM-SSL"}, {2095, "Webmail"},
        {2096, "Webmail-SSL"}, {2181, "Zookeeper"}, {3128, "Squid"},
        {3306, "MySQL"}, {3389, "RDP"}, {4333, "mSQL"}, {4899, "Radmin"},
        {5060, "SIP"}, {5432, "PostgreSQL"}, {5500, "VNC-HTTP"},
        {5631, "pcAnywhere"}, {5632, "pcAnywhere"}, {5800, "VNC-HTTP"},
        {5900, "VNC"}, {6379, "Redis"}, {6667, "IRC"}, {7000, "Cassandra"},
        {8000, "HTTP-Alt"}, {8008, "HTTP-Alt"}, {8080, "HTTP-Proxy"},
        {8181, "HTTP-Alt"}, {8443, "HTTPS-Alt"}, {8888, "HTTP-Alt"},
        {9000, "SonarQube"}, {9042, "Cassandra"}, {9200, "Elasticsearch"},
        {9300, "Elasticsearch"}, {11211, "Memcached"}, {27017, "MongoDB"},
        {50070, "Hadoop"}
    };

    return services.value(port, "Unknown");
}

QPair<QString, QString> PortScanWorker::grabBanner(const QString &ip, int port)
{
    QTcpSocket socket;
    socket.connectToHost(ip, port, QIODevice::ReadWrite);

    if (!socket.waitForConnected(3000)) {
        return qMakePair(QString(), QString());
    }

    // Send appropriate probe based on port
    QString probe = getProbeForPort(port);
    if (!probe.isEmpty()) {
        socket.write(probe.toUtf8());
        socket.flush();
    }

    // Wait for response
    if (socket.waitForReadyRead(2000)) {
        QByteArray data = socket.readAll();
        QString banner = QString::fromUtf8(data).trimmed();

        // Extract version information
        QString version = extractVersionFromBanner(banner, getServiceName(port));

        socket.disconnectFromHost();
        return qMakePair(banner, version);
    }

    socket.disconnectFromHost();
    return qMakePair(QString(), QString());
}

QString PortScanWorker::getProbeForPort(int port)
{
    static QMap<int, QString> probes = {
        {21, ""},  // FTP sends banner automatically
        {22, ""},  // SSH sends banner automatically
        {25, "EHLO test\r\n"},  // SMTP
        {53, ""},  // DNS
        {80, "GET / HTTP/1.0\r\n\r\n"},  // HTTP
        {110, ""},  // POP3 sends banner automatically
        {143, ""},  // IMAP sends banner automatically
        {443, ""},  // HTTPS - would need SSL
        {993, ""},  // IMAPS - would need SSL
        {995, ""}   // POP3S - would need SSL
    };

    return probes.value(port, "");
}

QString PortScanWorker::extractVersionFromBanner(const QString &banner, const QString &service)
{
    if (banner.isEmpty()) return QString();

    // Common version extraction patterns
    QRegularExpression versionRegex;

    if (service == "SSH") {
        versionRegex.setPattern(R"(SSH-[\d\.]+-([\w\.\-_]+))");
    } else if (service == "HTTP" || service == "HTTPS") {
        versionRegex.setPattern(R"(Server:\s*([^\r\n]+))");
    } else if (service == "FTP") {
        versionRegex.setPattern(R"(220[^\r\n]*([A-Za-z]+[\d\.]+))");
    } else if (service == "SMTP") {
        versionRegex.setPattern(R"(220[^\r\n]*([A-Za-z]+[\d\.]+))");
    } else if (service == "MySQL") {
        versionRegex.setPattern(R"([\d\.]+(-[^\s]+)?)");
    } else {
        // Generic version pattern
        versionRegex.setPattern(R"([\d]+\.[\d]+(?:\.[\d]+)?)");
    }

    QRegularExpressionMatch match = versionRegex.match(banner);
    if (match.hasMatch()) {
        return match.captured(1).isEmpty() ? match.captured(0) : match.captured(1);
    }

    return QString();
}

// ============================================================================
// Enhanced PingWorker implementation
// ============================================================================
PingWorker::PingWorker(const QString &ip, QObject *parent)
    : QObject(parent), targetIp(ip)
{
    setAutoDelete(true);
}

void PingWorker::run()
{
    QElapsedTimer timer;
    timer.start();

    // Try multiple ping methods for better reliability
    bool alive = false;
    int responseTime = -1;
    QString method = "";

    // Method 1: TCP ping to port 80
    alive = tcpPing(targetIp, 80, 2000);
    if (alive) {
        responseTime = timer.elapsed();
        method = "TCP-80";
    }

    // Method 2: TCP ping to port 443 if port 80 failed
    if (!alive) {
        timer.restart();
        alive = tcpPing(targetIp, 443, 2000);
        if (alive) {
            responseTime = timer.elapsed();
            method = "TCP-443";
        }
    }

    // Method 3: TCP ping to port 22 if both web ports failed
    if (!alive) {
        timer.restart();
        alive = tcpPing(targetIp, 22, 2000);
        if (alive) {
            responseTime = timer.elapsed();
            method = "TCP-22";
        }
    }

    // Method 4: Try a few more common ports
    if (!alive) {
        QList<int> commonPorts = {21, 23, 25, 53, 110, 143, 993, 995, 3389};
        for (int port : commonPorts) {
            timer.restart();
            if (tcpPing(targetIp, port, 1000)) {
                alive = true;
                responseTime = timer.elapsed();
                method = QString("TCP-%1").arg(port);
                break;
            }
        }
    }

    emit pingResult(targetIp, alive, responseTime);
    emit pingCompleted(targetIp, alive, responseTime, method);
}

bool PingWorker::tcpPing(const QString &ip, int port, int timeout)
{
    QTcpSocket socket;
    socket.connectToHost(ip, port, QIODevice::ReadOnly);
    bool connected = socket.waitForConnected(timeout);

    if (connected) {
        socket.disconnectFromHost();
        socket.waitForDisconnected(1000);
    }

    return connected;
}

// ============================================================================
// Network Discovery Worker
// ============================================================================
NetworkDiscoveryWorker::NetworkDiscoveryWorker(const QString &interface, QObject *parent)
    : QObject(parent), networkInterface(interface)
{
    setAutoDelete(true);
}

void NetworkDiscoveryWorker::run()
{
    try {
        emit discoveryStarted(networkInterface);

        // Sicherheitscheck
        if (networkInterface.isEmpty()) {
            emit discoveryCompleted(networkInterface);
            return;
        }

        const auto interfaces = QNetworkInterface::allInterfaces();
        bool interfaceFound = false;

        for (const QNetworkInterface &iface : interfaces) {
            if (networkInterface.isEmpty() || iface.name().contains(networkInterface)) {
                if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
                    !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

                    interfaceFound = true;
                    const auto addressEntries = iface.addressEntries();

                    for (const QNetworkAddressEntry &entry : addressEntries) {
                        if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                            QString network = entry.ip().toString();
                            int prefixLength = entry.prefixLength();
                            QString mac = iface.hardwareAddress();

                            emit networkFound(network, prefixLength, iface.name(), mac);

                            // Sichere Netzwerk-Scan Durchführung
                            try {
                                performEnhancedNetworkScan(network, prefixLength, iface.name());
                            } catch (...) {
                                qDebug() << "Exception im Enhanced Network Scan";
                            }
                        }
                    }
                }
            }
        }

        if (!interfaceFound) {
            qDebug() << "Interface nicht gefunden:" << networkInterface;
        }

        emit discoveryCompleted(networkInterface);

    } catch (const std::exception& e) {
        qDebug() << "Exception in Network Discovery:" << e.what();
        emit discoveryCompleted(networkInterface);
    } catch (...) {
        qDebug() << "Unbekannte Exception in Network Discovery";
        emit discoveryCompleted(networkInterface);
    }
}

void NetworkDiscoveryWorker::performEnhancedNetworkScan(const QString &baseIp, int prefixLength, const QString &interfaceName)
{
    Q_UNUSED(interfaceName)  // Suppress warning

    if (prefixLength < 16 || prefixLength > 30) {
        return; // Only handle reasonable subnet sizes
    }

    QStringList ipParts = baseIp.split('.');
    if (ipParts.size() != 4) return;

    QString networkBase = QString("%1.%2.%3.").arg(ipParts[0], ipParts[1], ipParts[2]);

    // Enhanced scanning strategy
    QList<int> priorityHosts = {1, 2, 254, 100, 10, 20}; // Likely gateways/servers
    QList<int> commonHosts;

    // Add range based on prefix length
    if (prefixLength >= 24) {
        for (int i = 1; i <= 254; i++) {
            if (!priorityHosts.contains(i)) {
                commonHosts.append(i);
            }
        }
    }

    int totalHosts = priorityHosts.size() + qMin(50, commonHosts.size()); // Limit for performance
    int scannedHosts = 0;

    // Scan priority hosts first
    for (int host : priorityHosts) {
        QString targetIp = networkBase + QString::number(host);
        if (targetIp == baseIp) continue;

        if (scanHost(targetIp)) {
            QString deviceType = determineDeviceType(targetIp, host);
            QString hostname = resolveHostname(targetIp);
            emit deviceFound(targetIp, hostname, "", deviceType);
        }

        scannedHosts++;
        emit discoveryProgress(scannedHosts, totalHosts);
        QThread::msleep(100); // Rate limiting
    }

    // Quick scan of sample hosts (limit to 50 for performance)
    int sampleSize = qMin(50, commonHosts.size());
    for (int i = 0; i < sampleSize; i++) {
        int hostIndex = i * (commonHosts.size() / qMax(1, sampleSize));
        if (hostIndex >= commonHosts.size()) break;

        int host = commonHosts[hostIndex];
        QString targetIp = networkBase + QString::number(host);
        if (targetIp == baseIp) continue;

        if (quickScanHost(targetIp)) {
            QString deviceType = determineDeviceType(targetIp, host);
            QString hostname = resolveHostname(targetIp);
            emit deviceFound(targetIp, hostname, "", deviceType);
        }

        scannedHosts++;
        emit discoveryProgress(scannedHosts, totalHosts);
        QThread::msleep(50); // Faster rate limiting
    }
}

bool NetworkDiscoveryWorker::scanHost(const QString &ip)
{
    // Multi-port TCP ping for better detection
    QList<int> ports = {22, 23, 53, 80, 135, 139, 443, 445, 993, 995, 3389, 5900, 8080};

    for (int port : ports) {
        QTcpSocket socket;
        socket.connectToHost(ip, port, QIODevice::ReadOnly);
        if (socket.waitForConnected(1000)) {
            socket.disconnectFromHost();
            return true;
        }
    }
    return false;
}

bool NetworkDiscoveryWorker::quickScanHost(const QString &ip)
{
    // Quick scan on most common ports
    QList<int> quickPorts = {22, 80, 443, 135, 445};

    for (int port : quickPorts) {
        QTcpSocket socket;
        socket.connectToHost(ip, port, QIODevice::ReadOnly);
        if (socket.waitForConnected(500)) {
            socket.disconnectFromHost();
            return true;
        }
    }
    return false;
}

QString NetworkDiscoveryWorker::determineDeviceType(const QString &ip, int hostNumber)
{
    // Heuristics for device type determination
    if (hostNumber == 1 || hostNumber == 254) {
        return "Router/Gateway";
    }

    // Check for common server ports
    QList<int> serverPorts = {22, 23, 25, 53, 80, 110, 143, 443, 993, 995, 3389, 5432, 3306};
    int serverPortCount = 0;

    for (int port : serverPorts) {
        QTcpSocket socket;
        socket.connectToHost(ip, port, QIODevice::ReadOnly);
        if (socket.waitForConnected(500)) {
            serverPortCount++;
            socket.disconnectFromHost();
        }
        if (serverPortCount >= 2) break; // Early exit for performance
    }

    if (serverPortCount >= 3) {
        return "Server";
    } else if (serverPortCount > 0) {
        return "Computer/Device";
    } else {
        return "Unknown Device";
    }
}

QString NetworkDiscoveryWorker::resolveHostname(const QString &ip)
{
    QHostInfo hostInfo = QHostInfo::fromName(ip);
    if (hostInfo.error() == QHostInfo::NoError && !hostInfo.hostName().isEmpty()) {
        QString hostname = hostInfo.hostName();
        if (hostname != ip && !hostname.isEmpty()) {
            return hostname;
        }
    }
    return QString();
}

// ============================================================================
// ARP Scanner Worker Implementation
// ============================================================================
ArpScanWorker::ArpScanWorker(const QString &interface, QObject *parent)
    : QObject(parent), networkInterface(interface)
{
    setAutoDelete(true);
}

void ArpScanWorker::run()
{
    // Simple ARP scan implementation
    emit arpScanCompleted(0); // Placeholder - would need platform-specific implementation
}

QString ArpScanWorker::getMacVendor(const QString &mac)
{
    Q_UNUSED(mac)
    return "Unknown"; // Placeholder - would need MAC vendor database
}

// ============================================================================
// Traceroute Worker Implementation
// ============================================================================
TracerouteWorker::TracerouteWorker(const QString &target, QObject *parent)
    : QObject(parent), targetHost(target)
{
    setAutoDelete(true);
}

void TracerouteWorker::run()
{
    // Placeholder implementation
    emit tracerouteCompleted(targetHost, 0);
}

// ============================================================================
// DNS Lookup Worker Implementation
// ============================================================================
DnsLookupWorker::DnsLookupWorker(const QString &target, QObject *parent)
    : QObject(parent), targetHost(target)
{
    setAutoDelete(true);
}

void DnsLookupWorker::run()
{
    // DNS lookup implementation
    QHostInfo hostInfo = QHostInfo::fromName(targetHost);

    if (hostInfo.error() == QHostInfo::NoError) {
        QStringList addresses;
        for (const QHostAddress &address : hostInfo.addresses()) {
            addresses.append(address.toString());
        }
        emit dnsResult(hostInfo.hostName(), addresses);
    }

    emit dnsCompleted(targetHost);
}

// ============================================================================
// WHOIS Worker Implementation
// ============================================================================
WhoisWorker::WhoisWorker(const QString &target, QObject *parent)
    : QObject(parent), targetHost(target)
{
    setAutoDelete(true);
}

void WhoisWorker::run()
{
    // Placeholder - would need WHOIS protocol implementation
    emit whoisResult(targetHost, "WHOIS functionality not yet implemented");
    emit whoisCompleted(targetHost);
}

// ============================================================================
// Service Detection Worker Implementation
// ============================================================================
ServiceDetectionWorker::ServiceDetectionWorker(const QString &ip, int port, QObject *parent)
    : QObject(parent), targetIp(ip), targetPort(port)
{
    setAutoDelete(true);
}

void ServiceDetectionWorker::run()
{
    QString service = "Unknown";
    QString version = "Unknown";
    QString banner = "";

    // Try to connect and grab banner
    QTcpSocket socket;
    socket.connectToHost(targetIp, targetPort, QIODevice::ReadWrite);

    if (socket.waitForConnected(3000)) {
        // Service-specific detection
        if (targetPort == 80 || targetPort == 8080) {
            service = detectHttpService(targetIp, targetPort);
        } else if (targetPort == 22) {
            service = detectSshService(targetIp, targetPort);
        } else if (targetPort == 21) {
            service = detectFtpService(targetIp, targetPort);
        } else if (targetPort == 25) {
            service = detectSmtpService(targetIp, targetPort);
        }

        // Try to read banner
        if (socket.waitForReadyRead(2000)) {
            banner = QString::fromUtf8(socket.readAll()).trimmed();
        }

        socket.disconnectFromHost();
    }

    emit serviceDetected(targetIp, targetPort, service, version, banner);
}

QString ServiceDetectionWorker::detectHttpService(const QString &ip, int port)
{
    QTcpSocket socket;
    socket.connectToHost(ip, port, QIODevice::ReadWrite);

    if (socket.waitForConnected(2000)) {
        QString request = "GET / HTTP/1.0\r\nHost: " + ip + "\r\n\r\n";
        socket.write(request.toUtf8());
        socket.flush();

        if (socket.waitForReadyRead(3000)) {
            QString response = QString::fromUtf8(socket.readAll());
            if (response.contains("Server:")) {
                QRegularExpression serverRegex(R"(Server:\s*([^\r\n]+))");
                QRegularExpressionMatch match = serverRegex.match(response);
                if (match.hasMatch()) {
                    return QString("HTTP (%1)").arg(match.captured(1));
                }
            }
            return "HTTP Server";
        }
        socket.disconnectFromHost();
    }

    return "HTTP";
}

QString ServiceDetectionWorker::detectSshService(const QString &ip, int port)
{
    QTcpSocket socket;
    socket.connectToHost(ip, port, QIODevice::ReadWrite);

    if (socket.waitForConnected(2000)) {
        if (socket.waitForReadyRead(3000)) {
            QString banner = QString::fromUtf8(socket.readAll());
            if (banner.startsWith("SSH-")) {
                return QString("SSH (%1)").arg(banner.trimmed());
            }
        }
        socket.disconnectFromHost();
    }

    return "SSH";
}

QString ServiceDetectionWorker::detectFtpService(const QString &ip, int port)
{
    QTcpSocket socket;
    socket.connectToHost(ip, port, QIODevice::ReadWrite);

    if (socket.waitForConnected(2000)) {
        if (socket.waitForReadyRead(3000)) {
            QString banner = QString::fromUtf8(socket.readAll());
            if (banner.startsWith("220")) {
                return QString("FTP (%1)").arg(banner.split('\n').first().trimmed());
            }
        }
        socket.disconnectFromHost();
    }

    return "FTP";
}

QString ServiceDetectionWorker::detectSmtpService(const QString &ip, int port)
{
    QTcpSocket socket;
    socket.connectToHost(ip, port, QIODevice::ReadWrite);

    if (socket.waitForConnected(2000)) {
        if (socket.waitForReadyRead(3000)) {
            QString banner = QString::fromUtf8(socket.readAll());
            if (banner.startsWith("220")) {
                return QString("SMTP (%1)").arg(banner.split('\n').first().trimmed());
            }
        }
        socket.disconnectFromHost();
    }

    return "SMTP";
}

// ============================================================================
// Network Health Worker Implementation
// ============================================================================
NetworkHealthWorker::NetworkHealthWorker(const QStringList &targets, QObject *parent)
    : QObject(parent), targetHosts(targets)
{
    setAutoDelete(true);
}

void NetworkHealthWorker::run()
{
    try {
        emit healthCheckStarted(targetHosts.size());

        int reachableCount = 0;
        int unreachableCount = 0;

        for (const QString &ip : targetHosts) {
            try {
                int responseTime = -1;
                int packetLoss = 0;
                bool reachable = performPingTest(ip, responseTime, packetLoss);

                QString status = getHealthStatus(reachable, responseTime, packetLoss);

                emit hostHealthResult(ip, reachable, responseTime, packetLoss, status);

                if (reachable) {
                    reachableCount++;
                } else {
                    unreachableCount++;
                }

                // Rate limiting
                QThread::msleep(100);

            } catch (...) {
                qDebug() << "Exception beim Health Check von IP:" << ip;
                unreachableCount++;
            }
        }

        emit healthCheckCompleted(targetHosts.size(), reachableCount, unreachableCount);

    } catch (const std::exception& e) {
        qDebug() << "Exception in Network Health Check:" << e.what();
        emit healthCheckCompleted(0, 0, 0);
    } catch (...) {
        qDebug() << "Unbekannte Exception in Network Health Check";
        emit healthCheckCompleted(0, 0, 0);
    }
}

bool NetworkHealthWorker::performPingTest(const QString &ip, int &responseTime, int &packetLoss)
{
    // Multi-method ping test for reliability
    QElapsedTimer timer;
    timer.start();

    // Method 1: TCP ping to common ports
    QList<int> testPorts = {80, 443, 22, 23, 25, 53, 135, 139, 445};
    bool tcpSuccess = false;

    for (int port : testPorts) {
        QTcpSocket socket;
        timer.restart();
        socket.connectToHost(ip, port, QIODevice::ReadOnly);
        if (socket.waitForConnected(2000)) {
            responseTime = timer.elapsed();
            tcpSuccess = true;
            socket.disconnectFromHost();
            break;
        }
    }

    if (tcpSuccess) {
        packetLoss = 0; // TCP connection successful
        return true;
    }

    // Method 2: Multiple quick attempts to estimate packet loss
    int attempts = 3;
    int successes = 0;
    QList<int> responseTimes;

    for (int i = 0; i < attempts; i++) {
        timer.restart();
        QTcpSocket socket;
        socket.connectToHost(ip, 80, QIODevice::ReadOnly);
        if (socket.waitForConnected(1500)) {
            responseTimes.append(timer.elapsed());
            successes++;
            socket.disconnectFromHost();
        }
        QThread::msleep(500); // Wait between attempts
    }

    if (successes > 0) {
        // Calculate average response time
        int totalTime = 0;
        for (int time : responseTimes) {
            totalTime += time;
        }
        responseTime = totalTime / successes;
        packetLoss = ((attempts - successes) * 100) / attempts;
        return true;
    }

    responseTime = -1;
    packetLoss = 100;
    return false;
}

QString NetworkHealthWorker::getHealthStatus(bool reachable, int responseTime, int packetLoss)
{
    if (!reachable) {
        return "Nicht erreichbar 🔴";
    }

    if (packetLoss == 0 && responseTime < 50) {
        return "Ausgezeichnet 🟢";
    } else if (packetLoss < 20 && responseTime < 200) {
        return "Gut 🟡";
    } else if (packetLoss < 50 && responseTime < 1000) {
        return "Mäßig 🟠";
    } else {
        return "Kritisch 🔴";
    }
}
