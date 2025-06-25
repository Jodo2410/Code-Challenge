#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include "IpScanner.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Advanced IP Scanner");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("NetworkTools");
    app.setOrganizationDomain("networktools.com");

    // Set application icon if available
    app.setWindowIcon(QIcon(":/icons/scanner.png"));

    // Create main window
    IpScanner scanner;
    scanner.show();

    return app.exec();
}
