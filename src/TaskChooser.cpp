#include "QATextOnly.hpp"
#include "TaskChooser.hpp"

#include <QtGlobal>

TaskChooser::TaskChooser(const dirmap_t& dirs, QTranslator *translator) :
QWidget(0, Qt::Window | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint),
dirs(dirs),
translator(translator)
{
    setWindowTitle(trUtf8("Grønlandsk for voksne 2"));
    setContentsMargins(5,5,5,5);
    setStyleSheet("margin: 0; padding: 0; text-align: left;");

    QPushButton *button;

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0,0,0,0);

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

    vbox->addSpacing(10);

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

    vbox->addSpacing(10);

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

    vbox->addSpacing(10);

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

    vbox->addSpacing(10);

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

    vbox->setAlignment(Qt::AlignCenter|Qt::AlignTop);

    setLayout(vbox);
}

void TaskChooser::showExercise_qaTextOnly(QString which, QString title) {
    QATextOnly *qa = new QATextOnly(dirs, which, title);
    qa->show();
    qa->raise();
    qa->activateWindow();
}

void TaskChooser::showExercise_1_3_text() {
    showExercise_qaTextOnly("exercises/1.3", trUtf8("Øvelse 1.3: Brug Vb+TAR I (tekst)"));
}

void TaskChooser::showExercise_1_4_text() {
    showExercise_qaTextOnly("exercises/1.4", trUtf8("Øvelse 1.4: Brug Vb+TAR II (tekst)"));
}

void TaskChooser::showExercise_1_5_text() {
    showExercise_qaTextOnly("exercises/1.5", trUtf8("Øvelse 1.5: Brug participium (tekst)"));
}

void TaskChooser::showExercise_1_6_text() {
    showExercise_qaTextOnly("exercises/1.6", trUtf8("Øvelse 1.6: Brug N{-mik} (tekst)"));
}

void TaskChooser::showExercise_1_7_text() {
    showExercise_qaTextOnly("exercises/1.7", trUtf8("Øvelse 1.7: Brug {+mut} og væn dig til C1C2-reglen (tekst)"));
}

void TaskChooser::showExercise_1_8_text() {
    showExercise_qaTextOnly("exercises/1.8", trUtf8("Øvelse 1.8: Brug N-U{+vuq} (tekst)"));
}

void TaskChooser::showExercise_2_1_text() {
}

void TaskChooser::showExercise_2_2_text() {
    showExercise_qaTextOnly("exercises/2.2", trUtf8("Øvelse 2.2: Lokalis på hhv. p-stammer og up-stammer (tekst)"));
}

void TaskChooser::showExercise_2_3_1_text() {
    showExercise_qaTextOnly("exercises/2.3.1", trUtf8("Øvelse 2.3.1: p-stammer og up-stammer (tekst)"));
}

void TaskChooser::showExercise_2_3_2_text() {
    showExercise_qaTextOnly("exercises/2.3.2", trUtf8("Øvelse 2.3.2: p-stammer og up-stammer (tekst)"));
}

void TaskChooser::showExercise_2_4_text() {
    showExercise_qaTextOnly("exercises/2.4", trUtf8("Øvelse 2.4: Ledsagemåde som indikativ nummer 2 (tekst)"));
}

void TaskChooser::showExercise_2_5_text() {
    showExercise_qaTextOnly("exercises/2.5", trUtf8("Øvelse 2.5: Ledsagemåden til det sekundære udsagn (tekst)"));
}

void TaskChooser::showExercise_3_1_text() {
    showExercise_qaTextOnly("exercises/3.1", trUtf8("Øvelse 3.1: Personendelser I (tekst)"));
}

void TaskChooser::showExercise_3_2_text() {
    showExercise_qaTextOnly("exercises/3.2", trUtf8("Øvelse 3.2: Personendelser II (tekst)"));
}

void TaskChooser::showExercise_3_3_text() {
    showExercise_qaTextOnly("exercises/3.3", trUtf8("Øvelse 3.3: Participium med efterhængt UNA (tekst)"));
}

void TaskChooser::showExercise_3_4_text() {
    showExercise_qaTextOnly("exercises/3.4", trUtf8("Øvelse 3.4: Ledsagemåden fortsat (tekst)"));
}

void TaskChooser::showExercise_4_1_text() {
    showExercise_qaTextOnly("exercises/4.1", trUtf8("Øvelse 4.1: Tilhængspartiklerne LI og LU (tekst)"));
}

void TaskChooser::showExercise_4_2_text() {
    showExercise_qaTextOnly("exercises/4.2", trUtf8("Øvelse 4.2: p-stammer og up-stammer i flertal (tekst)"));
}

void TaskChooser::showExercise_4_3_text() {
    showExercise_qaTextOnly("exercises/4.3", trUtf8("Øvelse 4.3: Possessum I (tekst)"));
}

void TaskChooser::showExercise_4_4_1_text() {
    showExercise_qaTextOnly("exercises/4.4.1", trUtf8("Øvelse 4.4.1: Possessum II (tekst)"));
}

void TaskChooser::showExercise_4_4_2_text() {
    showExercise_qaTextOnly("exercises/4.4.2", trUtf8("Øvelse 4.4.2: Possessum II (tekst)"));
}

void TaskChooser::showExercise_4_5_text() {
    showExercise_qaTextOnly("exercises/4.5", trUtf8("Øvelse 4.5: LI og LU med ledsagemåde (tekst)"));
}

void TaskChooser::showExercise_5_1_text() {
    showExercise_qaTextOnly("exercises/5.1", trUtf8("Øvelse 5.1: De første causativer (tekst)"));
}

void TaskChooser::showExercise_5_2_text() {
    showExercise_qaTextOnly("exercises/5.2", trUtf8("Øvelse 5.2: Gradbøjning med +NIRU (tekst)"));
}

void TaskChooser::showExercise_5_3_text() {
    showExercise_qaTextOnly("exercises/5.3", trUtf8("Øvelse 5.3: Variationer over ә I (tekst)"));
}

void TaskChooser::showExercise_5_4_1_text() {
    showExercise_qaTextOnly("exercises/5.4.1", trUtf8("Øvelse 5.4.1: Variationer over ә II (tekst)"));
}

void TaskChooser::showExercise_5_4_2_text() {
    showExercise_qaTextOnly("exercises/5.4.2", trUtf8("Øvelse 5.4.2: Variationer over ә II (tekst)"));
}

void TaskChooser::showExercise_5_5_text() {
    showExercise_qaTextOnly("exercises/5.5", trUtf8("Øvelse 5.5: Variationer over ә III (tekst)"));
}

void TaskChooser::showExercise_5_6_text() {
    showExercise_qaTextOnly("exercises/5.6", trUtf8("Øvelse 5.6: Ledsagemåde og participium (tekst)"));
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
