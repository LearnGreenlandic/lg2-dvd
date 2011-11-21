#ifndef UPDOWNDATA_HPP
#define UPDOWNDATA_HPP

#include "common.hpp"
#include <QtGui>

class UpdownData
{
public:
    UpdownData(const dirmap_t& dirs, QString which);

    void loadRandom();
    QString glossUpperDetailed(QString upper);
    QString glossUpperShort(QString upper);

    QVector< QPair<QString,QString> > updowns;
    QMap<QString,QString> glosses;
    QSet<QString> used;

private:
    const dirmap_t& dirs;
    QString which;
};

#endif // UPDOWNDATA_HPP
