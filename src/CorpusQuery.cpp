#include "CryptFile.hpp"
#include "CorpusQuery.hpp"
#include <algorithm>

CorpusQuery::CorpusQuery(TaskChooser& tc, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc)
{
    setWindowTitle(title);
    setMinimumSize(800, 500);

    QProgressDialog progress("Parsing Greenlandic plain text...", "", 0, 3727);
    progress.setWindowModality(Qt::WindowModal);
    progress.setCancelButton(0);
    progress.show();

    {
        progress.setLabelText("Parsing Greenlandic plain text...");
        progress.setMaximum(3727);
        progress.setValue(0);

        QString txtfn = find_newest(tc.dirs, which + "/text_greenlandic.dat");
        if (txtfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/text_greenlandic.dat");
            throw(-1);
        }

        CryptFile txtf(txtfn);
        if (!txtf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/text_greenlandic.dat");
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

        QString txtfn = find_newest(tc.dirs, which + "/text_english.dat");
        if (txtfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/text_english.dat");
            throw(-1);
        }

        CryptFile txtf(txtfn);
        if (!txtf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/text_english.dat");
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

        QString txtfn = find_newest(tc.dirs, which + "/text_danish.dat");
        if (txtfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/text_danish.dat");
            throw(-1);
        }

        CryptFile txtf(txtfn);
        if (!txtf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/text_danish.dat");
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
            QFile dbf("/Temp/cohorts.dat");
            if (!dbf.open(QIODevice::WriteOnly)) {
                QMessageBox::critical(0, "Bad Data!", QString("Could not write /Temp/cohorts.dat"));
                throw(-1);
            }

            QTextStream to(&dbf);
            to.setCodec("UTF-8");

            for (int i=0 ; i<kalc.size() ; ++i) {
                to << kalc.at(i).line << '\t' << kalc.at(i).word << '\t' << kalc.at(i).wordform << '\t' << kalc.at(i).reading << '\n';
            }
        }

        {
            QFile dbf("/Temp/tags.dat");
            if (!dbf.open(QIODevice::WriteOnly)) {
                QMessageBox::critical(0, "Bad Data!", QString("Could not write /Temp/tags.dat"));
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

        QString cohortsfn = find_newest(tc.dirs, which + "/cohorts.dat");
        if (cohortsfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/cohorts.dat");
            throw(-1);
        }

        CryptFile cohortsf(cohortsfn);
        if (!cohortsf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/cohorts.dat");
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

        QString tagsfn = find_newest(tc.dirs, which + "/tags.dat");
        if (tagsfn.isEmpty()) {
            QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/tags.dat");
            throw(-1);
        }

        CryptFile tagsf(tagsfn);
        if (!tagsf.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/tags.dat");
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

    QVBoxLayout *vbox = new QVBoxLayout;
    QLabel *text = new QLabel(tr("corpus query text"));
    text->setWordWrap(true);
    vbox->addWidget(text);

    QHBoxLayout *hbox = new QHBoxLayout;

    input = new QLineEdit;
    connect(input, SIGNAL(returnPressed()), this, SLOT(doSearch()));
    hbox->addWidget(input);
    QPushButton *search = new QPushButton(tr("Søg"));
    connect(search, SIGNAL(clicked()), this, SLOT(doSearch()));
    hbox->addWidget(search);

    hbox->addSpacing(20);

    cbox = new QComboBox;
    cbox->addItem(tr("Indsæt et tag..."));
    for (std::set<QString>::iterator it = tags.begin() ; it != tags.end() ; ++it) {
        cbox->addItem(*it);
    }
    connect(cbox, SIGNAL(currentIndexChanged(int)), this, SLOT(insertTag(int)));
    hbox->addWidget(cbox);

    QGridLayout *opts = new QGridLayout;
    opts->addWidget(new QLabel("<b>" + tr("Søgningstilvalg") + "</b>"), 0, 0, 1, 2);
#define ADDXSX(name, text, slot, y, x) \
    xsx[name] = new QCheckBox(text); \
    connect(xsx[name], SIGNAL(stateChanged(int)), this, SLOT(slot(int))); \
    opts->addWidget(xsx[name], y, x);

    ADDXSX("text", tr("Søg i tekst"), optText, 1, 0);
    ADDXSX("tags", tr("Søg i tags"), optTags, 2, 0);
    ADDXSX("cases", tr("Store/små bogstavsforskel"), optCases, 1, 1);
    ADDXSX("whole", tr("Kun hele ord"), optWhole, 2, 1);
    ADDXSX("regex", tr("Regular expression"), optRegex, 3, 1);

    xsx["text"]->setChecked(true);

    hbox->addLayout(opts);

    vbox->addLayout(hbox);

    vbox->addSpacing(15);
    vbox->addWidget(new QLabel("<b>" + tr("Resultater") + "</b>"));

    results = new QScrollArea;
    results->setWidget(new QLabel("..."));
    results->setWidgetResizable(true);
    vbox->addWidget(results);

    setLayout(vbox);
    adjustSize();
}

void CorpusQuery::doSearch() {
    QProgressDialog progress(tr("Søger..."), "", 0, kalc.size());
    progress.setWindowModality(Qt::WindowModal);
    progress.setCancelButton(0);
    progress.show();

    QList<int> ms;

    QString query = input->text().simplified();

    if (!xsx["text"]->isChecked() && !xsx["tags"]->isChecked()) {
        xsx["text"]->setChecked(true);
    }

    if (!xsx["regex"]->isChecked()) {
        query = QRegExp::escape(query);
    }
    if (xsx["whole"]->isChecked()) {
        query = QString("(\\b|\\s)") + query + "(\\b|\\s)";
    }

    QRegExp rx(query, static_cast<Qt::CaseSensitivity>(xsx["cases"]->isChecked()));

    for (int i=0 ; i<kalc.size() ; ++i) {
        bool match = false;
        if (xsx["text"]->isChecked() && rx.indexIn(kalc.at(i).wordform) != -1) {
            match = true;
        }
        if (xsx["tags"]->isChecked() && rx.indexIn(kalc.at(i).reading) != -1) {
            match = true;
        }
        if (match) {
            ms.push_back(i);
        }
        if (i % 50 == 0) {
            progress.setValue(i);
        }
    }

    if (ms.isEmpty()) {
        results->setWidget(new QLabel(tr("Søgningen gav ingen resultater.")));
    }
    else {
        if (ms.size() > 300) {
            progress.setLabelText("Picking 300 random...");
            progress.setMaximum(ms.size());
            progress.setValue(ms.size());

            std::random_shuffle(ms.begin(), ms.end());
            while (ms.size() > 300) {
                ms.pop_back();
                if (ms.size() % 20 == 0) {
                    progress.setValue(ms.size());
                }
            }
            std::sort(ms.begin(), ms.end());
        }

        progress.setLabelText("Rendering...");
        progress.setMaximum(ms.size());
        progress.setValue(0);

        QWidget *w = new QWidget;
        QGridLayout *grid = new QGridLayout;
        grid->addWidget(new QLabel("<b>" + tr("Linie") + "</b>"), 0, 0, Qt::AlignLeft);
        grid->addWidget(new QLabel("<b>" + tr("Venstre kontekst") + "</b>"), 0, 1, Qt::AlignHCenter);
        grid->addWidget(new QLabel("<b>" + tr("Match") + "</b>"), 0, 2, Qt::AlignHCenter);
        grid->addWidget(new QLabel("<b>" + tr("Højre kontekst") + "</b>"), 0, 3, Qt::AlignHCenter);
        grid->addWidget(new QLabel("<b>" + tr("Mere") + "</b>"), 0, 4, Qt::AlignRight);

        for (int i=0 ; i<ms.size() ; ++i) {
            progress.setValue(i);
            QLabel *l;

            l = new QLabel(QVariant(kalc.at(ms.at(i)).line).toString());
            grid->addWidget(l, i+1, 0, Qt::AlignLeft|Qt::AlignTop);

            l = new QLabel;
            l->setWordWrap(true);
            l->setAlignment(Qt::AlignRight|Qt::AlignTop);
            for (int j=ms.at(i)-1 ; j>0 ; --j) {
                if (kalc.at(j).word > kalc.at(ms.at(i)).word) {
                    break;
                }
                QString wf = Qt::escape(kalc.at(j).wordform);
                QString rd = Qt::escape(kalc.at(j).reading);
                l->setText(wf + " " + l->text());
                l->setToolTip("<tr><td>" + wf + "</td><td>" + rd + "</td></tr>" + l->toolTip());
                if (l->text().size() > 100) {
                    l->setText("... " + l->text());
                    break;
                }
            }
            l->setText(l->text().simplified());
            l->setToolTip("<table border='1' cellspacing='1' cellpadding='3'><tr><th>Form</th><th>Tags</th></tr>" + l->toolTip() + "</table>");
            grid->addWidget(l, i+1, 1, Qt::AlignRight|Qt::AlignTop);

            QString wf = Qt::escape(kalc.at(ms.at(i)).wordform);
            QString rd = Qt::escape(kalc.at(ms.at(i)).reading);
            l = new QLabel("<b>" + wf + "</b>");
            l->setToolTip("<table border='1' cellspacing='1' cellpadding='3'><tr><th>Form</th><th>Tags</th></tr><tr><td>" + wf + "</td><td>" + rd + "</td></tr></table>");
            grid->addWidget(l, i+1, 2, Qt::AlignHCenter|Qt::AlignTop);

            l = new QLabel;
            l->setWordWrap(true);
            l->setAlignment(Qt::AlignLeft|Qt::AlignTop);
            for (int j=ms.at(i)+1 ; j<kalc.size() ; ++j) {
                if (kalc.at(j).word < kalc.at(ms.at(i)).word) {
                    break;
                }
                QString wf = Qt::escape(kalc.at(j).wordform);
                QString rd = Qt::escape(kalc.at(j).reading);
                l->setText(l->text() + " " + wf);
                l->setToolTip(l->toolTip() + "<tr><td>" + wf + "</td><td>" + rd + "</td></tr>");
                if (l->text().size() > 100) {
                    l->setText(l->text() + " ...");
                    break;
                }
            }
            l->setText(l->text().simplified());
            l->setToolTip("<table border='1' cellspacing='1' cellpadding='3'><tr><th>Form</th><th>Tags</th></tr>" + l->toolTip() + "</table>");
            grid->addWidget(l, i+1, 3, Qt::AlignLeft|Qt::AlignTop);

            QPushButton *b = new QPushButton(tr("Oversæt"));
            mapping[b] = ms.at(i);
            connect(b, SIGNAL(clicked()), this, SLOT(openMore()));
            grid->addWidget(b, i+1, 4, Qt::AlignRight|Qt::AlignTop);
        }

        w->setLayout(grid);
        results->setWidget(w);
    }
}

void CorpusQuery::insertTag(int i) {
    if (i) {
        input->setText(QString(input->text() + " " + cbox->itemText(i)).simplified());
        cbox->setCurrentIndex(0);
        xsx["tags"]->setChecked(true);
    }
}

void CorpusQuery::optCases(int /*i*/) {
}

void CorpusQuery::optTags(int /*i*/) {
}

void CorpusQuery::optText(int /*i*/) {
}

void CorpusQuery::optWhole(int i) {
    if (i == 2) {
        xsx["regex"]->setChecked(false);
    }
}

void CorpusQuery::optRegex(int i) {
    if (i == 2) {
        xsx["whole"]->setChecked(false);
    }
}

void CorpusQuery::openMore() {
    int i = mapping[sender()], line = kalc.at(i).line;
    QWidget *lp = new QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    lp->setWindowTitle(tr("Linie %1 kontekst og oversættelse").arg(line));
    lp->setMinimumWidth(500);

    QLabel *l;
    QString s;

    QVBoxLayout *vbox = new QVBoxLayout;

    vbox->addWidget(new QLabel("<h3>" + tr("Grønlandsk") + "</h3>"));
    if (line > 0) {
        s = kals.at(line-1).join(" ");
        l = new QLabel(QVariant(line-1).toString() + ": " + s);
        l->setWordWrap(true);
        l->setTextFormat(Qt::PlainText);
        vbox->addWidget(l);
    }
    s = Qt::escape(kals.at(line).join(" ")).replace(Qt::escape(kalc.at(i).wordform), "<b>" + Qt::escape(kalc.at(i).wordform) + "</b>");
    l = new QLabel(QVariant(line).toString() + ": " + s);
    l->setWordWrap(true);
    vbox->addWidget(l);
    if (line < kals.size()-1) {
        s = kals.at(line+1).join(" ");
        l = new QLabel(QVariant(line+1).toString() + ": " + s);
        l->setWordWrap(true);
        l->setTextFormat(Qt::PlainText);
        vbox->addWidget(l);
    }

    vbox->addWidget(new QLabel("<h3>" + tr("Dansk") + "</h3>"));
    if (line > 0) {
        l = new QLabel(QVariant(line-1).toString() + ": " + dans.at(line-1));
        l->setWordWrap(true);
        l->setTextFormat(Qt::PlainText);
        vbox->addWidget(l);
    }
    l = new QLabel(QVariant(line).toString() + ": " + dans.at(line));
    l->setWordWrap(true);
    l->setTextFormat(Qt::PlainText);
    vbox->addWidget(l);
    if (line < dans.size()-1) {
        l = new QLabel(QVariant(line+1).toString() + ": " + dans.at(line+1));
        l->setWordWrap(true);
        l->setTextFormat(Qt::PlainText);
        vbox->addWidget(l);
    }

    vbox->addWidget(new QLabel("<h3>" + tr("Engelsk") + "</h3>"));
    if (line > 0) {
        l = new QLabel(QVariant(line-1).toString() + ": " + engs.at(line-1));
        l->setWordWrap(true);
        l->setTextFormat(Qt::PlainText);
        vbox->addWidget(l);
    }
    l = new QLabel(QVariant(line).toString() + ": " + engs.at(line));
    l->setWordWrap(true);
    l->setTextFormat(Qt::PlainText);
    vbox->addWidget(l);
    if (line < engs.size()-1) {
        l = new QLabel(QVariant(line+1).toString() + ": " + engs.at(line+1));
        l->setWordWrap(true);
        l->setTextFormat(Qt::PlainText);
        vbox->addWidget(l);
    }

    lp->setLayout(vbox);
    lp->adjustSize();

    lp->show();
    lp->raise();
    lp->activateWindow();
    lp->adjustSize();
}
