#ifndef CRYPTFILE_HPP
#define CRYPTFILE_HPP

#include <QtGui>

class CryptFile : public QFile {
    Q_OBJECT

public:
    explicit CryptFile(QString fname, QObject *parent = 0);

    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 maxSize);
};

#endif // CRYPTFILE_HPP
