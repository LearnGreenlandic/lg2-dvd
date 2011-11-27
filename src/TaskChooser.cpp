#include "QATextOnly.hpp"
#include "UpdownOne.hpp"
#include "UpdownTwo.hpp"
#include "UpdownThree.hpp"
#include "LecturePlayer.hpp"
#include "TaskChooser.hpp"

#include <QtGlobal>

#define MK_LECTURE(which, title) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture())); \
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

#define MK_EXERCISE_TEXT(which, title) \
    p = QPair<QString,QString>(which, title); \
    button = new QPushButton(p.second); \
    button->setFlat(true); \
    mappings[button] = p; \
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_qaTextOnly())); \
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

    MK_LECTURE("1.1", tr("Forelæsning 1.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/1.1"), tr("FST 1.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/1.1"), tr("FST 1.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/1.1"), tr("FST 1.1.3: Oversættelse"));
    MK_LECTURE("1.2", tr("Forelæsning 1.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.3"), tr("Øvelse 1.3: Brug Vb+TAR I (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.4"), tr("Øvelse 1.4: Brug Vb+TAR II (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.5"), tr("Øvelse 1.5: Brug participium (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.6"), tr("Øvelse 1.6: Brug N{-mik} (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.7"), tr("Øvelse 1.7: Brug {+mut} og væn dig til C1C2-reglen (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/1.8"), tr("Øvelse 1.8: Brug N-U{+vuq} (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/1.2"), tr("FST 1.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/1.2"), tr("FST 1.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/1.2"), tr("FST 1.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE("2.1", tr("Forelæsning 2.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/2.1"), tr("FST 2.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/2.1"), tr("FST 2.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/2.1"), tr("FST 2.1.3: Oversættelse"));
    MK_LECTURE("2.2", tr("Forelæsning 2.2"));
    /*
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.1"), tr("Øvelse 2.1: Øv din lydopfattelse"));
    //*/
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.2"), tr("Øvelse 2.2: Lokalis på hhv. p-stammer og up-stammer (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.3.1"), tr("Øvelse 2.3.1: p-stammer og up-stammer (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.3.2"), tr("Øvelse 2.3.2: p-stammer og up-stammer (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.4"), tr("Øvelse 2.4: Ledsagemåde som indikativ nummer 2 (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/2.5"), tr("Øvelse 2.5: Ledsagemåden til det sekundære udsagn (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/2.2"), tr("FST 2.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/2.2"), tr("FST 2.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/2.2"), tr("FST 2.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE("3.1", tr("Forelæsning 3.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/3.1"), tr("FST 3.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/3.1"), tr("FST 3.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/3.1"), tr("FST 3.1.3: Oversættelse"));
    MK_LECTURE("3.2", tr("Forelæsning 3.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.1"), tr("Øvelse 3.1: Personendelser I (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.2"), tr("Øvelse 3.2: Personendelser II (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.3"), tr("Øvelse 3.3: Participium med efterhængt UNA (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/3.4"), tr("Øvelse 3.4: Ledsagemåden fortsat (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/3.2"), tr("FST 3.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/3.2"), tr("FST 3.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/3.2"), tr("FST 3.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE("4.1", tr("Forelæsning 4.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/4.1"), tr("FST 4.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/4.1"), tr("FST 4.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/4.1"), tr("FST 4.1.3: Oversættelse"));
    MK_LECTURE("4.2", tr("Forelæsning 4.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.1"), tr("Øvelse 4.1: Tilhængspartiklerne LI og LU (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.2"), tr("Øvelse 4.2: p-stammer og up-stammer i flertal (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.3"), tr("Øvelse 4.3: Possessum I (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.4.1"), tr("Øvelse 4.4.1: Possessum II (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.4.2"), tr("Øvelse 4.4.2: Possessum II (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/4.5"), tr("Øvelse 4.5: LI og LU med ledsagemåde (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/4.2"), tr("FST 4.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/4.2"), tr("FST 4.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/4.2"), tr("FST 4.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE("5.1", tr("Forelæsning 5.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/5.1"), tr("FST 5.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/5.1"), tr("FST 5.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/5.1"), tr("FST 5.1.3: Oversættelse"));
    MK_LECTURE("5.2", tr("Forelæsning 5.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.1"), tr("Øvelse 5.1: De første causativer (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.2"), tr("Øvelse 5.2: Gradbøjning med +NIRU (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.3"), tr("Øvelse 5.3: Variationer over ә I (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.4.1"), tr("Øvelse 5.4.1: Variationer over ә II (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.4.2"), tr("Øvelse 5.4.2: Variationer over ә II (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.5"), tr("Øvelse 5.5: Variationer over ә III (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/5.6"), tr("Øvelse 5.6: Ledsagemåde og participium (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/5.2"), tr("FST 5.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/5.2"), tr("FST 5.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/5.2"), tr("FST 5.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE("6.1", tr("Forelæsning 6.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/6.1"), tr("FST 6.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/6.1"), tr("FST 6.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/6.1"), tr("FST 6.1.3: Oversættelse"));
    MK_LECTURE("6.2", tr("Forelæsning 6.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.1"), tr("Øvelse 6.1: Øv N-U og N-U-NNGIT sammen med tilhængspartiklerne LI og LU (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.2"), tr("Øvelse 6.2: Øv N-INNAQ (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.3.1"), tr("Øvelse 6.3.1: Øv Vb+GUSUP (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.3.2"), tr("Øvelse 6.3.2: Øv Vb+GUSUP (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.4.1"), tr("Øvelse 6.4.1: Øv Vb+TAR (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.4.2"), tr("Øvelse 6.4.2: Øv Vb+TAR (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.5"), tr("Øvelse 6.5: Øv <Terminalis-KAR (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/6.6"), tr("Øvelse 6.6: Øv <Terminalis-KAR med yderligere tilhæng (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/6.2"), tr("FST 6.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/6.2"), tr("FST 6.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/6.2"), tr("FST 6.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE("7.1", tr("Forelæsning 7.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/7.1"), tr("FST 7.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/7.1"), tr("FST 7.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/7.1"), tr("FST 7.1.3: Oversættelse"));
    MK_LECTURE("7.2", tr("Forelæsning 7.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.1"), tr("Øvelse 7.1: Brug den spørgende dummy su- (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.2"), tr("Øvelse 7.2: Øv dig i brugen af ila (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.3.1"), tr("Øvelse 7.3.1: Absolut og relativ (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.3.2"), tr("Øvelse 7.3.2: Absolut og relativ (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.4"), tr("Øvelse 7.4: Leg lidt med halvtransitiv I (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/7.5"), tr("Øvelse 7.5: Leg lidt med halvtransitiv II (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/7.2"), tr("FST 7.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/7.2"), tr("FST 7.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/7.2"), tr("FST 7.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE("8.1", tr("Forelæsning 8.1"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/8.1"), tr("FST 8.1.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/8.1"), tr("FST 8.1.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/8.1"), tr("FST 8.1.3: Oversættelse"));
    MK_LECTURE("8.2", tr("Forelæsning 8.2"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.2.1"), tr("Øvelse 8.2.1: Øv strukturen Vb+TUQ-QAR (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.2.2"), tr("Øvelse 8.2.2: Øv strukturen Vb+TUQ-QAR (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.3.1"), tr("Øvelse 8.3.1: Øv strukturen Vb+TUQ-RUJUK=SUAQ-U (tekst)"));
    MK_EXERCISE_TEXT(QT_TR_NOOP("exercises/8.3.2"), tr("Øvelse 8.3.2: Øv strukturen Vb+TUQ-RUJUK=SUAQ-U-NNGIT (tekst)"));
    MK_FST_RECEPTION(QT_TR_NOOP("fsts/8.2"), tr("FST 8.2.1: Reception"));
    MK_FST_DFG(QT_TR_NOOP("fsts/8.2"), tr("FST 8.2.2: Produktion"));
    MK_FST_DFT(QT_TR_NOOP("fsts/8.2"), tr("FST 8.2.3: Oversættelse"));

    vbox->addSpacing(10);

    MK_LECTURE("9.1", tr("Forelæsning 9.1"));
    MK_LECTURE("9.2", tr("Forelæsning 9.2"));
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
