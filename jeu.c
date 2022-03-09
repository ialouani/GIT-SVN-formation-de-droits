#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CARDS       52
#define MAX_PLAYERS     4

enum couleur {
    TREFLE, CARREAU, COEUR, PIQUE
};

enum valeur {
    DEUX=2, TROIS, QUATRE, CINQ,
    SIX, SEPT, HUIT, NEUF, DIX,
    VALET, DAME, ROI, AS
};
//il faut pour toute carte une valeur cad. de type enum valeur (~INT)
//+une designation via la couleur(voire les donnees)==>>>type enum couleur.
struct carte {
  enum valeur valeur;
  enum couleur couleur;
};
//length
//+structuration!!22 par un tableau de cartes decrites avant: struct carte cartes[52]...
struct paquet {
    int nombre;
    struct carte cartes[52];
};

struct carte make_card(enum valeur v, enum couleur c)
{
    struct carte cd = {v, c};
    return cd;
}

struct carte retirer(struct paquet *p)
{
    return p->cartes[--p->nombre];
}

void ajouter(struct paquet *p, struct carte c)
{
    if (p->nombre >= MAX_CARDS) return;
    p->cartes[p->nombre++] = c;
}

void melange_paquet(struct paquet *p)
{
    for (int i = 0; i < p->nombre - 1; i++) {
        int idx = rand() % (p->nombre - i) + i;
        struct carte temp = p->cartes[idx];
        p->cartes[idx] = p->cartes[i];
        p->cartes[i] = temp;
    }
}

struct paquet nouveau_paquet(enum valeur plus_basse)
{
    struct paquet p = {};
    for (enum couleur c = TREFLE; c <= PIQUE; c ++)
        for (enum valeur v = plus_basse; v <= AS; v ++)
            ajouter(&p, make_card(v, c));
    return p;
}

int distribue_paquet(int cartes, struct paquet *pioche,
        int joueurs, struct paquet paquets[])
{
    for (int i = 0; i < cartes; i ++)
        for (int j = 0; j < joueurs; j++) {
            if (pioche->nombre == 0)
                return i*j + j - 1;
            ajouter(paquets + j, retirer(pioche));
        }
    return cartes * joueurs;
}

const char *valeurs[] = {
    "2", "3", "4", "5", "6", "7", "8",
    "9", "0", "V", "D", "R", "A",
};

const char *couleurs[] = { "T", "K", "C", "P" };

const char *texte_couleur(enum couleur c)
{
    return couleurs[c - TREFLE];
}

const char *texte_valeur(enum valeur v)
{
    return valeurs[v - DEUX];
}

void affiche_carte(const struct carte *c)
{
    printf("<%s%s>",
            texte_valeur(c->valeur),
            texte_couleur(c->couleur));
}

void affiche_paquet(const struct paquet *p)
{
    for (int i = 0; i < p->nombre; i ++)
        affiche_carte(&p->cartes[i]);
    printf("\n");
}

void init_rand(const char *envvar)
{
    const char *seed = getenv(envvar);
    srand(seed ? atoi(seed) : time(NULL));
}

int main(int argc, char *argv[])
{
    init_rand("SEED");

    if (argc < 2) {
        printf("%s <nombre de cartes a donner>\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    struct paquet mains[4] = {};
    struct paquet pioche = nouveau_paquet(SEPT);

    melange_paquet(&pioche);
    distribue_paquet(atoi(argv[1]), &pioche, 4, mains);

    for (int i = 0; i < 4; i ++)
        affiche_paquet(mains + i);
    affiche_paquet(&pioche);

    return EXIT_SUCCESS;
}
