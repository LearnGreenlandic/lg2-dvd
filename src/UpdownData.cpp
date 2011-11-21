#include "UpdownData.hpp"
#include <algorithm>

UpdownData::UpdownData(const dirmap_t& dirs, QString which) :
dirs(dirs),
which(which)
{
    QSettings settings;
    QString lang = settings.value("language").toString();

    QString glossfn = find_newest(dirs, QString("fsts/") + lang + "_gloss.txt");
    if (glossfn.isEmpty()) {
        QMessageBox::critical(0, "Missing Data!", QString("Could not find fsts/") + lang + "_gloss.txt");
        throw(-1);
    }

    QString tmp;

    QFile fg(glossfn);
    fg.open(QIODevice::ReadOnly);
    QTextStream tg(&fg);
    tg.setCodec("UTF-8");

    while (!tg.atEnd()) {
        tmp = tg.readLine();
        if (tmp.isNull()) {
            break;
        }
        tmp = tmp.trimmed();
        if (!tmp.isEmpty() && tmp.at(0) != '#' && tmp.contains('\t')) {
            QStringList ls = tmp.split('\t');
            if (ls.size() == 2) {
                QString a = ls.at(0).trimmed();
                QString b = ls.at(1).trimmed();
                glosses[a] = b;
            }
        }
    }

    loadRandom();

    if (glosses.empty() || updowns.empty()) {
        QMessageBox::critical(0, "Data Error!", "Failed to read data files!");
        throw(-1);
    }

    std::random_shuffle(updowns.begin(), updowns.end());
}

void UpdownData::loadRandom() {
    QString fstfn = find_newest(dirs, which + ".fst");
    if (fstfn.isEmpty()) {
        QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + ".fst");
        throw(-1);
    }

#ifdef Q_WS_WIN
    QString foma = find_newest(dirs, "foma/win32/foma.exe");
#elif Q_WS_MAC
    QString foma = find_newest(dirs, "foma/osx/foma");
#elif Q_OS_LINUX
    QString foma;
    if (sizeof(void*) == 8) {
        foma = find_newest(dirs, "foma/linux_64/foma");
    }
    else {
        foma = find_newest(dirs, "foma/linux/foma");
    }
#else
    #error "Sorry, please provide a Foma for this arch."
#endif

    QStringList ups, downs;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("CYGWIN", "nodosfilewarning");

    {
        QProcess pf;
        pf.setProcessChannelMode(QProcess::MergedChannels);
        pf.setProcessEnvironment(env);
        QStringList args;
        args.push_back("-e");
        args.push_back(QString("load ") + fstfn);
        args.push_back("-e");
        args.push_back("random-upper 10000");
        args.push_back("-e");
        args.push_back("quit");

        pf.start(foma, args, QIODevice::ReadOnly);
        if (!pf.waitForStarted()) {
            QMessageBox::critical(0, "Foma Error!", QString("Could not launch ") + foma);
            throw(-1);
        }

        pf.closeWriteChannel();
        if (!pf.waitForReadyRead()) {
            QMessageBox::critical(0, "Foma Error!", "Could not read from Foma");
            throw(-1);
        }

        QTextStream tg(&pf);
        tg.setCodec("UTF-8");
        QRegExp rx("^\\[\\d+\\]\\s+(.+)$");
        QString tmp;
        while (!tg.atEnd()) {
            tmp = tg.readLine();
            if (tmp.isNull()) {
                break;
            }
            tmp = tmp.trimmed();
            if (rx.indexIn(tmp) != -1) {
                QString s = rx.capturedTexts().at(1);
                ups.push_back(s);
            }
        }

        if (!pf.waitForFinished()) {
            QMessageBox::critical(0, "Foma Error!", "Foma did not terminate");
            throw(-1);
        }
    }

    {
        QProcess pf;
        //pf.setProcessChannelMode(QProcess::MergedChannels);
        pf.setProcessEnvironment(env);
        QStringList args;
        args.push_back("-e");
        args.push_back(QString("load ") + fstfn);

        pf.start(foma, args);
        if (!pf.waitForStarted()) {
            QMessageBox::critical(0, "Foma Error!", QString("Could not launch ") + foma);
            throw(-1);
        }

        QByteArray ab;
        foreach (QString s, ups) {
            ab.append("down ");
            ab.append(s.toUtf8());
            ab.append("\n");
        }
        ab.append("quit\n");
        if (pf.write(ab) != ab.size()) {
            QMessageBox::critical(0, "Foma Error!", "Could not write to Foma");
            throw(-1);
        }

        pf.closeWriteChannel();
        if (!pf.waitForReadyRead()) {
            QMessageBox::critical(0, "Foma Error!", "Could not read from Foma");
            throw(-1);
        }

        if (!pf.waitForFinished()) {
            QMessageBox::critical(0, "Foma Error!", "Foma did not terminate");
            throw(-1);
        }
        ab = pf.readAllStandardOutput();

        QTextStream tg(&ab, QIODevice::ReadOnly);
        tg.setCodec("UTF-8");
        QRegExp rx("^foma\\[\\d+\\]:.*\\s(.+)$");
        QString tmp;
        while (!tg.atEnd()) {
            tmp = tg.readLine();
            if (tmp.isNull()) {
                break;
            }
            tmp = tmp.trimmed();
            if (rx.indexIn(tmp) != -1) {
                QString s = rx.capturedTexts().at(1);
                downs.push_back(s);
            }
        }
    }

    for (int i=0 ; i<ups.size() ; ++i) {
        updowns.push_back(qMakePair(downs.at(i), ups.at(i)));
    }
}

QString UpdownData::glossUpperDetailed(QString upper) {
    QStringList ql;
    QRegExp qr("[-+]");
    int o = 0, n = 0;
    while ((n = qr.indexIn(upper, o)) != -1) {
        if (o) {
            --o;
        }
        QString t = upper.mid(o, n-o);
        if (glosses.find(t) != glosses.end()) {
            ql.push_back(QString("<tr><td>") + t + "</td><td width='20'>&nbsp;</td><td>" + glosses[t] + "</td></tr>");
        }
        else {
            ql.push_back(QString("<tr><td>") + t + "</td><td width='20'>&nbsp;</td><td>" + t + "</td></tr>");
        }
        o = n+1;
    }

    if (o) {
        --o;
    }
    QString t = upper.mid(o);
    if (glosses.find(t) != glosses.end()) {
        ql.push_back(QString("<tr><td>") + t + "</td><td width='20'>&nbsp;</td><td>" + glosses[t] + "</td></tr>");
    }
    else {
        ql.push_back(QString("<tr><td>") + t + "</td><td width='20'>&nbsp;</td><td>" + t + "</td></tr>");
    }

    return QString("<font size='+2'><table>") + ql.join("\n") + "</table></font>";
}

QString UpdownData::glossUpperShort(QString upper) {
    QStringList ql;
    QRegExp qr("[-+]");
    int o = 0, n = 0;
    while ((n = qr.indexIn(upper, o)) != -1) {
        if (o) {
            --o;
        }
        QString t = upper.mid(o, n-o);
        if (glosses.find(t) != glosses.end()) {
            ql.push_back(glosses[t]);
        }
        else {
            ql.push_back(t);
        }
        o = n+1;
    }

    if (o) {
        --o;
    }
    QString t = upper.mid(o);
    if (glosses.find(t) != glosses.end()) {
        ql.push_back(glosses[t]);
    }
    else {
        ql.push_back(t);
    }

    return QString("<font size='+2'>") + ql.join(" + ") + "</font>";
}
