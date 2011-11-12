#include "CryptFile.hpp"

std::string skey;
const char *key = 0;
qint64 kl = 0;

inline void xor_wrap(char *data, qint64 size, qint64 offset) {
    offset %= kl;

    for (qint64 i=0 ; i<size ; ++i) {
        data[i] ^= key[offset++];
        if (offset >= kl) {
            offset -= kl;
        }
    }
}

CryptFile::CryptFile(QString fname, QObject *parent) :
QFile(fname, parent)
{
    QSettings settings;

    skey = settings.value("encryption_key", "You must have a valid program key and login to use this. http://learngreenlandic.com/").toString().toStdString();
    key = skey.c_str();
    kl = skey.length()+1;
}

qint64 CryptFile::readData(char *data, qint64 maxSize) {
    qint64 p = pos();
    qint64 r = QFile::readData(data, maxSize);

    xor_wrap(data, r, p);

    return r;
}

const size_t bz = 16384;
char buf[bz];

qint64 CryptFile::writeData(const char *data, qint64 maxSize) {
    qint64 r = 0;

    qint64 i = 0;
    for ( ; i+bz < maxSize ; i += bz) {
        memcpy(buf, &data[i], bz);
        xor_wrap(buf, bz, pos());
        r += QFile::writeData(buf, bz);
    }

    memcpy(buf, &data[i], maxSize-i);
    xor_wrap(buf, maxSize-i, pos());
    r += QFile::writeData(buf, maxSize-i);

    return r;
}
