#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>

using namespace std;

struct User {
    int idUser;
    string userName, password;
};

struct Recipient {
    int id = 0;
    int idUser = 0;
    string name = "", surname = "", phoneNr = "", email = "", address = "";
};

vector<string> split(string& linia, char delimiter);
int loadAllFriendsFromFile(vector<Recipient> &recipientsTemporary);
void viewRecipientsFromFile (vector<Recipient> &recipients, int recipientsAmount);
void searchRecipientByName (vector<Recipient> &recipients);
void searchRecipientBySurname (vector<Recipient> &recipients);
void loadRecipientsToAFile(vector<Recipient> &recipients, int loggedUserID);
void editRecipient (vector<Recipient> &recipients, int loggedUserID);
void changePassword(vector<User> &usersContainer, int usersAmount, int loggedUserID);
void loadUsersToAFile(vector<User> &usersContainer);
void addNewFriend(vector<Recipient> &recipients, vector<Recipient> &recipientsTemporary, int loggedUserID);
int loadUsersFromFile (vector<User> &usersContainer);
int logToTheSystem (vector<User> &usersContainer, int usersAmount);
void userRegistration (vector<User> &usersContainer, int &usersAmount);
void updateVectorOfRecipients (vector<Recipient> &recipients, int loggedUserID);
void deleteRecipient (vector<Recipient> &recipients, vector<Recipient> &recipientsTemporary, int loggedUserID);

void enterNewFriendToFile(Recipient recipientData) {
    fstream plik;
    plik.open("Adresaci.txt", ios::out | ios::app);

    if (plik.good() == true) {
        plik << recipientData.id << "|";
        plik << recipientData.idUser << "|";
        plik << recipientData.name << "|";
        plik << recipientData.surname << "|";
        plik << recipientData.phoneNr << "|";
        plik << recipientData.email << "|";
        plik << recipientData.address << "|" << endl;
        plik.close();
    } else {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        system("pause");
    }
}

int main() {

    vector<User> usersContainer;
    vector<Recipient> recipients;
    vector<Recipient> recipientsTemporary;
    recipients = recipientsTemporary;

    int loggedUserID = 0;
    int usersAmount = loadUsersFromFile (usersContainer);
    int recipientsAmount = loadAllFriendsFromFile(recipientsTemporary);

    char wybor;
    char choiceNr;

    while(1) {
        if (loggedUserID == 0) {
            system ("cls");
            cout << "1. Logowanie" << endl;
            cout << "2. Rejestracja" << endl;
            cout << "9. Zakoncz program" << endl;
            cin >> wybor;

            if (wybor == '1') {
                loggedUserID = logToTheSystem (usersContainer, usersAmount);
                updateVectorOfRecipients (recipients, loggedUserID);
            }

            if (wybor == '2') {
                userRegistration (usersContainer, usersAmount);
            }

            if (wybor == '9') {
                exit(0);
            }
        }

        else {
            system ("cls");
            cout << "KSIAZKA ADRESOWA" << endl;
            cout << "1. Dodaj adresata" << endl;
            cout << "2. Wyszukaj po imieniu" << endl;
            cout << "3. Wyszukaj po nazwisku" << endl;
            cout << "4. Wyswietl wszystkich adresatow" << endl;
            cout << "5. Usun adresata" << endl;
            cout << "6. Edytuj adresata" << endl;
            cout << "7. Zmiana hasla" << endl;
            cout << "8. Wylogowanie" << endl;
            cout << "Twoj wybor: ";

            choiceNr = getch();

            if (choiceNr == '1') {
                addNewFriend(recipients, recipientsTemporary, loggedUserID);
            }
            if (choiceNr == '2') {
                searchRecipientByName(recipients);
            }
            if (choiceNr == '3') {
                searchRecipientBySurname (recipients);
            }
            if (choiceNr == '4') {
                viewRecipientsFromFile (recipients, recipientsAmount);
            }
            if (choiceNr == '5') {
                deleteRecipient (recipients, recipientsTemporary, loggedUserID);
            }
            if (choiceNr == '6') {
                editRecipient (recipients, loggedUserID);
            }
            if (choiceNr == '7') {
                changePassword(usersContainer, usersAmount, loggedUserID);
                loadUsersToAFile(usersContainer);
            } else if (choiceNr == '8') {
                loggedUserID = 0;
            }
        }
    }
    return 0;
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

int loadAllFriendsFromFile(vector<Recipient> &recipientsTemporary) {

    Recipient recipientData;
    int recipientsAmount = 0;
    int laneNumber = 1;
    string linia;
    char delimiter = '|';
    vector<string> newSplit;

    fstream plik;
    plik.open("Adresaci.txt",ios::in);

    if (plik.good() == true) {
        while(getline(plik,linia)) {

            switch(laneNumber) {

            case 1:
                int i = 0;
                vector<string> newSplit = split(linia, delimiter);
                recipientData.id = atoi(newSplit[i].c_str());
                recipientData.idUser = atoi(newSplit[i+1].c_str());
                recipientData.name = newSplit[i+2];
                recipientData.surname = newSplit[i+3];
                recipientData.phoneNr = newSplit[i+4];
                recipientData.email = newSplit[i+5];
                recipientData.address = newSplit[i+6];
                recipientsTemporary.push_back(recipientData);
                newSplit.clear();

                if (laneNumber == 1) {
                    laneNumber = 0;
                }
                laneNumber++;
            }
        }
        plik.close();
    } else if ( plik.good() == false) {
        cout << "Nie udalo sie otworzyc pliku!";
        plik.open("Adresaci.txt",ios::out | ios::app);
    }

    if (recipientsTemporary.empty() == true) {
        recipientsAmount = 0;
    } else {
        recipientsAmount = recipientsTemporary.back().id;
    }
    return recipientsAmount;
}

void viewRecipientsFromFile (vector<Recipient> &recipients, int recipientsAmount) {

    system("cls");
    cout << "Lista osob:" << endl << endl;

    for (int i = 0; i < recipients.size(); i++) {

        cout << "ID:           " << recipients[i].id << endl;
        cout << "IDUser:       " << recipients[i].idUser << endl;
        cout << "Imie:         " << recipients[i].name << endl;
        cout << "Nazwisko:     " << recipients[i].surname << endl;
        cout << "NrTelefonu:   " << recipients[i].phoneNr << endl;
        cout << "Email:        " << recipients[i].email << endl;
        cout << "Adres:        " << recipients[i].address << endl << endl;
    }
    if (recipientsAmount == 0) {
        cout << "Lista osob jest pusta" << endl << endl;
    }
    system("pause");

}
void searchRecipientByName (vector<Recipient> &recipients) {

    string imieDoWyszukania = "";
    int iloscAdresatow = 0;

    system("cls");

    cout << ">>> WYSZUKIWANIE ADRESATOW O IMIENIU <<<" << endl << endl;

    cout << "Wyszukaj adresatow o imieniu: ";
    cin >> imieDoWyszukania;

    for (vector<Recipient>::iterator  itr = recipients.begin(); itr != recipients.end(); itr++) {
        if (itr->name == imieDoWyszukania) {
            cout << endl;
            cout << "Id:                 " << itr->id << endl;
            cout << "Imie:               " << itr->name << endl;
            cout << "Nazwisko:           " << itr->surname << endl;
            cout << "Numer Telefonu:     " << itr->phoneNr << endl;
            cout << "Nr Email:           " << itr->email << endl;
            cout << "Adres:              " << itr->address << endl;
            iloscAdresatow++;
        }
    }
    if (iloscAdresatow == 0) {
        cout << endl << "Nie ma adresatow z tym imieniem w ksiazce adresowej" << endl;
    } else {
        cout << endl << "Ilosc adresatow z imieniem: >>> " << imieDoWyszukania << " <<<";
        cout << " w ksiazce adresowej wynosi: " << iloscAdresatow << endl << endl;
    }
    system("pause");
}

void searchRecipientBySurname (vector<Recipient> &recipients) {

    string nazwiskoDoWyszukania = "";
    int iloscAdresatow = 0;

    system("cls");

    cout << ">>> WYSZUKIWANIE ADRESATOW O NAZWISKU <<<" << endl << endl;

    cout << "Wyszukaj adresatow o nazwisku: ";
    cin >> nazwiskoDoWyszukania;

    for (vector<Recipient>::iterator  itr = recipients.begin(); itr != recipients.end(); itr++) {
        if (itr->surname == nazwiskoDoWyszukania) {
            cout << endl;
            cout << "Id:                 " << itr->id << endl;
            cout << "Imie:               " << itr->name << endl;
            cout << "Nazwisko:           " << itr->surname << endl;
            cout << "Numer Telefonu:     " << itr->phoneNr << endl;
            cout << "Nr Email:           " << itr->email << endl;
            cout << "Adres:              " << itr->address << endl;
            iloscAdresatow++;
        }
    }
    if (iloscAdresatow == 0) {
        cout << endl << "Nie ma adresatow o tym nazwisku w ksiazce adresowej" << endl;
    } else {
        cout << endl << "Ilosc adresatow o nazwisku: >>> " << nazwiskoDoWyszukania << " <<<";
        cout << " w ksiazce adresowej wynosi: " << iloscAdresatow << endl << endl;
    }
    system("pause");
}

void loadRecipientsToAFile(vector<Recipient> &recipients, int loggedUserID) {

    fstream plik;
    fstream temp;
    Recipient recipientData;

    plik.open("Adresaci.txt",ios::in);
    temp.open("Adresaci_tymczasowy.txt",ios::out);

    int laneNumber = 1;
    string linia;
    char delimiter = '|';
    vector<string> newSplit;
    int temporary = 0;

    if (plik.good() == true) {
        while(getline(plik,linia)) {

            switch(laneNumber) {

            case 1:
                int i = 0;
                vector<string> newSplit = split(linia, delimiter);

                if(atoi(newSplit[i+1].c_str()) != loggedUserID) {
                    temp << linia << endl;
                    newSplit.clear();
                } else {
                    temp << recipients[temporary].id << "|";
                    temp << recipients[temporary].idUser << "|";
                    temp << recipients[temporary].name << "|";
                    temp << recipients[temporary].surname << "|";
                    temp << recipients[temporary].phoneNr << "|";
                    temp << recipients[temporary].email << "|";
                    temp << recipients[temporary].address << "|" << endl;
                    temporary++;
                }
                if (laneNumber == 1) {
                    laneNumber = 0;
                }
                laneNumber++;
            }
        }
    }
    plik.close();
    temp.close();
    remove("Adresaci.txt");
    rename("Adresaci_tymczasowy.txt", "Adresaci.txt");
}

void editRecipient (vector<Recipient> &recipients, int loggedUserID) {

    int idOfRecipientToEdit;
    char choiceNr;
    string editParticularInfoAboutRecipient;

    system("cls");
    cout << "Wybierz id adresata, ktorego chcesz edytowac: ";
    cin >> idOfRecipientToEdit;

    for (int i = 0; i < recipients.size(); i++){
    if ((recipients[i].id == idOfRecipientToEdit) && (recipients[i].idUser == loggedUserID)){
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
        loadRecipientsToAFile (recipients, loggedUserID);
        cout << endl << "imie zostalo pomyslnie zmienione" << endl;
        Sleep(2000);
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
        loadRecipientsToAFile (recipients, loggedUserID);
        cout << endl << "nazwisko zostalo pomyslnie zmienione" << endl;
        Sleep(2000);
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
        loadRecipientsToAFile (recipients, loggedUserID);
        cout << endl << "Nr telefonu zostal pomyslnie zmieniony" << endl;
        Sleep(2000);
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
        loadRecipientsToAFile (recipients, loggedUserID);
        cout << endl << "adres email zostal pomyslnie zmieniony" << endl;
        Sleep(2000);
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
        loadRecipientsToAFile (recipients, loggedUserID);
        cout << endl << "adres zostal pomyslnie zmieniony" << endl;
        Sleep(2000);
        break;
    }
    case '6':
        break;
    }
    }
    else cout << "Nie masz uprawnien do edytowania Adresata o numerze ID: " << idOfRecipientToEdit << endl;
        Sleep(3000);
        break;
    }
}
void changePassword(vector<User> &usersContainer, int usersAmount, int loggedUserID) {
    string newPassword;
    cout << "Podaj nowe haslo: ";
    cin >> newPassword;
    for (int i=0; i < usersAmount; i++) {
        if (usersContainer[i].idUser == loggedUserID) {
            usersContainer[i].password = newPassword;
            cout << "Haslo zostalo zmienione" << endl;
            Sleep (1500);
        }
    }
}
void loadUsersToAFile(vector<User> &usersContainer) {

    fstream plik;
    plik.open("Uzytkownicy.txt",ios::out);

    if (plik.good() == true) {
        for (int i = 0; i < usersContainer.size(); i++) {
            plik << usersContainer[i].idUser << "|";
            plik << usersContainer[i].userName << "|";
            plik << usersContainer[i].password << "|" << endl;
        }
        plik.close();
    }
}
void addNewFriend(vector<Recipient> &recipients, vector<Recipient> &recipientsTemporary, int loggedUserID) {

    string name, surname, phoneNr, email, address;
    Recipient recipientData;

    system("cls");
    cout << ">>> DODAWANIE NOWEGO ADRESATA <<<" << endl << endl;
    cout << "Podaj imie uzytkownika: ";
    cin >> name;
    cout << "Podaj nazwisko uzytkownika: ";
    cin >> surname;
    cout << "Podaj numer telefonu uzytkownika: ";
    cin.sync();
    getline(cin, phoneNr);
    cout << "Podaj email uzytkownika: ";
    cin >> email;
    cout << "Podaj adres uzytkownika: ";
    cin.sync();
    getline (cin, address);

    if (recipientsTemporary.empty() == true) {
        recipientData.id = 1;
    } else {
        recipientData.id = recipientsTemporary.back().id + 1;
    }
    recipientData.idUser = loggedUserID;
    recipientData.name = name;
    recipientData.surname = surname;
    recipientData.phoneNr = phoneNr;
    recipientData.email = email;
    recipientData.address = address;
    recipients.push_back(recipientData);

    enterNewFriendToFile(recipientData);

    recipientsTemporary.push_back(recipientData);
    cout<<"Osoba zostala dodana";
    Sleep(2000);

}
int loadUsersFromFile (vector<User> &usersContainer) {

    User userData;
    int usersAmount = 0;
    int laneNumber = 1;
    string linia;
    char delimiter = '|';
    vector<string> newSplit;

    fstream plik;
    plik.open("Uzytkownicy.txt", ios::in);

    if (plik.good() == true) {
        while(getline(plik,linia)) {

            switch(laneNumber) {

            case 1:
                int i = 0;
                vector<string> newSplit = split(linia, delimiter);
                userData.idUser = atoi(newSplit[i].c_str());
                userData.userName = newSplit[i+1];
                userData.password = newSplit[i+2];
                usersContainer.push_back(userData);
                newSplit.clear();

                if (laneNumber == 1) {
                    usersAmount++;
                    laneNumber = 0;
                }
                laneNumber++;
            }
        }
        plik.close();
    }

    else if ( plik.good() == false) {
        cout << "Nie udalo sie otworzyc pliku!";
        plik.open("Uzytkownicy.txt",ios::out | ios::app);
    }
    return usersAmount;
}
int logToTheSystem (vector<User> &usersContainer, int usersAmount) {
    string userName, password;
    cout << "Podaj nazwe: ";
    cin >> userName;

    int i = 0;
    while (i < usersAmount) {
        if ( usersContainer[i].userName == userName ) {
            for (int choiceAmount = 0; choiceAmount < 3 ; choiceAmount++) {
                cout << "Podaj haslo. Pozostalo prob "<< 3-choiceAmount <<": ";
                cin >> password;
                if (usersContainer[i].password == password) {
                    cout << "Zalogowales sie." << endl;
                    Sleep (1000);
                    return usersContainer[i].idUser;
                }
            }
            Sleep (3000);
            return 0;
        }
        i++;
    }
    cout << "Nie ma uzytkownika z takim loginem" <<endl;
    Sleep(1500);
    return 0;
}

void userRegistration (vector<User> &usersContainer, int &usersAmount) {

    string userName, password;
    User userData;

    cout << "Podaj nazwe uzytkownika: ";
    cin >> userName;

    int i = 0;
    while (i < usersAmount) {
        if ( usersContainer[i].userName == userName ) {
            cout << "Taki uzytkownik istnieje, wpisz inna nazwe uzytkownika: ";
            cin >> userName;
            i = 0;
        } else {
            i++;
        }
    }
    cout << "Podaj haslo: ";
    cin >> password;

    userData.idUser = usersAmount + 1;
    userData.userName = userName;
    userData.password = password;
    usersContainer.push_back(userData);

    cout << "Konto zalozone" << endl;
    usersAmount++;
    Sleep(1000);

    fstream plik;
    plik.open("Uzytkownicy.txt", ios::out | ios::app);

    if (plik.good() == true) {
        plik << userData.idUser << "|";
        plik << userName << "|";
        plik << password << "|" << endl;
    } else {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        system("pause");
    }
}
void updateVectorOfRecipients (vector<Recipient> &recipients, int loggedUserID) {

    recipients.clear();
    Recipient recipientData;
    int recipientsAmount = 0;
    int laneNumber = 1;
    string linia;
    char delimiter = '|';
    vector<string> newSplit;

    fstream plik;
    plik.open("Adresaci.txt",ios::in);

    if (plik.good() == true) {
        while(getline(plik,linia)) {

            switch(laneNumber) {

            case 1:
                int i = 0;
                vector<string> newSplit = split(linia, delimiter);
                if(atoi(newSplit[i+1].c_str()) == loggedUserID) {
                    recipientData.id = atoi(newSplit[i].c_str());
                    recipientData.idUser = atoi(newSplit[i+1].c_str());
                    recipientData.name = newSplit[i+2];
                    recipientData.surname = newSplit[i+3];
                    recipientData.phoneNr = newSplit[i+4];
                    recipientData.email = newSplit[i+5];
                    recipientData.address = newSplit[i+6];
                    recipients.push_back(recipientData);
                    newSplit.clear();

                    if (laneNumber == 1) {
                        laneNumber = 0;
                    }
                    laneNumber++;
                }
            }
        }
        plik.close();
    }
}
void deleteRecipient (vector<Recipient> &recipients, vector<Recipient> &recipientsTemporary, int loggedUserID) {

    int idNr;
    char choiceNr;
    cout << endl;
    cout << "Podaj id osoby do usuniecia z Ksiazki adresowej: ";
    cin >> idNr;
    stringstream ss;
    ss << idNr;
    string idInString = ss.str();

    for (int i = 0; i < recipients.size(); i++){
    if ((recipients[i].id == idNr) && (recipients[i].idUser == loggedUserID)){

    cout << "Czy na pewno chcesz aby Adresat o id: " << idInString << " zostal usuniety" << endl;
    cout << "wybierz (t) jezeli tak" << endl;
    choiceNr = getch();

    if (choiceNr == 't') {

        fstream plik;
        fstream temp;
        Recipient recipientData;

        plik.open("Adresaci.txt",ios::in);
        temp.open("Adresaci_tymczasowy.txt",ios::out);

        int laneNumber = 1;
        string linia;
        char delimiter = '|';
        vector<string> newSplit;
        int temporary = 0;

        if (plik.good() == true) {
            while(getline(plik,linia)) {

                switch(laneNumber) {

                case 1:
                    int i = 0;
                    vector<string> newSplit = split(linia, delimiter);

                    if(newSplit[i] != idInString) {
                        temp << linia << endl;
                        newSplit.clear();
                    }
                    if (laneNumber == 1) {
                        laneNumber = 0;
                    }
                    laneNumber++;
                }
            }
        }
        plik.close();
        temp.close();
        remove("Adresaci.txt");
        rename("Adresaci_tymczasowy.txt", "Adresaci.txt");


        for (int i = 0; i < recipients.size(); i++) {
            if (recipients[i].id == idNr) {
                recipients.erase(recipients.begin() + i);
                for (int i = 0; i < recipientsTemporary.size(); i++) {
                if(recipientsTemporary[i].id == idNr) {
                    recipientsTemporary.erase(recipientsTemporary.begin() + i);
                }
            }
            cout<<"Adresat o numerze ID: " << idInString << " zostal usuniety" << endl;
                Sleep(2000);
                    }
                }
            }
        }
        else cout << "Nie masz uprawnien do usuniecia Adresata o numerze ID: " << idNr << endl;
        Sleep(3000);
        break;
    }
}


