#pragma once
#include "db.h"
#include "timer.h"


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

private:
    DB &db_;
};