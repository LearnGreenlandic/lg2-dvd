#ifndef TASKCHOOSER_HPP
#define TASKCHOOSER_HPP

#include "common.hpp"
#include <QtGui>

class TaskChooser : public QWidget {
    Q_OBJECT

public:
    TaskChooser(const dirmap_t& dirs, QTranslator *translator);
    void showNext(const QString& title);

    const dirmap_t& dirs;

public slots:
    void toggleLanguage();
    void switchSection();
    void showFirstLecture();
    void showLecture();
    void showLecturePDF();
    void showSlidesPDF();
    void showFillout11();
    void showFillout12();
    void showFillout81();
    void showCorpusQuery();
    void showListenRepeat();
    void showListenRepeatAudio();
    void showExercise_qaTextOnly();
    void showExercise_qaAudio();
    void showFST_Reception();
    void showFST_DownFromGloss();
    void showFST_DownFromTranslate();

private:
    QHash<QObject*,QPair<QString,QString> > mappings;
    QList<QPair<QString,QPushButton*> > buttons;
    QHash<QPushButton*,QObject*> button_sect;
    QList<QObject*> section_list;
    QList<QString> section_texts;
    QStackedLayout *stack;
    QTranslator *translator;
};

#endif // TASKCHOOSER_HPP
