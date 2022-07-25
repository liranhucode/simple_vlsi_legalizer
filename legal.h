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
    int findClosestRowForCellInsertCellToSubRow(int cell_id);
    bool InsertCellToSubRow(int cell_id, int row_id, int subrow_id);
    bool isFitSubRow(int cell_id, int row_id, int subrow_id);
    void PlaceRow(int row_id, int subrow_id);
    void Clapse(int row_id, int subrow_id);
    double CalculateDisp(int rid, int j);
    void disp(int cell_id, int new_x, int new_y);

private:
    DB &db_;
};