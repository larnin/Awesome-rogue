#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <vector>

void load(const std::string & filename);
void save(const std::string & filename);

std::vector<std::string> filesIn(const std::string & dir, const std::string & extension);

#endif // SERIALIZER_H
