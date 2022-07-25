#include "legal.h"

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

void Legalizer::Run()
{
    int num_movable_cell = db_.GetNumCell() - db_.GetNumFixedCell();

    std::vector<Cell> &cells = db_.GetCells();
    for (int i = 0; i < num_movable_cell; ++i)
    {
        double best_cost = INT_MAX;
        int best_row = -1;
        int best_subrow = -1;

        //TO DO : 
        // Find closest subrow to speeding up.
        int start_row = findClosestRowForCell(cells[i]);
        //up
        for (int rid = start_row; rid < db_.GetNumRow(); ++rid)
        {
            std::vector<SubRow> &subrow = db_.GetSubRow(rid);
            for (int j = 0; j < subrow.size(); ++j)
            {
                SubRow tmp = subrow[j];
                if (insertCellToSubRow(subrow[j], cells[i]))
                {
                    placeRow(subrow[j]);
                    double cost = calculateSubRowDisp(subrow[j]);
                    if (cost < best_cost)
                    {
                        best_cost = cost;
                        best_row = rid;
                        best_subrow = j;
                    }
                }
                subrow[j] = tmp;
            }

            if (best_cost != INT_MAX)
            {
                break;
            }
        }
        //down 
        for (int rid = start_row - 1; rid >= 0; --rid)
        {
            std::vector<SubRow> &subrow = db_.GetSubRow(rid);
            int is_smaller = 0;
            for (int j = 0; j < subrow.size(); ++j)
            {
                SubRow tmp = subrow[j];
                if (insertCellToSubRow(subrow[j], cells[i]))
                {
                    placeRow(subrow[j]);
                    double cost = calculateSubRowDisp(subrow[j]);
                    if (cost < best_cost)
                    {
                        best_cost = cost;
                        best_row = rid;
                        best_subrow = j;
                    } 
                    else
                    {
                        is_smaller++;
                    } 
                }
                subrow[j] = tmp;
                if (is_smaller > 33)
                {
                    break;
                }
            }
            if (is_smaller > 33)
            {
                break;
            }
        }
        if (best_row != -1 && best_subrow != -1)
        {
            if (insertCellToSubRow(db_.GetSubRow()[best_row][best_subrow], cells[i]))
            {
                placeRow(db_.GetSubRow()[best_row][best_subrow]);
            }
        }
        else
        {
            std::cout << "Can not find location for cell: " << cells[i].name << std::endl;
            //return ;
        }
    }

    if (!check_legal_placement())
    {
        std::cout << "Check placement failed" << std::endl;
        return ;
    }
}

void Legalizer::placeRow(SubRow &subrow)
{
    if (subrow.clusters.empty())
    {
        Cluster new_c;
        if (subrow.trial.init_x < subrow.x_coord)
        {
            new_c.start_x = subrow.x_coord;
        }
        else if (subrow.trial.init_x + subrow.trial.width   > subrow.x_coord + subrow.width )
        {
            new_c.start_x = subrow.x_coord + subrow.width  - subrow.trial.width ;
        }
        else
        {
            new_c.start_x = subrow.trial.init_x;
        }

        subrow.trial.new_x = new_c.start_x;
        subrow.trial.new_y = subrow.y_coord;
        new_c.total_width = subrow.trial.width;
        new_c.total_cost = disp(subrow.trial, subrow.trial.new_x, subrow.trial.new_y);

        new_c.cells.push_back(subrow.trial);
        subrow.clusters.push_back(new_c);
    }
    else 
    {
        int last_index = subrow.clusters.size() - 1;
        Cluster &last_c = subrow.clusters[last_index];
        //has overlap with last clsuter
        if (subrow.trial.new_x < last_c.start_x + last_c.total_width)
        {
            last_c.cells.push_back(subrow.trial);
            last_c.total_cost += disp(subrow.trial, last_c.start_x + last_c.total_width , subrow.y_coord);
            last_c.total_width += subrow.trial.width;
            collapse(subrow);
        }
        else
        {
            Cluster new_c;
            if (subrow.trial.init_x < subrow.x_coord)
            {
                new_c.start_x = subrow.x_coord;
            }
            else if (subrow.trial.init_x + subrow.trial.width > subrow.x_coord + subrow.width)
            {
                new_c.start_x = subrow.x_coord + subrow.width - subrow.trial.width ;
            }
            else 
            {
                new_c.start_x = subrow.trial.init_x;
            }
            subrow.trial.new_x = new_c.start_x;
            subrow.trial.new_y = subrow.y_coord;
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

    if (subrow.clusters[last_index].start_x + subrow.clusters[last_index].total_width > subrow.x_coord + subrow.width)
    {
        subrow.clusters[last_index].start_x = subrow.x_coord + subrow.width - subrow.clusters[last_index].total_width;
    }

    if (last_index > 0 && subrow.clusters[last_index].start_x < subrow.clusters[last_index-1].start_x + subrow.clusters[last_index-1].total_width)
    {
        subrow.clusters[last_index-1].total_width += subrow.clusters[last_index].total_width;

        for (auto &cell : subrow.clusters[last_index].cells)
        {
            subrow.clusters[last_index-1].cells.push_back(cell);
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
        //cost += cluster.total_cost;
    }
    return cost;
}

void Legalizer::updateSubRowCostAndCellLocation(SubRow &subrow)
{
    for (auto &cluster : subrow.clusters)
    {
        cluster.total_cost = 0.0;
        int start_x = cluster.start_x;
        for (int i = 0; i < cluster.cells.size(); ++i)
        {

            cluster.cells[i].new_x = start_x;
            cluster.cells[i].new_y = subrow.y_coord;
            int id = cluster.cells[i].id;
            db_.GetCells()[id].new_x = start_x;
            db_.GetCells()[id].new_y = subrow.y_coord;
            start_x += cluster.cells[i].width;
            cluster.total_cost += disp(cluster.cells[i], start_x, subrow.y_coord);
        }
    }
}

int Legalizer::findClosestRowForCell(const Cell &cell)
{
    double d = cell.init_y - db_.GetRows()[0].y_coord;
    int r = round(d / db_.GetSiteHeight());
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
    if (subrow.clusters.empty()) {
        if (cell.width < subrow.width) {
            return true;
        }
    }
    else 
    {
        int total_width = 0;
        for (const auto & cluster : subrow.clusters)
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
    return sqrt(delta_x * delta_x + delta_y + delta_y);
}

bool Legalizer::check_legal_placement()
{
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
}

void Legalizer::Report()
{
    const std::vector<Cell> &cells = db_.GetCells();
    for (int i = 0; i < db_.GetNumCell(); ++i)
    {
        if (legal_option.is_debug)
        {
            std::cout << "cell id: " << cells[i].id << " name:" << cells[i].name << " ";
            std::cout << " init x:" << cells[i].init_x << " y:" << cells[i].init_y << " -> "
                      << " new x:" << cells[i].new_x << " y:" << cells[i].new_y << std::endl;
        }
    }
}

