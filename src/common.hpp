#ifndef COMMON_HPP
#define COMMON_HPP

#include <QtGui>
#include <map>

const int revision = 7676;

typedef std::multimap<size_t,QString,std::greater<size_t> > dirmap_t;

QString find_newest(const dirmap_t& dirs, const QString& name);

size_t read_revision(const QString& name);

#endif // COMMON_HPP
