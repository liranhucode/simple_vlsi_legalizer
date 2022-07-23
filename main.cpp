#include "legal.h"

void usage()
{
    std::cout << "./Legalizer test.aux" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage();
        return false;
        //exit(1);
    }

    std::cout << "Start legalization" << std::endl;

    std::string filename(argv[1]);
    DB db(filename);

    Legalizer legal(db);
    legal.run();
    legal.report();
    std::cout << "Done legalization" << std::endl;
    return 0;
}