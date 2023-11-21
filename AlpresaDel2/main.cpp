#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;



class Transaktion
{
private:
    string datum;
    string typ;
    string namn;
    double belopp;
    int    antal_kompisar;
    string *kompisar;

public:
    Transaktion();
    Transaktion(string datum, string typ, string namn, double belopp);
    ~Transaktion();
    Transaktion& operator=( const Transaktion& t);
    string hamtaNamn() const;
    double hamtaBelopp() const;
    int    hamtaAntalKompisar() const;
    bool   finnsKompis( const string &namnet) const;
    bool   lasIn(istream &is);
    void   skrivUt(ostream &os) const;
    void   skrivTitel(ostream &os) const;


    void assign(const Transaktion &t); //assign
};



class Person
{
private:
    string namn;
    double betalat_andras ; // ligger ute med totalt
    double skyldig;        // skyldig totalt
public:
    Person();
    Person(const string &namn, double betalat_andras, double skyldig);
    string hamtaNamn();
    double hamtaBetalat();
    double hamtaSkyldig();
    void   skrivUt(ostream &os);
};


class PersonLista
{
private:
    int    antal_personer=0;
    Person *personer;

public:
    PersonLista();
    ~PersonLista();
    PersonLista& operator=( const PersonLista& p);
    void   laggTill(Person ny_person);
    void   skrivUtOchFixa(ostream &os);
    double summaSkyldig() const;
    double summaBetalat() const;
    bool   finnsPerson(const string& namn);

    void resize(int n);  //Resize metoden ändrar storleken på den dynamiska arrayen.

};


class TransaktionsLista
{
private:
    int         antal_transaktioner=0;
    Transaktion *transaktioner;
public:
    TransaktionsLista();
    ~TransaktionsLista();
    void   lasIn(istream & is);
    void   skrivUt(ostream & os);
    void   laggTill(Transaktion & t);
    double totalKostnad() const;
    double liggerUteMed(const string &namnet);
    double arSkyldig(const string &namnet);


    void resize(int n);             //Jag lägger till 2 metoder. Resize ändrar storleken på den dynamiska arrayen
    void addToEnd(Transaktion &t);  //addToEnd lägger till ett objekt sist i arrayen.


    PersonLista FixaPersoner();
};

int main()
{
    setlocale(LC_ALL,"");
    cout << "Startar med att läsa från en fil." << endl;


    TransaktionsLista transaktioner;
    std::ifstream     is("resa.txt");
    if(is)
    {
        transaktioner.lasIn(is);


        int operation = 1;
        while (operation != 0)
        {
            cout << endl;
            cout << "Välj i menyn nedan:" << endl;
            cout << "0. Avsluta. Alla transaktioner sparas på fil." << endl;
            cout << "1. Skriv ut information om alla transaktioner." << endl;
            cout << "2. Läs in en transaktion från tangentbordet." << endl;
            cout << "3. Beräkna totala kostnaden." << endl;
            cout << "4. Hur mycket ärr en viss person skyldig?" << endl;
            cout << "5. Hur mycket ligger en viss person ute med?" << endl;
            cout << "6. Lista alla personer mm och FIXA" << endl;

            cin >> operation;
            cout << endl;

            switch (operation)
            {
            case 1:
            {
                transaktioner.skrivUt(cout);
                break;
            }
            case 2:
            {
                Transaktion transaktion;
                cout << "Ange transaktion i följande format på separata rader" << endl;
                transaktion.skrivTitel(cout);
                transaktion.lasIn(cin);
                transaktioner.laggTill(transaktion);
                break;
            }
            case 3:
            {
                cout << "Den totala kostnanden för resan var "
                     << transaktioner.totalKostnad() << endl;
                break;
            }
            case 4:
            {
                cout << "Ange personen: ";
                string namn;
                cin >> namn;
                double ar_skyldig = transaktioner.arSkyldig(namn);
                if (ar_skyldig == 0.)
                    cout << "Kan inte hitta personen " << namn << endl;
                else
                    cout << namn << " är skyldig " << ar_skyldig << endl;
                break;
            }
            case 5:
            {
                cout << "Ange personen: ";
                string namn;
                cin >> namn;
                double ligger_ute_med = transaktioner.liggerUteMed(namn);
                if (ligger_ute_med == 0.)
                    cout << "Kan inte hitta personen " << namn << endl;
                else
                    cout << namn << " ligger ute med " << ligger_ute_med << endl;
                break;
            }
            case 6:
            {
                cout << "Nu skapar vi en personarray och reder ut det hela!" << endl;
                PersonLista lista = transaktioner.FixaPersoner();
                lista.skrivUtOchFixa(cout);
                break;
            }
            }
        }

        std::ofstream os("transaktioner.txt");
        transaktioner.skrivUt(os);


        return 0;
    }
}
PersonLista::PersonLista()//Konstruktor för PersonLsta
{
}
PersonLista::~PersonLista()//Destructor för PersonLista
{
    delete [] personer; //Avallokerar minne för personer
    personer =0;
}

PersonLista & PersonLista :: operator=( const PersonLista& p)
{
    if (this != &p)
    {

        antal_personer  = p.antal_personer;
        personer       = new Person[antal_personer];
        for (int i=0; i < antal_personer; i++)
            personer[i] = p.personer[i];
    }
    return *this;

}

Person::Person()//Konstruktor för Person
{
    namn="";
    betalat_andras=0;
    skyldig=0;
}
Person::Person(const string &namn, double betalat, double skyldig)//Överlagrad konstruktor för Person
{
    (*this).namn = namn;
    betalat_andras=betalat;
    this->skyldig = skyldig;
}
Transaktion::Transaktion()//Konstruktor för Transaktion
{
    belopp=0;
}
Transaktion::~Transaktion()//Destructor för Transaktion
{
    delete [] kompisar; //Avallokerar minne för den dynamiska arrayen.
    kompisar =0;        //Och sätter pekaren att peka på 0
}
Transaktion& Transaktion::operator=( const Transaktion & t)
{
    if (this != &t)
    {

        datum          = t.datum;
        typ            = t.typ;
        namn           = t.namn;
        belopp         = t.belopp;
        antal_kompisar = t.antal_kompisar;
        kompisar       = new string[antal_kompisar];
        for (int i=0; i < antal_kompisar; i++)
            kompisar[i] = t.kompisar[i];
    }

    return *this;
}
TransaktionsLista::TransaktionsLista()//Konstruktor för TransaktionsLista
{
    antal_transaktioner=0;
    transaktioner=0;
}

TransaktionsLista:: ~TransaktionsLista()//Destructor för TransaktionsLista
{
    delete [] transaktioner;    //Avallokerar minne för transaktioner
    transaktioner =0;
}

//Metod i Transaktion som returnerar  antalet kompisar
int Transaktion::hamtaAntalKompisar() const
{
    return antal_kompisar;
}

//Metod i Transaktion som returnerar beloppet
double Transaktion::hamtaBelopp() const
{
    return belopp;
}

//Metod i Transaktion som returnerar ett namn som en string
string Transaktion::hamtaNamn() const
{
    return namn;
}

//Funktion i Transaktion som kontrollera om namnet finns i transaktionen.
//om det finns så returnerar metoden true annars false
bool Transaktion::finnsKompis(const string &namnet) const
{
    for(int i =0; i<antal_kompisar; i++)
    {
        if(namnet==kompisar[i])
            return true;
    }
    return false;
}

//Metoden i Transaktion som skriver ut info on transaktionen antingen till konsollen eller till en fil
void  Transaktion::skrivUt(ostream &os) const
{
    os<<datum<<setw(15)<<typ<<setw(15)<<namn<<setw(15)<<belopp<<setw(15)<<antal_kompisar<<setw(15);
    for(int i =0; i<antal_kompisar; i++)
    {
        os<<kompisar[i]<<'\t';
    }

}

//Metoden som skriver ut titeln i klassen Transaktion
void  Transaktion::skrivTitel(ostream &os)const
{
    os<<"Datum"<<setw(15)<<"Typ"<<setw(15)<<"Namn"<<setw(15)<<setw(15)<<"Belopp"<<setw(20)<<"Antal kompisar"<<setw(20)<<"Ange kompisar"<<endl;
}

//Metod i transaktion som laser in
bool Transaktion::lasIn(istream &is)
{
    bool lyckades =false;

    is>>datum;

    if(!is.eof()) //om inläsningen av datum lyckades så körs if-satsen och attributen tilldelas inkommande värden
    {
        is>>typ;
        is>>namn;
        is>>belopp;
        is>>antal_kompisar;

        kompisar = new string[antal_kompisar]; //Börjar använda den dynamiska arrayen, sätter den til storleken av antal_kompisar
        for(int i=0; i < antal_kompisar; i++) //läser in alla kompisar beroende på hur många de är
        {
            is>>kompisar[i];
        }

        lyckades =true; //Bool variabeln blir sann eftersom inläsningen lyckades
    }

//Returnerar bool-variabeln

    return lyckades;
}

//Metod i Person-klassen som returnerar namnet på personen
string Person::hamtaNamn()
{
    return namn;
}

//Metod i klassen person som returnerar hur mycket  den betalat andras
double Person::hamtaBetalat()
{
    return betalat_andras;
}

//Returnerar vad personen är skylldig
double Person::hamtaSkyldig()
{
    return skyldig;
}

//Metoden i klassen Person som srkiver ut info om vad personen är skyldig eller ska ha från pottem
void Person::skrivUt(ostream &os)
{

    if(betalat_andras>skyldig) //Om betalat är större än skyldig så körs denna if-sats
    {
        double resultat = betalat_andras-skyldig;
        os<<namn<<" ligger ute med "<<betalat_andras<<" och är skyldig ";
        os<<skyldig<<". skall ha "<<resultat<<" från potten!"<<endl;
    }
    if(betalat_andras<skyldig)//Om betalat är mindre än skyldig så körs denna if-sats
    {
        double resultat = skyldig-betalat_andras;
        os<<namn<<" ligger ute med "<<betalat_andras<<" och är skyldig ";
        os<<skyldig<<". Skall betala "<<resultat<<" till potten!"<<endl;
    }
    else if(betalat_andras==skyldig) //Om personen inte är skyldig eller ska ha några pengar körs denna if-sats
    {
        os<<namn<<" ligger ute med "<<betalat_andras<<" och är skyldig ";
        os<<skyldig<<". Allt har gått jämt ut, ska inte ta eller ge nått till potten."<<endl;
    }
}

//Metoden som lägger till en ny person i personlistan.
void PersonLista::laggTill(Person ny_person)
{

    resize(antal_personer+1); //Kallar på metoden som ändrar storleken på arrayen
    personer[antal_personer-1]=ny_person; //lägger till ny_person objektet i arrayen sist i listan

}
void PersonLista::resize(int n) //Metoden som ändrar storlek på arrayen.
{
    Person *p = 0; //Skapar en ny pekare av typen person som jag sätter ska peka på 0

    if ( n > 0 )    //om storleken (n) är över 0 så körs denna if-sats
    {
        p = new Person[n]; // Gör så att p pekar på en nyskapad array av typen person med n storlek

        int i;
        for ( i = 0; i < n
                && i < antal_personer; i++ )    //Kopierar de som finns i personer till p.
        {
            p[i] = personer[i];
        }

    }

    delete [] personer; // Avallokera minne.
    personer = p;       // Sätt array till nya arrayen.
    antal_personer = n; //ändrar värddet på antal_personer till n

}
//Metoden som skriver ut och fixar iordning hur läget i potten ser ut
void PersonLista::skrivUtOchFixa(ostream &os)
{
    double potten=0;
    for(int i =0; i<antal_personer; i++) //Loopar igenom alla personer i klassen
    {
        double betalat=0;// 2 lokala variabler som uppdateras i varje loop
        double skyldig =0;

        betalat=personer[i].hamtaBetalat(); //Hämtar värdena för personen på plats [i] och tilldelar de till de lokala variablerna
        skyldig=personer[i].hamtaSkyldig();

        potten =potten+(betalat-skyldig); // Uppdaterar potten med värdena som hämtats

        if(betalat>skyldig) //om personen betlat mer än den är skyldig så körs denna if-sats
            os<<personer[i].hamtaNamn()<<" ligger ute med "<<betalat<<" och är skyldig "<<skyldig<<". Skall ha "<<double(betalat-skyldig)<<" från potten!"<<endl;
        if(betalat<skyldig) //Om den är skyldig mer körs denna if-sats
            os<<personer[i].hamtaNamn()<<" ligger ute med "<<betalat<<" och är skyldig "<<skyldig<<". Skall betala "<<double(betalat-skyldig)*-1<<" till potten!"<<endl;
    }
    if(potten==0) //Kontrollerar hur läget i potten ser ut och gör en utskrift om det.
        os<<"potten är "<<potten<<". Allt stämmer"<<endl;
    else
    {
        os<<"Potten är inte 0, något har blivit fel!"<<endl;
    }
}

//Meoden som kontrollerar om en viss person finns i personlista. Returnerar true eller false beroende på om den finns eller ej
bool  PersonLista::finnsPerson(const string& namn)
{
    for (int i =0; i<antal_personer; i++)
    {
        if(personer[i].hamtaNamn()==namn) //om namnet på plats i är = parameter-namnet så returneras true
            return true;
    }
    //Transaktion nya (trans);
    return false;
}

//Metoden som läser in information
void   TransaktionsLista::lasIn(istream & is)
{

    Transaktion trans; //Skapar ett objekt av typen transaktion
    while(trans.lasIn(is)) //Sålänge det går att läsa in i objektets metod lasIn så körs metoden läggTill där vi skickar med ett transaktions-objekt
    {
        {
            laggTill (trans);
        }
    }
}

//Metod som skriver ut info om alla transaktioner
void TransaktionsLista::skrivUt(ostream &os)
{
    os <<"Antal trans = "<<antal_transaktioner<<endl;
    for(int i =0; i<antal_transaktioner; i++)
    {
        transaktioner[i].skrivUt(os); //Kallar på metoden skrvUt i klassen transaktion för objektet på platsen i
        os<<endl; //Byter rad
    }

}


// MEtod som tar emot ett transaktions-objek och lägger till det i arrayen transaktioner
void TransaktionsLista::laggTill(Transaktion & t)
{
    resize(antal_transaktioner+1); //kallar på metoden som ändar storleken  arrayen

    transaktioner[antal_transaktioner-1]=t; //Sätter in transaktionsobjektet sist i arrayen
}



void TransaktionsLista::resize(int n) //Metoden som ändrar storlek på arrayen.
{
    Transaktion *p = 0; //Skapar en ny pekare av typen Transaktion som jag sätter ska peka på 0

    if ( n > 0 )     //om storleken (n) är över 0 så körs denna if-sats
    {
        p = new Transaktion[n]; // Gör så att p pekar på en nyskapad array av typen Transaktion med n storlek

        int i;
        for ( i = 0; i < n   // Fyll med element.
                && i < antal_transaktioner; i++ )
        {
            p[i] = transaktioner[i]; //Kopierar de som finns i transaktoner till p
        }

    }

    delete [] transaktioner; // Avallokera minne.

    transaktioner = p;       // Sätt array till nya arrayen.
    antal_transaktioner = n; //sätter antal_transaktioner till n

}

//Metod som hämtar totalkostnade för hela resan.
double TransaktionsLista::totalKostnad() const
{
    double summa=0;
    for(int i=0; i<antal_transaktioner; i++)
        summa=summa+transaktioner[i].hamtaBelopp();
    return summa;
}

//Metoden som hämtar hur mycket en viss person i transaktionslistan ligger ute med
double TransaktionsLista::liggerUteMed(const string &namn)
{
    double summa =0;

    for (int i = 0; i < antal_transaktioner; i++) //Loopar igenom alla transaktioner
        if (transaktioner[i].hamtaNamn() == namn) //Om namnet på platsen i arrayen för loopen stämmer överens med parametern namn så körs if-satsen
            summa += transaktioner[i].hamtaBelopp() *
                     (1.0 - 1.0 / (transaktioner[i].hamtaAntalKompisar()+ 1)); //Räknar ut hur mycket en person ligger ute med
    return summa;//Returnerar resultatet för uträkningen
}

//Metoden som kontrollerar hyr mycket en viss person är skyldig de andra
double TransaktionsLista::arSkyldig(const string &namn)
{
    double summa =0;

    for (int i = 0; i < antal_transaktioner; i++) //Loopar igenom listan
        if (transaktioner[i].finnsKompis(namn))// Om personen på platsen i listan i loopen är samma som namnet som skickas in körs if-satsen
        {
            summa += transaktioner[i].hamtaBelopp() /(transaktioner[i].hamtaAntalKompisar()+ 1); //Uppdaterar summa med summa + de belopp som hämtas / antal kompisar + 1.
        }
    return summa; //Returnerar vad personen är skyldig andra
}

// Metoden som tar fram alla egenskaper för personerna och lägger till de i personlistan
PersonLista  TransaktionsLista::FixaPersoner()
{
    int tal =0; //Lokal variabel som kommer användas senare
    PersonLista person; //Skapar ett objekt av typen PersonLista
    Person *p = 0;      //Skapar en pekare p av typen Person som pekar på 0
    p   = new Person[antal_transaktioner];      //sätter att p ska peka på en personArray med antal_transaktoner som storlek

    for (int i =0; i<antal_transaktioner; i++) //Loopar igenom alla transaktioner
    {
        bool finns=false; //Skapar en lokal bool-variabel
        string namn = transaktioner[i].hamtaNamn(); //Skapar en lokal variabel av typen string som blir namnet som betalat transaktionen
        for(int j =0; j<antal_transaktioner; j++) //Loopar igenom antal_transaktioner igen
        {
            if(namn==p[j].hamtaNamn()) //Om namnet finns i den lokala arrayen så sätts finns till true
            {
                finns = true;
            }
        }
        if(finns==false) // om finns är false betyder det att personen inte fanns i den lokala arrayen så då körs denna if-sats
        {
            //Hämtar all information om personen på platsen namn och skapar ett nytt personobjekt där jag skickar med namn, vad den ligger ute med och är skyldig till konstruktorn
            //De nyskapta objektet läggs till i den lokala dynamiska arrayen på plats tal.
            double uteMed=liggerUteMed(namn);
            double skyldig=arSkyldig(namn);
            p[tal]=Person(namn,uteMed, skyldig);
            tal++; //Adderar variabeln tal med 1
        }

    }


    for (int i=0; i<tal; i++) //I och med att det bara finns värden på platsen mindre än tal tar jag hjälp av den variabeln i en loop
        person.laggTill(p[i]); //Lägger till person-objektet på platsen i till personlista objektet

    delete[] p; //Avvallokerar minne för p och sätter den till att peka på 0
    p=0;
    return person; //Returnerar personlistan

}
