#include "QATextOnly.hpp"
#include "QAAudio.hpp"
#include "UpdownOne.hpp"
#include "UpdownTwo.hpp"
#include "UpdownThree.hpp"
#include "LecturePlayer.hpp"
#include "ListenRepeatPlayer.hpp"
#include "ListenRepeatAudio.hpp"
#include "Fillout11.hpp"
#include "TaskChooser.hpp"

#include <QtGlobal>

#define MK_ANY(which, title, function) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(function())); \
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

#define MK_LECTURE(which, title) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture())); \
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

#define MK_LISTENREPEAT(which, title) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(showListenRepeat())); \
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

#define MK_EXERCISE_TEXT(which, title) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_qaTextOnly())); \
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

#define MK_EXERCISE_AUDIO(which, title) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_qaAudio())); \
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

#define MK_FST_RECEPTION(which, title) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(showFST_Reception())); \
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

#define MK_FST_DFG(which, title) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(showFST_DownFromGloss())); \
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

#define MK_FST_DFT(which, title) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(showFST_DownFromTranslate())); \
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

TaskChooser::TaskChooser(const dirmap_t& dirs, QTranslator *translator) :
dirs(dirs),
translator(translator)
{
    setWindowTitle(trUtf8("Grønlandsk for voksne 2"));
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setStyleSheet("margin: 0; padding: 0; text-align: left;");

    QWidget *widget = new QWidget(this);
    widget->setContentsMargins(5,5,5,5);
    widget->setStyleSheet("margin: 0; padding: 0; text-align: left;");

    QPushButton *button;
    QPair<QString,QString> p;

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0,0,0,0);

    button = new QPushButton(trUtf8("Switch to English"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(toggleLanguage()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    vbox->addSpacing(10);

    MK_LECTURE(tr("lectures/danish") + "/1.1", tr("Forelæsning 1.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/1.1"), tr("FST 1.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/1.1"), tr("FST 1.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/1.1"), tr("FST 1.1.3: Oversættelse"));
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/1"), tr("LFG 1"));
    MK_ANY(QT_TR_NOOP("exercises/1.1"), tr("Øvelse 1.1"), showFillout11);
    // Exercise 1.2
    MK_LECTURE(tr("lectures/danish") + "/1.2", tr("Forelæsning 1.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.3"), tr("Øvelse 1.3 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.3"), tr("Øvelse 1.3 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.4"), tr("Øvelse 1.4 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.4"), tr("Øvelse 1.4 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.5"), tr("Øvelse 1.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.5"), tr("Øvelse 1.5 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.6"), tr("Øvelse 1.6 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.6"), tr("Øvelse 1.6 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.7"), tr("Øvelse 1.7 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.7"), tr("Øvelse 1.7 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.8"), tr("Øvelse 1.8 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.8"), tr("Øvelse 1.8 (audio)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/1.2"), tr("FST 1.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/1.2"), tr("FST 1.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/1.2"), tr("FST 1.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE(tr("lectures/danish") + "/2.1", tr("Forelæsning 2.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/2.1"), tr("FST 2.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/2.1"), tr("FST 2.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/2.1"), tr("FST 2.1.3: Oversættelse"));
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/2"), tr("LFG 2"));
    MK_LECTURE(tr("lectures/danish") + "/2.2", tr("Forelæsning 2.2"));
    MK_ANY(QT_TR_NOOP("exercises/2.1"), tr("Øvelse 2.1"), showListenRepeatAudio);
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.2"), tr("Øvelse 2.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.2"), tr("Øvelse 2.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.3.1"), tr("Øvelse 2.3.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.3.1"), tr("Øvelse 2.3.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.3.2"), tr("Øvelse 2.3.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.3.2"), tr("Øvelse 2.3.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.4"), tr("Øvelse 2.4 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.4"), tr("Øvelse 2.4 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.5"), tr("Øvelse 2.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.5"), tr("Øvelse 2.5 (audio)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/2.2"), tr("FST 2.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/2.2"), tr("FST 2.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/2.2"), tr("FST 2.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE(tr("lectures/danish") + "/3.1", tr("Forelæsning 3.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/3.1"), tr("FST 3.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/3.1"), tr("FST 3.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/3.1"), tr("FST 3.1.3: Oversættelse"));
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/3"), tr("LFG 3"));
    MK_LECTURE(tr("lectures/danish") + "/3.2", tr("Forelæsning 3.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.1"), tr("Øvelse 3.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/3.1"), tr("Øvelse 3.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.2"), tr("Øvelse 3.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/3.2"), tr("Øvelse 3.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.3"), tr("Øvelse 3.3 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/3.3"), tr("Øvelse 3.3 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.4"), tr("Øvelse 3.4 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/3.4"), tr("Øvelse 3.4 (audio)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/3.2"), tr("FST 3.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/3.2"), tr("FST 3.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/3.2"), tr("FST 3.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE(tr("lectures/danish") + "/4.1", tr("Forelæsning 4.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/4.1"), tr("FST 4.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/4.1"), tr("FST 4.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/4.1"), tr("FST 4.1.3: Oversættelse"));
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/4"), tr("LFG 4"));
    MK_LECTURE(tr("lectures/danish") + "/4.2", tr("Forelæsning 4.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.1"), tr("Øvelse 4.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/4.1"), tr("Øvelse 4.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.2"), tr("Øvelse 4.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/4.2"), tr("Øvelse 4.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.3"), tr("Øvelse 4.3 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/4.3"), tr("Øvelse 4.3 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.4.1"), tr("Øvelse 4.4.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/4.4.1"), tr("Øvelse 4.4.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.4.2"), tr("Øvelse 4.4.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/4.4.2"), tr("Øvelse 4.4.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.5"), tr("Øvelse 4.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/4.5"), tr("Øvelse 4.5 (audio)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/4.2"), tr("FST 4.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/4.2"), tr("FST 4.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/4.2"), tr("FST 4.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE(tr("lectures/danish") + "/5.1", tr("Forelæsning 5.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/5.1"), tr("FST 5.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/5.1"), tr("FST 5.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/5.1"), tr("FST 5.1.3: Oversættelse"));
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/5"), tr("LFG 5"));
    MK_LECTURE(tr("lectures/danish") + "/5.2", tr("Forelæsning 5.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.1"), tr("Øvelse 5.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/5.1"), tr("Øvelse 5.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.2"), tr("Øvelse 5.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/5.2"), tr("Øvelse 5.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.3"), tr("Øvelse 5.3 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/5.3"), tr("Øvelse 5.3 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.4.1"), tr("Øvelse 5.4.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/5.4.1"), tr("Øvelse 5.4.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.4.2"), tr("Øvelse 5.4.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/5.4.2"), tr("Øvelse 5.4.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.5"), tr("Øvelse 5.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/5.5"), tr("Øvelse 5.5 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.6"), tr("Øvelse 5.6 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/5.6"), tr("Øvelse 5.6 (audio)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/5.2"), tr("FST 5.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/5.2"), tr("FST 5.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/5.2"), tr("FST 5.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE(tr("lectures/danish") + "/6.1", tr("Forelæsning 6.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/6.1"), tr("FST 6.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/6.1"), tr("FST 6.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/6.1"), tr("FST 6.1.3: Oversættelse"));
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/6"), tr("LFG 6"));
    MK_LECTURE(tr("lectures/danish") + "/6.2", tr("Forelæsning 6.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.1"), tr("Øvelse 6.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/6.1"), tr("Øvelse 6.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.2"), tr("Øvelse 6.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/6.2"), tr("Øvelse 6.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.3.1"), tr("Øvelse 6.3.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/6.3.1"), tr("Øvelse 6.3.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.3.2"), tr("Øvelse 6.3.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/6.3.2"), tr("Øvelse 6.3.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.4.1"), tr("Øvelse 6.4.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/6.4.1"), tr("Øvelse 6.4.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.4.2"), tr("Øvelse 6.4.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/6.4.2"), tr("Øvelse 6.4.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.5"), tr("Øvelse 6.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/6.5"), tr("Øvelse 6.5 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.6"), tr("Øvelse 6.6 (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/6.2"), tr("FST 6.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/6.2"), tr("FST 6.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/6.2"), tr("FST 6.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE(tr("lectures/danish") + "/7.1", tr("Forelæsning 7.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/7.1"), tr("FST 7.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/7.1"), tr("FST 7.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/7.1"), tr("FST 7.1.3: Oversættelse"));
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/7"), tr("LFG 7"));
    MK_LECTURE(tr("lectures/danish") + "/7.2", tr("Forelæsning 7.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.1"), tr("Øvelse 7.1 (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.2"), tr("Øvelse 7.2 (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.3.1"), tr("Øvelse 7.3.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/7.3.1"), tr("Øvelse 7.3.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.3.2"), tr("Øvelse 7.3.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/7.3.2"), tr("Øvelse 7.3.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.4"), tr("Øvelse 7.4 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/7.4"), tr("Øvelse 7.4 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.5"), tr("Øvelse 7.5 (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/7.2"), tr("FST 7.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/7.2"), tr("FST 7.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/7.2"), tr("FST 7.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE(tr("lectures/danish") + "/8.1", tr("Forelæsning 8.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/8.1"), tr("FST 8.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/8.1"), tr("FST 8.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/8.1"), tr("FST 8.1.3: Oversættelse"));
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/8"), tr("LFG 8"));
    MK_LECTURE(tr("lectures/danish") + "/8.2", tr("Forelæsning 8.2"));
    // Exercise 8.1
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.2.1"), tr("Øvelse 8.2.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/8.2.1"), tr("Øvelse 8.2.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.2.2"), tr("Øvelse 8.2.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/8.2.2"), tr("Øvelse 8.2.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.3.1"), tr("Øvelse 8.3.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/8.3.1"), tr("Øvelse 8.3.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.3.2"), tr("Øvelse 8.3.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/8.3.2"), tr("Øvelse 8.3.2 (audio)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/8.2"), tr("FST 8.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/8.2"), tr("FST 8.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/8.2"), tr("FST 8.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE(tr("lectures/danish") + "/9", tr("Forelæsning 9"));
    vbox->setAlignment(Qt::AlignCenter|Qt::AlignTop);

    widget->setLayout(vbox);

    setWidget(widget);
    setMinimumWidth(widget->width()+50);
}

void TaskChooser::showLecture() {
    const QPair<QString,QString>& p = mappings[sender()];
    LecturePlayer *lp = new LecturePlayer(*this, p.first, p.second);
    lp->show();
    lp->raise();
    lp->activateWindow();
}

void TaskChooser::showFillout11() {
    const QPair<QString,QString>& p = mappings[sender()];
    Fillout11 *lp = new Fillout11(*this, p.first, p.second);
    lp->show();
    lp->raise();
    lp->activateWindow();
}

void TaskChooser::showListenRepeat() {
    const QPair<QString,QString>& p = mappings[sender()];
    ListenRepeatPlayer *lp = new ListenRepeatPlayer(*this, p.first, p.second);
    lp->show();
    lp->raise();
    lp->activateWindow();
}

void TaskChooser::showListenRepeatAudio() {
    const QPair<QString,QString>& p = mappings[sender()];
    ListenRepeatAudio *lp = new ListenRepeatAudio(*this, p.first, p.second);
    lp->show();
    lp->raise();
    lp->activateWindow();
}

void TaskChooser::showFST_Reception() {
    const QPair<QString,QString>& p = mappings[sender()];
    UpdownOne *ud = new UpdownOne(*this, p.first, p.second);
    ud->show();
    ud->raise();
    ud->activateWindow();
}

void TaskChooser::showFST_DownFromGloss() {
    const QPair<QString,QString>& p = mappings[sender()];
    UpdownTwo *ud = new UpdownTwo(*this, p.first, p.second);
    ud->show();
    ud->raise();
    ud->activateWindow();
}

void TaskChooser::showFST_DownFromTranslate() {
    const QPair<QString,QString>& p = mappings[sender()];
    UpdownThree *ud = new UpdownThree(*this, p.first, p.second);
    ud->show();
    ud->raise();
    ud->activateWindow();
}

void TaskChooser::showExercise_qaTextOnly() {
    const QPair<QString,QString>& p = mappings[sender()];
    QATextOnly *qa = new QATextOnly(*this, p.first, p.second);
    qa->show();
    qa->raise();
    qa->activateWindow();
}

void TaskChooser::showExercise_qaAudio() {
    const QPair<QString,QString>& p = mappings[sender()];
    QAAudio *qa = new QAAudio(*this, p.first, p.second);
    qa->show();
    qa->raise();
    qa->activateWindow();
}

void TaskChooser::toggleLanguage() {
    QSettings settings;

    QString lang = settings.value("language").toString();
    if (lang == "english") {
        settings.setValue("language", "danish");
    }
    else {
        settings.setValue("language", "english");
    }

    lang = settings.value("language").toString();
    if (lang == "english") {
        translator->load(find_newest(dirs, "i18n/texts_en.qm"));
    }
    else {
        translator->load(find_newest(dirs, "i18n/texts_da.qm"));
    }

    TaskChooser *tc = new TaskChooser(dirs, translator);
    tc->show();

    close();
}
