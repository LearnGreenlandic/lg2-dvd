#include "QAData.hpp"
#include <algorithm>

QAData::QAData(const dirmap_t& dirs, QString which) :
dirs(dirs),
which(which)
{
    QString pairfn = find_newest(dirs, which + "/pairs.txt");
    if (pairfn.isEmpty()) {
        QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/pairs.txt");
        throw(-1);
    }

    QFile pairf(pairfn);
    if (!pairf.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/pairs.txt");
        throw(-1);
    }

    QTextStream tg(&pairf);
    tg.setCodec("UTF-8");

    while (!tg.atEnd()) {
        QString tmp = tg.readLine();
        tmp = tmp.trimmed();
        if (!tmp.isEmpty() && tmp.at(0) != '#' && tmp.contains('\t')) {
            QStringList ls = tmp.split('\t');
            while (ls.size() < 4) {
                ls.push_back("");
            }
            qas.push_back(ls.at(0));

            QString qfn = find_newest(dirs, which + "/" + ls.at(0) + " Q.wav");
            ls.push_back(qfn);
            QString afn = find_newest(dirs, which + "/" + ls.at(0) + " A.wav");
            ls.push_back(afn);

            QString k = ls.takeFirst();
            data[k] = ls;
        }
    }

    QSettings settings;
    if (settings.value("language") == "english") {
        QString enfn = find_newest(dirs, which + "/pairs-en.txt");
        if (!enfn.isEmpty()) {
            QFile pairf(enfn);
            if (!pairf.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/pairs-en.txt");
                throw(-1);
            }

            QTextStream tg(&pairf);
            tg.setCodec("UTF-8");

            while (!tg.atEnd()) {
                QString tmp = tg.readLine();
                tmp = tmp.trimmed();
                if (!tmp.isEmpty() && tmp.at(0) != '#' && tmp.contains('\t')) {
                    QStringList ls = tmp.split('\t');
                    ls.pop_front();
                    QStringList &qls = data[ls.at(0)];
                    for (int i=0 ; i<ls.size() ; ++i) {
                        qls[i] = ls.at(i);
                    }
                }
            }
        }
    }

    std::random_shuffle(qas.begin(), qas.end());
}

int QAData::size() const {
    return qas.size();
}

const QString& QAData::getQ(int i) const {
    return data[qas.at(i)].at(0);
}

const QString& QAData::getA(int i) const {
    return data[qas.at(i)].at(1);
}

const QString& QAData::getStar(int i) const {
    return data[qas.at(i)].at(2);
}

const QString& QAData::getQW(int i) const {
    return data[qas.at(i)].at(3);
}

const QString& QAData::getAW(int i) const {
    return data[qas.at(i)].at(4);
}

