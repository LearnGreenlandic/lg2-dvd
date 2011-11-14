#include "LecturePlayer.hpp"
#include <QtGlobal>
#if defined(Q_WS_WIN)

#include <QProgressDialog>
#include <QCryptographicHash>
#include <iostream>

LecturePlayer::LecturePlayer(TaskChooser& tc, QString lecDir, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc)
{
    setWindowTitle(title);
    setMinimumSize(minimumSizeHint());

    QString lecfile = find_newest(tc.dirs, lecDir + "/lecture.dat");
    if (lecfile.isEmpty()) {
        QMessageBox::critical(0, "Missing Lecture Data!", "Could not locate lecture.dat!");
        throw(-1);
    }

    QString slide0 = find_newest(tc.dirs, lecDir + "/0.png");
    if (slide0.isEmpty()) {
        QMessageBox::critical(0, "Missing Slides!", "Could not locate slides for lecture!");
        throw(-1);
    }

    QFileInfoList slideFiles = QDir(QFileInfo(slide0).absoluteDir()).entryInfoList(QStringList() << "*.png");
    foreach (QFileInfo slideFile, slideFiles) {
        uint32_t key = slideFile.baseName().toULong();
        slides[key] = slideFile.canonicalFilePath();
    }

    video = new QAxWidget("{6BF52A52-394A-11D3-B153-00C04F79FAA6}");
    controls = video->querySubObject("controls");

    QCryptographicHash sha1(QCryptographicHash::Sha1);
    sha1.addData(lecfile.toUtf8());
    QDir tmpdir(QDir::tempPath());
    tmpfile = tmpdir.absoluteFilePath(QString(sha1.result().toHex()) + "-lecture.avi");

    if (!tmpdir.exists(tmpfile)) {
        CryptFile input(lecfile);
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

    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));

    QPixmap qpx;
    curSlide = slides.begin().value();
    qpx.load(curSlide);

    scene = new QGraphicsScene;
    scene->setBackgroundBrush(QBrush(QColor(0, 51, 102), Qt::SolidPattern));
    slide = scene->addPixmap(qpx);
    slide->setTransformationMode(Qt::SmoothTransformation);

    view = new ScalingGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setContentsMargins(0, 0, 0, 0);

    QGridLayout *qgl = new QGridLayout;
    qgl->addWidget(video, 0, 0);
    qgl->addWidget(view, 0, 1);
    qgl->setColumnStretch(0, 545);
    qgl->setColumnStretch(1, 455);
    qgl->setSpacing(2);
    qgl->setContentsMargins(0, 0, 0, 0);

    setLayout(qgl);
    setContentsMargins(0, 0, 0, 0);
}

void LecturePlayer::closeEvent(QCloseEvent *event) {
    controls->dynamicCall("stop()");
    controls->clear();
    video->clear();
    QFile::remove(tmpfile);
    event->accept();
}

void LecturePlayer::show() {
    QWidget::show();
    controls->dynamicCall("play()");
}

void LecturePlayer::tick() {
    uint32_t i = controls->property("currentPosition").toDouble();

    QString nSlide = curSlide;
    slides_t::const_iterator it = slides.find(i);
    if (it != slides.end()) {
        nSlide = it.value();
    }
    else {
        it = slides.lowerBound(i);
        --it;
        nSlide = it.value();
    }
    if (nSlide != curSlide) {
        curSlide = nSlide;
        QPixmap qp;
        qp.load(curSlide);
        slide->setPixmap(qp);
        slide->update();
    }
}

QSize LecturePlayer::sizeHint() const {
    return QSize(1010, 335);
}

QSize LecturePlayer::minimumSizeHint() const {
    return QSize(1010, 335);
}

#else

LecturePlayer::LecturePlayer(TaskChooser& tc, QString lecDir, QString title) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
tc(tc)
{
    setWindowTitle(title);
    setMinimumSize(minimumSizeHint());

    QString lecfile = find_newest(tc.dirs, lecDir + "/lecture.dat");
    if (lecfile.isEmpty()) {
        QMessageBox::critical(0, "Missing Lecture Data!", "Could not locate lecture.dat!");
        throw(-1);
    }

    QString slide0 = find_newest(tc.dirs, lecDir + "/0.png");
    if (slide0.isEmpty()) {
        QMessageBox::critical(0, "Missing Slides!", "Could not locate slides for lecture!");
        throw(-1);
    }

    QFileInfoList slideFiles = QDir(QFileInfo(slide0).absoluteDir()).entryInfoList(QStringList() << "*.png");
    foreach (QFileInfo slideFile, slideFiles) {
        uint32_t key = slideFile.baseName().toULong();
        slides[key] = slideFile.canonicalFilePath();
    }

    media = new Phonon::MediaObject;
    video = new Phonon::VideoWidget;
    Phonon::createPath(media, video);

    audio = new Phonon::AudioOutput(Phonon::VideoCategory);
    Phonon::createPath(media, audio);

    mediafile = new CryptFile(lecfile);
    media->setCurrentSource(mediafile);
    media->setTickInterval(1000);
    connect(media, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
    connect(media, SIGNAL(finished()), this, SLOT(finished()));

    video->setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
    video->setMinimumSize(400, 225);
    video->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    video->setContentsMargins(0, 0, 0, 0);

    QPixmap qpx;
    curSlide = slides.begin().value();
    qpx.load(curSlide);

    scene = new QGraphicsScene;
    scene->setBackgroundBrush(QBrush(QColor(0, 51, 102), Qt::SolidPattern));
    slide = scene->addPixmap(qpx);
    slide->setTransformationMode(Qt::SmoothTransformation);

    view = new ScalingGraphicsView(scene);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setContentsMargins(0, 0, 0, 0);

    QGridLayout *qgl = new QGridLayout;
    qgl->addWidget(video, 0, 0);
    qgl->addWidget(view, 0, 1);
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
}

void LecturePlayer::closeEvent(QCloseEvent *event) {
    media->stop();
    media->clear();
    event->accept();
}

void LecturePlayer::show() {
    QWidget::show();
    media->play();
}

void LecturePlayer::tick(qint64 time) {
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);
    timeLcd->display(displayTime.toString("mm:ss"));

    uint32_t i = time/1000;
    QString nSlide = curSlide;
    slides_t::const_iterator it = slides.find(i);
    if (it != slides.end()) {
        nSlide = it.value();
    }
    else {
        it = slides.lowerBound(time/1000);
        --it;
        nSlide = it.value();
    }
    if (nSlide != curSlide) {
        curSlide = nSlide;
        QPixmap qp;
        qp.load(curSlide);
        slide->setPixmap(qp);
        slide->update();
    }
}

void LecturePlayer::togglePlay() {
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

void LecturePlayer::finished() {

}

QSize LecturePlayer::sizeHint() const {
    return QSize(1010, 335);
}

QSize LecturePlayer::minimumSizeHint() const {
    return QSize(1010, 335);
}

#endif
