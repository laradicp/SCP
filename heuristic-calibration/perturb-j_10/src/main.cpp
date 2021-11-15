#include "Data.h"
#include "Heuristic.h"
#include <unistd.h>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Usage: ./scp-heuristic instance" << std::endl;
        return 0;
    }

    Data data(argv[1]);
    std::vector<int> s;
    auto begin = std::chrono::system_clock::now();

    heuristic(data, s);

    std::chrono::duration<double> time = std::chrono::system_clock::now() - begin;
    std::ofstream out;

    std::string instance = argv[1];
    out.open("output/" + instance.substr(16), std::ios_base::app);

    if(!out.is_open())
    {
        std::cout << "Problem opening file for writing." << std::endl;
        exit(1);
    }

    out << time.count() << std::endl;
    out << s.size() << std::endl;

    usleep(1000000);

    return 0;
}
