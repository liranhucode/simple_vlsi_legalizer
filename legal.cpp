#include "legal.h"


void Legalizer::run()
{
    int num_movable_cell = db_.GetNumCell() - db_.GetNumFixedCell();

    for (int i = 0; i < num_movable_cell; ++i)
    {

        int best_cost = INT_MAX;
        int best_row = -1;
        int best_subrow = -1;
        
        int start_row = findClosestRowForCell(i);
        
        //up
        for (int rid = start_row; rid < db_.GetNumRow(); ++rid)
        {
            for (int j = 0; j < db_.GetSubRow(rid).size(); ++j)
            {
                SubRow tmp = SubRow[rid][j];
                if (InsertCellToSubRow(i, rid, j))
                {
                    PlaceRow(rid, j);
                    double cost = CalculateDisP(rid, j);
                    if (cost < best_cost)
                    {
                        best_cost = cost;
                        best_row = rid;
                        best_subrow = j;
                    }
                }
                SubRow[rid][j] = tmp;
            }

            if (best_cost != INT_MAX)
            {
                break;
            }
        }
        for (int rid = start_row; rid >= 0; --rid)
        {
            for (int j = 0; j < db_.GetSubRow(rid).size(); ++j)
            {
                SubRow tmp = SubRow[rid][j];
                if (InsertCellToSubRow(i, rid, j))
                {
                    PlaceRow(rid, j);
                    int cost = CalculateDisp(rid, j);
                    if (cost < best_cost)
                    {
                        best_cost = cost;
                        best_row = rid;
                        best_subrow = j;
                    }
                }
                SubRow[rid][j] = tmp;
            }
        }
    }
}

void Legalizer::PlaceRow(int row_id, int subrow_id)
{
    if (subrow_[row_id][subrow_id].cluster.empty())
    {
        Cluster new_c;
        new_c.start_x = subrow_[row_id][subrow_id].x_coord;
        new_c.total_width = subrow_[row_id][subrow_id].trial.width;
    }
    else 
    {
        Cluster last_c = subrow_[row_id][subrow_id].cluster.back();
        if (trial > last_c.start_x + last_c.total_width)
        {
            Cluster new_c;
        }
        else 
        {
            Collapse(row_id, subrow_id);
        }
    }
}

void Legalizer::Collapse(int row_id, int subrow_id)
{

}

void Legalizer::disp(int cell_id, int new_x, int new_y)
{

}

double Legalizer::CalculateDisp(rid, j)
{
}


int Legalizer::findClosestRowForCell(int id)
{
    return db_.FindClosestRowForCell(id);
}

bool Legalizer::InsertCellToSubRow(int cell_id, int row_id, int subrow_id)
{
    //db_.InsertCellToSubRow(cell_id, row_id, subrow_id);
    if (IsFitSubRow(cell_id, row_id, subrow_id))
    {
        subrows_[row_id][subrow_id].trial = cells_[cell_id];
        return true;
    }

    return false;
}

bool Legalizer::IsFitSubRow(int cell_id, int row_id, int subrow_id)
{
}

void Legalizer::check_legal_placement()
{

}

void Legalizer::report()
{



}

