#include "Fillout12.hpp"
#include <algorithm>

Fillout12::Fillout12(TaskChooser& tc, QString which, QString title) :
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

    grid->addWidget(new QLabel(QString("<b>") + tr("Indikativ") + "</b>"), 0, 0);
    grid->addWidget(new QLabel(tr("\"mig\"")), 0, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"dig\"")), 0, 2, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"ham\"")), 0, 3, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"os\"")), 0, 4, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"Jer\"")), 0, 5, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"dem\"")), 0, 6, Qt::AlignHCenter|Qt::AlignVCenter);

    grid->addWidget(new QLabel(tr("\"jeg\"")), 1, 0);
    grid->addWidget(new QLabel(tr("\"du\"")), 2, 0);
    grid->addWidget(new QLabel(tr("\"han\"")), 3, 0);
    grid->addWidget(new QLabel(tr("\"vi\"")), 4, 0);
    grid->addWidget(new QLabel(tr("\"I\"")), 5, 0);
    grid->addWidget(new QLabel(tr("\"de\"")), 6, 0);

    grid->addWidget(new QLabel(QString("<b>") + tr("Interrogativ") + "</b>"), 7, 0);
    grid->addWidget(new QLabel(tr("\"mig\"")), 7, 1, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"dig\"")), 7, 2, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"ham\"")), 7, 3, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"os\"")), 7, 4, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"Jer\"")), 7, 5, Qt::AlignHCenter|Qt::AlignVCenter);
    grid->addWidget(new QLabel(tr("\"dem\"")), 7, 6, Qt::AlignHCenter|Qt::AlignVCenter);

    grid->addWidget(new QLabel(tr("\"du\"")), 8, 0);
    grid->addWidget(new QLabel(tr("\"I\"")), 9, 0);

    inputs.push_back(new QLineEdit);
    connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));
    inputs.push_back(new QLineEdit);
    connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));
    inputs.push_back(new QLineEdit);
    connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));
    inputs.push_back(new QLineEdit);
    connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));

    checks.push_back(new QLabel);
    checks.push_back(new QLabel);
    checks.push_back(new QLabel);
    checks.push_back(new QLabel);

    QVBoxLayout *vbox;
    vbox = new QVBoxLayout;
    vbox->addWidget(inputs[0]);
    vbox->addWidget(checks[0]);
    grid->addLayout(vbox, 1, 5);
    vbox = new QVBoxLayout;
    vbox->addWidget(inputs[1]);
    vbox->addWidget(checks[1]);
    grid->addLayout(vbox, 6, 1);
    vbox = new QVBoxLayout;
    vbox->addWidget(inputs[2]);
    vbox->addWidget(checks[2]);
    grid->addLayout(vbox, 6, 3);
    vbox = new QVBoxLayout;
    vbox->addWidget(inputs[3]);
    vbox->addWidget(checks[3]);
    grid->addLayout(vbox, 8, 3);

    for (int i=0 ; i<10 ; ++i) {
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

void Fillout12::showNext() {
    ++curAt;
    if (curAt >= words.size()) {
        QMessageBox mbox(QMessageBox::Question, tr("Færdig!"), tr("Der er ikke mere i denne øvelse. Vil du fortsætte med næste øvelse?"));
        QPushButton *yes = mbox.addButton(tr("Ja, næste øvelse"), QMessageBox::YesRole);
        mbox.addButton(tr("Nej, tilbage til menuen"), QMessageBox::NoRole);
        mbox.exec();

        if (mbox.clickedButton() == yes) {
            //tc.showStructureTwo();
        }
        close();
        return;
    }

    foreach (QLineEdit *l, inputs) {
        l->setText("");
    }

    foreach (QLabel *l, checks) {
        l->setText("");
    }

    query->setText(tr("Brug ordet:") + QString(" <b>") + words.at(curAt).at(0) + "</b>");

    adjustSize();
}

void Fillout12::checkInputs() {
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
