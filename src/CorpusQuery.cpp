#include "CorpusQuery.hpp"
#include <algorithm>

CorpusQuery::CorpusQuery(TaskChooser& tc, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc)
{
    setWindowTitle(title);

    QProgressDialog progress("Parsing Greenlandic plain text...", "", 0, 3727);
    progress.setWindowModality(Qt::WindowModal);
    progress.setCancelButton(0);
    progress.show();

    {
        progress.setLabelText("Parsing Greenlandic plain text...");
        progress.setMaximum(3727);
        progress.setValue(0);

        QString txtfn = find_newest(tc.dirs, which + "/text_greenlandic.txt");
        if (txtfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/text_greenlandic.txt");
            throw(-1);
        }

        QFile txtf(txtfn);
        if (!txtf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/text_greenlandic.txt");
            throw(-1);
        }

        QTextStream txts(&txtf);
        txts.setCodec("UTF-8");

        for (int i=0 ; !txts.atEnd() ; ++i) {
            QString tmp = txts.readLine();
            tmp = tmp.simplified();
            kals.push_back(tmp.split(QRegExp("\\s")));
            if (i % 50 == 0) {
                progress.setValue(i);
            }
        }
    }

    {
        progress.setLabelText("Parsing English plain text...");
        progress.setMaximum(3727);
        progress.setValue(0);

        QString txtfn = find_newest(tc.dirs, which + "/text_english.txt");
        if (txtfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/text_english.txt");
            throw(-1);
        }

        QFile txtf(txtfn);
        if (!txtf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/text_english.txt");
            throw(-1);
        }

        QTextStream txts(&txtf);
        txts.setCodec("UTF-8");

        for (int i=0 ; !txts.atEnd() ; ++i) {
            QString tmp = txts.readLine();
            tmp = tmp.simplified();
            engs.push_back(tmp);
            if (i % 50 == 0) {
                progress.setValue(i);
            }
        }
    }

    {
        progress.setLabelText("Parsing Danish plain text...");
        progress.setMaximum(3727);
        progress.setValue(0);

        QString txtfn = find_newest(tc.dirs, which + "/text_danish.txt");
        if (txtfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/text_danish.txt");
            throw(-1);
        }

        QFile txtf(txtfn);
        if (!txtf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/text_danish.txt");
            throw(-1);
        }

        QTextStream txts(&txtf);
        txts.setCodec("UTF-8");

        for (int i=0 ; !txts.atEnd() ; ++i) {
            QString tmp = txts.readLine();
            tmp = tmp.simplified();
            dans.push_back(tmp);
            if (i % 50 == 0) {
                progress.setValue(i);
            }
        }
    }

    {
        progress.setLabelText("Checking consistency...");
        progress.setMaximum(kals.size());
        progress.setValue(0);

        for (int i=0 ; i<kals.size() ; ++i) {
            if (!kals.at(i).empty() && !kals.at(i).at(0).isEmpty() && kals.at(i).at(0).at(0) == '!') {
                if (dans.at(i).isEmpty() || dans.at(i).at(0) != '!') {
                    QMessageBox::critical(0, "Corrupt Data!", "Greenlandic-Danish mismatch!");
                    throw(-1);
                }
                if (engs.at(i).isEmpty() || engs.at(i).at(0) != '!') {
                    QMessageBox::critical(0, "Corrupt Data!", "Greenlandic-English mismatch!");
                    throw(-1);
                }
            }
            if (i % 50 == 0) {
                progress.setValue(i);
            }
        }
    }

    if (0) {
        progress.setLabelText("Parsing Greenlandic cohorts...");
        progress.setMaximum(79662);
        progress.setValue(0);

        QString cohortsfn = find_newest(tc.dirs, which + "/cohorts.txt");
        if (cohortsfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/cohorts.txt");
            throw(-1);
        }

        QFile cohortsf(cohortsfn);
        if (!cohortsf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/cohorts.txt");
            throw(-1);
        }

        QTextStream tg(&cohortsf);
        tg.setCodec("UTF-8");

        Cohort c;
        bool in_c = false;
        int line = 0, word = 0, streak = 0;
        QRegExp wfx("^\"<(.*)>\"$"), wwx("[\\w\\d]"), wsx;
        for (int i=0 ; !tg.atEnd() ; ++i) {
            QString tmp = tg.readLine();
            if (i % 100 == 0) {
                progress.setValue(i);
            }
            if (in_c && (tmp.indexOf("\"<") == 0 || tmp.trimmed().isEmpty())) {
                bool found = false;
                wsx.setPattern(QString("\\b") + QRegExp::escape(c.wordform) + "\\b");
                int tword = word;
                for (int l=line ; l<kals.size() ; ++l) {
                    for (int w=tword ; w<kals.at(l).size() ; ++w) {
                        QString ws = kals.at(l).at(w);
                        if (ws == c.wordform || wsx.indexIn(ws) != -1) {
                            found = true;
                            line = c.line = l;
                            word = c.word = w;
                            kalc.push_back(c);
                            in_c = false;
                            streak = 0;
                            break;
                        }
                    }
                    if (found) {
                        break;
                    }
                    if (l > line+7) {
                        c.line = line;
                        c.word = word;
                        if (!kalc.empty()) {
                            c.line = kalc.back().line;
                            c.word = kalc.back().word + 1;
                        }
                        kalc.push_back(c);
                        in_c = false;
                        ++streak;
                        break;
                    }
                    if (streak > 2) {
                        while (kalc.size() > 50) {
                            kalc.pop_front();
                        }
                        break;
                    }
                    tword = 0;
                }
            }
            if (tmp.indexOf("\"<") == 0) {
                wfx.indexIn(tmp);
                c.wordform = wfx.capturedTexts().at(1);
                if (wwx.indexIn(c.wordform) == -1) {
                    in_c = false;
                    continue;
                }
                c.line = line;
                c.word = word;
                c.reading.clear();
                in_c = true;
            }
            else if (tmp.indexOf("\t") == 0 && in_c) {
                tmp = tmp.simplified();
                c.reading = tmp;
                QStringList ts = tmp.mid(tmp.lastIndexOf("\"")).split(QRegExp("\\s"));
                foreach (QString t, ts) {
                    tags.insert(t);
                }
            }
        }

        {
            QFile dbf("/Temp/cohorts.map");
            if (!dbf.open(QIODevice::WriteOnly)) {
                QMessageBox::critical(0, "Bad Data!", QString("Could not write /Temp/cohorts.map"));
                throw(-1);
            }

            QTextStream to(&dbf);
            to.setCodec("UTF-8");

            for (int i=0 ; i<kalc.size() ; ++i) {
                to << kalc.at(i).line << '\t' << kalc.at(i).word << '\t' << kalc.at(i).wordform << '\t' << kalc.at(i).reading << '\n';
            }
        }

        {
            QFile dbf("/Temp/tags.txt");
            if (!dbf.open(QIODevice::WriteOnly)) {
                QMessageBox::critical(0, "Bad Data!", QString("Could not write /Temp/tags.txt"));
                throw(-1);
            }

            QTextStream to(&dbf);
            to.setCodec("UTF-8");

            for (std::set<QString>::iterator it = tags.begin() ; it != tags.end() ; ++it) {
                to << *it << '\n';
            }
        }
    }

    if (1) {
        progress.setLabelText("Parsing Greenlandic cohorts...");
        progress.setMaximum(31146);
        progress.setValue(0);

        QString cohortsfn = find_newest(tc.dirs, which + "/cohorts.map");
        if (cohortsfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/cohorts.map");
            throw(-1);
        }

        QFile cohortsf(cohortsfn);
        if (!cohortsf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/cohorts.map");
            throw(-1);
        }

        QTextStream tg(&cohortsf);
        tg.setCodec("UTF-8");

        Cohort c;
        for (int i=0 ; !tg.atEnd() ; ++i) {
            QStringList tmp = tg.readLine().split('\t');
            c.line = QVariant(tmp.at(0)).toInt();
            c.word = QVariant(tmp.at(1)).toInt();
            c.wordform = tmp.at(2);
            c.reading = tmp.at(3);
            kalc.push_back(c);
            if (i % 100 == 0) {
                progress.setValue(i);
            }
        }
    }

    if (1) {
        progress.setLabelText("Parsing Greenlandic tags...");
        progress.setMaximum(460);
        progress.setValue(0);

        QString tagsfn = find_newest(tc.dirs, which + "/tags.txt");
        if (tagsfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/tags.txt");
            throw(-1);
        }

        QFile tagsf(tagsfn);
        if (!tagsf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/tags.txt");
            throw(-1);
        }

        QTextStream tg(&tagsf);
        tg.setCodec("UTF-8");

        for (int i=0 ; !tg.atEnd() ; ++i) {
            QString tmp = tg.readLine();
            tags.insert(tmp);
            progress.setValue(i);
        }
    }
}
