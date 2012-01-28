#include "QATextOnly.hpp"

QATextOnly::QATextOnly(TaskChooser& tc, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc),
data(tc.dirs, which),
curAt(-1)
{
    setWindowTitle(title);

    QVBoxLayout *qvbl = new QVBoxLayout;

    QLabel *ql = new QLabel(tc.tr(which.toStdString().c_str()));
    ql->setWordWrap(true);
    qvbl->addWidget(ql);
    qvbl->addSpacing(5);

    query = new QLabel;
    qvbl->addWidget(query);

    qvbl->addSpacing(5);

    input = new QLineEdit;
    connect(input, SIGNAL(returnPressed()), this, SLOT(checkInput()));
    qvbl->addWidget(new QLabel(tr("Skriv svaret:")));
    qvbl->addWidget(input);

    QPushButton *check = new QPushButton(tr("Check"));
    connect(check, SIGNAL(clicked()), this, SLOT(checkInput()));
    qvbl->addWidget(check);

    result = new QLabel;
    qvbl->addWidget(result);
    result->hide();

    media = new Phonon::MediaObject;
    audio = new Phonon::AudioOutput;
    Phonon::createPath(media, audio);
    playanswer = new QPushButton(tr("Hør det rigtige igen"));
    connect(playanswer, SIGNAL(clicked()), this, SLOT(playAnswer()));
    qvbl->addWidget(playanswer);
    playanswer->hide();

    yield = new QPushButton(tr("Giv op..."));
    connect(yield, SIGNAL(clicked()), this, SLOT(yieldWord()));

    qvbl->addWidget(yield);
    yield->hide();

    qvbl->addSpacing(15);

    QPushButton *nb = new QPushButton(tr("Gå til næste opgave"));
    connect(nb, SIGNAL(clicked()), this, SLOT(showNext()));
    qvbl->addWidget(nb);

    setLayout(qvbl);

    showNext();
}

void QATextOnly::showNext() {
    ++curAt;
    if (curAt >= data.size()) {
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
    QString text = QString("<b>") + data.getQ(curAt) + "</b>";
    if (!data.getStar(curAt).isEmpty()) {
        text += QString("<br/><i>") + data.getStar(curAt) + "</i>";
    }
    query->setText(text);
    result->hide();
    media->stop();
    playanswer->hide();
    yield->hide();
    input->setText("");
    input->setFocus();
    adjustSize();
}

void QATextOnly::checkInput() {
    playanswer->hide();
    yield->show();
    QStringList as = data.getA(curAt).split('|');
    foreach (QString a, as) {
        if (input->text() == a) {
            result->setText(QString("<center><span style='color: darkgreen;'><b>") + tr("Korrekt!") + "</b></span></center>");
            if (!data.getAW(curAt).isEmpty()) {
                media->stop();
                media->setCurrentSource(data.getAW(curAt));
                playanswer->show();
                media->play();
            }
            yield->hide();
            break;
        }
        else if (input->text().compare(a, Qt::CaseInsensitive) == 0) {
            result->setText(QString("<center><span style='color: darkyellow;'><b>") + tr("Næsten korrekt.\nStore og små bogstaver gælder...") + "</b></span></center>");
            break;
        }
        else {
            result->setText(QString("<center><span style='color: darkred;'><b>") + tr("Ikke korrekt.\nPrøv igen...") + "</b></span></center>");
        }
    }
    result->show();
    input->setFocus();
    input->selectAll();
    adjustSize();
}

void QATextOnly::yieldWord() {
    QString a = data.getA(curAt);
    QMessageBox::information(0, tr("Hrhm..."), QString("<h1>") + tr("Det korrekte svar var:") + QString("</h1><br>") + a.replace('|', "<br>"));
    showNext();
}

void QATextOnly::playAnswer() {
    media->stop();
    media->play();
}
