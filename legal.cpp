#include "legal.h"


void Legalizer::run()
{
    AssignFixedCellToCloestRow();
}

void Legalizer::check_legal_placement()
{

}

void Legalizer::report()
{



}

void Legalizer::AssignFixedCellToCloestRow()
{
    int n = 0; 
    for (const auto &cell : db_.GetCells())
    {
        if (cell.is_fixed)
        {
            n++;
            //assert(fmod(cell.init_y, db_.GetSiteHeight()) != 0);
            int r = floor(cell.init_y / db_.GetSiteHeight());
            db_.GetRows()[r].fixed_cells.emplace_back(cell);
        }
    }
    assert(n == db_.GetNumFixedCell());
}
