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
    std::stringstream word(line); 

    std::string tmp;
    std::string cell_file;
    std::string gp_file;
    std::string scl_file;
    word >> tmp >> tmp;
    word >> cell_file >> gp_file >> scl_file;

    ParserCellFile(cell_file);
    ParserLocationFile(gp_file);
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
    std::string name = "";
    double x = 0, y = 0;
    std::string ori = "";
    bool isCellLine = false;
    int n = 0;
    while (getline(fin, buff))
    {
        if (buff.empty() || (n = buff.find('#')) != std::string::npos || 
            (n = buff.find("UCLA")) != std::string::npos)
        {
            continue;
        } 
    }

    std::string tmp;
    std::stringstream word(buff);
    word >> buff >> buff >> num_cell_;
    getline(fin, buff);
    word << buff;
    word >> tmp >> tmp >> num_fixed_cell_;

    int i = 0;
    while (getline(fin, buff))
    {
        if (buff.empty())
        {
            continue;
        } 

        word << buff;
        word >> tmp >> cells_[i].width >> cells_[i].height;

        if (i > num_cell_ - num_fixed_cell_)
        {
            word >> cells_[i].is_fixed; 
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
    while (getline(fin, buff))
	{
        if (i < 2)
        {
            continue;
        }
        //cout << buff<<endl;
        std::stringstream word(buff);
        word >> name >> x >> y >> tmp >> ori;
        Cell cell(name, i - 2, x, y, ori);
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
}