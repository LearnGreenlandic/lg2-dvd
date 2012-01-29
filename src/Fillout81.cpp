#include "Fillout81.hpp"
#include <algorithm>

Fillout81::Fillout81(TaskChooser& tc, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc),
curAt(-1)
{
    setWindowTitle(title);

    QString wordsfn = find_newest(tc.dirs, which + "/words.txt");
    if (wordsfn.isEmpty()) {
        QMessageBox::critical(0, "Missing Data!", QString("Could not find ") + which + "/words.txt");
        throw(-1);
    }

    QFile wordsf(wordsfn);
    if (!wordsf.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Bad Data!", QString("Could not read ") + which + "/words.txt");
        throw(-1);
    }

    QTextStream tg(&wordsf);
    tg.setCodec("UTF-8");

    while (!tg.atEnd()) {
        QString tmp = tg.readLine();
        tmp = tmp.trimmed();
        if (!tmp.isEmpty() && tmp.at(0) != '#' && tmp.contains('\t')) {
            QStringList ls = tmp.split('\t');
            words.push_back(ls);
        }
    }

    std::random_shuffle(words.begin(), words.end());

    QVBoxLayout *qvbl = new QVBoxLayout;

    QLabel *ql = new QLabel(tc.tr(which.toStdString().c_str()));
    ql->setWordWrap(true);
    qvbl->addWidget(ql);
    qvbl->addSpacing(5);

    query = new QLabel;
    qvbl->addWidget(query);

    qvbl->addSpacing(5);

    QGridLayout *grid = new QGridLayout;

    grid->addWidget(new QLabel(QString("<b>") + tr("Absolut") + "</b>"), 0, 1, 1, 2, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(QString("<b>") + tr("Relativ") + "</b>"), 0, 3, 1, 2, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(QString("<b>") + tr("Lokalis") + "</b>"), 0, 5, 1, 2, Qt::AlignHCenter|Qt::AlignVCenter);

    grid->addWidget(new QLabel(tr("<u>N’s tal ⇒</u><br>⇓ “ejers” person")), 1, 0);
    grid->addWidget(new QLabel(tr("Ental")), 1, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("Flertal")), 1, 2, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("Ental")), 1, 3, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("Flertal")), 1, 4, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("Ental")), 1, 5, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("Flertal")), 1, 6, Qt::AlignHCenter|Qt::AlignVCenter);

    grid->addWidget(new QLabel(tr("Uden\nejerperson")), 2, 0);
    grid->addWidget(new QLabel(tr("<i>uanga</i>")), 3, 0);
    grid->addWidget(new QLabel(tr("<i>illit</i>")), 4, 0);
    grid->addWidget(new QLabel(tr("<i>taassuma</i>")), 5, 0);
    grid->addWidget(new QLabel(tr("4. ental")), 6, 0);
    grid->addWidget(new QLabel(tr("<i>uagut</i>")), 7, 0);
    grid->addWidget(new QLabel(tr("<i>ilissi</i>")), 8, 0);
    grid->addWidget(new QLabel(tr("<i>taakku</i>")), 9, 0);
    grid->addWidget(new QLabel(tr("4. flertal")), 10, 0);

    for (int i=0 ; i<22 ; ++i) {
        inputs.push_back(new QLineEdit);
        connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));
        checks.push_back(new QLabel);
    }

    QVBoxLayout *vbox;

#define ADDIN(i, y, x) \
    vbox = new QVBoxLayout; \
    vbox->addWidget(inputs[i]); \
    vbox->addWidget(checks[i]); \
    grid->addLayout(vbox, y, x);

    ADDIN(0, 2, 1);
    ADDIN(1, 2, 2);
    ADDIN(2, 2, 3);
    ADDIN(3, 2, 4);
    ADDIN(4, 2, 5);
    ADDIN(5, 2, 6);
    ADDIN(6, 3, 1);
    ADDIN(7, 3, 2);
    ADDIN(8, 3, 3);
    ADDIN(9, 3, 4);
    ADDIN(10, 3, 5);
    ADDIN(11, 3, 6);
    ADDIN(12, 5, 1);
    ADDIN(13, 5, 2);
    ADDIN(14, 5, 3);
    ADDIN(15, 5, 5);
    ADDIN(16, 6, 1);
    ADDIN(17, 6, 5);
    ADDIN(18, 6, 6);
    ADDIN(19, 9, 1);
    ADDIN(20, 9, 2);
    ADDIN(21, 9, 3);

    for (int i=0 ; i<11 ; ++i) {
        for (int j=0 ; j<7 ; ++j) {
            if (grid->itemAtPosition(i, j) == 0) {
                grid->addWidget(new QLabel("..."), i, j, Qt::AlignHCenter|Qt::AlignVCenter);
            }
        }
    }

    qvbl->addLayout(grid);

    qvbl->addSpacing(15);
    QPushButton *check = new QPushButton(tr("Check rigtigt/forkert"));
    connect(check, SIGNAL(clicked()), this, SLOT(checkInputs()));
    qvbl->addWidget(check);

    qvbl->addSpacing(15);
    QPushButton *nb = new QPushButton(tr("Gå til næste ord"));
    connect(nb, SIGNAL(clicked()), this, SLOT(showNext()));
    qvbl->addWidget(nb);

    setLayout(qvbl);

    showNext();
}

void Fillout81::showNext() {
    ++curAt;
    if (curAt >= words.size()) {
        QMessageBox mbox(QMessageBox::Question, tr("Færdig!"), tr("Der er ikke mere i denne del. Vil du fortsætte med næste del?"));
        QPushButton *yes = mbox.addButton(tr("Ja, næste del"), QMessageBox::YesRole);
        mbox.addButton(tr("Nej, tilbage til menuen"), QMessageBox::NoRole);
        mbox.exec();

        if (mbox.clickedButton() == yes) {
            tc.showNext(windowTitle());
        }
        close();
        return;
    }

    for (int i=0 ; i<inputs.size() ; ++i) {
        inputs[i]->setText("");
        inputs[i]->setStyleSheet(QString("width: %1ex;").arg(words.at(curAt).at(i+1).size()+3));
    }

    foreach (QLabel *l, checks) {
        l->setText("");
    }

    query->setText(tr("Brug ordet:") + QString(" <b>") + words.at(curAt).at(0) + "</b>");

    adjustSize();
}

void Fillout81::checkInputs() {
    for (int i=0 ; i<inputs.size() ; ++i) {
        QString text = inputs.at(i)->text().trimmed();
        if (text.isEmpty()) {
            checks.at(i)->setText("");
        }
        else if (text == words.at(curAt).at(i+1)) {
            checks.at(i)->setText(QString("<center><span style='color: darkgreen;'><b>") + tr("Korrekt!") + "</b></span></center>");
        }
        else if (text.compare(words.at(curAt).at(i+1), Qt::CaseInsensitive) == 0) {
            checks.at(i)->setText(QString("<center><span style='color: darkyellow;'><b>") + tr("Næsten korrekt") + "</b></span></center>");
        }
        else {
            checks.at(i)->setText(QString("<center><span style='color: darkred;'><b>") + tr("Ikke korrekt") + "</b></span></center>");
        }
    }

    adjustSize();
}
