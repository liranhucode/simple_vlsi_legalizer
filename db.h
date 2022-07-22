#pragma once
#include <iostream>

class Cell
{

};

class Row
{

};


class DB
{
public:
    DB(string filename) {
        Parser(filename);
        Print();
    }

    void Print()
    {

    }

    void Parser(string filename);

private:
    vector<Cell> cells_;
    vector<Row> rows_;
    max_displacement_;
    int num_cell_;
    int num_fixed_cell_;
    int site_width_;
    int site_height_;

};