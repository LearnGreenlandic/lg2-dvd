#include "ValidateKey.hpp"
#include "ClickLabel.hpp"
#include "QATextOnly.hpp"
#include "QAAudio.hpp"
#include "UpdownOne.hpp"
#include "UpdownTwo.hpp"
#include "UpdownThree.hpp"
#include "LecturePlayer.hpp"
#include "ListenRepeatPlayer.hpp"
#include "ListenRepeatAudio.hpp"
#include "Fillout11.hpp"
#include "Fillout12.hpp"
#include "Fillout81.hpp"
#include "CorpusQuery.hpp"
#include "TaskChooser.hpp"

#include <QtGlobal>

#define MK_ANY(which, title, function) \
    p = qMakePair(QString(which), title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    buttons.push_back(qMakePair(title,button)); \
    button_sect[button] = section; \
    connect(button, SIGNAL(clicked()), this, SLOT(function())); \
    vbox->addWidget(button, Qt::AlignLeft|Qt::AlignTop);

#define MK_LECTURE(which, title) \
    MK_ANY(which, title, showLecture); \
    button->setIconSize(QSize(85, 63)); \
    button->setIcon(QIcon(find_newest(dirs, which + "/0.png"))); \
    button->setStyleSheet("* { font-weight: bold; text-align: left; font-size: 200%; valign: top; }");

#define MK_LISTENREPEAT(which, title) \
    MK_ANY(which, title, showListenRepeat); \
    button->setIconSize(QSize(20, 20)); \
    button->setIcon(QIcon(find_newest(dirs, "gfx/listen_icon.png")));

#define MK_SLIDES(which, title) \
    MK_ANY(which, title, showSlidesPDF); \
    buttons.pop_back(); \
    button->setIconSize(QSize(20, 20)); \
    button->setIcon(QIcon(find_newest(dirs, "gfx/pdf_icon.png")));

#define MK_LECTURE_PDF(which, title) \
    MK_ANY(which, title, showLecturePDF); \
    buttons.pop_back(); \
    button->setIconSize(QSize(20, 20)); \
    button->setIcon(QIcon(find_newest(dirs, "gfx/pdf_icon.png")));

#define MK_EXERCISE_TEXT(which, title) \
    MK_ANY(which, title, showExercise_qaTextOnly); \
    button->setIconSize(QSize(20, 20)); \
    button->setIcon(QIcon(find_newest(dirs, "gfx/text_icon.png")));

#define MK_EXERCISE_AUDIO(which, title) \
    MK_ANY(which, title, showExercise_qaAudio); \
    button->setIconSize(QSize(20, 20)); \
    button->setIcon(QIcon(find_newest(dirs, "gfx/audio_icon.png")));

#define MK_FST_RECEPTION(which, title) \
    MK_ANY(which, title, showFST_Reception); \
    button->setIconSize(QSize(20, 20)); \
    button->setIcon(QIcon(find_newest(dirs, "gfx/fst_icon.png")));

#define MK_FST_DFG(which, title) \
    MK_ANY(which, title, showFST_DownFromGloss); \
    button->setIconSize(QSize(20, 20)); \
    button->setIcon(QIcon(find_newest(dirs, "gfx/fst_icon.png")));

#define MK_FST_DFT(which, title) \
    MK_ANY(which, title, showFST_DownFromTranslate); \
    button->setIconSize(QSize(20, 20)); \
    button->setIcon(QIcon(find_newest(dirs, "gfx/fst_icon.png")));

#define MK_SECTION(which) \
    section_texts.push_back(which); \
    section = new ClickLabel(QString("<h2>") + which + "</h2>"); \
    connect(section, SIGNAL(onClick()), this, SLOT(switchSection())); \
    section_list.push_back(section); \
    sections->addWidget(section);

#define COLUMNS_BEGIN \
    grid = new QGridLayout; \
    grid->setAlignment(Qt::AlignLeft|Qt::AlignTop); \
    grid->setSpacing(5); \
    vbox = new QVBoxLayout; \
    vbox->setAlignment(Qt::AlignLeft|Qt::AlignTop);

#define COLUMN_BR(y, x) \
    grid->addLayout(vbox, y, x, Qt::AlignLeft|Qt::AlignTop); \
    vbox = new QVBoxLayout; \
    vbox->setAlignment(Qt::AlignLeft|Qt::AlignTop);

#define COLUMNS_STACK \
    hbox = new QHBoxLayout; \
    hbox->addLayout(grid, 1); \
    hbox->addStretch(99); \
    vbox->addLayout(hbox, 1); \
    vbox->addStretch(99); \
    widget = new QWidget; \
    widget->setStyleSheet("margin: 0; padding: 0; text-align: left;"); \
    widget->setLayout(vbox); \
    stack->addWidget(widget);


TaskChooser::TaskChooser(const dirmap_t& dirs, QTranslator *translator) :
dirs(dirs),
translator(translator)
{
    setWindowTitle(tr("Grønlandsk for voksne 2"));
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setStyleSheet("margin: 0; padding: 0; text-align: left;");

    QPushButton *button = 0;
    QPair<QString,QString> p;

    QVBoxLayout *outerVBox = new QVBoxLayout;
    outerVBox->setContentsMargins(0,0,0,0);
    outerVBox->setSpacing(0);

    QHBoxLayout *middleHBox = new QHBoxLayout;
    middleHBox->setContentsMargins(0,0,0,0);
    middleHBox->setSpacing(0);

    QVBoxLayout *sections = new QVBoxLayout;
    sections->setContentsMargins(0,0,0,0);
    sections->setSpacing(0);
    ClickLabel *section = 0;
    MK_SECTION(tr("Intro"));
    MK_SECTION(tr("Kapitel 1"));
    MK_SECTION(tr("Kapitel 2"));
    MK_SECTION(tr("Kapitel 3"));
    MK_SECTION(tr("Kapitel 4"));
    MK_SECTION(tr("Kapitel 5"));
    MK_SECTION(tr("Kapitel 6"));
    MK_SECTION(tr("Kapitel 7"));
    MK_SECTION(tr("Kapitel 8"));
    MK_SECTION(tr("Kapitel 9"));
    middleHBox->addLayout(sections);

    stack = new QStackedLayout;
    stack->setContentsMargins(5,5,5,5);
    QWidget *widget = 0;

    QHBoxLayout *hbox = 0;
    QVBoxLayout *vbox = 0;
    QGridLayout *grid = 0;

    vbox = new QVBoxLayout;
    section = dynamic_cast<ClickLabel*>(section_list.at(0));
    QLabel *l = new QLabel(tr("introduktion tekst"));
    l->setWordWrap(true);
    l->setTextFormat(Qt::RichText);
    l->setTextInteractionFlags(Qt::LinksAccessibleByMouse|Qt::LinksAccessibleByKeyboard);
    l->setOpenExternalLinks(true);
    vbox->addWidget(l);
    vbox->addSpacing(15);
    MK_ANY(QT_TR_NOOP("null"), tr("Gå til første forelæsning"), showFirstLecture);
    button->setStyleSheet("font-weight: bold;");
    widget = new QWidget;
    widget->setStyleSheet("margin: 0; padding: 0; text-align: center;");
    widget->setLayout(vbox);
    stack->addWidget(widget);

    COLUMNS_BEGIN;
    section = dynamic_cast<ClickLabel*>(section_list.at(1));
    MK_LECTURE(tr("lectures/danish") + "/1.1", tr("Forelæsning 1.1"));
    COLUMN_BR(0,0);
    MK_SLIDES(tr("lectures/danish") + "/1.1", tr("Forelæsning 1.1 dias"));
    MK_SLIDES(tr("lectures/danish") + "/1.2", tr("Forelæsning 1.2 dias"));
    MK_LECTURE_PDF(tr("lectures/danish") + "/1.1", tr("Kapitel 1 som PDF"));
    COLUMN_BR(0,2);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/1.1"), tr("FST 1.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/1.1"), tr("FST 1.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/1.1"), tr("FST 1.1.3: Oversættelse"));
    vbox->addSpacing(20);
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/1"), tr("LFG 1"));
    COLUMN_BR(1,0);
    MK_LECTURE(tr("lectures/danish") + "/1.2", tr("Forelæsning 1.2"));
    COLUMN_BR(0,1);
    vbox->addSpacing(10);
    MK_ANY(QT_TR_NOOP("exercises/1.1"), tr("Øvelse 1.1"), showFillout11);
    button->setIconSize(QSize(20, 20));
    button->setIcon(QIcon(find_newest(dirs, "gfx/table_icon.png")));
    MK_ANY(QT_TR_NOOP("exercises/1.2"), tr("Øvelse 1.2"), showFillout12);
    button->setIconSize(QSize(20, 20));
    button->setIcon(QIcon(find_newest(dirs, "gfx/table_icon.png")));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.3"), tr("Øvelse 1.3 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.3"), tr("Øvelse 1.3 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.4"), tr("Øvelse 1.4 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.4"), tr("Øvelse 1.4 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.5"), tr("Øvelse 1.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.5"), tr("Øvelse 1.5 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.6"), tr("Øvelse 1.6 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.6"), tr("Øvelse 1.6 (audio)"));
    COLUMN_BR(1,1);
    vbox->addSpacing(10);
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.7"), tr("Øvelse 1.7 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.7"), tr("Øvelse 1.7 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.8"), tr("Øvelse 1.8 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/1.8"), tr("Øvelse 1.8 (audio)"));
    vbox->addSpacing(20);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/1.2"), tr("FST 1.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/1.2"), tr("FST 1.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/1.2"), tr("FST 1.2.3: Oversættelse"));
    COLUMN_BR(1,2);
    COLUMNS_STACK;

    COLUMNS_BEGIN;
    section = dynamic_cast<ClickLabel*>(section_list.at(2));
    MK_LECTURE(tr("lectures/danish") + "/2.1", tr("Forelæsning 2.1"));
    COLUMN_BR(0,0);
    MK_SLIDES(tr("lectures/danish") + "/2.1", tr("Forelæsning 2.1 dias"));
    MK_SLIDES(tr("lectures/danish") + "/2.2", tr("Forelæsning 2.2 dias"));
    MK_LECTURE_PDF(tr("lectures/danish") + "/2.1", tr("Kapitel 2 som PDF"));
    COLUMN_BR(0,2);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/2.1"), tr("FST 2.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/2.1"), tr("FST 2.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/2.1"), tr("FST 2.1.3: Oversættelse"));
    vbox->addSpacing(20);
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/2"), tr("LFG 2"));
    COLUMN_BR(1,0);
    MK_LECTURE(tr("lectures/danish") + "/2.2", tr("Forelæsning 2.2"));
    COLUMN_BR(0,1);
    vbox->addSpacing(10);
    MK_ANY(QT_TR_NOOP("exercises/2.1"), tr("Øvelse 2.1"), showListenRepeatAudio);
    button->setIconSize(QSize(20, 20));
    button->setIcon(QIcon(find_newest(dirs, "gfx/listen_icon.png")));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.2"), tr("Øvelse 2.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.2"), tr("Øvelse 2.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.3.1"), tr("Øvelse 2.3.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.3.1"), tr("Øvelse 2.3.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.3.2"), tr("Øvelse 2.3.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.3.2"), tr("Øvelse 2.3.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.4"), tr("Øvelse 2.4 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.4"), tr("Øvelse 2.4 (audio)"));
    COLUMN_BR(1,1);
    vbox->addSpacing(10);
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.5"), tr("Øvelse 2.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/2.5"), tr("Øvelse 2.5 (audio)"));
    vbox->addSpacing(20);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/2.2"), tr("FST 2.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/2.2"), tr("FST 2.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/2.2"), tr("FST 2.2.3: Oversættelse"));
    COLUMN_BR(1,2);
    COLUMNS_STACK;

    COLUMNS_BEGIN;
    section = dynamic_cast<ClickLabel*>(section_list.at(3));
    MK_LECTURE(tr("lectures/danish") + "/3.1", tr("Forelæsning 3.1"));
    COLUMN_BR(0,0);
    MK_SLIDES(tr("lectures/danish") + "/3.1", tr("Forelæsning 3.1 dias"));
    MK_SLIDES(tr("lectures/danish") + "/3.2", tr("Forelæsning 3.2 dias"));
    MK_LECTURE_PDF(tr("lectures/danish") + "/3.1", tr("Kapitel 3 som PDF"));
    COLUMN_BR(0,2);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/3.1"), tr("FST 3.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/3.1"), tr("FST 3.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/3.1"), tr("FST 3.1.3: Oversættelse"));
    vbox->addSpacing(20);
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/3"), tr("LFG 3"));
    COLUMN_BR(1,0);
    MK_LECTURE(tr("lectures/danish") + "/3.2", tr("Forelæsning 3.2"));
    COLUMN_BR(0,1);
    vbox->addSpacing(10);
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.1"), tr("Øvelse 3.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/3.1"), tr("Øvelse 3.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.2"), tr("Øvelse 3.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/3.2"), tr("Øvelse 3.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.3"), tr("Øvelse 3.3 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/3.3"), tr("Øvelse 3.3 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.4"), tr("Øvelse 3.4 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/3.4"), tr("Øvelse 3.4 (audio)"));
    COLUMN_BR(1,1);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/3.2"), tr("FST 3.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/3.2"), tr("FST 3.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/3.2"), tr("FST 3.2.3: Oversættelse"));
    COLUMN_BR(1,2);
    COLUMNS_STACK;

    COLUMNS_BEGIN;
    section = dynamic_cast<ClickLabel*>(section_list.at(4));
    MK_LECTURE(tr("lectures/danish") + "/4.1", tr("Forelæsning 4.1"));
    COLUMN_BR(0,0);
    MK_SLIDES(tr("lectures/danish") + "/4.1", tr("Forelæsning 4.1 dias"));
    MK_SLIDES(tr("lectures/danish") + "/4.2", tr("Forelæsning 4.2 dias"));
    MK_LECTURE_PDF(tr("lectures/danish") + "/4.1", tr("Kapitel 4 som PDF"));
    COLUMN_BR(0,2);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/4.1"), tr("FST 4.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/4.1"), tr("FST 4.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/4.1"), tr("FST 4.1.3: Oversættelse"));
    vbox->addSpacing(20);
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/4"), tr("LFG 4"));
    COLUMN_BR(1,0);
    MK_LECTURE(tr("lectures/danish") + "/4.2", tr("Forelæsning 4.2"));
    COLUMN_BR(0,1);
    vbox->addSpacing(10);
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
    COLUMN_BR(1,1);
    vbox->addSpacing(10);
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.5"), tr("Øvelse 4.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/4.5"), tr("Øvelse 4.5 (audio)"));
    vbox->addSpacing(20);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/4.2"), tr("FST 4.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/4.2"), tr("FST 4.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/4.2"), tr("FST 4.2.3: Oversættelse"));
    COLUMN_BR(1,2);
    COLUMNS_STACK;

    COLUMNS_BEGIN;
    section = dynamic_cast<ClickLabel*>(section_list.at(5));
    MK_LECTURE(tr("lectures/danish") + "/5.1", tr("Forelæsning 5.1"));
    COLUMN_BR(0,0);
    MK_SLIDES(tr("lectures/danish") + "/5.1", tr("Forelæsning 5.1 dias"));
    MK_SLIDES(tr("lectures/danish") + "/5.2", tr("Forelæsning 5.2 dias"));
    MK_LECTURE_PDF(tr("lectures/danish") + "/5.1", tr("Kapitel 5 som PDF"));
    COLUMN_BR(0,2);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/5.1"), tr("FST 5.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/5.1"), tr("FST 5.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/5.1"), tr("FST 5.1.3: Oversættelse"));
    vbox->addSpacing(20);
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/5"), tr("LFG 5"));
    COLUMN_BR(1,0);
    MK_LECTURE(tr("lectures/danish") + "/5.2", tr("Forelæsning 5.2"));
    COLUMN_BR(0,1);
    vbox->addSpacing(10);
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
    COLUMN_BR(1,1);
    vbox->addSpacing(10);
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.5"), tr("Øvelse 5.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/5.5"), tr("Øvelse 5.5 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.6"), tr("Øvelse 5.6 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/5.6"), tr("Øvelse 5.6 (audio)"));
    vbox->addSpacing(20);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/5.2"), tr("FST 5.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/5.2"), tr("FST 5.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/5.2"), tr("FST 5.2.3: Oversættelse"));
    COLUMN_BR(1,2);
    COLUMNS_STACK;

    COLUMNS_BEGIN;
    section = dynamic_cast<ClickLabel*>(section_list.at(6));
    MK_LECTURE(tr("lectures/danish") + "/6.1", tr("Forelæsning 6.1"));
    COLUMN_BR(0,0);
    MK_SLIDES(tr("lectures/danish") + "/6.1", tr("Forelæsning 6.1 dias"));
    MK_SLIDES(tr("lectures/danish") + "/6.2", tr("Forelæsning 6.2 dias"));
    MK_LECTURE_PDF(tr("lectures/danish") + "/6.1", tr("Kapitel 6 som PDF"));
    COLUMN_BR(0,2);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/6.1"), tr("FST 6.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/6.1"), tr("FST 6.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/6.1"), tr("FST 6.1.3: Oversættelse"));
    vbox->addSpacing(20);
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/6"), tr("LFG 6"));
    COLUMN_BR(1,0);
    MK_LECTURE(tr("lectures/danish") + "/6.2", tr("Forelæsning 6.2"));
    COLUMN_BR(0,1);
    vbox->addSpacing(10);
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
    COLUMN_BR(1,1);
    vbox->addSpacing(10);
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.4.2"), tr("Øvelse 6.4.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/6.4.2"), tr("Øvelse 6.4.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.5"), tr("Øvelse 6.5 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/6.5"), tr("Øvelse 6.5 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.6"), tr("Øvelse 6.6 (tekst)"));
    vbox->addSpacing(20);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/6.2"), tr("FST 6.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/6.2"), tr("FST 6.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/6.2"), tr("FST 6.2.3: Oversættelse"));
    COLUMN_BR(1,2);
    COLUMNS_STACK;

    COLUMNS_BEGIN;
    section = dynamic_cast<ClickLabel*>(section_list.at(7));
    MK_LECTURE(tr("lectures/danish") + "/7.1", tr("Forelæsning 7.1"));
    COLUMN_BR(0,0);
    MK_SLIDES(tr("lectures/danish") + "/7.1", tr("Forelæsning 7.1 dias"));
    MK_SLIDES(tr("lectures/danish") + "/7.2", tr("Forelæsning 7.2 dias"));
    MK_LECTURE_PDF(tr("lectures/danish") + "/7.1", tr("Kapitel 7 som PDF"));
    COLUMN_BR(0,2);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/7.1"), tr("FST 7.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/7.1"), tr("FST 7.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/7.1"), tr("FST 7.1.3: Oversættelse"));
    vbox->addSpacing(20);
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/7"), tr("LFG 7"));
    COLUMN_BR(1,0);
    MK_LECTURE(tr("lectures/danish") + "/7.2", tr("Forelæsning 7.2"));
    COLUMN_BR(0,1);
    vbox->addSpacing(10);
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.1"), tr("Øvelse 7.1 (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.2"), tr("Øvelse 7.2 (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.3.1"), tr("Øvelse 7.3.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/7.3.1"), tr("Øvelse 7.3.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.3.2"), tr("Øvelse 7.3.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/7.3.2"), tr("Øvelse 7.3.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.4"), tr("Øvelse 7.4 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/7.4"), tr("Øvelse 7.4 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.5"), tr("Øvelse 7.5 (tekst)"));
    COLUMN_BR(1,1);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/7.2"), tr("FST 7.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/7.2"), tr("FST 7.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/7.2"), tr("FST 7.2.3: Oversættelse"));
    COLUMN_BR(1,2);
    COLUMNS_STACK;

    COLUMNS_BEGIN;
    section = dynamic_cast<ClickLabel*>(section_list.at(8));
    MK_LECTURE(tr("lectures/danish") + "/8.1", tr("Forelæsning 8.1"));
    COLUMN_BR(0,0);
    MK_SLIDES(tr("lectures/danish") + "/8.1", tr("Forelæsning 8.1 dias"));
    MK_SLIDES(tr("lectures/danish") + "/8.2", tr("Forelæsning 8.2 dias"));
    MK_LECTURE_PDF(tr("lectures/danish") + "/8.1", tr("Kapitel 8 som PDF"));
    COLUMN_BR(0,2);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/8.1"), tr("FST 8.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/8.1"), tr("FST 8.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/8.1"), tr("FST 8.1.3: Oversættelse"));
    vbox->addSpacing(20);
    MK_LISTENREPEAT(QT_TR_NOOP("listenrepeat/8"), tr("LFG 8"));
    COLUMN_BR(1,0);
    MK_LECTURE(tr("lectures/danish") + "/8.2", tr("Forelæsning 8.2"));
    COLUMN_BR(0,1);
    vbox->addSpacing(10);
    MK_ANY(QT_TR_NOOP("exercises/8.1"), tr("Øvelse 8.1"), showFillout81);
    button->setIconSize(QSize(20, 20));
    button->setIcon(QIcon(find_newest(dirs, "gfx/table_icon.png")));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.2.1"), tr("Øvelse 8.2.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/8.2.1"), tr("Øvelse 8.2.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.2.2"), tr("Øvelse 8.2.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/8.2.2"), tr("Øvelse 8.2.2 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.3.1"), tr("Øvelse 8.3.1 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/8.3.1"), tr("Øvelse 8.3.1 (audio)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.3.2"), tr("Øvelse 8.3.2 (tekst)"));
    MK_EXERCISE_AUDIO(QT_TR_NOOP("exercises/8.3.2"), tr("Øvelse 8.3.2 (audio)"));
    COLUMN_BR(1,1);
    vbox->addSpacing(10);
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/8.2"), tr("FST 8.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/8.2"), tr("FST 8.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/8.2"), tr("FST 8.2.3: Oversættelse"));
    COLUMN_BR(1,2);
    COLUMNS_STACK;

    COLUMNS_BEGIN;
    section = dynamic_cast<ClickLabel*>(section_list.at(9));
    MK_LECTURE(tr("lectures/danish") + "/9", tr("Forelæsning 9"));
    COLUMN_BR(0,0);
    MK_SLIDES(tr("lectures/danish") + "/9", tr("Forelæsning 9 dias"));
    MK_LECTURE_PDF(tr("lectures/danish") + "/9", tr("Kapitel 9 som PDF"));
    COLUMN_BR(0,1);
    vbox->addSpacing(10);
    MK_ANY(QT_TR_NOOP("corpus"), tr("Korpussøgeinterface"), showCorpusQuery);
    button->setIconSize(QSize(24, 24));
    button->setIcon(QIcon(find_newest(dirs, "gfx/corpus_icon.png")));
    COLUMN_BR(1,0);
    vbox->addLayout(grid);
    QLabel *text = new QLabel(tr("outtro text"));
    text->setWordWrap(true);
    text->setMinimumWidth(800);
    vbox->addWidget(text, Qt::AlignLeft|Qt::AlignTop);
    widget = new QWidget;
    widget->setStyleSheet("margin: 0; padding: 0; text-align: left;");
    widget->setLayout(vbox);
    stack->addWidget(widget);

    middleHBox->addSpacing(10);
    middleHBox->addLayout(stack);

    outerVBox->addLayout(middleHBox);

    widget = new QWidget;
    widget->setContentsMargins(0,0,0,0);
    widget->setStyleSheet("QWidget { background-color: #fff; border-top: 2px solid #000; }");
    middleHBox = new QHBoxLayout;
    middleHBox->setContentsMargins(5, 5, 5, 5);
    middleHBox->setAlignment(Qt::AlignRight|Qt::AlignTop);

    button = new QPushButton(tr("Switch to English"));
    button->setStyleSheet("QWidget { border-top: 0; }");
    button->setFlat(true);
    button->setIconSize(QSize(44, 22));
    button->setIcon(QIcon(find_newest(dirs, tr("gfx/english.png"))));
    connect(button, SIGNAL(clicked()), this, SLOT(toggleLanguage()));
    middleHBox->addWidget(button);

    middleHBox->addSpacing(20);

    button = new QPushButton(tr("Afslut LG2"));
    button->setStyleSheet("QWidget { border-top: 0; }");
    button->setFlat(true);
    button->setIconSize(QSize(22, 22));
    button->setIcon(QIcon(find_newest(dirs, "gfx/exit.png")));
    connect(button, SIGNAL(clicked()), this, SLOT(close()));
    middleHBox->addWidget(button);

    widget->setLayout(middleHBox);
    outerVBox->addWidget(widget);

    setLayout(outerVBox);

    QSettings settings;
    int si = settings.value("chapter", 0).toInt();
    dynamic_cast<ClickLabel*>(section_list.at(si))->mousePressEvent(0);
}

void TaskChooser::showNext(const QString& title) {
    for (int i=0 ; i<buttons.size() ; ++i) {
        if (buttons.at(i).first == title) {
            ++i;
            if (i >= buttons.size()) {

            }
            else {
                dynamic_cast<ClickLabel*>(button_sect[buttons.at(i).second])->click();
                buttons.at(i).second->click();
            }
            break;
        }
    }
}

void TaskChooser::showFirstLecture() {
    const QPair<QString,QString>& p = mappings[sender()];
    showNext(p.second);
}

void TaskChooser::showLecture() {
    const QPair<QString,QString>& p = mappings[sender()];
    LecturePlayer *lp = new LecturePlayer(*this, p.first, p.second);
    lp->show();
    lp->raise();
    lp->activateWindow();
}

void TaskChooser::showLecturePDF() {
    const QPair<QString,QString>& p = mappings[sender()];
    QDesktopServices::openUrl(QUrl::fromLocalFile(find_newest(dirs, p.first + "/lecture.pdf")));
}

void TaskChooser::showSlidesPDF() {
    const QPair<QString,QString>& p = mappings[sender()];
    QDesktopServices::openUrl(QUrl::fromLocalFile(find_newest(dirs, p.first + "/slides.pdf")));
}

void TaskChooser::showFillout11() {
    const QPair<QString,QString>& p = mappings[sender()];
    Fillout11 *lp = new Fillout11(*this, p.first, p.second);
    lp->show();
    lp->raise();
    lp->activateWindow();
}

void TaskChooser::showFillout12() {
    const QPair<QString,QString>& p = mappings[sender()];
    Fillout12 *lp = new Fillout12(*this, p.first, p.second);
    lp->show();
    lp->raise();
    lp->activateWindow();
}

void TaskChooser::showFillout81() {
    const QPair<QString,QString>& p = mappings[sender()];
    Fillout81 *lp = new Fillout81(*this, p.first, p.second);
    lp->show();
    lp->raise();
    lp->activateWindow();
}

void TaskChooser::showCorpusQuery() {
    const QPair<QString,QString>& p = mappings[sender()];
    CorpusQuery *cq = new CorpusQuery(*this, p.first, p.second);
    cq->show();
    cq->raise();
    cq->activateWindow();
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

void TaskChooser::switchSection() {
    QSettings settings;
    QObject *s = sender();

    for (int i=0 ; i<section_list.length() ; ++i) {
        ClickLabel *l = dynamic_cast<ClickLabel*>(section_list.at(i));
        if (section_list.at(i) == s) {
            l->setText(QString("<h1>") + section_texts.at(i) + " &raquo;</h1>");
            l->setStyleSheet("QLabel { background-color: #007030; color: #fff; }");
            stack->setCurrentIndex(i);
            settings.setValue("chapter", i);
        }
        else {
            l->setText(QString("<h2>") + section_texts.at(i) + "</h2>");
            l->setStyleSheet("QLabel { background-color: #fff; color: #000; border-right: 2px solid #000; }");
        }
    }
}

void TaskChooser::checkFirstRun() {
    QSettings settings;
    if (settings.value("license_key", "").toString().isEmpty() || settings.value("encryption_key", "").toString().isEmpty() || settings.value("encryption_key", "").toString().at(0) != 'P') {
        ValidateKey *vk = new ValidateKey;
        vk->show();
        vk->raise();
        vk->activateWindow();
    }
}
