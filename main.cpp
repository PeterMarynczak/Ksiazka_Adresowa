#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>

using namespace std;

struct Recipient {
    int id = 0;
    string name = "", surname = "", phoneNr = "", email = "", address = "";
};

int addNewFriend(vector<Recipient> &recipients, int recipientsAmount);
vector<string> split(string& linia, char delimiter);
int loadAllFriendsFromFile(vector<Recipient> &recipients);
void viewRecipientsFromFile (vector<Recipient> &recipients, int recipientsAmount);
void searchRecipientByName (vector<Recipient> &recipients, int recipientsAmount);
void searchRecipientBySurname (vector<Recipient> &recipients, int recipientsAmount);
void deleteRecipient (vector<Recipient> &recipients, int &recipientsAmount);
void loadRecipientsToAFile(fstream &plik, vector<Recipient> &recipients);
void editRecipient (vector<Recipient> &recipients);


int main() {

    vector<Recipient> recipients;
    int recipientsAmount = loadAllFriendsFromFile(recipients);

    char choiceNr;

    while (true) {

        system("cls");
        cout << "KSIAZKA ADRESOWA" << endl;
        cout << "1. Dodaj adresata" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl wszystkich adresatow" << endl;
        cout << "5. Usun adresata" << endl;
        cout << "6. Edytuj adresata" << endl;
        cout << "9. Zakoncz program" << endl;
        cout << "Twoj wybor: ";

        choiceNr = getch();

        if (choiceNr == '1') {
            recipientsAmount = addNewFriend(recipients, recipientsAmount);
        }
        if (choiceNr == '2') {
            loadAllFriendsFromFile(recipients);
            searchRecipientByName (recipients, recipientsAmount);
        }
        if (choiceNr == '3') {
            loadAllFriendsFromFile(recipients);
            searchRecipientBySurname (recipients, recipientsAmount);
        }
        if (choiceNr == '4') {
            viewRecipientsFromFile (recipients, recipientsAmount);
        }
        if (choiceNr == '5') {
            deleteRecipient (recipients, recipientsAmount);
        }
        if (choiceNr == '6') {
            editRecipient (recipients);

        } else if (choiceNr == '9') {
            exit(0);
        }
    }
    return 0;
}

int addNewFriend(vector<Recipient> &recipients, int recipientsAmount) {

    string name, surname, phoneNr, email, address;

    system("cls");
    cout << "Dodawanie osoby" << endl;
    cout << "Podaj imie uzytkownika: ";
    cin >> name;
    cout << "Podaj nazwisko uzytkownika: ";
    cin >> surname;
    cout << "Podaj numer telefonu uzytkownika: ";
    cin.sync(); // oprozniam bufor
    getline(cin, phoneNr); // wczytuje razem ze spacja
    cout << "Podaj email uzytkownika: ";
    cin >> email;
    cout << "Podaj adres uzytkownika: ";
    cin.sync();
    getline (cin, address);

    int id;
    if (recipientsAmount == 0)
        id = recipientsAmount + 1;
    else {
        for (int i = 0; i < recipients.size(); i++) {
            if(i == recipients.size() - 1) {
                id = recipients[i].id + 1;
            }
        }
    }

    Recipient recipientData;

    recipientData.id = id;
    recipientData.name = name;
    recipientData.surname = surname;
    recipientData.phoneNr = phoneNr;
    recipientData.email = email;
    recipientData.address = address;
    recipients.push_back(recipientData);

    cout<<"Osoba zostala dodana";
    Sleep(2000);

    fstream plik;
    plik.open("KsiazkaAdresowa.txt", ios::out | ios::app);

    if (plik.good() == true) {
        plik << id << "|";
        plik << name << "|";
        plik << surname << "|";
        plik << phoneNr << "|";
        plik << email << "|";
        plik << address << "|" << endl;
        plik.close();
    } else {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        system("pause");
    }
    return recipientsAmount + 1;
}

vector<string> split(string& linia, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(linia);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int loadAllFriendsFromFile(vector<Recipient> &recipients) {

    Recipient recipientData;
    int recipientsAmount = 0;
    int laneNumber = 1;
    string linia;
    char delimiter = '|';
    vector<string> newSplit; // = split(linia, delimiter);

    fstream plik;
    plik.open("KsiazkaAdresowa.txt",ios::in);

    if (plik.good() == true) {
        while(getline(plik,linia)) {

            switch(laneNumber) {

            case 1:
                int i = 0;
                vector<string> newSplit = split(linia, delimiter);
                recipientData.id = atoi(newSplit[i].c_str());
                recipientData.name = newSplit[i+1];
                recipientData.surname = newSplit[i+2];
                recipientData.phoneNr = newSplit[i+3];
                recipientData.email = newSplit[i+4];
                recipientData.address = newSplit[i+5];
                recipients.push_back(recipientData);
                newSplit.clear();

                if (laneNumber == 1) {
                    recipientsAmount++;
                    laneNumber = 0;
                }
                laneNumber++;
            }
        }
        plik.close();
    }

    else if ( plik.good() == false) {
        cout << "Nie udalo sie otworzyc pliku!";
        plik.open("KsiazkaAdresowa.txt",ios::out | ios::app);
    }
    return recipientsAmount;
}

void viewRecipientsFromFile (vector<Recipient> &recipients, int recipientsAmount) {

    system("cls");
    cout << "Lista osob:" << endl << endl;

    for (int i = 0; i < recipientsAmount; i++) {

        cout << "ID:           " << recipients[i].id << endl;
        cout << "Imie:         " << recipients[i].name << endl;
        cout << "Nazwisko:     " << recipients[i].surname << endl;
        cout << "NrTelefonu:   " << recipients[i].phoneNr << endl;
        cout << "Email:        " << recipients[i].email << endl;
        cout << "Adres:        " << recipients[i].address << endl << endl;
    }
    if (recipientsAmount == 0)
        {
            cout << "Lista osob jest pusta" << endl << endl;
        }
    system("pause");
}

void searchRecipientByName (vector<Recipient> &recipients, int recipientsAmount) {

    string imieDoWyszukania;
    system("cls");

    cout << "Podaj imie do wyszukania: ";
    cin >> imieDoWyszukania;
    cout << "Oto lista twoich znajomych o imieniu: " << imieDoWyszukania << endl;

    int i = 0;
    while (i < recipientsAmount) {
        if (recipients[i].name == imieDoWyszukania) {
            cout << "id: " << recipients[i].id << endl;
            cout << recipients[i].name << " " << recipients[i].surname << endl;
            cout << "zamieszkaly w " << recipients[i].address << endl;
            cout << "numer tel. " << recipients[i].phoneNr << endl;
            cout << "adres email: " << recipients[i].email << endl;
            cout << endl;
        }
        i++;
    }
    system("pause");
}

void searchRecipientBySurname (vector<Recipient> &recipients, int recipientsAmount) {

    string nazwiskoDoWyszukania;
    system("cls");

    cout << "Podaj imie do wyszukania: ";
    cin >> nazwiskoDoWyszukania;
    cout << "Oto lista twoich znajomych o nazwisku: " << nazwiskoDoWyszukania << endl;

    int i = 0;
    while (i < recipientsAmount) {
        if (recipients[i].surname == nazwiskoDoWyszukania) {
            cout << "id: " << recipients[i].id << endl;
            cout << recipients[i].name << " " << recipients[i].surname << endl;
            cout << "zamieszkaly w " << recipients[i].address << endl;
            cout << "numer tel. " << recipients[i].phoneNr << endl;
            cout << "adres email: " << recipients[i].email << endl;
            cout << endl;
        }
        i++;
    }
    system("pause");
}

void deleteRecipient (vector<Recipient> &recipients, int &recipientsAmount) {

    string id, name, surname, phoneNr, email, address;
    int idNr;
    char choiceNr;

    cout << endl;
    cout << "Podaj id osoby do usuniecia z Ksiazki adresowej: ";
    cin >> idNr;
    stringstream ss;
    ss << idNr;
    string idInString = ss.str();

    cout << "Czy na pewno chcesz aby Adresat o id: " << idInString << " zostal usuniety" << endl;
    cout << "wybierz (t) jezeli tak" << endl;
    choiceNr = getch();

    if (choiceNr == 't') {
        for (int i = 0; i < recipients.size(); i++) {
            if(recipients[i].id == idNr) {
                recipients.erase(recipients.begin() + i);
            }
        }
        fstream plik;
        plik.open("KsiazkaAdresowa.txt",ios::out);

        if (plik.good() == true) {
            for (int i = 0; i < recipients.size(); i++) {
                plik << recipients[i].id << "|";
                plik << recipients[i].name << "|";
                plik << recipients[i].surname << "|";
                plik << recipients[i].phoneNr << "|";
                plik << recipients[i].email << "|";
                plik << recipients[i].address << "|" << endl;
            }
            plik.close();
        } else {
            cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
            system("pause");
        }
        plik.close();
        recipientsAmount--;
        cout<<"Adresat o id: " << idInString << " zostal usuniety" << endl;
        Sleep(2000);
    }
}

void loadRecipientsToAFile(vector<Recipient> &recipients) {

    fstream plik;
    plik.open("KsiazkaAdresowa.txt",ios::out);

    if (plik.good() == true) {
        for (int i = 0; i < recipients.size(); i++) {
            plik << recipients[i].id << "|";
            plik << recipients[i].name << "|";
            plik << recipients[i].surname << "|";
            plik << recipients[i].phoneNr << "|";
            plik << recipients[i].email << "|";
            plik << recipients[i].address << "|" << endl;
        }
        plik.close();
    }
}

void editRecipient (vector<Recipient> &recipients) {

    int idOfRecipientToEdit;
    char choiceNr;
    string editParticularInfoAboutRecipient;

    system("cls");
    cout << "Wybierz id adresata, ktorego chcesz edytowac: ";
    cin >> idOfRecipientToEdit;
    system("cls");
    cout << "Ktora dana adresata chcesz edytowac: " << endl;
    cout << "1. imie" << endl;
    cout << "2. nazwisko" << endl;
    cout << "3. numer telefonu" << endl;
    cout << "4. email" << endl;
    cout << "5. adres" << endl;
    cout << "6. powrot do menu" << endl;
    cout << "Twoj wybor: ";
    choiceNr = getch ();

    switch(choiceNr) {
    case '1': {
        cout << "Wprowadz nowe imie adresata: ";
        cin >> editParticularInfoAboutRecipient;
        for (int i = 0; i < recipients.size(); i++) {
            if(recipients[i].id == idOfRecipientToEdit) {
                recipients[i].name = editParticularInfoAboutRecipient;
            }
        }
        loadRecipientsToAFile (recipients);
        break;
    }
    case '2': {
        cout << "Wprowadz nowe nazwisko adresata: ";
        cin >> editParticularInfoAboutRecipient;
        for (int i = 0; i < recipients.size(); i++) {
            if(recipients[i].id == idOfRecipientToEdit) {
                recipients[i].surname = editParticularInfoAboutRecipient;
            }
        }
        loadRecipientsToAFile (recipients);
        break;
    }
    case '3': {
        cout << "Wprowadz nowy numer telefonu adresata: ";
        cin.sync();
        getline(cin, editParticularInfoAboutRecipient);
        for (int i = 0; i < recipients.size(); i++) {
            if(recipients[i].id == idOfRecipientToEdit) {
                recipients[i].phoneNr = editParticularInfoAboutRecipient;
            }
        }
        loadRecipientsToAFile (recipients);
        break;
    }
    case '4': {
        cout << "Wprowadz nowy adres email adresata: ";
        cin.sync();
        getline(cin, editParticularInfoAboutRecipient);
        for (int i = 0; i < recipients.size(); i++) {
            if(recipients[i].id == idOfRecipientToEdit) {
                recipients[i].email = editParticularInfoAboutRecipient;
            }
        }
        loadRecipientsToAFile (recipients);
        break;
    }
    case '5': {
        cout << "Wprowadz nowy adres adresata: ";
        cin.sync();
        getline(cin, editParticularInfoAboutRecipient);
        for (int i = 0; i < recipients.size(); i++) {
            if(recipients[i].id == idOfRecipientToEdit) {
                recipients[i].address = editParticularInfoAboutRecipient;
            }
        }
        loadRecipientsToAFile (recipients);
        break;
    }
    case '6':
        break;
    }
}

