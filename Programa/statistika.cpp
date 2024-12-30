#include <algorithm>
#include "mokinys.h"

using namespace std;

double skaiciuoti_vidurki(vector<double>* pazymiai)
{
    // jeigu pazymiu nera grazina 0
    if (pazymiai->size() == 0)
    {
        return 0;
    }

    double nd_suma = 0;
    double vidurkis = 0;
    for (int j = 0; j < pazymiai->size(); j++)
    {
        nd_suma += (*pazymiai)[j];
    }
    vidurkis = nd_suma / pazymiai->size();
    return vidurkis;
}

double skaiciuoti_mediana(vector<double>* pazymiai)
{
    double mediana = 0;
    sort(pazymiai->begin(), pazymiai->end());
    if (pazymiai->size() % 2 == 0)
    {
        auto pirmas = (*pazymiai)[pazymiai->size() / 2 - 1];
        auto antras = (*pazymiai)[pazymiai->size() / 2];
        mediana = (pirmas + antras) / 2.0;
    }
    else
    {
        mediana = (*pazymiai)[pazymiai->size() / 2];
    }
    return mediana;
}