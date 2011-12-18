#include "Fillout11.hpp"
#include <algorithm>

Fillout11::Fillout11(TaskChooser& tc, QString which, QString title) :
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

    grid->addWidget(new QLabel(QString("<b>") + tr("Intransitiv indikativ / fremsættemåde") + "</b>"), 0, 0, 1, 3);
    grid->addWidget(new QLabel(tr("1.ental \"jeg\"")), 1, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("2.ental \"du\"") + "</i>"), 2, 0);
    grid->addWidget(new QLabel(tr("3.ental \"han\"")), 3, 0);
    grid->addWidget(new QLabel(tr("1.flertal \"vi\"")), 4, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("2.flertal \"I\"") + "</i>"), 5, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("3.flertal \"de\"") + "</i>"), 6, 0);

    grid->addWidget(new QLabel(QString("<b>") + tr("Intransitiv interrogativ / spørgemåde") + "</b>"), 7, 0, 1, 3);
    grid->addWidget(new QLabel(tr("2.ental \"du\"")), 8, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("3.ental \"han\"") + "</i>"), 9, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("2.flertal \"I\"") + "</i>"), 10, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("3.flertal \"de\"") + "</i>"), 11, 0);

    grid->addWidget(new QLabel(QString("<b>") + tr("Intransitiv participium / navnemåde") + "</b>"), 12, 0, 1, 3);
    grid->addWidget(new QLabel(tr("1.ental \"jeg\"")), 13, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("2.ental \"du\"") + "</i>"), 14, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("3.ental \"han\"") + "</i>"), 15, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("1.flertal \"vi\"") + "</i>"), 16, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("2.flertal \"I\"") + "</i>"), 17, 0);
    grid->addWidget(new QLabel(QString("<i>") + tr("3.flertal \"de\"") + "</i>"), 18, 0);

    inputs.push_back(new QLineEdit);
    connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));
    inputs.push_back(new QLineEdit);
    connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));
    inputs.push_back(new QLineEdit);
    connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));
    inputs.push_back(new QLineEdit);
    connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));
    inputs.push_back(new QLineEdit);
    connect(inputs.back(), SIGNAL(editingFinished()), this, SLOT(checkInputs()));

    grid->addWidget(inputs[0], 1, 1);
    grid->addWidget(inputs[1], 3, 1);
    grid->addWidget(inputs[2], 4, 1);
    grid->addWidget(inputs[3], 8, 1);
    grid->addWidget(inputs[4], 13, 1);

    checks.push_back(new QLabel);
    checks.push_back(new QLabel);
    checks.push_back(new QLabel);
    checks.push_back(new QLabel);
    checks.push_back(new QLabel);

    grid->addWidget(checks[0], 1, 2);
    grid->addWidget(checks[1], 3, 2);
    grid->addWidget(checks[2], 4, 2);
    grid->addWidget(checks[3], 8, 2);
    grid->addWidget(checks[4], 13, 2);

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

void Fillout11::showNext() {
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

void Fillout11::checkInputs() {
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
