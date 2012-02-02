#include "TaskChooser.hpp"
#include "common.hpp"
#include <QtGui>
#include <ctime>
#include <cstdlib>
#include <stdint.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

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

    if (settings.contains("paths")) {
        int z = settings.beginReadArray("paths");
        for (int i=0 ; i<z ; ++i) {
            settings.setArrayIndex(i);
            size_t rev = settings.value("revision").toUInt();
            QString path = settings.value("path").toString();
            dirs.insert(std::make_pair(rev,path));
        }
        settings.endArray();
    }

    if (dirs.empty() || dirs.begin()->first < lg2_revision || !check_files(dirs)) {
        QDir tDir(app.applicationDirPath());
        do {
            size_t rev = 0;
            if (tDir.exists("./lessons2/revision.txt") && (rev = read_revision(tDir.absoluteFilePath("./lessons2/revision.txt")))) {
                dirs.insert(std::make_pair(rev, tDir.absoluteFilePath("lessons2")));
            }
            if (tDir.exists("./demo2/lessons2/revision.txt") && (rev = read_revision(tDir.absoluteFilePath("./demo2/lessons2/revision.txt")))) {
                dirs.insert(std::make_pair(rev, tDir.absoluteFilePath("demo2/lessons2")));
            }
        } while (tDir.cdUp());

        {
            QProgressDialog progress("Checking all drives for LG2 data...", "", 0, 26);
            progress.setWindowModality(Qt::WindowModal);
            progress.setCancelButton(0);
            progress.show();
            QFileInfoList drives = QDir::drives();
            drives.append(QDir("/mnt/").entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot));
            drives.append(QDir("/media/").entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot));
            drives.append(QDir("/Volumes/").entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot));
            progress.setMaximum(drives.size());

            foreach (QFileInfo drive, drives) {
                size_t rev = 0;
                progress.setLabelText(QString("Trying to read ") + drive.absoluteDir().absoluteFilePath("./lessons2/revision.txt") + " ...");
                if (drive.absoluteDir().exists("./lessons2/revision.txt") && (rev = read_revision(drive.absoluteDir().absoluteFilePath("./lessons2/revision.txt")))) {
                    dirs.insert(std::make_pair(rev, drive.absoluteDir().absoluteFilePath("lessons2")));
                }
                progress.setValue(progress.value()+1);
            }
            progress.setValue(drives.size());
        }

        if (dirs.empty() || find_newest(dirs, "./revision.txt").isEmpty() || !check_files(dirs)) {
            QMessageBox::information(0, "Missing Data / Manglende Data!",
                                     "English: Could not find a suitable lessons2 folder. Maybe you forgot to insert the DVD or mount a network share? You will now be asked to find the revisions.txt file from the lessons2 folder.\n\n"
                                     "Dansk: Kunne ikke finde en passende lessons2 mappe. Måske har du glemt at indsætte DVD'en eller forbinde til netværket? Du vil nu blive bedt om at finde revision.txt fra lessons2 mappen.");
            QFileInfo revfile = QFileDialog::getOpenFileName(0, "Find lessons2/revision.txt", QString(), "revision.txt (*.txt)");
            size_t rev = read_revision(revfile.absoluteFilePath());
            dirs.insert(std::make_pair(rev, revfile.absoluteDir().absolutePath()));

            if (dirs.empty() || find_newest(dirs, "./revision.txt").isEmpty() || !check_files(dirs)) {
                QMessageBox::critical(0, "Missing Data / Manglende Data!",
                                         "English: Could not find a suitable lessons2 folder. Maybe you forgot to insert the DVD or mount a network share?\n\n"
                                         "Dansk: Kunne ikke finde en passende lessons2 mappe. Måske har du glemt at indsætte DVD'en eller forbinde til netværket?");
                app.exit(-1);
                return -1;
            }
        }
    }

    if (settings.value("revision", uint(0)).toUInt() > lg2_revision) {
        QMessageBox runoldq(QMessageBox::Warning,
                          "Old version / Gammel version",
                          "English: You have previously run a newer version of LG2 on this machine.\n"
                          "Are you sure you want to run this older version instead?\n\n"
                          "Dansk: Du har tidligere brugt en nyere version af LG2 på denne maskine.\n"
                          "Er du sikker på at du vil køre denne ældre version i stedet?"
                          );
        QPushButton *yes = runoldq.addButton("Run old / Kør gammel", QMessageBox::YesRole);
        runoldq.addButton("Exit / Afslut", QMessageBox::NoRole);

        runoldq.exec();
        if (runoldq.clickedButton() != yes) {
            app.quit();
            return 0;
        }
    }
    else {
        settings.setValue("revision", lg2_revision);
    }

    if (!dirs.empty()) {
        int z = dirs.size(), i = 0;
        settings.beginWriteArray("paths", z);
        for (dirmap_t::const_iterator it = dirs.begin() ; it != dirs.end() ; ++it, ++i) {
            settings.setArrayIndex(i);
            settings.setValue("revision", uint(it->first));
            settings.setValue("path", it->second);
        }
        settings.endArray();
    }

    QTranslator xtl;
    xtl.load(find_newest(dirs, "i18n/texts_da.qm"));
    app.installTranslator(&xtl);

    QString lang = settings.value("language").toString();
    if (lang != "english" && lang != "danish") {
        QMessageBox langq(QMessageBox::Question,
                          "English / Dansk...",
                          "Do you prefer English or Danish?\n"
                          "You can change language later from the main menu.\n\n"
                          "Foretrækker du engelsk eller dansk?\n"
                          "Du kan skife sprog senere fra hovedmenuen.");
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

    TaskChooser *tc = new TaskChooser(dirs, &translator);
    tc->show();
    tc->raise();
    tc->activateWindow();

    QTimer::singleShot(1000, tc, SLOT(checkFirstRun()));

    return app.exec();
}
