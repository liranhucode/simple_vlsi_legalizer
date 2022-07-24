#pragma once
#include "db.h"


class Legalizer
{
public:
    Legalizer(DB &db) : db_(db)
    {
        db_.Init();
    }

    void run();

    void report();

    void check_legal_placement();
    
    int findClosestRowForCell(int cell_id);

private:
    DB &db_;
};