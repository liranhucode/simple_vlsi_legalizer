#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>


class Cell
{
public:
    Cell(std::string name, int id, double x, double y, std::string ori) :
    name(name),
    id(id),
    init_x(x),
    init_y(y),
    ori(ori) {} 

    std::string name;
    int id;
    int width;
    int height;
    double init_x;
    double init_y;
    std::string ori;
    bool is_fixed;
};

class Row
{
public:

    int x_coord;
    int y_coord;
    int width;

};


class DB
{
public:
    DB(std::string &filename) {
        Parser(filename);
        Print();
    }

    void Print()
    {
        std::cout << "num cells: " << num_cell_ << std::endl;
        std::cout << "num fixed cells: " << num_fixed_cell_ << std::endl;
        std::cout << "num rows: " << num_row_ << std::endl;
        std::cout << "site width: " << site_width_ << std::endl;
        std::cout << "site height: " << site_height_ << std::endl;
        std::cout << "max_displacement: " << max_displacement_ << std::endl;
    }

    void Parser(std::string &filename);
    void ParserLocationFile(std::string &file_name);
    void ParserSCLFile(std::string &file_name);
    void ParserCellFile(std::string &file_name);

private:
    std::vector<Cell> cells_;
    std::vector<Row> rows_;
    int num_row_;
    int max_displacement_;
    int num_cell_;
    int num_fixed_cell_;
    int site_width_;
    int site_height_;

};