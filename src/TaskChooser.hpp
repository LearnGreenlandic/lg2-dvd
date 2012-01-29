#ifndef TASKCHOOSER_HPP
#define TASKCHOOSER_HPP

#include "common.hpp"
#include "ClickLabel.hpp"
#include <QtGui>

class TaskChooser : public QWidget {
    Q_OBJECT

public:
    TaskChooser(const dirmap_t& dirs, QTranslator *translator);

    const dirmap_t& dirs;

public slots:
    void toggleLanguage();
    void switchSection();
    void showLecture();
    void showLecturePDF();
    void showSlidesPDF();
    void showFillout11();
    void showFillout12();
    void showFillout81();
    void showListenRepeat();
    void showListenRepeatAudio();
    void showExercise_qaTextOnly();
    void showExercise_qaAudio();
    void showFST_Reception();
    void showFST_DownFromGloss();
    void showFST_DownFromTranslate();

private:
    QHash<QObject*,QPair<QString,QString> > mappings;
    QList<QObject*> section_list;
    QList<QString> section_texts;
    QStackedLayout *stack;
    QTranslator *translator;
};

#endif // TASKCHOOSER_HPP
