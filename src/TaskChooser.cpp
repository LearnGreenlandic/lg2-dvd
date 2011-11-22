#include "QATextOnly.hpp"
#include "UpdownOne.hpp"
#include "UpdownTwo.hpp"
#include "UpdownThree.hpp"
#include "LecturePlayer.hpp"
#include "TaskChooser.hpp"

#include <QtGlobal>

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

    button = new QPushButton(trUtf8("Forelæsning 1.1"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_1_1()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/1.1", tr("FST 1.1.1: Reception"));
    MK_FST_DFG("fsts/1.1", tr("FST 1.1.2: Produktion"));
    MK_FST_DFT("fsts/1.1", tr("FST 1.1.3: Oversættelse"));

    button = new QPushButton(trUtf8("Forelæsning 1.2"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_1_2()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 1.3: Brug Vb+TAR I (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_1_3_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 1.4: Brug Vb+TAR II (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_1_4_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 1.5: Brug participium (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_1_5_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 1.6: Brug N{-mik} (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_1_6_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 1.7: Brug {+mut} og væn dig til C1C2-reglen (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_1_7_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 1.8: Brug N-U{+vuq} (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_1_8_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/1.2", tr("FST 1.2.1: Reception"));
    MK_FST_DFG("fsts/1.2", tr("FST 1.2.2: Produktion"));
    MK_FST_DFT("fsts/1.2", tr("FST 1.2.3: Oversættelse"));

    vbox->addSpacing(10);

    button = new QPushButton(trUtf8("Forelæsning 2.1"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_2_1()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/2.1", tr("FST 2.1.1: Reception"));
    MK_FST_DFG("fsts/2.1", tr("FST 2.1.2: Produktion"));
    MK_FST_DFT("fsts/2.1", tr("FST 2.1.3: Oversættelse"));

    button = new QPushButton(trUtf8("Forelæsning 2.2"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_2_2()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    /*
    button = new QPushButton(trUtf8("Øvelse 2.1: Øv din lydopfattelse"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_2_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);
    //*/

    button = new QPushButton(trUtf8("Øvelse 2.2: Lokalis på hhv. p-stammer og up-stammer (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_2_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 2.3.1: p-stammer og up-stammer (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_2_3_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 2.3.2: p-stammer og up-stammer (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_2_3_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 2.4: Ledsagemåde som indikativ nummer 2 (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_2_4_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 2.5: Ledsagemåden til det sekundære udsagn (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_2_5_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/2.2", tr("FST 2.2.1: Reception"));
    MK_FST_DFG("fsts/2.2", tr("FST 2.2.2: Produktion"));
    MK_FST_DFT("fsts/2.2", tr("FST 2.2.3: Oversættelse"));

    vbox->addSpacing(10);

    button = new QPushButton(trUtf8("Forelæsning 3.1"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_3_1()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/3.1", tr("FST 3.1.1: Reception"));
    MK_FST_DFG("fsts/3.1", tr("FST 3.1.2: Produktion"));
    MK_FST_DFT("fsts/3.1", tr("FST 3.1.3: Oversættelse"));

    button = new QPushButton(trUtf8("Forelæsning 3.2"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_3_2()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 3.1: Personendelser I (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_3_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 3.2: Personendelser II (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_3_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 3.3: Participium med efterhængt UNA (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_3_3_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 3.4: Ledsagemåden fortsat (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_3_4_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/3.2", tr("FST 3.2.1: Reception"));
    MK_FST_DFG("fsts/3.2", tr("FST 3.2.2: Produktion"));
    MK_FST_DFT("fsts/3.2", tr("FST 3.2.3: Oversættelse"));

    vbox->addSpacing(10);

    button = new QPushButton(trUtf8("Forelæsning 4.1"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_4_1()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/4.1", tr("FST 4.1.1: Reception"));
    MK_FST_DFG("fsts/4.1", tr("FST 4.1.2: Produktion"));
    MK_FST_DFT("fsts/4.1", tr("FST 4.1.3: Oversættelse"));

    button = new QPushButton(trUtf8("Forelæsning 4.2"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_4_2()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 4.1: Tilhængspartiklerne LI og LU (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_4_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 4.2: p-stammer og up-stammer i flertal (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_4_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 4.3: Possessum I (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_4_3_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 4.4.1: Possessum II (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_4_4_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 4.4.2: Possessum II (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_4_4_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 4.5: LI og LU med ledsagemåde (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_4_5_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/4.2", tr("FST 4.2.1: Reception"));
    MK_FST_DFG("fsts/4.2", tr("FST 4.2.2: Produktion"));
    MK_FST_DFT("fsts/4.2", tr("FST 4.2.3: Oversættelse"));

    vbox->addSpacing(10);

    button = new QPushButton(trUtf8("Forelæsning 5.1"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_5_1()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/5.1", tr("FST 5.1.1: Reception"));
    MK_FST_DFG("fsts/5.1", tr("FST 5.1.2: Produktion"));
    MK_FST_DFT("fsts/5.1", tr("FST 5.1.3: Oversættelse"));

    button = new QPushButton(trUtf8("Forelæsning 5.2"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_5_2()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 5.1: De første causativer (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_5_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 5.2: Gradbøjning med +NIRU (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_5_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 5.3: Variationer over ә I (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_5_3_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 5.4.1: Variationer over ә II (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_5_4_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 5.4.2: Variationer over ә II (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_5_4_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 5.5: Variationer over ә III (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_5_5_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 5.6: Ledsagemåde og participium (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_5_6_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/5.2", tr("FST 5.2.1: Reception"));
    MK_FST_DFG("fsts/5.2", tr("FST 5.2.2: Produktion"));
    MK_FST_DFT("fsts/5.2", tr("FST 5.2.3: Oversættelse"));

    vbox->addSpacing(10);

    button = new QPushButton(trUtf8("Forelæsning 6.1"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_6_1()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/6.1", tr("FST 6.1.1: Reception"));
    MK_FST_DFG("fsts/6.1", tr("FST 6.1.2: Produktion"));
    MK_FST_DFT("fsts/6.1", tr("FST 6.1.3: Oversættelse"));

    button = new QPushButton(trUtf8("Forelæsning 6.2"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_6_2()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 6.1: Øv N-U og N-U-NNGIT sammen med tilhængspartiklerne LI og LU (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_6_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 6.2: Øv N-INNAQ (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_6_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 6.3.1: Øv Vb+GUSUP (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_6_3_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 6.3.2: Øv Vb+GUSUP (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_6_3_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 6.4.1: Øv Vb+TAR (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_6_4_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 6.4.2: Øv Vb+TAR (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_6_4_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 6.5: Øv <Terminalis-KAR (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_6_5_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 6.6: Øv <Terminalis-KAR med yderligere tilhæng (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_6_6_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/6.2", tr("FST 6.2.1: Reception"));
    MK_FST_DFG("fsts/6.2", tr("FST 6.2.2: Produktion"));
    MK_FST_DFT("fsts/6.2", tr("FST 6.2.3: Oversættelse"));

    vbox->addSpacing(10);

    button = new QPushButton(trUtf8("Forelæsning 7.1"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_7_1()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/7.1", tr("FST 7.1.1: Reception"));
    MK_FST_DFG("fsts/7.1", tr("FST 7.1.2: Produktion"));
    MK_FST_DFT("fsts/7.1", tr("FST 7.1.3: Oversættelse"));

    button = new QPushButton(trUtf8("Forelæsning 7.2"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_7_2()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 7.1: Brug den spørgende dummy su- (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_7_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 7.2: Øv dig i brugen af ila (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_7_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 7.3.1: Absolut og relativ (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_7_3_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 7.3.2: Absolut og relativ (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_7_3_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 7.4: Leg lidt med halvtransitiv I (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_7_4_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 7.5: Leg lidt med halvtransitiv II (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_7_5_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/7.2", tr("FST 7.2.1: Reception"));
    MK_FST_DFG("fsts/7.2", tr("FST 7.2.2: Produktion"));
    MK_FST_DFT("fsts/7.2", tr("FST 7.2.3: Oversættelse"));

    vbox->addSpacing(10);

    button = new QPushButton(trUtf8("Forelæsning 8.1"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_8_1()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/8.1", tr("FST 8.1.1: Reception"));
    MK_FST_DFG("fsts/8.1", tr("FST 8.1.2: Produktion"));
    MK_FST_DFT("fsts/8.1", tr("FST 8.1.3: Oversættelse"));

    button = new QPushButton(trUtf8("Forelæsning 8.2"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_8_2()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 8.2.1: Øv strukturen Vb+TUQ-QAR (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_8_2_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 8.2.2: Øv strukturen Vb+TUQ-QAR (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_8_2_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 8.3.1: Øv strukturen Vb+TUQ-RUJUK=SUAQ-U (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_8_3_1_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Øvelse 8.3.2: Øv strukturen Vb+TUQ-RUJUK=SUAQ-U-NNGIT (tekst)"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showExercise_8_3_2_text()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    MK_FST_RECEPTION("fsts/8.2", tr("FST 8.2.1: Reception"));
    MK_FST_DFG("fsts/8.2", tr("FST 8.2.2: Produktion"));
    MK_FST_DFT("fsts/8.2", tr("FST 8.2.3: Oversættelse"));

    vbox->addSpacing(10);

    button = new QPushButton(trUtf8("Forelæsning 9.1"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_9_1()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    button = new QPushButton(trUtf8("Forelæsning 9.2"));
    button->setFlat(true);
    connect(button, SIGNAL(clicked()), this, SLOT(showLecture_9_2()));
    vbox->addWidget(button, 0, Qt::AlignLeft|Qt::AlignTop);

    vbox->setAlignment(Qt::AlignCenter|Qt::AlignTop);

    widget->setLayout(vbox);

    setWidget(widget);
    setMinimumWidth(widget->width()+50);
}

void TaskChooser::showLecture(QString which, QString title) {
    LecturePlayer *lp = new LecturePlayer(*this, which, title);
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

void TaskChooser::showExercise_qaTextOnly(QString which, QString title) {
    QATextOnly *qa = new QATextOnly(*this, which, title);
    qa->show();
    qa->raise();
    qa->activateWindow();
}

void TaskChooser::showLecture_1_1() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/1.1", trUtf8("Forelæsning 1.1"));
}

void TaskChooser::showLecture_1_2() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/1.2", trUtf8("Forelæsning 1.2"));
}

void TaskChooser::showLecture_2_1() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/2.1", trUtf8("Forelæsning 2.1"));
}

void TaskChooser::showLecture_2_2() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/2.2", trUtf8("Forelæsning 2.2"));
}

void TaskChooser::showLecture_3_1() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/3.1", trUtf8("Forelæsning 3.1"));
}

void TaskChooser::showLecture_3_2() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/3.2", trUtf8("Forelæsning 3.2"));
}

void TaskChooser::showLecture_4_1() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/4.1", trUtf8("Forelæsning 4.1"));
}

void TaskChooser::showLecture_4_2() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/4.2", trUtf8("Forelæsning 4.2"));
}

void TaskChooser::showLecture_5_1() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/5.1", trUtf8("Forelæsning 5.1"));
}

void TaskChooser::showLecture_5_2() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/5.2", trUtf8("Forelæsning 5.2"));
}

void TaskChooser::showLecture_6_1() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/6.1", trUtf8("Forelæsning 6.1"));
}

void TaskChooser::showLecture_6_2() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/6.2", trUtf8("Forelæsning 6.2"));
}

void TaskChooser::showLecture_7_1() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/7.1", trUtf8("Forelæsning 7.1"));
}

void TaskChooser::showLecture_7_2() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/7.2", trUtf8("Forelæsning 7.2"));
}

void TaskChooser::showLecture_8_1() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/8.1", trUtf8("Forelæsning 8.1"));
}

void TaskChooser::showLecture_8_2() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/8.2", trUtf8("Forelæsning 8.2"));
}

void TaskChooser::showLecture_9_1() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/9.1", trUtf8("Forelæsning 9.1"));
}

void TaskChooser::showLecture_9_2() {
    QSettings settings;
    showLecture(QString("lectures/") + settings.value("language").toString() + "/9.2", trUtf8("Forelæsning 9.2"));
}

void TaskChooser::showExercise_1_3_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/1.3"), trUtf8("Øvelse 1.3: Brug Vb+TAR I (tekst)"));
}

void TaskChooser::showExercise_1_4_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/1.4"), trUtf8("Øvelse 1.4: Brug Vb+TAR II (tekst)"));
}

void TaskChooser::showExercise_1_5_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/1.5"), trUtf8("Øvelse 1.5: Brug participium (tekst)"));
}

void TaskChooser::showExercise_1_6_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/1.6"), trUtf8("Øvelse 1.6: Brug N{-mik} (tekst)"));
}

void TaskChooser::showExercise_1_7_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/1.7"), trUtf8("Øvelse 1.7: Brug {+mut} og væn dig til C1C2-reglen (tekst)"));
}

void TaskChooser::showExercise_1_8_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/1.8"), trUtf8("Øvelse 1.8: Brug N-U{+vuq} (tekst)"));
}

void TaskChooser::showExercise_2_1_text() {
}

void TaskChooser::showExercise_2_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/2.2"), trUtf8("Øvelse 2.2: Lokalis på hhv. p-stammer og up-stammer (tekst)"));
}

void TaskChooser::showExercise_2_3_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/2.3.1"), trUtf8("Øvelse 2.3.1: p-stammer og up-stammer (tekst)"));
}

void TaskChooser::showExercise_2_3_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/2.3.2"), trUtf8("Øvelse 2.3.2: p-stammer og up-stammer (tekst)"));
}

void TaskChooser::showExercise_2_4_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/2.4"), trUtf8("Øvelse 2.4: Ledsagemåde som indikativ nummer 2 (tekst)"));
}

void TaskChooser::showExercise_2_5_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/2.5"), trUtf8("Øvelse 2.5: Ledsagemåden til det sekundære udsagn (tekst)"));
}

void TaskChooser::showExercise_3_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/3.1"), trUtf8("Øvelse 3.1: Personendelser I (tekst)"));
}

void TaskChooser::showExercise_3_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/3.2"), trUtf8("Øvelse 3.2: Personendelser II (tekst)"));
}

void TaskChooser::showExercise_3_3_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/3.3"), trUtf8("Øvelse 3.3: Participium med efterhængt UNA (tekst)"));
}

void TaskChooser::showExercise_3_4_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/3.4"), trUtf8("Øvelse 3.4: Ledsagemåden fortsat (tekst)"));
}

void TaskChooser::showExercise_4_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/4.1"), trUtf8("Øvelse 4.1: Tilhængspartiklerne LI og LU (tekst)"));
}

void TaskChooser::showExercise_4_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/4.2"), trUtf8("Øvelse 4.2: p-stammer og up-stammer i flertal (tekst)"));
}

void TaskChooser::showExercise_4_3_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/4.3"), trUtf8("Øvelse 4.3: Possessum I (tekst)"));
}

void TaskChooser::showExercise_4_4_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/4.4.1"), trUtf8("Øvelse 4.4.1: Possessum II (tekst)"));
}

void TaskChooser::showExercise_4_4_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/4.4.2"), trUtf8("Øvelse 4.4.2: Possessum II (tekst)"));
}

void TaskChooser::showExercise_4_5_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/4.5"), trUtf8("Øvelse 4.5: LI og LU med ledsagemåde (tekst)"));
}

void TaskChooser::showExercise_5_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/5.1"), trUtf8("Øvelse 5.1: De første causativer (tekst)"));
}

void TaskChooser::showExercise_5_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/5.2"), trUtf8("Øvelse 5.2: Gradbøjning med +NIRU (tekst)"));
}

void TaskChooser::showExercise_5_3_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/5.3"), trUtf8("Øvelse 5.3: Variationer over ә I (tekst)"));
}

void TaskChooser::showExercise_5_4_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/5.4.1"), trUtf8("Øvelse 5.4.1: Variationer over ә II (tekst)"));
}

void TaskChooser::showExercise_5_4_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/5.4.2"), trUtf8("Øvelse 5.4.2: Variationer over ә II (tekst)"));
}

void TaskChooser::showExercise_5_5_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/5.5"), trUtf8("Øvelse 5.5: Variationer over ә III (tekst)"));
}

void TaskChooser::showExercise_5_6_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/5.6"), trUtf8("Øvelse 5.6: Ledsagemåde og participium (tekst)"));
}

void TaskChooser::showExercise_6_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/6.1"), trUtf8("Øvelse 6.1: Øv N-U og N-U-NNGIT sammen med tilhængspartiklerne LI og LU (tekst)"));
}

void TaskChooser::showExercise_6_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/6.2"), trUtf8("Øvelse 6.2: Øv N-INNAQ (tekst)"));
}

void TaskChooser::showExercise_6_3_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/6.3.1"), trUtf8("Øvelse 6.3.1: Øv Vb+GUSUP (tekst)"));
}

void TaskChooser::showExercise_6_3_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/6.3.2"), trUtf8("Øvelse 6.3.2: Øv Vb+GUSUP (tekst)"));
}

void TaskChooser::showExercise_6_4_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/6.4.1"), trUtf8("Øvelse 6.4.1: Øv Vb+TAR (tekst)"));
}

void TaskChooser::showExercise_6_4_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/6.4.2"), trUtf8("Øvelse 6.4.2: Øv Vb+TAR (tekst)"));
}

void TaskChooser::showExercise_6_5_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/6.5"), trUtf8("Øvelse 6.5: Øv <Terminalis-KAR (tekst)"));
}

void TaskChooser::showExercise_6_6_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/6.6"), trUtf8("Øvelse 6.6: Øv <Terminalis-KAR med yderligere tilhæng (tekst)"));
}

void TaskChooser::showExercise_7_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/7.1"), trUtf8("Øvelse 7.1: Brug den spørgende dummy su- (tekst)"));
}

void TaskChooser::showExercise_7_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/7.2"), trUtf8("Øvelse 7.2: Øv dig i brugen af ila (tekst)"));
}

void TaskChooser::showExercise_7_3_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/7.3.1"), trUtf8("Øvelse 7.3.1: Absolut og relativ (tekst)"));
}

void TaskChooser::showExercise_7_3_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/7.3.2"), trUtf8("Øvelse 7.3.2: Absolut og relativ (tekst)"));
}

void TaskChooser::showExercise_7_4_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/7.4"), trUtf8("Øvelse 7.4: Leg lidt med halvtransitiv I (tekst)"));
}

void TaskChooser::showExercise_7_5_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/7.5"), trUtf8("Øvelse 7.5: Leg lidt med halvtransitiv II (tekst)"));
}

void TaskChooser::showExercise_8_2_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/8.2.1"), trUtf8("Øvelse 8.2.1: Øv strukturen Vb+TUQ-QAR (tekst)"));
}

void TaskChooser::showExercise_8_2_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/8.2.2"), trUtf8("Øvelse 8.2.2: Øv strukturen Vb+TUQ-QAR (tekst)"));
}

void TaskChooser::showExercise_8_3_1_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/8.3.1"), trUtf8("Øvelse 8.3.1: Øv strukturen Vb+TUQ-RUJUK=SUAQ-U (tekst)"));
}

void TaskChooser::showExercise_8_3_2_text() {
    showExercise_qaTextOnly(QT_TR_NOOP("exercises/8.3.2"), trUtf8("Øvelse 8.3.2: Øv strukturen Vb+TUQ-RUJUK=SUAQ-U-NNGIT (tekst)"));
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
