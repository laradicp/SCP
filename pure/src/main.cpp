#include "Data.h"
#include <string>
#include <chrono>
#include <ilcplex/ilocplex.h>

bool solve(Data &data, int positions, std::vector<int> &s, std::chrono::duration<double> time)
{
    IloEnv env;
    IloModel model(env);

    std::vector<int> jobsPerCadence(data.getCadencesSize(), 0);
    std::vector<std::vector<int>> intersect(data.getCadencesSize(), std::vector<int>(data.getCadencesSize(), 0));
    for(int f = 0; f < data.getFamiliesSize(); f++)
    {
        for(int c1 = 0; c1 < data.getCadencesSize(); c1++)
        {
            if(data.getCadencesPerFamily(f, c1))
            {
                jobsPerCadence[c1] += data.getFamilySize(f);
                for(int c2 = 0; c2 < data.getCadencesSize(); c2++)
                {
                    if(data.getCadencesPerFamily(f, c2))
                    {
                        intersect[c1][c2] += data.getFamilySize(f);
                    }
                }
            }
        }
    }

    // let Xfp assume value 1 if any job f ∈ F is assigned to position p ∈ P, and 0 otherwise
    IloArray<IloBoolVarArray> x(env, data.getFamiliesSize());
    for(int f = 0; f < data.getFamiliesSize(); f++)
    {
        IloBoolVarArray v(env, positions + 1);
        x[f] = v;
    }

    // add variable x to the model
    for(int f = 0; f < data.getFamiliesSize(); f++)
    {
        for(int p = 1; p < positions + 1; p++)
        {
            char name[100];
            sprintf(name, "X(%d,%d)", f, p);
            x[f][p].setName(name);
            model.add(x[f][p]);
        }
    }

    // constraints 1: respect family demand
    for(int f = 0; f < data.getFamiliesSize(); f++) 
    {
        IloExpr sumX(env);
        for(int p = 1; p < positions + 1; p++)
        {
            sumX += x[f][p];
        }

        IloRange r = (sumX <= data.getFamilySize(f));
        char name[100];
        sprintf(name, "CarsProduced(%d)", f);
        r.setName(name);
        model.add(r);
    }

    // constraints 2: each position is occupied by one car
    for(int p = 1; p < positions + 1; p++) 
    {
        IloExpr sumX(env);
        for(int f = 0; f < data.getFamiliesSize(); f++)
        {
            sumX += x[f][p];
        }

        IloRange r = (sumX == 1);
        char name[100];
        sprintf(name, "PositionOccupied(%d)", p);
        r.setName(name);
        model.add(r);
    }
    
    std::vector<bool> satisfied(data.getCadencesSize(), false);
    for(int c = 0; c < data.getCadencesSize(); c++)
    {
        if(data.cadenceType(c) == 1)
        {
            for(int c2 = 0; c2 < data.getCadencesSize(); c2++)
            {
                if(data.cadenceType(c) == 2)
                {
                    if((jobsPerCadence[c] - intersect[c][c2] + std::ceil(data.getCadence(c)/(data.getCadence(c2) + 1)) <
                        data.getCadence(c))&&(intersect[c][c2]*data.getCadence(c2) >= data.getCadence(c)))
                    {
                        satisfied[c] = true;
                        break;
                    }
                }
            }
        }
    }

    // constraints 3: feasible cadences from the first group
    for(int p = 1; p < positions + 1; p++)
    {
        for(int c = 0; c < data.getCadencesSize(); c++)
        {
            if(data.cadenceType(c) == 1)
            {
                if(satisfied[c])
                {
                    continue;
                }

                IloExpr sumX(env);

                for(int f = 0; f < data.getFamiliesSize(); f++)
                {
                    if(data.getCadencesPerFamily(f, c))
                    {
                        for(int k = 0; k < data.getCadence(c) + 1; k++)
                        {
                            if(p + k < positions + 1)
                            {
                                sumX += x[f][p + k];
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                IloRange r = (sumX <= 1);
                char name[100];
                sprintf(name, "FeasibleCadences1(%d,%d)", p, c);
                r.setName(name);
                model.add(r);
            }
        }
    }
    
    // constraints 4: feasible cadences from the second group
    for(int p = 1; p < positions + 1; p++)
    {
        for(int c = 0; c < data.getCadencesSize(); c++)
        {
            if(data.cadenceType(c) == 2)
            {
                IloExpr sumX(env);

                for(int f = 0; f < data.getFamiliesSize(); f++)
                {
                    if(data.getCadencesPerFamily(f, c))
                    {
                        for(int k = 0; k < data.getCadence(c) + 1; k++)
                        {
                            if(p + k < positions + 1)
                            {
                                sumX += x[f][p + k];
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                IloRange r = (sumX <= data.getCadence(c));
                char name[100];
                sprintf(name, "FeasibleCadences2(%d,%d)", p, c);
                r.setName(name);
                model.add(r);
            }
        }
    }

    // solve the model
    IloCplex scp(model);
    scp.setOut(env.getNullStream());
    scp.setParam(IloCplex::Param::TimeLimit, 10*60 - time.count());
    scp.setParam(IloCplex::Param::Threads, 1);
    scp.setParam(IloCplex::Param::MIP::Strategy::VariableSelect, 3);
    // bpp.setParam(IloCplex::Param::MIP::Interval, 1);
    // bpp.setParam(IloCplex::Param::MIP::Display, 5);
    scp.exportModel("model.lp");

    try
    {
        if(scp.solve())
        {
            s.clear();

            std::vector<int> iterFamily(data.getFamiliesSize(), 0);
            for(int p = 1; p < positions + 1; p++)
            {
                for(int f = 0; f < data.getFamiliesSize(); f++)
                {
                    if(scp.getValue(x[f][p]) > 0.5)
                    {
                        s.push_back(data.getFamilyMember(f, iterFamily[f]++));
                        break;
                    }
                }
            }

            return true;
        }
        
        return false;   
    }
    catch(IloException& e)
    {
        std::cout << e;
    }
}

bool solve2(Data &data, std::vector<int> &s)
{
    IloEnv env;
    IloModel model(env);

    int positions = data.getDimension();
    std::vector<int> jobsPerCadence(data.getCadencesSize(), 0);
    std::vector<std::vector<int>> intersect(data.getCadencesSize(), std::vector<int>(data.getCadencesSize(), 0));
    for(int f = 0; f < data.getFamiliesSize(); f++)
    {
        for(int c1 = 0; c1 < data.getCadencesSize(); c1++)
        {
            if(data.getCadencesPerFamily(f, c1))
            {
                jobsPerCadence[c1] += data.getFamilySize(f);
                for(int c2 = 0; c2 < data.getCadencesSize(); c2++)
                {
                    if(data.getCadencesPerFamily(f, c2))
                    {
                        intersect[c1][c2] += data.getFamilySize(f);
                    }
                }
            }
        }
    }

    // let Xfp assume value 1 if any job f ∈ F is assigned to position p ∈ P, and 0 otherwise
    IloArray<IloBoolVarArray> x(env, data.getFamiliesSize());
    for(int f = 0; f < data.getFamiliesSize(); f++)
    {
        IloBoolVarArray v(env, data.getDimension() + 1);
        x[f] = v;
    }

    // add variable x to the model
    for(int f = 0; f < data.getFamiliesSize(); f++)
    {
        for(int p = 1; p < positions + 1; p++)
        {
            char name[100];
            sprintf(name, "X(%d,%d)", f, p);
            x[f][p].setName(name);
            model.add(x[f][p]);
        }
    }

    // add objective function (OF)
    IloExpr sumX(env);
    for(int f = 0; f < data.getFamiliesSize(); f++) 
    {
        for(int p = 1; p < data.getDimension() + 1; p++)
        {
            sumX += x[f][p];
        }
    }
    model.add(IloMaximize(env, sumX));

    // constraints 1: respect family demand
    for(int f = 0; f < data.getFamiliesSize(); f++) 
    {
        IloExpr sumX(env);
        for(int p = 1; p < positions + 1; p++)
        {
            sumX += x[f][p];
        }

        IloRange r = (sumX <= data.getFamilySize(f));
        char name[100];
        sprintf(name, "CarsProduced(%d)", f);
        r.setName(name);
        model.add(r);
    }

    // constraints 2: each position is occupied by one car
    for(int p = 1; p < positions + 1; p++) 
    {
        IloExpr sumX(env);
        for(int f = 0; f < data.getFamiliesSize(); f++)
        {
            sumX += x[f][p];
        }

        IloRange r = (sumX <= 1);
        char name[100];
        sprintf(name, "PositionOccupied(%d)", p);
        r.setName(name);
        model.add(r);
    }
    
    std::vector<bool> satisfied(data.getCadencesSize(), false);
    for(int c = 0; c < data.getCadencesSize(); c++)
    {
        if(data.cadenceType(c) == 1)
        {
            for(int c2 = 0; c2 < data.getCadencesSize(); c2++)
            {
                if(data.cadenceType(c) == 2)
                {
                    if((jobsPerCadence[c] - intersect[c][c2] + std::ceil(data.getCadence(c)/(data.getCadence(c2) + 1)) <
                        data.getCadence(c))&&(intersect[c][c2]*data.getCadence(c2) >= data.getCadence(c)))
                    {
                        satisfied[c] = true;
                        break;
                    }
                }
            }
        }
    }

    // constraints 3: feasible cadences from the first group
    for(int p = 1; p < positions + 1; p++)
    {
        for(int c = 0; c < data.getCadencesSize(); c++)
        {
            if(data.cadenceType(c) == 1)
            {
                if(satisfied[c])
                {
                    continue;
                }

                IloExpr sumX(env);

                for(int f = 0; f < data.getFamiliesSize(); f++)
                {
                    if(data.getCadencesPerFamily(f, c))
                    {
                        for(int k = 0; k < data.getCadence(c) + 1; k++)
                        {
                            if(p + k < data.getDimension() + 1)
                            {
                                sumX += x[f][p + k];
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                IloRange r = (sumX <= 1);
                char name[100];
                sprintf(name, "FeasibleCadences1(%d,%d)", p, c);
                r.setName(name);
                model.add(r);
            }
        }
    }
    
    // constraints 4: feasible cadences from the second group
    for(int p = 1; p < positions + 1; p++)
    {
        for(int c = 0; c < data.getCadencesSize(); c++)
        {
            if(data.cadenceType(c) == 2)
            {
                IloExpr sumX(env);

                for(int f = 0; f < data.getFamiliesSize(); f++)
                {
                    if(data.getCadencesPerFamily(f, c))
                    {
                        for(int k = 0; k < data.getCadence(c) + 1; k++)
                        {
                            if(p + k < data.getDimension() + 1)
                            {
                                sumX += x[f][p + k];
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                IloRange r = (sumX <= data.getCadence(c));
                char name[100];
                sprintf(name, "FeasibleCadences2(%d,%d)", p, c);
                r.setName(name);
                model.add(r);
            }
        }
    }

    // constraints 5: there should be unoccupied spaces only at the end
    for(int p = 1; p < positions; p++) 
    {
        IloExpr sumX1(env);
        IloExpr sumX2(env);
        for(int f = 0; f < data.getFamiliesSize(); f++)
        {
            sumX1 += x[f][p];
            sumX2 += x[f][p + 1];
        }

        IloRange r = (sumX2 - sumX1 <= 0);
        char name[100];
        sprintf(name, "UnoccupiedEnd(%d)", p);
        r.setName(name);
        model.add(r);
    }
    
    // solve the model
    IloCplex scp(model);
    // scp.setOut(env.getNullStream());
    scp.setParam(IloCplex::Param::TimeLimit, 10*60);
    scp.setParam(IloCplex::Param::Threads, 1);
    scp.setParam(IloCplex::Param::MIP::Strategy::VariableSelect, 3);
    // scp.setParam(CutUp);
    // bpp.setParam(IloCplex::Param::MIP::Interval, 1);
    // bpp.setParam(IloCplex::Param::MIP::Display, 5);
    scp.exportModel("model.lp");

    try
    {
        if(scp.solve())
        {
            s.clear();

            std::vector<int> iterFamily(data.getFamiliesSize(), 0);
            for(int p = 1; p < positions + 1; p++)
            {
                for(int f = 0; f < data.getFamiliesSize(); f++)
                {
                    if(scp.getValue(x[f][p]) > 0.5)
                    {
                        s.push_back(data.getFamilyMember(f, iterFamily[f]++));
                        break;
                    }
                }
            }

            int sSize = s.size();
            for(int p = 0; p < sSize; p++)
            {
                std::cout << "Position " << p + 1 << ":\t";
                std::cout << s[p] << std::endl;
            }
            std::cout << "Primal:\t\t" << scp.getObjValue() << std::endl;
            std::cout << "Dual:\t\t" << scp.getBestObjValue() << std::endl;
            std::cout << "Status:\t\t" << scp.getStatus() << std::endl;
            
            return true;
        }

        return false;
    }
    catch(IloException& e)
    {
        std::cout << e;
    }
}

bool solve3(Data &data, std::vector<int> &s, std::string instance)
{
    IloEnv env;
    IloModel model(env);

    int positions = data.getDimension();

    //let Xfp assume value 1 if any job f ∈ F is assigned to position p ∈ P, and 0 otherwise
    IloArray<IloBoolVarArray> x(env, data.getFamiliesSize());
    for(int f = 0; f < data.getFamiliesSize(); f++)
    {
        IloBoolVarArray v(env, positions + 1);
        x[f] = v;
    }

    //add variable x to the model
    for(int f = 0; f < data.getFamiliesSize(); f++)
    {
        for(int p = 1; p < positions + 1; p++)
        {
            char name[100];
            sprintf(name, "X(%d,%d)", f, p);
            x[f][p].setName(name);
            model.add(x[f][p]);
        }
    }

    // add objective function (OF)
    IloExpr sumX(env);
    for(int f = 0; f < data.getFamiliesSize(); f++) 
    {
        for(int p = 1; p < positions + 1; p++)
        {
            sumX += x[f][p];
        }
    }
    model.add(IloMaximize(env, sumX));

    //constraints 1: respect family demand
    for(int f = 0; f < data.getFamiliesSize(); f++) 
    {
        IloExpr sumX(env);
        for(int p = 1; p < positions + 1; p++)
        {
            sumX += x[f][p];
        }

        IloRange r = (sumX <= data.getFamilySize(f));
        char name[100];
        sprintf(name, "CarsProduced(%d)", f);
        r.setName(name);
        model.add(r);
    }

    //constraints 2: each position is occupied by one car
    for(int p = 1; p < positions + 1; p++) 
    {
        IloExpr sumX(env);
        for(int f = 0; f < data.getFamiliesSize(); f++)
        {
            sumX += x[f][p];
        }

        IloRange r = (sumX <= 1);
        char name[100];
        sprintf(name, "PositionOccupied(%d)", p);
        r.setName(name);
        model.add(r);
    }
    
    //constraints 3: feasible cadences from the first group
    for(int p = 1; p < positions + 1; p++)
    {
        for(int c = 0; c < data.getCadencesSize(); c++)
        {
            if(data.cadenceType(c) == 1)
            {
                IloExpr sumX(env);

                for(int f = 0; f < data.getFamiliesSize(); f++)
                {
                    if(data.getCadencesPerFamily(f, c))
                    {
                        for(int k = 0; k < data.getCadence(c) + 1; k++)
                        {
                            if(p + k < data.getDimension() + 1)
                            {
                                sumX += x[f][p + k];
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                IloRange r = (sumX <= 1);
                char name[100];
                sprintf(name, "FeasibleCadences1(%d,%d)", p, c);
                r.setName(name);
                model.add(r);
            }
        }
    }
    
    //constraints 4: feasible cadences from the second group
    for(int p = 1; p < positions + 1; p++)
    {
        for(int c = 0; c < data.getCadencesSize(); c++)
        {
            if(data.cadenceType(c) == 2)
            {
                IloExpr sumX(env);

                for(int f = 0; f < data.getFamiliesSize(); f++)
                {
                    if(data.getCadencesPerFamily(f, c))
                    {
                        for(int k = 0; k < data.getCadence(c) + 1; k++)
                        {
                            if(p + k < data.getDimension() + 1)
                            {
                                sumX += x[f][p + k];
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }

                IloRange r = (sumX <= data.getCadence(c));
                char name[100];
                sprintf(name, "FeasibleCadences2(%d,%d)", p, c);
                r.setName(name);
                model.add(r);
            }
        }
    }

    //constraints 5: there should be unoccupied spaces only at the end
    for(int p = 1; p < positions; p++) 
    {
        IloExpr sumX1(env);
        IloExpr sumX2(env);
        for(int f = 0; f < data.getFamiliesSize(); f++)
        {
            sumX1 += x[f][p];
            sumX2 += x[f][p + 1];
        }

        IloRange r = (sumX2 - sumX1 <= 0);
        char name[100];
        sprintf(name, "UnoccupiedEnd(%d)", p);
        r.setName(name);
        model.add(r);
    }
    
    //solve the model
    IloCplex scp(model);
    //scp.setOut(env.getNullStream());
    scp.setParam(IloCplex::Param::TimeLimit, 10*60);
    scp.setParam(IloCplex::Param::Threads, 1);
    scp.setParam(IloCplex::Param::MIP::Strategy::VariableSelect, 3);
    //bpp.setParam(IloCplex::Param::MIP::Interval, 1);
    //bpp.setParam(IloCplex::Param::MIP::Display, 5);
    scp.exportModel("model.lp");

    std::ifstream in;
    instance = instance.substr(instance.find("instance-"), 30);
    in.open("solutions/" + instance);

    if(!in.is_open())
    {
        std::cout << "Problem opening file for reading." << std::endl;
        exit(1);
    }

    std::vector<std::vector<bool>> start(data.getFamiliesSize(), std::vector<bool>(positions + 1, false));

    for(int p = 1; ; p++)
    {
        int f;
        in >> f;

        if(in.eof())
        {
            break;
        }

        start[f][p] = true;
    }

    IloNumVarArray startVar(env);
    IloNumArray startVal(env);
    for (int f = 0; f < data.getFamiliesSize(); f++)
    {
        for (int p = 1; p < positions + 1; p++)
        {
            startVar.add(x[f][p]);
            startVal.add(start[f][p]);
        }
    }
    scp.addMIPStart(startVar, startVal);
    startVal.end();
    startVar.end();

    try
    {
        if(scp.solve())
        {
            s.clear();

            std::vector<int> iterFamily(data.getFamiliesSize(), 0);
            for(int p = 1; p < positions + 1; p++)
            {
                for(int f = 0; f < data.getFamiliesSize(); f++)
                {
                    if(scp.getValue(x[f][p]) > 0.5)
                    {
                        s.push_back(data.getFamilyMember(f, iterFamily[f]++));
                        break;
                    }
                }
            }

            int sSize = s.size();
            for(int p = 0; p < sSize; p++)
            {
                std::cout << "Position " << p + 1 << ":\t";
                std::cout << s[p] << std::endl;
            }
            std::cout << "Maximum profit:\t" << scp.getObjValue() << std::endl;
            std::cout << "Primal:\t\t" << scp.getObjValue() << std::endl;
            std::cout << "Dual:\t\t" << scp.getBestObjValue() << std::endl;
            std::cout << "Status:\t\t" << scp.getStatus() << std::endl;
            
            return true;
        }

        return false;
    }
    catch(IloException& e)
    {
        std::cout << e;
    }
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Usage: ./scp instance" << std::endl;
        return 0;
    }
    
    Data data(argv[1]);

    auto begin = std::chrono::system_clock::now();
    std::chrono::duration<double> time = std::chrono::system_clock::now() - begin;
    std::vector<int> s;

    solve2(data, s);

    time = std::chrono::system_clock::now() - begin;
    
    std::cout << "Time:\t\t" << time.count() << std::endl;

    return 0;
}
