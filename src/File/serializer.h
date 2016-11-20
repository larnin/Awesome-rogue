#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <vector>

void load(const std::string & filename);
void save(const std::string & filename);

const std::string saveDir("res/save/");

std::vector<std::string> filesIn(const std::string & dir, const std::string & extension);
std::vector<std::string> saveList();

std::string filenameFromDir(std::string dir);

#endif // SERIALIZER_H
