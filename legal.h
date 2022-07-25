#pragma once
#include "db.h"


class Legalizer
{
public:
    Legalizer(DB &db) : db_(db)
    {
        db_.Init();
    }

    void Run();
    void Report();

private:
    int findClosestRowForCell(const Cell &cell);
    bool insertCellToSubRow(SubRow &subrow, const Cell &cell);
    void placeRow(SubRow &subrow);
    void collapse(SubRow &subrow);
    bool isFitSubRow(const SubRow &subrow, const Cell &cell);
    double calculateSubRowDisp(const SubRow &subrow);
    void updateSubRowCellLocation(SubRow &subrow);
    double disp(const Cell &cell, int new_x, int new_y);
    bool check_legal_placement();

private:
    DB &db_;
};