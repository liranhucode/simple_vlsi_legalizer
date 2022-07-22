#pragma once
#include <iostream>
#include <string>
#include <vector>

class Cell
{
private：
    int id_;
    string name_;
    int width;
    int height;
    double init_x_;
    double init_y_;
    int ori_;
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