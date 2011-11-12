#include <QtGui>
#include <ctime>
#include <cstdlib>
#include <stdint.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationDomain("learngreenlandic.com");
    app.setOrganizationName("LearnGreenlandic");
    app.setApplicationName("Learn Greenlandic 2");
    app.setQuitOnLastWindowClosed(true);

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings settings;

    if (!settings.isWritable()) {
        QMessageBox::critical(0, "Settings Not Writable!", "Unable to store user-specific settings. Cannot continue...");
        app.exit(-1);
        return -1;
    }


    uint32_t seed = static_cast<uint32_t>(time(0)) ^ app.applicationPid();
    qsrand(seed);
    srand(seed);

    QDir tDir(app.applicationDirPath());
    while (!tDir.exists("./lessons/1/danish/0.png") && tDir.cdUp()) {
    }

    QDir dataDir(tDir.absolutePath() + "/lessons");
    if (!dataDir.exists() || !dataDir.exists("./1/danish/lecture.dat") || !dataDir.exists("./1/danish/0.png")) {
        QFileInfoList drives = QDir::drives();
        foreach (QFileInfo drive, drives) {
            if (drive.absoluteDir().exists("./lessons/1/danish/0.png")) {
                dataDir = drive.absoluteDir().absolutePath() + "/lessons";
                break;
            }
        }
    }
    if (!dataDir.exists() || !dataDir.exists("./1/danish/lecture.dat") || !dataDir.exists("./1/danish/0.png")) {
        QMessageBox::critical(0, "Missing Data!", "Could not find required files in lessons/1/danish/");
        app.exit(-1);
        return -1;
    }

    QString lang = settings.value("language").toString();
    if (lang != "english" && lang != "danish") {
        QMessageBox langq(QMessageBox::Question, "First question / Første spørgsmål...", "Do you prefer English or Danish?\nYou can change language later from the main menu.\n\nForetrækker du engelsk eller dansk?\nDu kan skife sprog senere fra hovedmenuen.");
        QPushButton *lang_eng = langq.addButton("I prefer English", QMessageBox::YesRole);
        langq.addButton("Jeg foretrækker dansk", QMessageBox::NoRole);

        langq.exec();
        if (langq.clickedButton() == lang_eng) {
            settings.setValue("language", "english");
        }
        else {
            settings.setValue("language", "danish");
        }
    }

    lang = settings.value("language").toString();
    QTranslator translator;
    if (lang == "english") {
        translator.load("texts_en", dataDir.absoluteFilePath("./i18n/"));
    }
    else {
        translator.load("texts_da", dataDir.absoluteFilePath("./i18n/"));
    }
    app.installTranslator(&translator);

    /*
    TaskChooser tc(dataDir, &translator);
    tc.show();
    tc.raise();
    tc.activateWindow();

    QTimer::singleShot(1000, &tc, SLOT(checkFirstRun()));
    //*/

    return app.exec();
}
