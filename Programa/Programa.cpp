#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <iterator>
#include <chrono>
#include <stdexcept>
#include <numeric>
#include <random>
#include <filesystem>

#include "mokinys.h"
#include "statistika.h"

using namespace std;
namespace fs = std::filesystem;

int generuoti_atsitiktini_pazymi()
{
    static random_device rd;
    static mt19937 mt(rd());
    static uniform_int_distribution<int> pazymiu_generavimas(1, 10);
    return pazymiu_generavimas(mt);
}
Mokinys nuskaityti_mokinio_duomenis()
{
    string vardas;
    string pavarde;
    int egzamino_rezultatas;
    vector<double> namu_darbu_rezultatai;
    cout << "Iveskite varda:" << endl;
    cin >> vardas;
    cout << "Ivedete varda " << vardas << endl;
    cout << "Iveskite pavarde" << endl;
    cin >> pavarde;
    cout << "Ivedete pavarde " << pavarde << endl;

    string str_pazymys;
    cout << "Iveskite pazymi (arba paspauskite Enter, kad baigtumete). Jei ivesite 0, bus sugeneruotas atsitiktinis balas: ";
    cin.ignore(10, '\n'); // Ignoruojame likusią eilutę po paskutinio cin
    while (getline(cin, str_pazymys) && !str_pazymys.empty())
    {
        int pazymys = stoi(str_pazymys);
        // jei pazymys yra 0 tai pakeiciame ji atsitiktiniu skaiciumi is intervalo [1, 10]
        if (pazymys == 0)
        {
            pazymys = generuoti_atsitiktini_pazymi();
            cout << "Sugeneruotas atsitiktinis pazymys: " << pazymys << endl;
        }
        namu_darbu_rezultatai.push_back(pazymys);
        cout << "Iveskite pazymi (arba paspauskite Enter, kad baigtumete): ";
    }

    cout << "Iveskite egzamino rezultata. Jei ivesite 0, bus sugeneruotas atsitiktinis balas:" << endl;
    cin >> egzamino_rezultatas;
    // jei pazymys yra 0 tai pakeiciame ji atsitiktiniu skaiciumi is intervalo [1, 10]
    if (egzamino_rezultatas == 0)
    {
        egzamino_rezultatas = generuoti_atsitiktini_pazymi();
        cout << "Sugeneruotas atsitiktinis pazymys: " << egzamino_rezultatas << endl;
    }

    cout << "Ivedete egzamino rezultata " << egzamino_rezultatas << endl;

    auto m = Mokinys(vardas, pavarde, egzamino_rezultatas, namu_darbu_rezultatai);
    cout << "Mokinio objektas saugomas atmintyje adresu: " << &m << endl;
    return m;
}

double skaiciuoti_galutini(Mokinys& m, const string& pasirinkimas)
{
    if (pasirinkimas == "med")
    {
        return m.galBalas(skaiciuoti_mediana);
    }
    else
    {
        return m.galBalas(skaiciuoti_vidurki);
    }
}

vector<Mokinys> duomenu_nuskaitymas_is_klaviaturos()
{
    int studentu_skaicius(0);
    while (true)
    {
        cout << "Iveskite studentu skaiciu" << endl;
        cin >> studentu_skaicius;
        if (studentu_skaicius <= 0)
        {
            cerr << "Studentų skaičius turi būti teigiamas sveikasis skaičius." << endl;
            continue; // prasykime vartotoja ivesti teisinga skaiciu
        }
        else
        {
            break; // jei skaicius teigiamas, nutraukime cikla
        }
    }

    vector<Mokinys> mokiniai;

    for (int k = 0; k < studentu_skaicius; ++k)
    {
        mokiniai.push_back(nuskaityti_mokinio_duomenis());
    }
    return mokiniai;
}

template <class K>
K duomenu_nuskaitymas_is_failo(string failo_vardas)
{
    K mokiniai;

    // nuskaitome likusias eilutes ir sudedame i mokinio struktura
    // pridedame mokini i mokiniu vektoriu

    // atidarome faila
    ifstream ivedimo_failas(failo_vardas);
    if (!ivedimo_failas)
    {
        cerr << "Nepavyko atidaryti failo " << failo_vardas << endl;
        return mokiniai;
    }

    string eilute;
    getline(ivedimo_failas, eilute);
    // kad galetume naudoti iteratoriu reikia paversti stringa i istringstream
    //istringstream iss(eilute);
    // nuskaitome pirma eilute ir suskaiciuojame kiek joje yra zodziu atskirtu tarpais, pagal tai nustatome kiek namu darbu irasu yra

    int namu_darbu_kiekis = 5;
    /*string zodis;
    while (iss >> zodis)
    {
        namu_darbu_kiekis++;
    }
    namu_darbu_kiekis -= 3; // atimame vardą, pavardę ir egzamino rezultatą
    */
  /*  while (getline(ivedimo_failas, eilute))
    {

        istringstream eilute_stream(eilute);
        auto m = Mokinys(eilute_stream);
        // naudojame ivesties operatoriaus perdengima
        eilute_stream >> m;
        mokiniai.push_back(m);
    }

    ivedimo_failas.close();*/
    return mokiniai;
}

bool mokiniu_palygintojas(const Mokinys& kairys, const Mokinys& desinys)
{
    if (kairys.pavarde() == desinys.pavarde())
    {
        return kairys.vardas() < desinys.vardas();
    }
    return kairys.pavarde() < desinys.pavarde();
}

template <class K>
void failu_irasymas(K& mokiniai, string failo_vardas)
{
    if (mokiniai.empty())
    {
        cerr << "Mokinių sąrašas yra tuščias." << endl;
        return;
    }
    ofstream failas(failo_vardas);
    if (!failas)
    {
        cerr << "Nepavyko sukurti failo " << failo_vardas << "." << endl;
        return;
    }

    // irasom pirma eilute
    failas << "Vardas Pavarde ";

    auto pazymiu_kiekis = mokiniai.front().ndSize();

    for (int i = 1; i <= pazymiu_kiekis; i++)
    {
        failas << "ND" << i << " ";
    }
    failas << "Egz." << endl;

    // naudojame isvesties operatoriaus perdengima
    for (auto mokinys : mokiniai)
    {
        failas << mokinys;
    }
}

template <class K>
void rusiuok(K& mokiniai);

template <>
void rusiuok<vector<Mokinys>>(vector<Mokinys>& mokiniai)
{
    sort(mokiniai.begin(), mokiniai.end(), mokiniu_palygintojas);
}

template <>
void rusiuok<list<Mokinys>>(list<Mokinys>& mokiniai)
{
    mokiniai.sort(mokiniu_palygintojas);
}

struct Laikai
{
    chrono::duration<double, milli> nuskaitymo_trukmes{ 0 };
    chrono::duration<double, milli> rikiavimo_trukmes{ 0 };
    chrono::duration<double, milli> galutinio_skaiciavimo_trukmes{ 0 };
    chrono::duration<double, milli> atskyrimas_trukmes{ 0 };
    chrono::duration<double, milli> silpnu_irasymas_trukmes{ 0 };
    chrono::duration<double, milli> protingu_irasymas_trukmes{ 0 };
    chrono::duration<double, milli> bendras_trukmes{ 0 };
};

template <class K>
Laikai eksperimentas(string eksperimento_failas)
{
    K mokiniai;

    auto pradzia_nuskaitymas = chrono::high_resolution_clock::now();
    auto pradzia_bendras = pradzia_nuskaitymas;
    mokiniai = duomenu_nuskaitymas_is_failo<K>(eksperimento_failas);
    auto pabaiga_nuskaitymas = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> nuskaitymo_trukme = pabaiga_nuskaitymas - pradzia_nuskaitymas;

    auto pradzia_rikiavimas = chrono::high_resolution_clock::now();
    rusiuok(mokiniai);
    auto pabaiga_rikiavimas = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> rikiavimo_trukme = pabaiga_rikiavimas - pradzia_rikiavimas;

    // kiekvieno mokinio galutinis pazymys bus skaiciuojamas pagal vidurki

    auto pradzia_galutinio_skaiciavimas = chrono::high_resolution_clock::now();
    // for (int i = 0; i < mokiniai.size(); i++)
    // {
    //     mokiniai[i].galutinis = skaiciuoti_galutini(mokiniai[i], "vid");
    // }

    for (auto& mokinys : mokiniai)
    {
        mokinys.galutinis = skaiciuoti_galutini(mokinys, "vid");
    }

    auto pabaiga_galutinio_skaiciavimas = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> galutinio_skaiciavimo_trukme = pabaiga_galutinio_skaiciavimas - pradzia_galutinio_skaiciavimas;

    K protingi;
    K silpni_moksluose;

    auto pradzia_atskyrimas = chrono::high_resolution_clock::now();
    for (auto mokinys : mokiniai)
    {
        if (mokinys.galBalas(skaiciuoti_vidurki) >= 5)
        {
            protingi.push_back(mokinys);
        }
        else
        {
            silpni_moksluose.push_back(mokinys);
        }
    }
    auto pabaiga_atskyrimas = chrono::high_resolution_clock::now();
    chrono::duration<double, milli>
        atskyrimas_trukme = pabaiga_atskyrimas - pradzia_atskyrimas;

    auto protingu_failas = "protingi_" + std::to_string(mokiniai.size()) + ".txt";
    auto pradzia_protingu_irasymas = chrono::high_resolution_clock::now();
    failu_irasymas(protingi, protingu_failas);
    auto pabaiga_protingu_irasymas = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> protingu_irasymas_trukme = pabaiga_protingu_irasymas - pradzia_protingu_irasymas;

    auto silpnu_failas = "silpni_moksluose_" + std::to_string(mokiniai.size()) + ".txt";
    auto pradzia_silpnu_irasymas = chrono::high_resolution_clock::now();
    failu_irasymas(silpni_moksluose, silpnu_failas);
    auto pabaiga_silpnu_irasymas = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> silpnu_irasymas_trukme = pabaiga_silpnu_irasymas - pradzia_silpnu_irasymas;

    auto pabaiga_bendras = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> bendras_trukme = pabaiga_bendras - pradzia_bendras;

    Laikai laikai;
    laikai.nuskaitymo_trukmes = nuskaitymo_trukme;
    laikai.rikiavimo_trukmes = rikiavimo_trukme;
    laikai.galutinio_skaiciavimo_trukmes = galutinio_skaiciavimo_trukme;
    laikai.atskyrimas_trukmes = atskyrimas_trukme;
    laikai.silpnu_irasymas_trukmes = silpnu_irasymas_trukme;
    laikai.protingu_irasymas_trukmes = protingu_irasymas_trukme;
    laikai.bendras_trukmes = bendras_trukme;
    return laikai;

    // cout << "---------[ Programos greicio analize naudojant faila:  " << eksperimento_failas << "]---------" << endl
    //      << endl;
    // cout << "failo nuskaitymas uztruko: " << nuskaitymo_trukme.count() << " ms" << endl;
    // cout << "rusiavimas uztruko: " << rusiavimo_trukme.count() << " ms" << endl;
    // cout << "galutiniu pazymiu skaiciavimas uztruko: " << galutinio_skaiciavimo_trukme.count() << " ms" << endl;
    // cout << "skirstymas i du sarasus uztruko: " << atskyrimas_trukme.count() << " ms" << endl;
    // cout << "silpnuju mokiniu isvedimas i faila uztruko: " << silpnu_irasymas_trukme.count() << " ms" << endl;
    // cout << "protingu mokiniu isvedimas i faila uztruko: " << protingu_irasymas_trukme.count() << " ms" << endl;
    // cout << "visa trukme: " << bendras_trukme.count() << " ms" << endl;

    // cout << endl
    //      << endl;
}

void spausdinti_laikus(Laikai laikai, int eksperimento_kartojimai)
{
    cout << "failo nuskaitymas uztruko: " << laikai.nuskaitymo_trukmes.count() / eksperimento_kartojimai << " ms" << endl;
    cout << "rusiavimas uztruko: " << laikai.rikiavimo_trukmes.count() / eksperimento_kartojimai << " ms" << endl;
    cout << "galutiniu pazymiu skaiciavimas uztruko: " << laikai.galutinio_skaiciavimo_trukmes.count() / eksperimento_kartojimai << " ms" << endl;
    cout << "skirstymas i dvi grupes uztruko: " << laikai.atskyrimas_trukmes.count() / eksperimento_kartojimai << " ms" << endl;
    cout << "silpnuju mokiniu isvedimas i faila uztruko: " << laikai.silpnu_irasymas_trukmes.count() / eksperimento_kartojimai << " ms" << endl;
    cout << "protingu mokiniu isvedimas i faila uztruko: " << laikai.protingu_irasymas_trukmes.count() / eksperimento_kartojimai << " ms" << endl;
    cout << "visa trukme: " << laikai.bendras_trukmes.count() / eksperimento_kartojimai << " ms" << endl;
}

void laiku_suma(Laikai& suma, Laikai l)
{
    suma.nuskaitymo_trukmes += l.nuskaitymo_trukmes;
    suma.rikiavimo_trukmes += l.rikiavimo_trukmes;
    suma.galutinio_skaiciavimo_trukmes += l.galutinio_skaiciavimo_trukmes;
    suma.atskyrimas_trukmes += l.atskyrimas_trukmes;
    suma.silpnu_irasymas_trukmes += l.silpnu_irasymas_trukmes;
    suma.protingu_irasymas_trukmes += l.protingu_irasymas_trukmes;
    suma.bendras_trukmes += l.bendras_trukmes;
}

int main(int argumentu_sk, char* argv[])
{
    bool nuskaityti_is_klaviaturos = false;

    if (argumentu_sk > 1 && string(argv[1]) == "--klaviatura")
    {
        nuskaityti_is_klaviaturos = true;
    }

    vector<string> eksperimentai{ "sugeneruoti_duomenys1000.txt" };

    if (nuskaityti_is_klaviaturos)
    {
        cout << "skaitom is klaviaturos" << endl;
        auto mokiniai = duomenu_nuskaitymas_is_klaviaturos();
        failu_irasymas(mokiniai, "mokiniai_klaviatura.txt");
        eksperimentai = { "mokiniai_klaviatura.txt" };
    }

    const int eksperimento_kartojimai = 2;

    cout << "Greicio eksperimentai naudojant vector" << endl;
    for (string eksperimento_failas : eksperimentai)
    {
        Laikai laikai;
        for (int i = 0; i < eksperimento_kartojimai; i++)
        {
            auto l = eksperimentas<vector<Mokinys>>(eksperimento_failas);
            laiku_suma(laikai, l);

            cout << "---------[ Programos greicio analize naudojant faila:  " << eksperimento_failas << " ir atliekant eksperimenta " << i + 1 << " is " << eksperimento_kartojimai << " ]---------" << endl
                << endl;
            spausdinti_laikus(l, 1);
        }

        cout << "---------[ Programos greicio analize naudojant faila:  " << eksperimento_failas << ". Vidutiniai laikai  ]---------" << endl
            << endl;
        spausdinti_laikus(laikai, eksperimento_kartojimai);
    }

    cout << "Greicio eksperimentai naudojant list" << endl;
    for (string eksperimento_failas : eksperimentai)
    {
        Laikai laikai;
        for (int i = 0; i < eksperimento_kartojimai; i++)
        {
            auto l = eksperimentas<list<Mokinys>>(eksperimento_failas);
            laiku_suma(laikai, l);
            cout << "---------[ Programos greicio analize naudojant faila:  " << eksperimento_failas << " ir atliekant eksperimenta " << i + 1 << " is " << eksperimento_kartojimai << " ]---------" << endl
                << endl;
            spausdinti_laikus(l, 1);
        }
        cout << "---------[ Programos greicio analize naudojant faila:  " << eksperimento_failas << ". Vidutiniai laikai  ]---------" << endl
            << endl;
        spausdinti_laikus(laikai, eksperimento_kartojimai);
    }
}

/*
CPU lscpu
RAM free -h
HDD lsblk
*/