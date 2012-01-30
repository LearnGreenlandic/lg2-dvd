#ifndef COMMON_HPP
#define COMMON_HPP

#include <QtGui>
#include <map>

const size_t lg2_revision = 7676;

typedef std::multimap<size_t,QString,std::greater<size_t> > dirmap_t;

QString find_newest(const dirmap_t& dirs, const QString& name);

size_t read_revision(const QString& name);

bool check_files(const dirmap_t& dirs);

#endif // COMMON_HPP
