#include "legal.h"


void Legalizer::run()
{
    int num_movable_cell = db_.GetNumCell() - db_.GetNumFixedCell();

    for (int i = 0; i < num_movable_cell; ++i)
    {

        int best_cost = INT_MAX;
        int best_row = -1;
        
        int start_row = findClosestRowForCell(i);
        
        //up
        for (int rid = start_row; rid < db_.GetNumRow(); ++rid)
        {

            for (int j = 0; j < db_.GetSubRow(rid).size(); ++j)
            {

            }


        }
        for (int down = start_row; down < db_.GetNumRow(); ++down)
        {



        }
    }
}


int Legalizer::findClosestRowForCell(int id)
{
    return db_.FindClosestRowForCell(id);
}

void Legalizer::check_legal_placement()
{

}

void Legalizer::report()
{



}

