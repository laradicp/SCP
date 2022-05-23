#include "Data.h"
#include "Heuristic.h"

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
    out.open("output/" + instance.substr(13));

    if(!out.is_open())
    {
        std::cout << "Problem opening file for writing." << std::endl;
        exit(1);
    }

    out << time.count() << std::endl;

    int sSize = s.size();
    // std::vector<int> iterFamily(data.getFamiliesSize(), 0);
    for(int p = 0; p < sSize; p++)
    {
        // out << data.getFamilyMember(s[p], iterFamily[s[p]]++) << std::endl;
        out << s[p] << std::endl;
    }

    out.close();

    return 0;
}