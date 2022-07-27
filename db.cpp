#include "db.h"


void DB::Init()
{
    Timer timer("Init DB");
    InitFixedCellToRow();
    CreateSubRow();
    timer.Report();
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
            int end_y = start_y + cell.height;
            end_y = std::min(end_y, num_row_);
            if (legal_option.is_debug)
            {
                std::cout << "[Fixed] cell: " << std::setw(6) << cell.name << " ";
                std::cout << "coord:" <<  std::setw(8) << cell.init_x << " " <<std::setw(8) << cell.init_y ;
                std::cout  << " width: " << std::setw(6) << cell.width << " height: " << std::setw(6) << cell.height << " ";
                std::cout << "assinged to row: " << std::setw(6) <<  start_y << " -> " << std::setw(6) << end_y << std::endl;
            }
            for (int r = start_y; r < end_y; ++r) 
            {
                rows_[r].fixed_cells.emplace_back(cell);
            }
        }
    }
    assert(n == num_fixed_cell_);
}

void DB::CreateSubRow()
{
    //std::vector<vector<SubRow>> subrows;
    subrows_.resize(num_row_);

    for (int i = 0; i < num_row_; i++)
    {
        if (rows_[i].fixed_cells.empty())
        {
            SubRow row;
            row.x_coord = rows_[i].x_coord;
            row.y_coord = rows_[i].y_coord;
            row.width = rows_[i].width;
            subrows_[i].push_back(row);
        } 
        else 
        {
            SortFixedCell(i);

            int start_x = rows_[i].x_coord;
            for (int j = 0; j < rows_[i].fixed_cells.size(); ++j)
            {
                if (rows_[i].fixed_cells[j].init_x == start_x * site_width_) 
                {
                    start_x = ceil(rows_[i].fixed_cells[j].init_x / site_width_) + rows_[i].fixed_cells[j].width;
                }
                else 
                {
                    SubRow row;
                    row.x_coord = start_x;
                    row.y_coord = rows_[i].y_coord;
                    row.width = floor(rows_[i].fixed_cells[j].init_x / site_width_) - start_x;
                    subrows_[i].push_back(row);
                }
            }

            unsigned last = rows_[i].fixed_cells.size() - 1;
            if (ceil(rows_[i].fixed_cells[last].init_x / site_width_) + rows_[i].fixed_cells[last].width < rows_[i].x_coord + rows_[i].width)
            {
                SubRow row;
                row.x_coord = ceil(rows_[i].fixed_cells[last].init_x / site_width_) + rows_[i].fixed_cells[last].width;
                row.y_coord = rows_[i].y_coord;
                row.width =  rows_[i].x_coord + rows_[i].width - row.x_coord;
                subrows_[i].push_back(row);
            }
        }
    }
}

void DB::SortFixedCell(int rid)
{

    if (rows_[rid].fixed_cells.size() == 1)
    {
        return ;
    }

    sort(rows_[rid].fixed_cells.begin(),rows_[rid].fixed_cells.end(), [] (const Cell &a, const Cell &b) -> bool {
        return a.init_x < b.init_x;
    });
}