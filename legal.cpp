#include "legal.h"


void Legalizer::run()
{
    int num_movable_cell = db_.GetNumCell() - db_.GetNumFixedCell();

    std::vector<Cell> &cells = db_.GetCells();
    for (int i = 0; i < num_movable_cell; ++i)
    {

        int best_cost = INT_MAX;
        int best_row = -1;
        int best_subrow = -1;

        int start_row = findClosestRowForCell(i);

        //up
        for (int rid = start_row; rid < db_.GetNumRow(); ++rid)
        {
            std::vector<SubRow> &subrow = db_.GetSubRow(rid);
            for (int j = 0; j < subrow.size(); ++j)
            {
                SubRow tmp = subrow[j];
                if (InsertCellToSubRow(subrow[j], cells[i]))
                {
                    placeRow(subrow[j]);
                    double cost = CalculateDisP(subrow[j]);
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
        for (int rid = start_row; rid >= 0; --rid)
        {
            for (int j = 0; j < subrow.size(); ++j)
            {
                SubRow tmp = subrow[j];
                if (InsertCellToSubRow(i, rid, j))
                {
                    placeRow(subrow[j]);
                    double cost = CalculateDisP(subrow[j]);
                    if (cost < best_cost)
                    {
                        best_cost = cost;
                        best_row = rid;
                        best_subrow = j;
                    }
                }
                subrow[j] = tmp;
            }
        }
    }
}

void Legalizer::placeRow(SubRow &subrow, Cell &cell)
{
    if (subrow.cluster.empty())
    {
        Cluster new_c;
        if (subrow.trial.init_x < subrow.x_coord)
        {
            new_c.start_x = subrow.x_coord;
        }
        else if (subrow.trial.init_x + subrow.trial.width > subrow.x_coord + subrow.width)
        {
            new_c.start_x = subrow.x_coord + subrow.width - subrow.trial.width;
        }
        else
        {
            new_c.start_x = subrow.trial.init_x;
        }

        subrow.trial.new_x = new_c.start_x;
        subrow.trial.new_y = subrow.y_coord;
        new_c.total_width = subrow.trial.width;
        new_c.total_cost = disp(subrow.trial, subrow.trail.new_x, subrow.trail.new_y);

        new_c.clusters.push_back(subrow.trial);
        subrow.clusters.push_back(new_c);
        return ;
    }
    else 
    {
        int last_index = subrow.size() - 1;
        Cluster &last_c = subrow[last_index];
        //has overlap with last clsuter
        if (subrow.trial.new_x < last_c.start_x + last_c.total_width)
        {
            last_c.push_back(subrow.trial);
            last_c.total_cost += disp(subrow.trial, last_c.start_x + last_c.total_width, subrow.y_coord);
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
                new_c.start_x = subrow.x_coord + subrow.width - subrow.trial.width;
            }
            else 
            {
                new_c.start_x = subrow.trial.init_x;
            }
            subrow.trial.new_x = new_c.start_x;
            subrow.trial.new_y = subrow.y_coord;
            new_c.total_width = subrow.trial.width;
            new_c.total_cost = disp(subrow.trial, subrow.trail.new_x, subrow.trail.new_y);

            new_c.clusters.push_back(subrow.trial);
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
        subrow.clusters[last_index].start_x = subrow.x_coord + subrow.width - subrow.clusters[last_index].width;
    }

    if (last_index > 0 && subrow.cluster[last_index].start_x < subrow.clusters[last_index-1].start_x + subrow.clusters[last_index-1].width)
    {
        subrow.clusters[last_index-1].total_width += subrow.clsuters[last_index].width;

        for (auto &cell : subrow.clsuters[last_index].cells)
        {
            subrow.clsuter[last_index-1].cells.push_back(cell);
        }
        subrow.cluster.pop_back();

        collapse(subrow);
    }
}

double Legalizer::calculateSubRowDisp(SubRow &subrow)
{
    double cost = 0.0;

    for (const auto &cluster : subrow.clusters)
    {
        /*
        for (const auto &cell : cluster.cells)
        {
            cost += disp(cell, cell.new_x, cell.new_y);
        }*/
        cost += cluster.total_cost;
    }
    return cost;
}

void Legalizer::updateSubRowCostAndCellLocation(SubRow &subrow)
{
    for (auto &cluster : subrow.clusters)
    {
        cluster.total_cost = 0.0;
        int start_x = cluster.start_x;
        for (int i = 0; i < cluster.size(); ++i)
        {
            cluster[i].new_x = start_x;
            cluster[i].new_y = subrow.y_coord;
            start_x += cluster[i].width;
            cluster.total_cost += disp(cluster[i], cluster[i].new_x, cluster[i].new_y);
        }
    }
}

int Legalizer::findClosestRowForCell(const Cell &cell)
{
    int r = round(cell.init_y / db_.GetSiteHeight());
    if (legal_option.is_debug)
    {
        std::cout << "cell: " << cell.name << " ";
        std::cout << "coord:" << cell.init_x << " " << cell.init_y << " ";
        std::cout << "nearest row: " << r << std::endl;
    }

    return r;
}

bool Legalizer::insertCellToSubRow(Subrow &subrow, Cell &cell)
{
    if (IsFitSubRow(subrow, cell))
    {
        subrows.trial = cell;
        return true;
    }
    return false;
}

bool Legalizer::isFitSubRow(const SubRow &subrow, const Cell &cell)
{
    if (subrow.cluster.empty()) {
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

double Legalizer::disp(Cell &cell, int new_x, int new_y)
{
    double delta_x = cell.init_x - new_x;
    double delta_y = cell.init_y - new_y;
    return sqrt(delta_x * delta_x + delta_y + delta_y);
}

void Legalizer::check_legal_placement()
{

}

void Legalizer::report()
{



}

