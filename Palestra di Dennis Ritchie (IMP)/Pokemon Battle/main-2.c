#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

struct Attacco {
    int fpConsumption;
    char name[20];
    int damage;
};

struct pokemon {
    int hp;
    int fp;
    struct Attacco attacksList[2]; 
    char name[20];
    bool isAlive;
};

struct partita {
    bool isFinished;
    int turno;
};

void showPossibleAttacks(struct pokemon pokemon) {
    printf("Attacchi disponibili per %s:\n", pokemon.name);
    for (int i = 0; i < 2; i++) {
        printf("%d. %s (Danno: %d, Consumo FP: %d)\n", i + 1, pokemon.attacksList[i].name, pokemon.attacksList[i].damage, pokemon.attacksList[i].fpConsumption);
    }
}

struct Attacco assegnaPotereUtente() {
    struct Attacco attack;
    attack.damage = rand() % 50 + 50; 
    attack.fpConsumption = rand() % 50 + 1; 
    return attack;
}

struct pokemon creaPokemonUtente() {
    struct pokemon userPokemon;
    printf("Crea il tuo pokemon, inserisci nome:\n");
    scanf("%19s", userPokemon.name);
    userPokemon.hp = 120;
    userPokemon.fp = 120;
    userPokemon.isAlive = true;

    for (int i = 0; i < 2; i++) {
        userPokemon.attacksList[i] = assegnaPotereUtente();
    }
    printf("Stats pokemon:\nNome: %s\nHP: %d\nFP: %d\n", userPokemon.name, userPokemon.hp, userPokemon.fp);
    return userPokemon;
}

void battleTurn(struct pokemon* attacker, struct pokemon* defender) {
    if (attacker->isAlive && defender->isAlive) {
        showPossibleAttacks(*attacker);

        int attackChoice;
        printf("Scegli l'attacco (1 o 2): ");
        scanf("%d", &attackChoice);

        if (attackChoice < 1 || attackChoice > 2) {
            printf("Scelta non valida! Turno saltato.\n");
            return;
        }

        struct Attacco chosenAttack = attacker->attacksList[attackChoice - 1];

        if (attacker->fp >= chosenAttack.fpConsumption) {
            attacker->fp -= chosenAttack.fpConsumption;
            defender->hp -= chosenAttack.damage;
            printf("%s usa %s! %s subisce %d danni.\n", attacker->name, chosenAttack.name, defender->name, chosenAttack.damage);
        }
        else {
            printf("%s non ha abbastanza FP per usare %s!\n", attacker->name, chosenAttack.name);
        }

        if (defender->hp <= 0) {
            defender->isAlive = false;
            printf("%s è stato sconfitto!\n", defender->name);
        }
    }
}

int main() {
    srand(time(NULL)); 

    printf("Inizio partita:\n");
    struct partita partitaStatus;
    partitaStatus.isFinished = false;
    partitaStatus.turno = 1;

    struct pokemon usrPokemon = creaPokemonUtente();

    struct pokemon botPokemon;
    strcpy(botPokemon.name, "Porfus");
    botPokemon.hp = 100;
    botPokemon.fp = 100;
    botPokemon.isAlive = true;

    while (!partitaStatus.isFinished) {
        printf("Turno: %d\n", partitaStatus.turno);

        printf("\nTurno dell'utente:\n");
        battleTurn(&usrPokemon, &botPokemon);
        if (!botPokemon.isAlive) {
            partitaStatus.isFinished = true;
            printf("Hai vinto!\n");
            break;
        }

        // Bot's turn (simple AI)
        printf("\nTurno del bot:\n");
        battleTurn(&botPokemon, &usrPokemon);
        if (!usrPokemon.isAlive) {
            partitaStatus.isFinished = true;
            printf("Hai perso!\n");
            break;
        }

        partitaStatus.turno++;
    }

    printf("Fine della partita!\n");
    return 0;
}
