#include "db.h"
#include <fstream>
#include <sstream>

void DB::Parser(string file_name)
{
    std::ifstream fin;
    fin.open(file_name, std::ios::in);
    if (!fin.is_open())
    {
        std::cerr << "cannot open the file";
        exit(1);
    }

    std::string line;
    std::getline(fin, line);
    stringstream word(line); 

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

void DB::ParserCellFile(string &file_name)
{
    std::ifstream fin;
    fin.open(file_name, std::ios::in);
    if (!fin.is_open())
    {
        std::cerr << "cannot open the file";
        exit(1);
    }
    string buff;
    int i = 0;
    string name = "";
    double x = 0, y = 0;
    string ori = "";
    bool isCellLine = false;
    int n = 0;
    while (getline(fin, buff))
    {
        if (buff.empty() || (n = buff.find('#')) != string::npos || 
            (n = buff.find("UCLA")) != string::npos)
        {
            continue;
        } 
    }

    stringstream word(buff);
    buff >> tmp >> tmp >> num_cell_;
    getline(fin, buff);
    word << buff;
    buff >> tmp >> tmp >> num_fixed_cell_;

    int i = 0;
    while (getline(fin, buff))
    {
        if (buff.empty())
        {
            continue;
        } 

        word << buff;
        word >> tmp >> cells_[i].width >> cell_[i].height;

        if (i > num_cell_ - num_fixed_cell_)
        {
            word >> cell_[i].is_fixed; 
        }
    }
}

void  ParserLocationFile(string &file_name)
{
    std::ifstream fin;
    fin.open(file_name, std::ios::in);
    if (!fin.is_open())
    {
        std::cerr << "cannot open the file";
        exit(1);
    }
    string buff;
    int i = 0;
    string name = "";
    double x = 0, y = 0;
    string ori = "";
    while (getline(fin, buff))
	{
        if (i < 2)
        {
            continue;
        }
        //cout << buff<<endl;
        stringstream word(buff);
        word >> name >> x >> y >> tmp >> ori;
        Cell cell(name, i - 2, x, y, ori);
        cells_.push_back(cell);
        i++;
    }
}

void  ParserSCLFile(string &file_name)
{
    std::ifstream fin;
    fin.open(file_name, std::ios::in);
    if (!fin.is_open())
    {
        std::cerr << "cannot open the file";
        exit(1);
    }
    string buff;

}