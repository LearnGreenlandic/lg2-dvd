#include "QAAudio.hpp"

QAAudio::QAAudio(TaskChooser& tc, QString which, QString title) :
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

    media_q = new Phonon::MediaObject;
    audio_q = new Phonon::AudioOutput;
    Phonon::createPath(media_q, audio_q);
    playquestion = new QPushButton(tr("Hør opgaven igen"));
    connect(playquestion, SIGNAL(clicked()), this, SLOT(playQuestion()));
    qvbl->addWidget(playquestion);

    qvbl->addSpacing(5);

    input = new QLineEdit;
    connect(input, SIGNAL(returnPressed()), this, SLOT(checkInput()));
    qvbl->addWidget(new QLabel(tr("Skriv og udtal svaret:")));
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

void QAAudio::showNext() {
    ++curAt;
    while (curAt < data.size() && data.getQW(curAt).isEmpty()) {
        ++curAt;
    }
    if (curAt >= data.size()) {
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
    media_q->setCurrentSource(data.getQW(curAt));
    media_q->stop();
    media_q->play();
    result->hide();
    media->stop();
    playanswer->hide();
    yield->hide();
    input->setText("");
    input->setFocus();
    adjustSize();
}

void QAAudio::checkInput() {
    playanswer->hide();
    yield->show();
    if (input->text() == data.getA(curAt)) {
        result->setText(QString("<center><span style='color: darkgreen;'><b>") + tr("Korrekt!") + "</b></span></center>");
        if (!data.getAW(curAt).isEmpty()) {
            media->stop();
            media->setCurrentSource(data.getAW(curAt));
            playanswer->show();
            media->play();
        }
        yield->hide();
    }
    else if (input->text().compare(data.getA(curAt), Qt::CaseInsensitive) == 0) {
        result->setText(QString("<center><span style='color: darkyellow;'><b>") + tr("Næsten korrekt.\nStore og små bogstaver gælder...") + "</b></span></center>");
    }
    else {
        result->setText(QString("<center><span style='color: darkred;'><b>") + tr("Ikke korrekt.\nPrøv igen...") + "</b></span></center>");
    }
    result->show();
    input->setFocus();
    input->selectAll();
    adjustSize();
}

void QAAudio::yieldWord() {
    QMessageBox::information(0, tr("Hrhm..."), QString("<h1>") + tr("Det korrekte svar var:") + QString("</h1><br>") + data.getA(curAt));
    showNext();
}

void QAAudio::playQuestion() {
    media_q->stop();
    media_q->play();
}

void QAAudio::playAnswer() {
    media->stop();
    media->play();
}
