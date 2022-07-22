#include "db.h"


void DB::Parser(string file_name)
{
    FILE *file = fopen(file_name, "r");
    char cell_file[1024];
    char gp_pl_file[1024];
    char scl_file[1024];
    ParserCellFile(std::string str(cell_file));
    ParserGPFile(std::string str(gp_pl_file));
    ParserSclFile(std::string str(scl_file));
}

void  ParserCellFile(string &file_name)
{
    FILE *file = fopen(file_name, "r");
    fclose(file);
}

void  ParserGPFile(string &file_name)
{
    FILE *file = fopen(file_name, "r");
    fclose(file);
}

void  ParserSclFile(string &file_name)
{
    FILE *file = fopen(file_name, "r");
    fclose(file);
}