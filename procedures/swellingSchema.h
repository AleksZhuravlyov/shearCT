#ifndef SWELLINGSCHEMA_H
#define SWELLINGSCHEMA_H

#include <string>
#include <map>
#include <variant>
#include <vector>

std::map<std::string,
        std::variant<double, std::vector<double>>> swellingSchema(
        std::map<std::string,
                std::variant<bool, int, double, std::string, std::vector<double>>> params);

#endif // SWELLINGSCHEMA_H
