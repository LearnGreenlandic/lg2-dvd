#ifndef QADATA_HPP
#define QADATA_HPP

#include "common.hpp"
#include <QtGui>

class QAData {
public:
    QAData(const dirmap_t& dirs, QString which);

    int size() const;
    const QString& getQ(int i) const;
    const QString& getA(int i) const;
    const QString& getStar(int i) const;
    const QString& getQW(int i) const;
    const QString& getAW(int i) const;

private:
    const dirmap_t& dirs;
    QString which;

    QMap<QString,QStringList> data;
    QVector<QString> qas;
};

#endif // QADATA_HPP
