#include "legal.h"

void usage()
{

}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage();
        exit(1);
    }
    else if (argc == 2)
    {
        string filename(argv[1]);
        DB db(filename);
    }

    std::cout << "Start legalization" << std::endl;
    Legalizer legal(db);
    legal.run();
    legal.report();
    std::cout << "Done legalization" << std::endl;
    return 0;
}