#include "CryptFile.hpp"
#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    app.setOrganizationDomain("learngreenlandic.com");
    app.setOrganizationName("LearnGreenlandic");
    app.setApplicationName("Learn Greenlandic 2");

    QStringList args = app.arguments();
    QIODevice *input, *output;

    if (args[1] == "-d") {
        input = new QFile(args[2], &app);
        output = new CryptFile(args[3], &app);
    }
    else {
        input = new CryptFile(args[1], &app);
        output = new QFile(args[2], &app);
    }

    if (!input->open(QIODevice::ReadOnly)) {
        std::cerr << "Failed to open input file: " << input->errorString().toStdString() << std::endl;
        return -1;
    }
    if (!output->open(QIODevice::WriteOnly)) {
        std::cerr << "Failed to open output file: " << output->errorString().toStdString() << std::endl;
        return -1;
    }

    char buf[32768];
    qint64 r;
    while (!input->atEnd() && (r = input->read(buf, sizeof(buf))) > 0) {
        if (output->write(buf, r) <= 0) {
            std::cerr << "Write failed at offsets " << input->pos() << "," << output->pos() << std::endl;
            return -1;
        }
    }

    output->close();
    input->close();

    return 0;
}
