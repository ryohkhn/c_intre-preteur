#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unbounded_int.c"

/*
 * https://developpement-informatique.com/article/215/lire-et-ecrire-dans-un-fichier-en-langage-c
 * Pour des renseignements sur comment lire et écrire dans des fichiers.
 */

typedef struct variable{
    char* nom;
    unbounded_int valeur;
}variable;

static size_t variable_array_size=1;
static variable* listeVar;


void printError(char* errorMessage){
    printf("Erreur: %s\n",errorMessage);
    exit(EXIT_FAILURE);
}

variable* getVariable(char* var){
    for (int i = 0; i < variable_array_size; i++) {
        if((listeVar+i)->nom!=NULL){
            if(strcmp(((listeVar+i)->nom),var)==0){
                return (listeVar+i);
            }
        }

    }
    return NULL;
}

char* printValeur(char *ligne, FILE *output) {
    int tailleMalloc=1;
    int tailleVar=0;
    int compteur=0;
    int foundSpace=0;
    char* var=malloc(sizeof(char)*tailleMalloc);
    char c=*ligne;
    char* stringOutput;
    variable* tmpVar;

    while(c==' '){ // on avance dans la ligne tant qu'il s'agit d'un espace
        compteur++;
        c=*(ligne+compteur);
    }

    while(c!=EOF && c!='\n'){
        if(c==' '){ // cas d'un espace dans la ligne
            foundSpace=1;
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) { // cas d'une lettre dans la variable
            if (foundSpace) { // si un espace a déjà été trouvé après un caractère, à l'encontre d'un nouveau caractère on provoque une erreur
                printError("espace invalide dans le print");
            }
            if(tailleVar==tailleMalloc){ // agrandissement de la mémoire pour stocker le nom de la variable
                tailleMalloc*=2;
                var=realloc(var,sizeof(char)*tailleMalloc);
            }
            *(var+tailleVar)=c;
            tailleVar++;
        }
        compteur++;
        c=*(ligne+compteur);
    }

    // récupération de notre pointeur de structure si la variable existe
    tmpVar=getVariable(var);

    if(tmpVar!=NULL){ // cas ou la variable existe
        char* tmpVarValeur=unbounded_int2string(tmpVar->valeur);
        stringOutput=malloc(sizeof(char)*(strlen(tmpVarValeur)+strlen(tmpVar->nom)+4));
        strcat(stringOutput,var);
        strcat(stringOutput," = ");
        strcat(stringOutput,tmpVarValeur);
        strcat(stringOutput,"\n");
    }
    else{ // on affiche dans le fichier que la variable vaut 0 sinon
        stringOutput=malloc(sizeof(char)* strlen(var)+5);
        strcat(stringOutput,var);
        strcat(stringOutput," = 0\n");
    }
    fputs(stringOutput,output);
}


void attribuerValeur(char *var, char *ligne) {
    //printf("Entrée dans attribuer valeur, var=%s\n",var);
    char c=*ligne;
    int compteur=0;
    while(c!=EOF && c!='\n'){
        compteur++;
        c=*(ligne+compteur);
    }
}

void interpreterLineByLine(FILE *source, FILE *sortie, char *ligne) {
    int tailleVar= 0;
    int tailleVarInitiale=5;
    int compteur=0;
    int foundSpace=0;
    char* var=malloc(sizeof(char)*tailleVarInitiale);
    char c=*ligne;
    while (c != EOF && c != '\n' ){ // on itere lettre par lettre jusqu'a reconnaitre une variable, un print ou une erreur.
        if(tailleVar==0 && c==' '){ // cas des escapes avant la variable
            compteur++;
            c=*(ligne+compteur);
        }
        else{
            if(tailleVar==tailleVarInitiale){
                tailleVarInitiale*=2;
                var=realloc(var,sizeof(char)*tailleVarInitiale);
            }
            if(tailleVar==0) { // cas de la premiere lettre
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                    *var=c;
                    tailleVar++;
                }
                else{
                    printError("la ligne ne commence pas par une lettre");
                }
            }
            else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) { // cas d'une lettre dans la variable
                if(foundSpace){
                    printError("espace invalide");
                }
                *(var+tailleVar)=c;
                tailleVar++;
            }
            else if(tailleVar==5){ // cas print
                if(strcmp(var, "print") == 0 && *(ligne+compteur)==' '){
                    printValeur(ligne+compteur+1, sortie);
                    return;
                }
            }
            else if(c=='='){ // cas attribution
                attribuerValeur(var,ligne+compteur+1);
                return;
            }
            else if(c==' '){
                foundSpace=1;
            }
            else{
                printError("caractère invalide");
            }
            compteur++;
            c=*(ligne+compteur);
        }
    }
    printf("Fin de ligne sans cas trouvé\n");
}

void interpreter(FILE* source, FILE* sortie){
    listeVar=malloc(sizeof(variable)*variable_array_size);
    int taille_max=1024;
    char* ligne=malloc(sizeof(char)*taille_max);
    while(fgets(ligne,taille_max,source)!=0){
        interpreterLineByLine(source, sortie, ligne);
    }
}



int main(int argc,char* argv[]){
    if(argc>5){
        printf("Erreur: nombre d'arguments invalides");
        return EXIT_FAILURE;
    }

    FILE* fichierEntree;
    FILE* fichierSortie;
    if(argc>1 && strcmp(argv[1],"-i")==0){
        fichierEntree=fopen(argv[2],"r");
        if(fichierEntree==NULL){
            printf("Erreur \"%s\": chemin d'accès invalide",argv[2]);
            return EXIT_FAILURE;
        }
        if(argc>3 && strcmp(argv[3],"-o")==0){
            fichierSortie=fopen(argv[4],"w");
            interpreter(fichierEntree,fichierSortie);
        }
        else{
            interpreter(fichierEntree,stdout);
        }
    }
    else if(argc>1 && strcmp(argv[1],"-o")==0){
        fichierSortie=fopen(argv[2],"w");
        interpreter(stdin,fichierSortie);
    }
    else{
        interpreter(stdin,stdout);
    }
}