#ifndef COMMON_HPP
#define COMMON_HPP

#include <QtGui>
#include <utility>
#include <set>

const size_t lg2_revision = 7942;

typedef std::pair<size_t,QString> dirmap_e;
typedef std::set<dirmap_e,std::greater<dirmap_e> > dirmap_t;

QString find_newest(const dirmap_t& dirs, const QString& name);

size_t read_revision(const QString& name);

bool check_files(const dirmap_t& dirs);

#endif // COMMON_HPP
