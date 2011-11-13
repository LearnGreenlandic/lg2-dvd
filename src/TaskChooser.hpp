#ifndef TASKCHOOSER_HPP
#define TASKCHOOSER_HPP

#include "common.hpp"
#include <QtGui>

class TaskChooser : public QWidget {
    Q_OBJECT

public:
    TaskChooser(const dirmap_t& dirs, QTranslator *translator);

public slots:
    void showExercise_1_3_text();
    void showExercise_1_4_text();
    void showExercise_1_5_text();
    void showExercise_1_6_text();
    void showExercise_1_7_text();
    void showExercise_1_8_text();

    void showExercise_2_1_text();
    void showExercise_2_2_text();
    void showExercise_2_3_1_text();
    void showExercise_2_3_2_text();
    void showExercise_2_4_text();
    void showExercise_2_5_text();

    void showExercise_3_1_text();
    void showExercise_3_2_text();
    void showExercise_3_3_text();
    void showExercise_3_4_text();

    void showExercise_4_1_text();
    void showExercise_4_2_text();
    void showExercise_4_3_text();
    void showExercise_4_4_1_text();
    void showExercise_4_4_2_text();
    void showExercise_4_5_text();

    void showExercise_5_1_text();
    void showExercise_5_2_text();
    void showExercise_5_3_text();
    void showExercise_5_4_1_text();
    void showExercise_5_4_2_text();
    void showExercise_5_5_text();
    void showExercise_5_6_text();

    void toggleLanguage();

private:
    void showExercise_qaTextOnly(QString which, QString title);

    const dirmap_t& dirs;
    QTranslator *translator;
};

#endif // TASKCHOOSER_HPP
