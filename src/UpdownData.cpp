#include "UpdownData.hpp"
#include <fstream>
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

#if defined(Q_WS_WIN)
    QString foma = find_newest(dirs, "foma/win32/foma.exe");
    QString flookup = find_newest(dirs, "foma/win32/flookup.exe");
#elif defined(Q_WS_MAC)
    QString foma = find_newest(dirs, "foma/osx/foma");
    QString flookup = find_newest(dirs, "foma/osx/flookup");
#elif defined(Q_OS_LINUX)
    QString foma;
    QString flookup;
    if (sizeof(void*) == 8) {
        foma = find_newest(dirs, "foma/linux_64/foma");
        flookup = find_newest(dirs, "foma/linux_64/flookup");
    }
    else {
        foma = find_newest(dirs, "foma/linux/foma");
        flookup = find_newest(dirs, "foma/linux/flookup");
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
        args.push_back("-q");
        args.push_back("-e");
        args.push_back(QString("load ") + fstfn);
        args.push_back("-e");
        args.push_back("random-upper 5000");
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

    if (0) {
        std::ofstream f("C:/Temp/lg2-foma-output.txt", std::ios::binary);
        foreach (QString s, ups) {
            f.write(s.toUtf8().constData(), s.toUtf8().size());
            f << '\n';
        }
    }

    {
        QProcess pf;
        //pf.setProcessChannelMode(QProcess::MergedChannels);
        pf.setProcessEnvironment(env);
        QStringList args;
        args.push_back("-x");
        args.push_back("-i");
        args.push_back(fstfn);

        pf.start(flookup, args);
        if (!pf.waitForStarted()) {
            QMessageBox::critical(0, "Flookup Error!", QString("Could not launch ") + flookup);
            throw(-1);
        }

        QByteArray ab;
        foreach (QString s, ups) {
            ab.append(s.toUtf8());
            ab.append("\n");
        }
        if (0) {
            std::ofstream f("C:/Temp/lg2-flookup-input.txt", std::ios::binary);
            f.write(ab.constData(), ab.size());
        }
        if (pf.write(ab) != ab.size()) {
            QMessageBox::critical(0, "Flookup Error!", "Could not write to Flookup");
            throw(-1);
        }

        pf.closeWriteChannel();
        if (!pf.waitForReadyRead()) {
            QMessageBox::critical(0, "Flookup Error!", "Could not read from Flookup");
            throw(-1);
        }

        if (!pf.waitForFinished()) {
            QMessageBox::critical(0, "Flookup Error!", "Flookup did not terminate");
            throw(-1);
        }
        ab = pf.readAllStandardOutput();

        if (0) {
            std::ofstream f("C:/Temp/lg2-flookup-output.txt", std::ios::binary);
            f.write(ab.constData(), ab.size());
        }

        QTextStream tg(&ab, QIODevice::ReadOnly);
        tg.setCodec("UTF-8");
        QString down;
        QString tmp;
        while (!tg.atEnd()) {
            tmp = tg.readLine();
            if (tmp.trimmed().isNull()) {
                break;
            }
            down += tmp;
            down += '\n';
            if (tmp.trimmed().isEmpty()) {
                down = down.trimmed();
                downs.push_back(down);
                if (down.at(0) == '+') {
                    downs.pop_back();
                    ups.erase(ups.begin()+downs.size());
                }
                else if (down.indexOf('\n') != -1) {
                    downs.pop_back();
                    ups.erase(ups.begin()+downs.size());
                }
                down.clear();
            }
        }
    }

    if (0) {
        std::ofstream f("C:/Temp/lg2-flookup-ups.txt", std::ios::binary);
        foreach (QString s, ups) {
            f.write(s.toUtf8().constData(), s.toUtf8().size());
            f << '\n';
        }
    }

    if (0) {
        std::ofstream f("C:/Temp/lg2-flookup-downs.txt", std::ios::binary);
        foreach (QString s, downs) {
            f.write(s.toUtf8().constData(), s.toUtf8().size());
            f << '\n';
        }
    }

    if (ups.size() != downs.size()) {
        QMessageBox::warning(0, "Foma Warning!", QString("Number of upper (%1) did not match lower (%2).").arg(ups.size()).arg(downs.size()));
    }

    for (int i=0 ; i<ups.size() && i<downs.size() ; ++i) {
        updowns.push_back(qMakePair(downs.at(i), ups.at(i)));
    }
}

QString UpdownData::glossUpperDetailed(QString upper) {
    QStringList ql;
    QRegExp qr("[-<>+]");
    int o = 0, n = 0;
    while ((n = qr.indexIn(upper, o)) != -1) {
        if (o) {
            --o;
        }
        QString t = upper.mid(o, n-o), tx = t;
        tx.replace("<", "&lt;").replace(">", "&gt;");
        if (glosses.find(t) != glosses.end()) {
            ql.push_back(QString("<tr><td>") + tx + "</td><td width='20'>&nbsp;</td><td>" + glosses[t] + "</td></tr>");
        }
        else {
            ql.push_back(QString("<tr><td>") + tx + "</td><td width='20'>&nbsp;</td><td>" + tx + "</td></tr>");
        }
        o = n+1;
    }

    if (o) {
        --o;
    }
    QString t = upper.mid(o), tx = t;
    tx.replace("<", "&lt;").replace(">", "&gt;");
    if (glosses.find(t) != glosses.end()) {
        ql.push_back(QString("<tr><td>") + tx + "</td><td width='20'>&nbsp;</td><td>" + glosses[t] + "</td></tr>");
    }
    else {
        ql.push_back(QString("<tr><td>") + tx + "</td><td width='20'>&nbsp;</td><td>" + tx + "</td></tr>");
    }

    return QString("<font size='+2'><table>") + ql.join("\n") + "</table></font>";
}

QString UpdownData::glossUpperShort(QString upper) {
    QStringList ql;
    QRegExp qr("[-<>+]");
    int o = 0, n = 0;
    while ((n = qr.indexIn(upper, o)) != -1) {
        if (o) {
            --o;
        }
        QString t = upper.mid(o, n-o), tx = t;
        tx.replace("<", "&lt;").replace(">", "&gt;");
        if (glosses.find(t) != glosses.end()) {
            ql.push_back(glosses[t]);
        }
        else {
            ql.push_back(tx);
        }
        o = n+1;
    }

    if (o) {
        --o;
    }
    QString t = upper.mid(o), tx = t;
    tx.replace("<", "&lt;").replace(">", "&gt;");
    if (glosses.find(t) != glosses.end()) {
        ql.push_back(glosses[t]);
    }
    else {
        ql.push_back(tx);
    }

    return QString("<font size='+2'>") + ql.join(" + ") + "</font>";
}
