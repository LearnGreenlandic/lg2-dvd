#include "ListenRepeatAudio.hpp"

ListenRepeatAudio::ListenRepeatAudio(TaskChooser& tc, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc),
curAt(-1)
{
    QString audiof = find_newest(tc.dirs, which + "/audio.wav");
    if (audiof.isEmpty()) {
        QMessageBox::critical(0, "Missing ListenRepeat Data!", QString("Could not locate ") + which + "/audio.wav");
        throw(-1);
    }

    QString textf = find_newest(tc.dirs, which + "/text.txt");
    if (textf.isEmpty()) {
        QMessageBox::critical(0, "Missing ListenRepeat Data!", QString("Could not locate ") + which + "/text.txt");
        throw(-1);
    }

    QFile input_f(textf);
    if (!input_f.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Read Error!", "Could not open text.txt from data folder!");
        throw(-1);
    }
    QTextStream input_t(&input_f);
    input_t.setCodec("UTF-8");
    QString input_s = input_t.readAll().trimmed();
    QRegExp rx("(\\S+\\s*)");
    int pos = 0;
    while ((pos = rx.indexIn(input_s, pos)) != -1) {
        words << rx.cap(1);
        pos += rx.matchedLength();
    }

    setWindowTitle(title);

    media = new Phonon::MediaObject;
    audio = new Phonon::AudioOutput;
    Phonon::createPath(media, audio);

    media->setCurrentSource(new QFile(audiof));
    media->setTickInterval(1000);
    connect(media, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));

    playpause = new QPushButton(style()->standardIcon(QStyle::SP_MediaPause), "Pause", this);
    playpause->setShortcut(QString("Space"));
    connect(playpause, SIGNAL(clicked()), this, SLOT(togglePlay()));

    seeker = new Phonon::SeekSlider(this);
    seeker->setMediaObject(media);

    QPalette palette;
    palette.setBrush(QPalette::Light, Qt::darkGray);

    timeLcd = new QLCDNumber;
    timeLcd->setPalette(palette);

    QHBoxLayout *qhbl = new QHBoxLayout;
    qhbl->addWidget(playpause, 1);
    qhbl->addWidget(seeker, 98);
    qhbl->addWidget(timeLcd, 1);

    QVBoxLayout *qvbl = new QVBoxLayout;

    QLabel *label = new QLabel(tr("Aflyt og nedkriv hvad der bliver sagt ord for ord."));
    //label->setWordWrap(true);
    qvbl->addWidget(label);

    qvbl->addSpacing(10);

    qvbl->addLayout(qhbl);

    qvbl->addSpacing(10);

    sum = new QLabel;
    sum->setWordWrap(true);
    qvbl->addWidget(sum);

    qvbl->addSpacing(10);

    query = new QLabel;
    qvbl->addWidget(query);

    input = new QLineEdit;
    connect(input, SIGNAL(returnPressed()), this, SLOT(checkInput()));
    QPushButton *check = new QPushButton(tr("Check"));
    connect(check, SIGNAL(clicked()), this, SLOT(checkInput()));
    yield = new QPushButton(tr("Giv op..."));
    connect(yield, SIGNAL(clicked()), this, SLOT(yieldWord()));
    result = new QLabel;
    qvbl->addWidget(input);
    qvbl->addWidget(check);
    qvbl->addWidget(result);
    qvbl->addWidget(yield);
    result->hide();
    yield->hide();

    qvbl->addSpacing(15);

    QPushButton *nb = new QPushButton(tr("Gå til næste ord"));
    connect(nb, SIGNAL(clicked()), this, SLOT(showNext()));
    qvbl->addWidget(nb);

    qvbl->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    qvbl->setContentsMargins(10, 10, 10, 10);

    setLayout(qvbl);

    setContentsMargins(0, 0, 0, 0);

    showNext();
}

void ListenRepeatAudio::closeEvent(QCloseEvent *event) {
    media->stop();
    media->clear();
    event->accept();
}

void ListenRepeatAudio::show() {
    QWidget::show();
    media->play();
}

void ListenRepeatAudio::tick(qint64 time) {
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);
    timeLcd->display(displayTime.toString("mm:ss"));
}

void ListenRepeatAudio::togglePlay() {
    if (media->state() == Phonon::PlayingState) {
        playpause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        playpause->setText("Play");
        media->pause();
    }
    else {
        playpause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        playpause->setText("Pause");
        media->play();
    }
}

void ListenRepeatAudio::showNext() {
    ++curAt;

    QString sumtext = "<i>";
    for (int i=0 ; i<curAt ; ++i) {
        sumtext += words.at(i);
    }
    sumtext += " ...</i>";
    sum->setText(sumtext.replace('\n', "<br>"));

    if (curAt >= words.size()) {
        /*
        QMessageBox mbox(QMessageBox::Question, tr("Færdig!"), tr("Der er ikke mere i denne øvelse. Vil du læse forelæsningen som PDF?"));
        QPushButton *yes = mbox.addButton(tr("Ja, åben PDF"), QMessageBox::YesRole);
        mbox.addButton(tr("Nej, tilbage til menuen"), QMessageBox::NoRole);
        mbox.exec();

        if (mbox.clickedButton() == yes) {
            tc.showLectureFourPDF();
        }
        //*/
        close();
        return;
    }

    query->setText(tr("Skriv ord nummer %1:").arg(curAt+1));

    input->setFocus();
    input->setText("");

    result->hide();
    yield->hide();

    adjustSize();
}

void ListenRepeatAudio::checkInput() {
    QString text = input->text();
    text.replace(QRegExp("\\W+"), "");

    QString correct = words.at(curAt);
    correct.replace(QRegExp("\\W+"), "");

    if (text == correct) {
        result->setText(QString("<center><span style='color: darkgreen;'><b>") + tr("Korrekt!") + "</b></span></center>");
        yield->hide();
    }
    else if (text.compare(correct, Qt::CaseInsensitive) == 0) {
        result->setText(QString("<center><span style='color: darkyellow;'><b>") + tr("Næsten korrekt.\nStore og små bogstaver gælder...") + "</b></span></center>");
        yield->show();
    }
    else {
        result->setText(QString("<center><span style='color: darkred;'><b>") + tr("Ikke korrekt.\nPrøv igen...") + "</b></span></center>");
        yield->show();
    }
    result->show();
    input->setFocus();
    input->selectAll();
    adjustSize();
}

void ListenRepeatAudio::yieldWord() {
    QMessageBox::information(0, tr("Hrhm..."), QString("<h1>") + tr("Det korrekte ord var:") + QString("</h1><br>") + words.at(curAt));
    showNext();
}
