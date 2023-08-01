// funktion som startar en strid med en entitet/monster
void BattleEntity (Entity &player, Entity &enemy, vector<Item> &itemList) {
    cout << "Initiating battle with " << enemy.name << " who has " << enemy.health << " HP.\n";
    cout << "Player " << player.name << " has " << player.health << " HP.\n";

    ClearScreen();

    char optionInBattle;

    while(true) {
        if (player.health <= 0) {
            cout << "Player " << player.name << " died. Running away from battle.\n";
            break;
        }
        if (monster.health <= 0) {
            cout << enemy.name << " died.\n";
            break;
        }
        cout << "What do you want to do? (A)ttack, (I)tem or (R)un?: ";
        cin >> optionInBattle;
        
        if (optionInBattle == 'r' || optionInBattle == 'R') {
            cout << "Player " << player.name << " ran away from battle.\n";
            break;
        }

        switch(optionInBattle) {
            case 'a': case 'A':
            {
                cout << "Player " << player.name << " attacks " << enemy.name << ".\n" << enemy.name << " lost 10 HP.\n";
                enemy.health -= 10;
                cout << "Player " << player.name << ": " << player.health << " HP.\n";
                cout << enemy.name << ": " << enemy.health << "HP.\n";
                ClearScreen();
                break;
            }
            case 'i': case 'I':
            {
                cout << "Your inventory:\n";
                for (int i = 0; i < itemList.size(); ++i) {
                    cout << "[" << i << "] " << itemList[i].amount << itemList[i].name << "(s). " << itemList[i].effect << ".\n";
                }
                cout << "Your pick: ";

                int itemToUse;

                if (cin >> itemToUse) {
                    GetItemEffect(itemList[itemToUse].name, player);
                    itemList[itemToUse].amount -= 1;

                    if (itemList[itemToUse].amount == 0) {
                        itemList.erase(itemList.begin());
                    } else {
                        itemList.erase(itemList.begin() + itemToUse);
                    }

                    cout << "Player " << player.name << ": " << player.health << " HP.\n";
                    cout << enemy.name << ": " << enemy.health << "HP.\n";
                }
                
                ClearScreen();

                break;
            }
            default:
            break;
        }
        if (player.health <= 0) {
            cout << "Player " << player.name << " died. Running away from battle.\n";
            break;
        }
        if (enemy.health <= 0) {
            cout << enemy.name << " died.\n";
            break;
        }
        cout << enemy.name << " attacks " << player.name << ".\n" << player.name << " lost 10 HP.\n";
        player.health -= 10;
        cout << "Player " << player.name << ": " << player.health << " HP.\n";
        cout << enemy.name << ": " << enemy.health << "HP.\n";
        ClearScreen();
    }
}