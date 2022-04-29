typedef struct chiffre{
    struct chiffre* suivant;
    char c;
    struct chiffre* precedent;
}chiffre;

typedef struct{
    char signe;
    size_t len;
    chiffre* premier;
    chiffre* dernier;
}unbounded_int;

/* transforme une chaîne de caractère représentant un entier et renvoie une valeur unbounded_int représentant cet entier */
unbounded_int string2unbounded_int(const char* e);

/* prend en argument un nombre long long et retourne la structure unbounded_int représentant ce nombre */
unbounded_int ll2unbounded_int(long long i);

/* prend en argument une structure unbounded_int représentant un entier et retourne cet entier sous la forme d'une chaîne de caractère */
char* unbounded_int2string(unbounded_int i);

/* retourne une des valeurs −1, 0, 1 si, respectivement, a<b, a==b, a>b (ou a,b sont les entiers representés par a et b) */
int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b);

/* renvoie la même chose que la fonction précédente, mais où le second argument est de type long long */
int unbounded_int_cmp_ll(unbounded_int a, long long b);

/* renvoie la représentation de la somme de deux entiers représentés par a et b */
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b);

/* renvoie la représentation de leur différence */
unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b);

/* renvoie la représentation de leur produit */
unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b);

/* fonction auxilliaire pour le produit. Renvoie le produit d'un unbounded_int par une valeur */
unbounded_int produit_simple(unbounded_int a, int value);
