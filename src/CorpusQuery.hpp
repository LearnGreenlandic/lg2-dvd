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
};

#endif // CORPUSQUERY_HPP
