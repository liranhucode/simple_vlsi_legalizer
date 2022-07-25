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
    bool insertCellToSubRow(SubRow &subrow, Cell &cell);
    void placeRow(SubRow &subrow);
    void collapse(SubRow &subrow);
    bool isFitSubRow(const SubRow &subrow, const Cell &cell);
    double calculateSubRowDisp(const SubRow &subrow);
    void updateSubRowCostAndCellLocation(SubRow &subrow);
    void updateFinialLocation();
    double disp(const Cell &cell, double new_x, double new_y);
    bool check_legal_placement();

private:
    DB &db_;
};