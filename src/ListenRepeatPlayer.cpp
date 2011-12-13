#include "ListenRepeatPlayer.hpp"
#if defined(Q_WS_WIN)

#include <QProgressDialog>
#include <QCryptographicHash>
#include <iostream>

ListenRepeatPlayer::ListenRepeatPlayer(TaskChooser& tc, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc),
curAt(-1)
{
    QString videof = find_newest(tc.dirs, which + "/video.dat");
    if (videof.isEmpty()) {
        QMessageBox::critical(0, "Missing ListenRepeat Data!", QString("Could not locate ") + which + "/video.dat");
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
    setMinimumSize(minimumSizeHint());

    video = new QAxWidget("{6BF52A52-394A-11D3-B153-00C04F79FAA6}");
    controls = video->querySubObject("controls");

    QCryptographicHash sha1(QCryptographicHash::Sha1);
    sha1.addData(videof.toUtf8());
    QDir tmpdir(QDir::tempPath());
    tmpfile = tmpdir.absoluteFilePath(QString(sha1.result().toHex()) + "-listenrepeat.avi");

    if (!tmpdir.exists(tmpfile)) {
        CryptFile input(videof);
        QFile output(tmpfile);

        input.open(QIODevice::ReadOnly);
        output.open(QIODevice::WriteOnly);

        QProgressDialog progress("Transcoding for Windows Media Player...", "", 0, input.size(), this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setCancelButton(0);

        char buf[32768];
        qint64 r;
        while (!input.atEnd() && (r = input.read(buf, sizeof(buf))) > 0) {
            if (output.write(buf, r) <= 0) {
                std::cerr << "Write failed at offsets " << input.pos() << ", " << output.pos() << std::endl;
                throw(-1);
            }
            progress.setValue(input.pos());
        }
        progress.setValue(input.size());

        output.close();
        input.close();
    }

    video->setMinimumSize(400, 225);
    video->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    video->setContentsMargins(0, 0, 0, 0);
    video->dynamicCall("setUiMode(QString)", "full");
    video->dynamicCall("setEnabled(bool)", true);
    video->dynamicCall("SetURL(QString)", QUrl::fromLocalFile(tmpfile));

    QHBoxLayout *outerHBox = new QHBoxLayout;

    QVBoxLayout *qvbl = new QVBoxLayout;
    qvbl->setContentsMargins(0, 0, 0, 0);
    qvbl->addWidget(video);

    outerHBox->addLayout(qvbl);

    qvbl = new QVBoxLayout;

    QLabel *label = new QLabel(tr("Aflyt og nedkriv hvad der bliver sagt i videoen ord for ord."));
    //label->setWordWrap(true);
    qvbl->addWidget(label);

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
    outerHBox->addLayout(qvbl);

    setLayout(outerHBox);

    setContentsMargins(0, 0, 0, 0);

    showNext();
}

void ListenRepeatPlayer::closeEvent(QCloseEvent *event) {
    controls->dynamicCall("stop()");
    controls->clear();
    video->clear();
    QFile::remove(tmpfile);
    event->accept();
}

void ListenRepeatPlayer::show() {
    QWidget::show();
    controls->dynamicCall("play()");
}

QSize ListenRepeatPlayer::sizeHint() const {
    return QSize(1010, 400);
}

QSize ListenRepeatPlayer::minimumSizeHint() const {
    return QSize(1010, 400);
}

#else

ListenRepeatPlayer::ListenRepeatPlayer(TaskChooser& tc, QString which, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc),
curAt(-1)
{
    QString videof = find_newest(tc.dirs, which + "/video.dat");
    if (videof.isEmpty()) {
        QMessageBox::critical(0, "Missing ListenRepeat Data!", QString("Could not locate ") + which + "/video.dat");
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
    setMinimumSize(minimumSizeHint());

    media = new Phonon::MediaObject;
    video = new Phonon::VideoWidget;
    Phonon::createPath(media, video);

    audio = new Phonon::AudioOutput(Phonon::VideoCategory);
    Phonon::createPath(media, audio);

    mediafile = new CryptFile(videof);
    media->setCurrentSource(mediafile);
    media->setTickInterval(1000);
    connect(media, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));

    video->setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
    video->setMinimumSize(400, 225);
    video->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    video->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *qvbl = new QVBoxLayout;

    QLabel *label = new QLabel(tr("Aflyt og nedkriv hvad der bliver sagt ord for ord."));
    //label->setWordWrap(true);
    qvbl->addWidget(label);

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

    QGridLayout *qgl = new QGridLayout;
    qgl->addWidget(video, 0, 0);
    qgl->addLayout(qvbl, 0, 1, Qt::AlignLeft|Qt::AlignVCenter);
    qgl->setColumnStretch(0, 545);
    qgl->setColumnStretch(1, 455);
    qgl->setSpacing(2);
    qgl->setContentsMargins(0, 0, 0, 0);

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
    qhbl->addWidget(seeker, 96);
    qhbl->addWidget(timeLcd, 1);

    qgl->addLayout(qhbl, 1, 0, 1, 2);

    setLayout(qgl);

    setContentsMargins(0, 0, 0, 0);

    showNext();
}

void ListenRepeatPlayer::closeEvent(QCloseEvent *event) {
    media->stop();
    media->clear();
    event->accept();
}

void ListenRepeatPlayer::show() {
    QWidget::show();
    media->play();
}

void ListenRepeatPlayer::tick(qint64 time) {
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);
    timeLcd->display(displayTime.toString("mm:ss"));
}

void ListenRepeatPlayer::togglePlay() {
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

QSize ListenRepeatPlayer::sizeHint() const {
    return QSize(1010, 335);
}

QSize ListenRepeatPlayer::minimumSizeHint() const {
    return QSize(1010, 335);
}

#endif

void ListenRepeatPlayer::showNext() {
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

void ListenRepeatPlayer::checkInput() {
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

void ListenRepeatPlayer::yieldWord() {
    QMessageBox::information(0, tr("Hrhm..."), QString("<h1>") + tr("Det korrekte ord var:") + QString("</h1><br>") + words.at(curAt));
    showNext();
}
