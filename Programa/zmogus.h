#ifndef ZMOGUS_H
#define ZMOGUS_H

#include <string>

using namespace std;

class Zmogus
{
protected:
    string vardas_;
    string pavarde_;

public:
    Zmogus() : vardas_(""), pavarde_("") {}; // numatytasis konstruktorius
    Zmogus(const string& vardas, const string& pavarde) : vardas_(vardas), pavarde_(pavarde) {}

    virtual ~Zmogus() = default;

    virtual void spausdink() const = 0; // grynoji virtualioji funkcija
};

#endif // ZMOGUS_H