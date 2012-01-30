#include "files.hpp"
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
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 0;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QVariant s(line);
        return s.toUInt();
    }

    return 0;
}

bool check_files(const dirmap_t& dirs) {
    int z = sizeof(file_list)/sizeof(*file_list);
    QProgressDialog progress("Verifying existence of LG2 data...", "", 0, z);
    progress.setWindowModality(Qt::WindowModal);
    progress.setCancelButton(0);
    progress.show();

    for (int i=0 ; i<z ; ++i) {
        progress.setLabelText(QString("Verifying ") + file_list[i] + " ...");
        progress.setValue(progress.value()+1);
        QString n = find_newest(dirs, file_list[i]);
        if (n.isEmpty()) {
            return false;
        }
    }

    return true;
}
