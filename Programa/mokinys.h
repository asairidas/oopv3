#ifndef mokinys_h
#define mokinys_h

#include <string>
#include <vector>
#include <iostream>
#include <ostream>

#include "statistika.h"
#include "zmogus.h"

using namespace std;

class Mokinys : public Zmogus
{
private:
    double egzaminas_;
    vector<double>* nd_;

public:
    double galutinis;
    Mokinys();          // numatytasis konstruktorius
    Mokinys(istream&); // konstruktorius
    Mokinys(string vardas, string pavarde, double egzaminas, vector<double>& nd);

    // kopijavimo konstruktorius
    Mokinys(const Mokinys& m);
    // destruktorius
    ~Mokinys();
    // kopijavimo priskyrimo operatorius
    Mokinys& operator=(const Mokinys& m);

    double galBalas(double (*)(std::vector<double>*)) const;
    inline string vardas() const { return vardas_; }   // geteriai
    inline string pavarde() const { return pavarde_; } // geteriai

    inline int ndSize() const { return (*nd_).size(); } // geteris

    istream& readStudent(istream&); // seteris

    friend istream& operator>>(istream& is, Mokinys& m);
    friend ostream& operator<<(ostream& os, const Mokinys& m);

    virtual void spausdink() const override;
};

bool compare(const Mokinys&, const Mokinys&);
bool comparePagalPavarde(const Mokinys&, const Mokinys&);
bool comparePagalEgza(const Mokinys&, const Mokinys&);

#endif
