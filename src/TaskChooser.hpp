#ifndef TASKCHOOSER_HPP
#define TASKCHOOSER_HPP

#include "common.hpp"
#include <QtGui>

class TaskChooser : public QScrollArea {
    Q_OBJECT

public:
    TaskChooser(const dirmap_t& dirs, QTranslator *translator);

    const dirmap_t& dirs;

public slots:
    void toggleLanguage();
    void showLecture();
    void showLecturePDF();
    void showSlidesPDF();
    void showFillout11();
    void showListenRepeat();
    void showListenRepeatAudio();
    void showExercise_qaTextOnly();
    void showExercise_qaAudio();
    void showFST_Reception();
    void showFST_DownFromGloss();
    void showFST_DownFromTranslate();

private:
    QHash<QObject*,QPair<QString,QString> > mappings;
    QTranslator *translator;
};

#endif // TASKCHOOSER_HPP
