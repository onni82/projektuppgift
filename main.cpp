/*
    Kurs: Programmering 1 PRRPRR01 100p
    Uppgift: Projektuppgift
    Elev: Onni Bucht

    Koden laddas även upp på https://github.com/onni82/projektuppgift
*/

#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <stdlib.h>
#include <conio.h>

using namespace std;

// denna struct kan skapa objekt som syns i ens väska
// name är namnet på objektet, effect är objektets effekt och amount är antalet man har av det objektet
struct Item {
    string name;
    string effect;
    int amount;

    // definierar en funktion som jämför objektets namn med det objekt man söker efter i sin väska
    static bool compareByName(const Item& p, const string& targetName) {
        return p.name < targetName;
    }
};

// denna struct kan skapa en entitet/monster som man möter
// name är namnet på objektet och health är antalet hit points (HP) som objektet har
struct Entity {
    string name;
    int health;
};

// binärsökningsfunktion för structer
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

void ClearScreen() {
    cout << "Hit a key to proceed.\n";
    getch();
    system("cls");
}

// funktion för att kunna lägga till objekt i inventory
// argument som skrivs in i funktionens anrop är namn, effekt och antal för just det objekt man vill lägga in
void AddItem(vector<Item> &itemList, string nameOfItem, string effectOfItem, int amountOfItem) {
    int result = binarySearch(itemList, nameOfItem, &Item::compareByName); // söker efter objektet i ens väska

    system("cls");

    if (result != -1) { // om objektet hittas i väskan
        itemList[result].amount += amountOfItem;
        cout << "Picked up " << amountOfItem << " " << nameOfItem << "(s). Now a total of " << itemList[result].amount << ".\n";
    } else { // om objektet inte hittas i väskan
        Item item;
        item.name = nameOfItem;
        item.effect = effectOfItem;
        item.amount = amountOfItem;
        itemList.push_back(item);
        cout << "Picked up " << amountOfItem << " " << nameOfItem << "(s).\n";
    }
    
}

void GetItemEffect(nameOfItemToBeUsed, Entity &player) {
    if (nameOfItemToBeUsed == "Potion") {
        player.health += 10;
    }
}

// funktion som startar en strid med en entitet/monster
void BattleEntity (Entity &player, Entity &monster, vector<Item> &itemList) {
    cout << "Initiating battle with " << monster.name << " who has " << monster.health << " HP.\n";
    cout << "Player" << player.name << " has " << player.health << " HP.\n";

    ClearScreen();

    char optionInBattle;

    while(true) {
        cout << "What do you want to do? (A)ttack, (I)tem or (R)un?: ";
        cin >> tolower(optionInBattle);

        if (player.health >= 0) {
            cout << "Player " << player.name << " died. Running away from battle.\n";
            break;
        }
        if (monster.health >= 0) {
            cout << monster.name << " died.\n"
            break;
        }
        if (optionInBattle == "r") {
            break;
        }

        switch(optionInBattle) {
            case 'a':
            {
                cout << "Player " << player.name << " attacks " << monster.name << ".\n" << monster.name << " lost 10 HP.\n";
                cout << "Player " << player.name << ": " << player.health << " HP.\n";
                cout << monster.name << ": " monster.name << "HP.\n";
                ClearScreen();
                break;
            }
            case 'i':
            {
                for (int i = 0; i < inventory.size()-1; ++i) {
                    cout << "[" << i << "] " << inventory[i].amount << inventory[i].name << "(s). " << inventory[i].effect << ".\n";
                }

                int itemToUse;
                cin >> itemToUse;

                GetItemEffect(inventory[itemToUse].name, player);
                inventory[itemToUse].amount -= 1;

                if (inventory[itemToUse].amount == 0) {
                    inventory.erase(inventory.begin());
                } else {
                    inventory.erase(inventory.begin() + itemToUse);
                }

                cout << "Player " << player.name << ": " << player.health << " HP.\n";
                cout << monster.name << ": " monster.name << "HP.\n";
                ClearScreen();

                break;
            }
            default:
            break;
        }
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

    cout << "Hello " << user.name << ". Let's start your adventure. You start with one potion.\n";
    ClearScreen();

    vector<Item> inventory = {
        {"Potion", "Heals 10 HP", 1}
    };

    Entity orc;
    orc.name = "Zug Zug the Orc";
    orc.health = 15;

    BattleEntity(user, orc, inventory);
    ClearScreen();

    cout << "You enter a big forest. There's a big bush. You check the bush.\n";
    ClearScreen();

    AddItem(inventory, "Potion", "Heals 10 HP", 5);

    cout << "Congratulations! You finished the game.\n";

    return 0;
}
