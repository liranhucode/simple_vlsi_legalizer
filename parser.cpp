#include "db.h"
#include <fstream>
#include <sstream>

void DB::Parser(std::string &file_name)
{
    std::ifstream fin;
    fin.open(file_name.c_str(), std::ios::in);
    if (!fin.is_open())
    {
        std::cerr << "cannot open the file " << file_name << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(fin, line);
    std::stringstream word; 
    word << line;

    //paser input file
    std::string tmp;
    std::string cell_file;
    std::string gp_file;
    std::string scl_file;
    word >> tmp >> tmp;
    word >> cell_file >> gp_file >> scl_file;


    //parser max diplacement factor
    word.clear();
    std::getline(fin, line);
    word << line;
    word >> tmp >> tmp >> max_displacement_;
    std::cout << max_displacement_ << std::endl;

    ParserLocationFile(gp_file);
    ParserCellFile(cell_file);
    ParserSCLFile(scl_file);
}

void DB::ParserCellFile(std::string &file_name)
{
    std::ifstream fin;
    //fin.open(file_name, std::ios::in);
    fin.open(file_name.c_str());
    if (!fin.is_open())
    {
        std::cerr << "cannot open the file " << file_name << std::endl;
        exit(1);
    }
    std::string buff;
    std::string tmp;
    std::string name = "";
    double x = 0, y = 0;
    std::string ori = "";
    bool isCellLine = false;
    int n = 0;
    int i = 0;
    while (getline(fin, buff))
    {
        if (buff.empty() || (n = buff.find('#')) != std::string::npos || 
            (n = buff.find("UCLA")) != std::string::npos)
        {
            continue;
        } 

        if (n = buff.find("NumNodes") != std::string::npos)
        {
            std::stringstream word(buff);
            word >> buff >> buff >> num_cell_;
        }
        else if (n = buff.find("NumTerminals") != std::string::npos)
        {
            std::stringstream word(buff);
            word >> buff >> buff >> num_fixed_cell_;
        }
        else 
        {
            std::stringstream word(buff);
            word >> tmp >> cells_[i].width >> cells_[i].height;
            if (i > num_cell_ - num_fixed_cell_)
            {
                word >> cells_[i].is_fixed;
            }
            i++;
        }
    }
}

void DB::ParserLocationFile(std::string &file_name)
{
    std::ifstream fin;
    //fin.open(file_name, std::ios::in);
    fin.open(file_name.c_str());
    if (!fin.is_open())
    {
        std::cerr << "cannot open the file " << file_name << std::endl;
        exit(1);
    }
    std::string buff;
    std::string tmp;
    int i = 0;
    std::string name = "";
    double x = 0, y = 0;
    std::string ori = "";
    int n = 0;
    std::stringstream word;
    while (getline(fin, buff))
	{
        if (buff.empty() || (n = buff.find('#')) != std::string::npos || 
            (n = buff.find("UCLA")) != std::string::npos)
        {
            continue;
        } 
        //std::cout << buff << std::endl;
        word.clear();
        word << buff;
        word >> name >> x >> y >> tmp >> ori;
        Cell cell(name, i, x, y, ori);
        cells_.push_back(cell);
        i++;
    }
}

void DB::ParserSCLFile(std::string &file_name)
{
    std::ifstream fin;
    //fin.open(file_name, std::ios::in);
    fin.open(file_name.c_str());
    if (!fin.is_open())
    {
        std::cerr << "cannot open the file " << file_name << std::endl;
        exit(1);
    }
    std::string buff;
    std::string tmp;
    std::string name = "";
    double x = 0, y = 0;
    std::string ori = "";
    bool isCellLine = false;
    int n = 0;
    int i = 0;
    while (getline(fin, buff))
    {
        if (buff.empty() || (n = buff.find('#')) != std::string::npos || 
            (n = buff.find("UCLA")) != std::string::npos)
        {
            continue;
        } 

        if (n = buff.find("NumRows") != std::string::npos)
        {
            std::stringstream word(buff);
            word >> buff >> buff >> num_row_;
            rows_.resize(num_row_, Row());
        }
        else 
        {
            if (n = buff.find("CoreRow") != std::string::npos)
            {
                std::stringstream word(buff);
            }
            else if (n = buff.find("Coordinate") != std::string::npos)
            {
                std::stringstream word(buff);
                Row row;
                word >> tmp >> tmp >> rows_[i].y_coord;
            }
            else if (n = buff.find("Height") != std::string::npos)
            {
                std::stringstream word(buff);
            }
            else if (n = buff.find("Sitewidth") != std::string::npos)
            {
                std::stringstream word(buff);
            }
            else if (n = buff.find("Sitespacing") != std::string::npos)
            {
                std::stringstream word(buff);
            }
            else if (n = buff.find("Siteorient") != std::string::npos)
            {
                std::stringstream word(buff);
            }
            else if (n = buff.find("Sitesymmetry") != std::string::npos)
            {
                std::stringstream word(buff);
            }
            else if (n = buff.find("SubrowOrigin") != std::string::npos)
            {
                std::stringstream word(buff);
                word >> tmp >> tmp >> rows_[i].x_coord >> tmp >> tmp >> rows_[i].width;
            }
            else if (n = buff.find("End") != std::string::npos)
            {
                std::stringstream word(buff);
                i++;
            }
            else {
                std::cout << buff << std::endl;
                exit(1);
            }
        }
    }
}