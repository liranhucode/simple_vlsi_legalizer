#pragma once
#include "db.h"

class Legalizer
{
public:
    Legalizer(DB &db) : db_(db) {}

    void run();

    void report();

    void check_legal_placement();

    void AssignFixedCellToCloestRow();
private:
    DB &db_;
};