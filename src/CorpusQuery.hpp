#ifndef CORPUSQUERY_HPP
#define CORPUSQUERY_HPP

#include "TaskChooser.hpp"
#include <QtGui>
#include <set>

class CorpusQuery : public QWidget {
    Q_OBJECT

public:
    CorpusQuery(TaskChooser& tc, QString which, QString title);

public slots:
    void doSearch();
    void insertTag(int i);
    void optCases(int i);
    void optTags(int i);
    void optText(int i);
    void optWhole(int i);
    void optRegex(int i);
    void openMore();
    void showHelp();

private:
    TaskChooser& tc;

    struct Cohort {
        int line, word;
        QString wordform;
        QString reading;

        Cohort(int line=0) : line(line) {
        }
    };

    QList<Cohort> kalc;
    QList<QStringList> kals;
    QStringList dans, engs;
    std::set<QString> tags;

    QLabel *text;
    QComboBox *cbox;
    QLineEdit *input;
    QHash<QString,QCheckBox*> xsx;
    QScrollArea *results;
    QHash<QObject*,int> mapping;
};

#endif // CORPUSQUERY_HPP
