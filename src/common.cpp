#include "common.hpp"

QString find_newest(const dirmap_t& dirs, const QString& name) {
    QString file;
    for (dirmap_t::const_iterator it = dirs.begin() ; it != dirs.end() ; ++it) {
        QString str(it->second);
        QDir dir(str);
        if (dir.exists(name)) {
            file = dir.absoluteFilePath(name);
            break;
        }
    }
    return file;
}

size_t read_revision(const QString& name) {
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QVariant s(line);
        return s.toULongLong();
    }

    return 0;
}
