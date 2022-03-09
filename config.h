#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

#include "globals.h"

struct Config
{
public:

    static const unsigned_t winWidth;
    static const unsigned_t winHeight;
    static const QString winTitle;
    static const unsigned_t gridWidth;
    static const unsigned_t gridHeight;
};

#endif // CONFIG_H
