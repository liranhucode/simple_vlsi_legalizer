#include "legal.h"

void Legalizer::Run()
{
    int num_movable_cell = db_.GetNumCell() - db_.GetNumFixedCell();
    int num_row = db_.GetNumRow();
    std::vector<Cell> cells;
    cells.reserve(num_movable_cell);

    std::vector<Cell> &db_cells = db_.GetCells();
    for (auto &cell : db_cells)
    {
        if (!cell.is_fixed)
        {
            cells.push_back(cell);
        }
    }
    std::vector<std::vector<SubRow>> &subrow = db_.GetSubRow();

    sort(cells.begin(), cells.end(), [](const Cell &a, const Cell &b) -> bool
         { return a.init_x < b.init_x; });

    int move = 0;

    for (int i = 0; i < num_movable_cell; ++i)
    // for (int i = 0; i < 10; ++i)
    {
        double best_cost = INT_MAX;
        int best_row = -1;
        int best_subrow = -1;

        // TO DO :
        //  Find closest subrow to speeding up.
        int start_row = findClosestRowForCell(cells[i]);
        // up
        for (int rid = start_row; rid < num_row; ++rid)
        {
            for (int j = 0; j < subrow[rid].size(); ++j)
            {
                SubRow tmp = subrow[rid][j];
                if (insertCellToSubRow(subrow[rid][j], cells[i]))
                {
                    placeRow(subrow[rid][j]);
                    double cost = calculateSubRowDisp(subrow[rid][j]);
                    if (cost < best_cost)
                    {
                        best_cost = cost;
                        best_row = rid;
                        best_subrow = j;
                    }
                }
                subrow[rid][j] = tmp;
            }

            if (best_cost != INT_MAX)
            {
                break;
            }
        }
        // down
        for (int rid = start_row - 1; rid >= 0; --rid)
        {
            bool is_smaller = false;
            for (int j = 0; j < subrow[rid].size(); ++j)
            {
                SubRow tmp = subrow[rid][j];
                if (insertCellToSubRow(subrow[rid][j], cells[i]))
                {
                    placeRow(subrow[rid][j]);
                    double cost = calculateSubRowDisp(subrow[rid][j]);
                    if (cost < best_cost)
                    {
                        best_cost = cost;
                        best_row = rid;
                        best_subrow = j;
                    }
                    else
                    {
                        is_smaller = true;
                    }
                }

                subrow[rid][j] = tmp;
                if (is_smaller)
                {
                    break;
                }
            }
            if (is_smaller)
            {
                break;
            }
        }

        // place cell at best row
        if (best_row != -1 && best_subrow != -1)
        {
            if (insertCellToSubRow(subrow[best_row][best_subrow], cells[i]))
            {
                move++;
                placeRow(subrow[best_row][best_subrow]);

                if (legal_option.is_debug)
                {
                    // std::cout << "#" << i << " cell name:" << std::setw(6) << cells[i].name << " ";
                    // std::cout << " init_x:" << std::setw(8) << cells[i].init_x << " init_y:" << std::setw(8) << cells[i].init_y << " -> "
                    //           << " new_x:" << std::setw(8) << cells[i].new_x << " new_y:" << std::setw(8) << cells[i].new_y << std::endl;

                    /*
                    for (auto &cluster : subrow[best_row][best_subrow].clusters)
                    {
                        for (const auto &cell : cluster.cells)
                        {
                            if (legal_option.is_debug)
                            {
                                std::cout << "[PlaceRow] #" << i++ << " cell name:" << std::setw(6) << cell.name << " ";
                                std::cout << " init_x:" << std::setw(8) << cell.init_x + db_.GetOriginX() << " init_y:" << std::setw(8) << cell.init_y + db_.GetOriginY() << " -> "
                                          << " new_x:" << std::setw(8) << cell.new_x + db_.GetOriginX() << " new_y:" << std::setw(8) << cell.new_y + db_.GetOriginY() << std::endl;
                            }
                        }
                    }*/
                }
            }
            else 
            {
                std::cout << "Can not insert it: " << cells[i].name << " id:" << cells[i].id << std::endl;
            }
        }
        else
        {
            std::cout << "Can not find location for cell: " << cells[i].name << " id:" << cells[i].id << std::endl;
            // return ;
        }
    }

}

void Legalizer::placeRow(SubRow &subrow)
{
    int site_width = db_.GetSiteWidth();
    int site_height = db_.GetSiteHeight();
    int origin_x = db_.GetOriginX();
    if (subrow.clusters.empty())
    {
        Cluster new_c;
        if (subrow.trial.init_x < subrow.x_coord * site_width) // left
        {
            new_c.start_x = subrow.x_coord;
        }
        else if (ceil(subrow.trial.init_x / site_width) + subrow.trial.width > subrow.x_coord + subrow.width) // right
        {
            new_c.start_x = subrow.x_coord + subrow.width - subrow.trial.width;
        }
        else
        {
            new_c.start_x = ceil(subrow.trial.init_x / site_width);
        }

        subrow.trial.new_x = new_c.start_x * site_width;
        subrow.trial.new_y = subrow.y_coord * site_height;
        new_c.total_width = subrow.trial.width;
        new_c.total_cost = disp(subrow.trial, subrow.trial.new_x, subrow.trial.new_y);

        new_c.cells.push_back(subrow.trial);
        subrow.clusters.push_back(new_c);
    }
    else
    {
        int last_index = subrow.clusters.size() - 1;
        Cluster &last_c = subrow.clusters[last_index];

        // has overlap with last clsuter, placed in it
        if (ceil(subrow.trial.init_x / site_width) < last_c.start_x + last_c.total_width)
        {
            last_c.cells.push_back(subrow.trial);
            // last_c.total_cost += disp(subrow.trial, last_c.start_x + last_c.total_width , subrow.y_coord);
            last_c.total_width += subrow.trial.width;
            collapse(subrow);
        }
        else
        {
            Cluster new_c;
            if (floor(subrow.trial.init_x / site_width) < subrow.x_coord) // might be removed
            {
                new_c.start_x = subrow.x_coord;
            }
            else if (ceil(subrow.trial.init_x / site_width) + subrow.trial.width > subrow.x_coord + subrow.width) // exceed right boundary
            {
                new_c.start_x = subrow.x_coord + subrow.width - subrow.trial.width;
            }
            else
            {
                new_c.start_x = floor(subrow.trial.init_x / site_width);
            }
            subrow.trial.new_x = new_c.start_x * site_width;
            subrow.trial.new_y = subrow.y_coord * site_height;
            new_c.total_width = subrow.trial.width;
            new_c.total_cost = disp(subrow.trial, subrow.trial.new_x, subrow.trial.new_y);

            new_c.cells.push_back(subrow.trial);
            subrow.clusters.push_back(new_c);
            collapse(subrow);
        }
    }
    updateSubRowCostAndCellLocation(subrow);
}

void Legalizer::collapse(SubRow &subrow)
{
    unsigned last_index = subrow.clusters.size() - 1;
    if (subrow.clusters[last_index].start_x < subrow.x_coord)
    {
        subrow.clusters[last_index].start_x = subrow.x_coord;
    }

    // exceed right boundary, shift to left
    if (subrow.clusters[last_index].start_x + subrow.clusters[last_index].total_width > subrow.x_coord + subrow.width)
    {
        subrow.clusters[last_index].start_x = subrow.x_coord + subrow.width - subrow.clusters[last_index].total_width;
    }

    if (last_index > 0 && subrow.clusters[last_index].start_x < subrow.clusters[last_index - 1].start_x + subrow.clusters[last_index - 1].total_width)
    {
        subrow.clusters[last_index - 1].total_width += subrow.clusters[last_index].total_width;

        for (auto &cell : subrow.clusters[last_index].cells)
        {
            subrow.clusters[last_index - 1].cells.push_back(cell);
        }
        subrow.clusters.pop_back();

        collapse(subrow);
    }
}

double Legalizer::calculateSubRowDisp(const SubRow &subrow)
{
    double cost = 0.0;

    for (const auto &cluster : subrow.clusters)
    {
        for (const auto &cell : cluster.cells)
        {
            cost += disp(cell, cell.new_x, cell.new_y);
        }
        // cost += cluster.total_cost;
    }
    return cost;
}

void Legalizer::updateSubRowCostAndCellLocation(SubRow &subrow)
{
    int site_width = db_.GetSiteWidth();
    int site_height = db_.GetSiteHeight();
    for (auto &cluster : subrow.clusters)
    {
        cluster.total_cost = 0.0;
        int start_x = cluster.start_x;
        for (int i = 0; i < cluster.cells.size(); ++i)
        {

            cluster.cells[i].new_x = start_x * site_width;
            cluster.cells[i].new_y = subrow.y_coord * site_height;
            int id = cluster.cells[i].id;
            db_.GetCells()[id].new_x = start_x * site_width;
            db_.GetCells()[id].new_y = subrow.y_coord * site_height;
            start_x += cluster.cells[i].width;
            cluster.total_cost += disp(cluster.cells[i], cluster.cells[i].new_x, cluster.cells[i].new_y);
        }
    }
}

int Legalizer::findClosestRowForCell(const Cell &cell)
{
    int r = round(cell.init_y / db_.GetSiteHeight());
    r = std::min(r, db_.GetNumRow() - 1);
    if (legal_option.is_debug)
    {
        /*
        std::cout << "cell: " << cell.name << " ";
        std::cout << "coord:" << cell.init_x << " " << cell.init_y << " ";
        std::cout << "nearest row: " << r << std::endl;
        */
    }

    return r;
}

bool Legalizer::insertCellToSubRow(SubRow &subrow, Cell &cell)
{
    if (isFitSubRow(subrow, cell))
    {
        subrow.trial = cell;
        return true;
    }
    return false;
}

bool Legalizer::isFitSubRow(const SubRow &subrow, const Cell &cell)
{
    if (subrow.clusters.empty())
    {
        if (cell.width < subrow.width)
        {
            return true;
        }
    }
    else
    {
        int total_width = 0;
        for (const auto &cluster : subrow.clusters)
        {
            total_width += cluster.total_width;
        }
        if (subrow.width - total_width > cell.width)
        {
            return true;
        }
    }
    return false;
}

double Legalizer::disp(const Cell &cell, double new_x, double new_y)
{
    double delta_x = cell.init_x - new_x;
    double delta_y = cell.init_y - new_y;
    return sqrt(delta_x * delta_x + delta_y * delta_y);
}

bool Legalizer::check_legal_placement()
{
    int site_width = db_.GetSiteWidth();
    int site_height = db_.GetSiteHeight();
    for (int i = 0; i < db_.GetNumRow(); ++i)
    {
        for (auto &subrow : db_.GetSubRow()[i])
        {
            for (auto &cluster : subrow.clusters)
            {
                for (const auto &cell : cluster.cells)
                {
                    if (cell.is_fixed)
                    {
                        continue;
                    }

                    if (fmod(cell.new_x - db_.GetOriginX() * site_width, site_width) || fmod(cell.new_y - db_.GetOriginY() * site_height, site_height))
                    {
                        std::cout << "Cell has illegal location: " << cell.name << " " << cell.new_x + db_.GetOriginX() << " " << cell.new_y + db_.GetOriginY() << std::endl;
                        // return false;
                    }
                }
            }
        }
    }

    return true;

    /*
    for (const auto &cell : db_.GetCells())
    {
        if (cell.is_fixed)
        {
            continue;
        }

        if (fmod(cell.new_x - db_.GetOriginX(), db_.GetSiteWidth()) || fmod(cell.new_y - db_.GetOriginY(), db_.GetSiteHeight()))
        {
            std::cout << "Cell has illegal location: " << cell.name << " " << cell.new_x << " " << cell.new_y << std::endl;
            return false;
        }
    }
    return true;
    */
}

void Legalizer::Report()
{
    int n = 0;
    double total_disp = 0;
    int org_x = db_.GetOriginX();
    int org_y = db_.GetOriginY();
    for (int i = 0; i < db_.GetNumRow(); ++i)
    {
        for (auto &subrow : db_.GetSubRow()[i])
        {
            for (auto &cluster : subrow.clusters)
            {
                for (const auto &cell : cluster.cells)
                {
                    if (legal_option.is_debug)
                    {
                        total_disp += disp(cell, cell.new_x, cell.new_y);
                        std::cout << "[Final] #" << n++ << " cell name:" << std::setw(6) << cell.name << " ";
                        std::cout << " init_x:" << std::setw(8) << cell.init_x + org_x << " init_y:" << std::setw(8) << cell.init_y + org_y << " -> "
                                  << " new_x:" << std::setw(8) << cell.new_x + org_x << " new_y:" << std::setw(8) << cell.new_y + org_y << std::endl;
                    }
                }
            }
        }
    }
    int num_movable_cell = db_.GetNumCell() - db_.GetNumFixedCell();
    std::cout << num_movable_cell << " " << n << std::endl;
    std::cout << "[Result] #"
              << " total displacement :" << std::setw(6) << total_disp << " ";
    std::cout << " avg displacement:" << std::setw(8) << total_disp / num_movable_cell << std::endl;

    if (!check_legal_placement())
    {
        std::cout << "Check placement failed" << std::endl;
    }
    else
    {
        std::cout << "[Check] Placement is legal" << std::endl;
    }

    /*
        const std::vector<Cell> &cells = db_.GetCells();
        for (int i = 0; i < db_.GetNumCell(); ++i)
        {
            if (legal_option.is_debug)
            {
                std::cout << "#" << i << " cell name:" << std::setw(6) << cells[i].name << " ";
                std::cout << " init_x:" << std::setw(8) << cells[i].init_x << " init_y:" << std::setw(8) << cells[i].init_y << " -> "
                          << " new_x:" << std::setw(8) << cells[i].new_x << " new_y:" << std::setw(8) << cells[i].new_y << std::endl;
            }
        }
        */
}

double Legalizer::reportTotalDisp()
{
    double total_disp = 0;
    for (int i = 0; i < db_.GetNumRow(); ++i)
    {
        for (auto &subrow : db_.GetSubRow()[i])
        {
            for (auto &cluster : subrow.clusters)
            {
                for (const auto &cell : cluster.cells)
                {
                    if (legal_option.is_debug)
                    {
                        total_disp += disp(cell, cell.new_x, cell.new_y);
                    }
                }
            }
        }
    }
    return total_disp;
}

void Legalizer::updateFinialLocation()
{
    /*
    for (const auto &subrow_by_rowid : db_.GetSubRow())
    {
        for (const auto &subrow : subrow_by_rowid)
        {
            for (const auto &cluster : subrow.clusters)
            {
                for (const auto &cell : cluster.cells)
                {
                    int id = cell.id;
                    db_.GetCells()[id].new_x *=

                }

            }
        }
    }
    */
}
