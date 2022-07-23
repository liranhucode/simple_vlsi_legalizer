#pragma once
#include "db.h"

class Legalizer
{
public:
    Legalizer(DB &db) : db_(db) {}

    void run();

    void report();

    void check_legal_placement();

private:
    DB &db_;
};