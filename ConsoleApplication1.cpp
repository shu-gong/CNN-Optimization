#include <iostream>
#include <cstdint>
#include <limits>

using namespace std;

double objective(double Ti, double Tj, double Tk, double Nk, double Ni, double Nj)
{
    return Nk * Ni * Nj * (1 / Tj + 1 / Ti + 1 / Tk);
}

bool constraint(double Ti, double Tj, double Tk, double C)
{
    return (Ti * Tk + Tj * Tk + Ti * Tj <= C);
}

int main()
{
    double Nk = 16384;
    double Ni = 16384;
    double Nj = 16384;
    double C = 1073741824;

    double best_objective_value = INT8_MAX;
    double best_Ti = 0;
    double best_Tj = 0;
    double best_Tk = 0;

    for (double Ti = 1; Ti <= Ni; ++Ti)
    {
        for (double Tj = 1; Tj <= Nj; ++Tj)
        {
            for (double Tk = 1; Tk <= Nk; ++Tk)
            {
                if (constraint(Ti, Tj, Tk, C))
                {
                    double objective_value = objective(Ti, Tj, Tk, Nk, Ni, Nj);
                    if (objective_value < best_objective_value)
                    {
                        best_objective_value = objective_value;
                        best_Ti = Ti;
                        best_Tj = Tj;
                        best_Tk = Tk;
                    }
                }
            }
        }
    }

    cout << "Best values:" << endl;
    cout << "Ti = " << best_Ti << endl;
    cout << "Tj = " << best_Tj << endl;
    cout << "Tk = " << best_Tk << endl;
}
