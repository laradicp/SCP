#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool compare(std::pair<double, int> p1, std::pair<double, int> p2) { return (p1.second<p2.second); }

int main()
{
    ifstream dimensionsfile;

    dimensionsfile.open("instances-dimension");

    if(!dimensionsfile.is_open())
    {
        std::cout << "Problem opening dimensions file for reading." << std::endl;
        exit(1);
    }

    vector<int> dimensions;

    while(1)
    {
        int dimension;

        dimensionsfile >> dimension;
        dimensions.push_back(dimension);

        if(dimensionsfile.eof())
        {
            break;
        }
    }

    dimensionsfile.close();

    int dimensionssize = dimensions.size();

    vector<vector<pair<double, int>>> perturb5, perturb10, perturb11, perturb12, perturb15, perturb20, perturb25, perturb50, perturb100;
    vector<string> instancessets = {"A", "B", "C", "D"};

    int setssize = instancessets.size();
    for(int set = 0; set < setssize; set++)
    {
        vector<pair<double, int>> objval_size_instance;

        // perturb 5
        ifstream p5file;
        p5file.open("heuristic-calibration/perturb-j_5/benchmark/" + instancessets[set] + "-objval");

        if(!p5file.is_open())
        {
            std::cout << "Problem opening perturb-j_5 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            p5file >> objval;

            if(p5file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        perturb5.push_back(objval_size_instance);

        objval_size_instance.clear();

        // perturb 10
        ifstream p10file;
        p10file.open("heuristic-calibration/perturb-j_10/benchmark/" + instancessets[set] + "-objval");

        if(!p10file.is_open())
        {
            std::cout << "Problem opening perturb-j_10 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            p10file >> objval;

            if(p10file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        perturb10.push_back(objval_size_instance);

        objval_size_instance.clear();

        // perturb 11
        ifstream p11file;
        p11file.open("heuristic-calibration/perturb-j_11/benchmark/" + instancessets[set] + "-objval");

        if(!p11file.is_open())
        {
            std::cout << "Problem opening perturb-j_11 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            p11file >> objval;

            if(p11file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        perturb11.push_back(objval_size_instance);

        objval_size_instance.clear();

        // perturb 12
        ifstream p12file;
        p12file.open("heuristic-calibration/perturb-j_12/benchmark/" + instancessets[set] + "-objval");

        if(!p12file.is_open())
        {
            std::cout << "Problem opening perturb-j_12 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            p12file >> objval;

            if(p12file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        perturb12.push_back(objval_size_instance);

        objval_size_instance.clear();

        // perturb 15
        ifstream p15file;
        p15file.open("heuristic-calibration/perturb-j_15/benchmark/" + instancessets[set] + "-objval");

        if(!p15file.is_open())
        {
            std::cout << "Problem opening perturb-j_15 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            p15file >> objval;

            if(p15file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        perturb15.push_back(objval_size_instance);

        objval_size_instance.clear();

        // perturb 20
        ifstream p20file;
        p20file.open("heuristic-calibration/perturb-j_20/benchmark/" + instancessets[set] + "-objval");

        if(!p20file.is_open())
        {
            std::cout << "Problem opening perturb-j_20 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            p20file >> objval;

            if(p20file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        perturb20.push_back(objval_size_instance);

        objval_size_instance.clear();

        // perturb 25
        ifstream p25file;
        p25file.open("heuristic-calibration/perturb-j_25/benchmark/" + instancessets[set] + "-objval");

        if(!p25file.is_open())
        {
            std::cout << "Problem opening perturb-j_25 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            p25file >> objval;

            if(p25file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        perturb25.push_back(objval_size_instance);

        objval_size_instance.clear();

        // perturb 50
        ifstream p50file;
        p50file.open("heuristic-calibration/perturb-j_50/benchmark/" + instancessets[set] + "-objval");

        if(!p50file.is_open())
        {
            std::cout << "Problem opening perturb-j_50 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            p50file >> objval;

            if(p50file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        perturb50.push_back(objval_size_instance);

        objval_size_instance.clear();

        // perturb 100
        ifstream p100file;
        p100file.open("heuristic-calibration/perturb-j_100/benchmark/" + instancessets[set] + "-objval");

        if(!p100file.is_open())
        {
            std::cout << "Problem opening perturb-j_100 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            p100file >> objval;

            if(p100file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        perturb100.push_back(objval_size_instance);

        // output
        ofstream out;
        out.open("heuristic-calibration/perturb-table" + instancessets[set]);

        for(int i = 0; i < dimensionssize; i++)
        {
            out << perturb5[set][i].first << "\t"
                << perturb10[set][i].first << "\t"
                << perturb11[set][i].first << "\t"
                << perturb12[set][i].first << "\t"
                << perturb15[set][i].first << "\t"
                << perturb20[set][i].first << "\t"
                << perturb25[set][i].first << "\t"
                << perturb50[set][i].first << "\t"
                << perturb100[set][i].first << endl;
        }
    }

    vector<vector<pair<double, int>>> ir5ilsj, ir5ilsj2, ir10ilsj, ir10ilsj2, ir15ilsj, ir15ilsj2;

    for(int set = 0; set < setssize; set++)
    {
        vector<pair<double, int>> objval_size_instance;

        // ir 5 ils j
        ifstream i5jfile;
        i5jfile.open("heuristic-calibration/ir-5-ils-j/benchmark/" + instancessets[set] + "-objval");

        if(!i5jfile.is_open())
        {
            std::cout << "Problem opening ir-5-ils-j objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            i5jfile >> objval;

            if(i5jfile.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        ir5ilsj.push_back(objval_size_instance);

        objval_size_instance.clear();

        // ir 5 ils j/2
        ifstream i5j2file;
        i5j2file.open("heuristic-calibration/ir-5-ils-j_2/benchmark/" + instancessets[set] + "-objval");

        if(!i5j2file.is_open())
        {
            std::cout << "Problem opening ir-5-ils-j_2 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            i5j2file >> objval;

            if(i5j2file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        ir5ilsj2.push_back(objval_size_instance);

        objval_size_instance.clear();

        // ir 10 ils j
        ifstream i10jfile;
        i10jfile.open("heuristic-calibration/ir-10-ils-j/benchmark/" + instancessets[set] + "-objval");

        if(!i10jfile.is_open())
        {
            std::cout << "Problem opening ir-10-ils-j objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            i10jfile >> objval;

            if(i10jfile.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        ir10ilsj.push_back(objval_size_instance);

        objval_size_instance.clear();

        // ir 10 ils j/2
        ifstream i10j2file;
        i10j2file.open("heuristic-calibration/ir-10-ils-j_2/benchmark/" + instancessets[set] + "-objval");

        if(!i10j2file.is_open())
        {
            std::cout << "Problem opening ir-10-ils-j_2 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            i10j2file >> objval;

            if(i10j2file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        ir10ilsj2.push_back(objval_size_instance);

        objval_size_instance.clear();

        // ir 15 ils j
        ifstream i15jfile;
        i15jfile.open("heuristic-calibration/ir-15-ils-j/benchmark/" + instancessets[set] + "-objval");

        if(!i15jfile.is_open())
        {
            std::cout << "Problem opening ir-15-ils-j objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            i15jfile >> objval;

            if(i15jfile.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        ir15ilsj.push_back(objval_size_instance);

        objval_size_instance.clear();

        // ir 15 ils j/2
        ifstream i15j2file;
        i15j2file.open("heuristic-calibration/ir-15-ils-j_2/benchmark/" + instancessets[set] + "-objval");

        if(!i15j2file.is_open())
        {
            std::cout << "Problem opening ir-15-ils-j_2 objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            i15j2file >> objval;

            if(i15j2file.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        ir15ilsj2.push_back(objval_size_instance);

        // output
        ofstream out;
        out.open("heuristic-calibration/ir-ils-table" + instancessets[set]);

        for(int i = 0; i < dimensionssize; i++)
        {
            out << ir5ilsj[set][i].first << "\t"
                << ir5ilsj2[set][i].first << "\t"
                << ir10ilsj[set][i].first << "\t"
                << ir10ilsj2[set][i].first << "\t"
                << ir15ilsj[set][i].first << "\t"
                << ir15ilsj2[set][i].first << endl;
        }
    }

    vector<vector<pair<double, int>>> swap, swapSmartUpdateCL, swapStoreSelected, swapUpdateCL;

    for(int set = 0; set < setssize; set++)
    {
        vector<pair<double, int>> objval_size_instance;

        // swap
        ifstream swapfile;
        swapfile.open("heuristic-calibration/swap/benchmark/" + instancessets[set] + "-objval");

        if(!swapfile.is_open())
        {
            std::cout << "Problem opening swap objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            swapfile >> objval;

            if(swapfile.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        swap.push_back(objval_size_instance);

        objval_size_instance.clear();

        // swap smart update cl
        ifstream swapsuclfile;
        swapsuclfile.open("heuristic-calibration/swap-smart-update-cl/benchmark/" + instancessets[set] + "-objval");

        if(!swapsuclfile.is_open())
        {
            std::cout << "Problem opening swapsucl objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            swapsuclfile >> objval;

            if(swapsuclfile.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        swapSmartUpdateCL.push_back(objval_size_instance);

        objval_size_instance.clear();

        // swap store selected
        ifstream swapssfile;
        swapssfile.open("heuristic-calibration/swap-store-selected/benchmark/" + instancessets[set] + "-objval");

        if(!swapssfile.is_open())
        {
            std::cout << "Problem opening swapss objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            swapssfile >> objval;

            if(swapssfile.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        swapStoreSelected.push_back(objval_size_instance);

        objval_size_instance.clear();

        // swap update cl
        ifstream swapuclfile;
        swapuclfile.open("heuristic-calibration/swap-update-cl/benchmark/" + instancessets[set] + "-objval");

        if(!swapuclfile.is_open())
        {
            std::cout << "Problem opening swapucl objval file for reading." << std::endl;
            exit(1);
        }
        
        for(int i = 0; ; i++)
        {
            double objval;
            swapuclfile >> objval;

            if(swapuclfile.eof() || i == dimensionssize)
            {
                break;
            }

            objval_size_instance.push_back(make_pair(objval, dimensions[i]));
        }

        sort(objval_size_instance.begin(), objval_size_instance.end(), compare);
        swapUpdateCL.push_back(objval_size_instance);

        // output
        ofstream out;
        out.open("heuristic-calibration/swap-table" + instancessets[set]);

        for(int i = 0; i < dimensionssize; i++)
        {
            out << swap[set][i].first << "\t"
                << swapSmartUpdateCL[set][i].first << "\t"
                << swapStoreSelected[set][i].first << "\t"
                << swapUpdateCL[set][i].first << endl;
        }
    }

    return 0;
}