#include "db.h"


void DB::Init()
{
    InitFixedCellToRow();
    CreateSubRow();
}

void DB::CreateSubRow()
{

}

void DB::InitFixedCellToRow()
{
    int n = 0; 
    for (const auto &cell : cells_)
    {
        if (cell.is_fixed)
        {
            n++;
            int start_y = floor(cell.init_y / site_height_);
            int end_y = ceil((cell.init_y + cell.height) / site_height_);
            end_y = std::min(end_y, num_row_);
            if (legal_option.is_debug)
            {
                std::cout << "Fixed cell: " << cell.name << " ";
                std::cout << "coord:" << cell.init_x << " " << cell.init_y << " width: " << cell.width << " height: " << cell.height << " ";
                std::cout << "assinged to row: " << start_y << " -> " << end_y << std::endl;
            }
            for (int r = start_y; r < end_y; ++r) 
            {
                rows_[r].fixed_cells.emplace_back(cell);
            }
            
        }
    }
    assert(n == num_fixed_cell_);
}