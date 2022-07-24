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
            subrows_[i].push_back(row);
        } 
        else 
        {
            SortFixedCell(i);

            int start_x = rows_[i].x_coord;
            for (int j = 0; j < rows_[i].fixed_cells.size(); ++j)
            {
                if (rows_[i].fixed_cells[j].init_x == start_x) 
                {
                    start_x = rows_[i].fixed_cells[j].init_x + rows_[i].fixed_cells[j].width;
                }
                else 
                {
                    SubRow row;
                    row.x_coord = start_x;
                    row.y_coord = rows_[i].y_coord;
                    row.width = rows_[i].y_coord - start_x;
                    subrows_[i].push_back(row);
                }
            }

            unsigned last = rows_[i].fixed_cells.size() - 1;
            if (rows_[i].fixed_cells[last].init_x + rows_[i].fixed_cells[last].width < rows_[i].x_coord + rows_[i].width)
            {
                SubRow row;
                row.x_coord = rows_[i].fixed_cells[last].init_x + rows_[i].fixed_cells[last].width;
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

int DB::FindClosestRowForCell(int id)
{
    int r = round(cells_[id].init_y / site_height_);
    if (legal_option.is_debug)
    {
        std::cout << "cell: " << cells_[id].name << " ";
        std::cout << "coord:" << cells_[id].init_x << " " << cells_[id].init_y << " ";
        std::cout << "nearest row: " << r << std::endl;
    }

    return r;
}