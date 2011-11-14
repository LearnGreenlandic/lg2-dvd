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
    void showLecture_1_1();
    void showLecture_1_2();
    void showExercise_1_3_text();
    void showExercise_1_4_text();
    void showExercise_1_5_text();
    void showExercise_1_6_text();
    void showExercise_1_7_text();
    void showExercise_1_8_text();

    void showLecture_2_1();
    void showLecture_2_2();
    void showExercise_2_1_text();
    void showExercise_2_2_text();
    void showExercise_2_3_1_text();
    void showExercise_2_3_2_text();
    void showExercise_2_4_text();
    void showExercise_2_5_text();

    void showLecture_3_1();
    void showLecture_3_2();
    void showExercise_3_1_text();
    void showExercise_3_2_text();
    void showExercise_3_3_text();
    void showExercise_3_4_text();

    void showLecture_4_1();
    void showLecture_4_2();
    void showExercise_4_1_text();
    void showExercise_4_2_text();
    void showExercise_4_3_text();
    void showExercise_4_4_1_text();
    void showExercise_4_4_2_text();
    void showExercise_4_5_text();

    void showLecture_5_1();
    void showLecture_5_2();
    void showExercise_5_1_text();
    void showExercise_5_2_text();
    void showExercise_5_3_text();
    void showExercise_5_4_1_text();
    void showExercise_5_4_2_text();
    void showExercise_5_5_text();
    void showExercise_5_6_text();

    void showLecture_6_1();
    void showLecture_6_2();
    void showExercise_6_1_text();
    void showExercise_6_2_text();
    void showExercise_6_3_1_text();
    void showExercise_6_3_2_text();
    void showExercise_6_4_1_text();
    void showExercise_6_4_2_text();
    void showExercise_6_5_text();
    void showExercise_6_6_text();

    void showLecture_7_1();
    void showLecture_7_2();
    void showExercise_7_1_text();
    void showExercise_7_2_text();
    void showExercise_7_3_1_text();
    void showExercise_7_3_2_text();
    void showExercise_7_4_text();
    void showExercise_7_5_text();

    void showLecture_8_1();
    void showLecture_8_2();
    void showExercise_8_2_1_text();
    void showExercise_8_2_2_text();
    void showExercise_8_3_1_text();
    void showExercise_8_3_2_text();

    void showLecture_9_1();
    void showLecture_9_2();

    void toggleLanguage();

private:
    void showLecture(QString which, QString title);
    void showExercise_qaTextOnly(QString which, QString title);

    QTranslator *translator;
};

#endif // TASKCHOOSER_HPP
