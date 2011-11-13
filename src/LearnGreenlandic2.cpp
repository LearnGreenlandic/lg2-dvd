#include "TaskChooser.hpp"
#include "common.hpp"
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
        QMessageBox::critical(0, "Settings Not Writable!", "Unable to store user-specific settings. Maybe you don't have any rights on this machine?");
        app.exit(-1);
        return -1;
    }


    uint32_t seed = static_cast<uint32_t>(time(0)) ^ app.applicationPid();
    qsrand(seed);
    srand(seed);

    dirmap_t dirs;

    QDir tDir(app.applicationDirPath());
    do {
        size_t rev = 0;
        if (tDir.exists("./lessons2/revision.txt") && (rev = read_revision(tDir.absoluteFilePath("./lessons2/revision.txt")))) {
            dirs.insert(std::make_pair(rev, tDir.absoluteFilePath("lessons2")));
        }
    } while (tDir.cdUp());

    QFileInfoList drives = QDir::drives();
    foreach (QFileInfo drive, drives) {
        size_t rev = 0;
        if (drive.absoluteDir().exists("./lessons2/revision.txt") && (rev = read_revision(drive.absoluteDir().absoluteFilePath("./lessons2/revision.txt")))) {
            dirs.insert(std::make_pair(rev, drive.absoluteDir().absoluteFilePath("lessons2")));
        }
    }
    if (dirs.empty() || find_newest(dirs, "./revision.txt").isEmpty()) {
        QMessageBox::critical(0, "Missing Data!", "Could not find a suitable lessons2 folder. Maybe you forgot to insert the DVD?");
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
        translator.load(find_newest(dirs, "i18n/texts_en.qm"));
    }
    else {
        translator.load(find_newest(dirs, "i18n/texts_da.qm"));
    }
    app.installTranslator(&translator);

    TaskChooser tc(dirs, &translator);
    tc.show();
    tc.raise();
    tc.activateWindow();

    /*
    QTimer::singleShot(1000, &tc, SLOT(checkFirstRun()));
    //*/

    return app.exec();
}
