/*
    Kurs: Programmering 1 PRRPRR01 100p
    Uppgift: Projektuppgift
    Elev: Onni Bucht

    Koden laddas även upp på https://github.com/onni82/projektuppgift
*/

#include <iostream> // för att kunna använda cout och cin
#include <string> // för att kunna använda stringvariabler
#include <cctype> // för att kunna använda tolower()
#include <vector> // för att kunna använda vectorarrayer
#include <stdlib.h> //
#include <conio.h> // för att kunna använda getch(). detta bibliotek fungerar endast på Windows. använd ncurses på Linux/macOS eller annan unix-like
#include <algorithm> // för att kunna använda sort()

using namespace std;

// denna struct kan skapa objekt som syns i ens väska
// name är namnet på objektet, effect är objektets effekt och amount är antalet man har av det objektet
struct Item {
    string name;
    int amount;

    // definierar en funktion som jämför objektets namn med det objekt man söker efter i sin väska
    static bool compareByName(const Item& p, const string& targetName) {
        return p.name < targetName;
    }
};

// denna funktion används för att jämföra objekt i ens väska när man ska sortera väskan
bool compareItems(const Item& a, const Item& b) {
    if (a.name != b.name) {
        return a.name < b.name;
    }
}

// denna struct kan skapa en entitet/monster som man möter
// name är namnet på objektet och health är antalet hit points (HP) som objektet har
struct Entity {
    string name;
    int health;
    int level;
    int currentExp;

};

// binärsökningsfunktion för structobjekt
// returnerar indexet av objektet man söker efter om det finns, annars returneras -1
template<typename T>
int binarySearch(const vector<T>& arr, const string& targetName, bool (*compareFunc)(const T&, const string&)) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (compareFunc(arr[mid], targetName)) {
            left = mid + 1; // objektet kan vara i den högra halvan
        } else if (compareFunc(arr[mid], targetName)) {
            right = mid - 1; // objektet kan vara i den vänstra halvan
        } else {
            return mid; // objektet hittades inte
        }
    }

    return -1; // objektet hittades inte
}

// funktion för att trimma strängvariabler för att inte innehålla nya rader
string trim(string input) {
    string output;

    for (char ch : input) {
        if (ch == '\n' || ch == '\r')
            continue;
        output += ch;
    }

    return output;
}

// funktion som ber användaren att trycka ner en knapp innan skärmen rensas
void ClearScreen() {
    cout << "Hit a key to proceed.\n";
    getch();
    system("cls");
}

//ger spelaren en mängd experience
//första argumentet är entiteten som man vill ge experience medan det andra argumentet är mängden experience man vill ge
void RewardExp(Entity &player, int amountOfExp) {
    player.currentExp += amountOfExp;
    cout << "Player " << player.name << " gained " << amountOfExp << " experience.\n";
}

// funktion som höjer nivån på entiteten som anges i argumentet
// alla entiteter börjar på nivå 0, men varje nivå återges som nivå+1 i spelet för att det ska se ut som att man börjar på nivå 1
void LevelUp(Entity &player) {
    static const int requiredExp[] = { // sätter mängden experience som behövs för att gå upp en nivå (ex. 50 från 0 till 1, 100 från 1 till 2)
        50, 100, 150, 200, 250, 300, 350, 400, 450, 500
    };

    while(player.currentExp >= requiredExp[player.level]) {
        ++player.level;
        cout << player.name << " leveled up to level " << player.level+1 << ".\n";
    }
}

// funktion för att kunna lägga till objekt i väskan
// argument som skrivs in i funktionens anrop är namn, effekt och antal för just det objekt man vill lägga in
void AddItem(vector<Item> &itemList, string nameOfItem, int amountOfItem) {
    int result = binarySearch(itemList, nameOfItem, &Item::compareByName); // söker efter objektet i ens väska

    if (result != -1) { // om objektet hittas i väskan
        itemList[result].amount += amountOfItem;
        if (amountOfItem > 1) {
            cout << "Picked up " << amountOfItem << " " << nameOfItem << "s. Now a total of " << itemList[result].amount << ".\n";
        } else {
            cout << "Picked up " << amountOfItem << " " << nameOfItem << ". Now a total of " << itemList[result].amount << ".\n";
        }
    } else { // om objektet inte hittas i väskan
        Item item;
        item.name = nameOfItem;
        item.amount = amountOfItem;
        itemList.push_back(item);
        cout << "Picked up " << amountOfItem << " " << nameOfItem;

        if (amountOfItem > 1) {
             cout << "s";
        }

        cout << ".\n";
    }
}

// funktion för att kunna använda objekt som finns i ens väska
// argumenten är den array man vill uppdatera efter användning av objekt, namnet på objektet och den entitet som objektet ska påverka
void UseItem(vector<Item> &itemList, string nameOfItem, Entity &player) {
    int result = binarySearch(itemList, nameOfItem, &Item::compareByName); // söker efter objektet i ens väska

    if (result != -1) { // om objektet hittas i väskan
        if (nameOfItem == "Potion") {
            player.health += 15;
        }

        itemList[result].amount -= 1;

        if (itemList[result].amount == 0) {
            itemList.erase(itemList.begin() + result);
        }

        cout << "Used " << nameOfItem << " on " << player.name << ".\n";
    }
}

// funktion som berättar vad ett objekt (som kan finnas i väskan) ger för effekt vid användning
string GetItemDescription(string nameOfItem) {
    string itemDescription;
    if (nameOfItem == "Potion") {
        itemDescription = "Heals 15 HP per item";
    } else {
        itemDescription = "No description";
    }
    return itemDescription;
}

// funktion som startar en strid med en entitet/monster
void BattleEntity (Entity &player, Entity &enemy, vector<Item> &itemList) {
    cout << "Initiating battle with level " << enemy.level+1 << " " << enemy.name << " who has " << enemy.health << " HP.\n";
    cout << "Level " << player.level+1 << " player " << player.name << " has " << player.health << " HP.\n";

    ClearScreen();

    char optionInBattle; // variabel för menyval i strid

    while(true) {
        if (player.health <= 0) { // kollar om spelaren har dött i strid
            cout << "Player " << player.name << " died. Running away from battle.\n";
            break;
        }
        if (enemy.health <= 0) { // kollar om monstret har dött i strid
            cout << enemy.name << " died.\n";
            RewardExp(player, enemy.level+1 * 15); // ge spelaren experience (= monstrets level * 15)
            LevelUp(player);
            break;
        }

        cout << "What do you want to do? (A)ttack, (I)tem or (R)un?: ";
        cin >> optionInBattle;

        if (optionInBattle == 'r' || optionInBattle == 'R') { // springer från striden
            cout << "Player " << player.name << " ran away from battle.\n";
            break;
        }

        switch(optionInBattle) { // kollar valet under striden
            case 'a': case 'A': // när man attackerar under striden
            {
                cout << "Player " << player.name << " attacks " << enemy.name << ".\n" << enemy.name << " lost 10 HP.\n";
                enemy.health -= 10;
                cout << "Player " << player.name << ": " << player.health << " HP.\n";
                cout << enemy.name << ": " << enemy.health << "HP.\n";
                ClearScreen();
                break;
            }
            case 'i': case 'I': // när man använder ett objekt under striden
            {
                cout << "Your inventory:\n";
                //sort(itemList.begin(), itemList.end()); // sorterar ens väska
                //for (int i = 0; i < itemList.size(); ++i) { // skriver ut varje objekt i ens väska
                for (unsigned i = 0; i < itemList.size(); ++i) { // skriver ut varje objekt i ens väska
                    cout << "[" << i << "] " << itemList[i].amount << " " << itemList[i].name;
                    if (itemList[i].amount > 1){
                        cout << "s";
                    }
                    cout << ". " << GetItemDescription(itemList[i].name) << ".\n";
                }
                cout << "Your pick: ";

                int itemToUse;

                if (cin >> itemToUse) { // använder ett objekt på spelaren
                    //sort(itemList.begin(), itemList.end());
                    UseItem(itemList, itemList[itemToUse].name, player);

                    cout << "Player " << player.name << ": " << player.health << " HP.\n";
                    cout << enemy.name << ": " << enemy.health << "HP.\n";
                }

                ClearScreen();

                break;
            }
            default:
            break;
        }
        if (player.health <= 0) { // kollar om spelaren har dött i strid
            cout << "Player " << player.name << " died. Running away from battle.\n";
            break;
        }
        if (enemy.health <= 0) { // kollar om monstret har dött i strid
            cout << enemy.name << " died.\n";
            RewardExp(player, enemy.level+1 * 15); // ge spelaren experience (= monstrets level * 15)
            LevelUp(player);
            break;
        }
        cout << enemy.name << " attacks " << player.name << ".\n" << player.name << " lost 10 HP.\n";
        player.health -= 10;
        cout << "Player " << player.name << ": " << player.health << " HP.\n";
        cout << enemy.name << ": " << enemy.health << "HP.\n";
        ClearScreen();
    }
}

int main(int argc, char* argv[]) {
    Entity user;

    if (argc > 1) {
        user.name = trim(argv[1]);
    } else {
        cout << "What's your name? ";
        string input;
        getline(cin, input);
        user.name = trim(input);
    }

    user.health = 20;
    user.level = 0;
    user.currentExp = 0;

    cout << "Hello " << user.name << ". Let's start your adventure. You start with two potions.\n";
    ClearScreen();

    vector<Item> inventory = {
        {"Potion", 2}
    };

    Entity orc;
    orc.name = "Zug Zug the Orc";
    orc.health = 25;
    orc.level = 0;
    orc.currentExp = 0;

    sort(inventory.begin(), inventory.end(), compareItems); // sorterar ens väska
    BattleEntity(user, orc, inventory);
    ClearScreen();

    cout << "You enter a big forest. There's a big bush. You check the bush.\n";
    ClearScreen();

    AddItem(inventory, "Potion", 5);
    sort(inventory.begin(), inventory.end(), compareItems); // sorterar ens väska
    cout << "As you walk further into the forest, you get ambushed by the Elven King and his army.\n";
    ClearScreen();

    Entity elf;
    orc.name = "Elven King";
    orc.health = 25;
    orc.level = 0;
    orc.currentExp = 0;

    sort(inventory.begin(), inventory.end(), compareItems); // sorterar ens väska
    BattleEntity(user, orc, inventory);
    ClearScreen();

    cout << "Congratulations! You finished the game.\n";
    ClearScreen();

    return 0;
}
