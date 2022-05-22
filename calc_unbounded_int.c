#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "unbounded_int.h"


typedef struct variable{
    char* nom;
    unbounded_int valeur;
}variable;

static size_t variable_array_allocated_size=1;
static size_t variable_array_size=1;
static variable* listeVar;


// affiche l'erreur et termine l'exécution avec EXIT_FAILURE
void printError(char* errorMessage){
    printf("Erreur: %s\n",errorMessage);
    exit(EXIT_FAILURE);
}

void testMalloc(void * ptr){
    if(ptr == NULL){
        printf("Erreur de malloc() ou realloc().\n");
        exit(EXIT_FAILURE);
    }
}


// retourne le pointeur de nom var, ou NULL si inexistante
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


// double la taille allouée à la variable si nécessaire
char* checkStringSize(char* string,int tailleVar,size_t* tailleMalloc){
    if(tailleVar==*(tailleMalloc)){
        *(tailleMalloc)*=2;
        string=realloc(string,sizeof(char)*(*tailleMalloc));
        testMalloc(string);
    }
    return string;
}


// met le caractère vide en fin de ligne et realloc le string à sa taille réelle
char* reallocToSize(char* string,int tailleString){
    *(string+tailleString)='\0';
    string=realloc(string,sizeof(char)*tailleString);
    testMalloc(string);
    return string;
}


// double ta taille du tableau de variables si nécessaire
void checkArraySize(){
    if(variable_array_allocated_size==variable_array_size){
        variable_array_allocated_size*=2;
        listeVar=realloc(listeVar,sizeof(variable)*variable_array_allocated_size);
        testMalloc(listeVar);
    }
}


// retourne True si le string est composé seulement de lettres
int stringIsAlpha(char* string){
    char c;
    for(int i=0;i<strlen(string);i++){
        c=*(string+i);
        if(!isalpha(c)){
            return 0;
        }
    }
    return 1;
}


// retourne True si le string est composé seulement de chiffres et d'un signe potentiel
int stringIsDigit(char* string){
    char c;
    int hasOp=0;
    if((*string=='+' || *string=='-')){
        hasOp=1;
    }
    for(int i=0;i<strlen(string);i++){
        c=*(string+i);
        if(!isdigit(c)){
            // on retourne 0 si le chiffre contient un caractère autre qu'un opérateur en début de ligne
            if(!hasOp || i!=0){
                return 0;
            }
        }
    }
    return 1;
}


// vérifie si la variable string est bien formée seulement de lettres ou de chiffres
void verifyStringFormat(char* string){
    if(!stringIsAlpha(string) && !stringIsDigit(string)){
        printError("La variable doit être composée seulement de lettres ou de chiffres");
        exit(EXIT_FAILURE);
    }
}


// vérifie que les variables utilisées existent et crée la variable à attribuer si inexistante
void checkVariablesAvecOp(char* leftVar,char* firstVar,char operateur,char* secondVar) {
    variable* tmpFirstVar;
    variable* tmpSecondVar;

    if (isalpha(*firstVar)) { // on vérifie que la variable dont on récupère la valeur existe
        tmpFirstVar= getVariable(firstVar);
        if (tmpFirstVar== NULL) {
            printError("Variable inexistante");
        }
    }
    if(isalpha(*secondVar)){ // on vérifie que la variable dont on récupère la valeur existe
        tmpSecondVar= getVariable(secondVar);
        if (tmpSecondVar== NULL) {
            printError("Variable inexistante");
        }
    }
    if(isalpha(*leftVar)){ // on vérifie que la variable attribuée existe sinon on la crée et on l'ajoute à la liste
        variable* newVar=getVariable(leftVar);
        if(newVar==NULL){
            newVar=malloc(sizeof(variable));
            testMalloc(newVar);
            checkArraySize();
            newVar->nom=leftVar;
            *(listeVar+variable_array_size)=*newVar;
            variable_array_size++;
        }
    }
}


// vérifie que la variable utilisée existe et crée la variable à attribuer si inexistante
void checkVariablesSansOp(char* leftVar,char* firstVar) {
    variable* tmpFirstVar;

    if (isalpha(*firstVar)) { // on vérifie que la variable dont on récupère la valeur existe
        tmpFirstVar=getVariable(firstVar);
        if (tmpFirstVar== NULL) {
            printError("Variable inexistante");
        }
    }
    if(isalpha(*leftVar)){ // on vérifie que la variable attribuée existe sinon on la crée et on l'ajoute à la liste
        variable* newVar=getVariable(leftVar);
        if(newVar==NULL){
            newVar=malloc(sizeof(variable));
            testMalloc(newVar);
            checkArraySize();
            newVar->nom=leftVar;
            *(listeVar+variable_array_size)=*newVar;
            variable_array_size++;
        }
    }
}


// récupère la valeur à afficher et l'affiche dans le fichier output, traite les différentes erreurs
void printValeur(const char *ligne, FILE *output) {
    size_t* tailleMalloc=malloc(sizeof(size_t));
    testMalloc(tailleMalloc);
    *tailleMalloc=1;
    int tailleVar=0;
    int compteur=0;
    int foundSpace=0;
    char* var=malloc(sizeof(char)*(*(tailleMalloc)));
    testMalloc(var);
    char c=*ligne;
    char* stringOutput;
    variable* tmpVar;

    while(c==' '){ // on avance dans la ligne tant que le caractère est un espace
        compteur++;
        c=*(ligne+compteur);
    }

    while(c!=EOF && c!='\n' && c!='\0'){
        if(c==' '){ // cas d'un espace dans la ligne
            foundSpace=1;
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) { // cas d'une lettre dans la variable
            if (foundSpace) { // si un espace a déjà été trouvé après un caractère, à l'encontre d'un nouveau caractère on provoque une erreur
                printError("Espace invalide dans le print");
            }

            // agrandissement du string si nécessaire
            var=checkStringSize(var,tailleVar,tailleMalloc);

            *(var+tailleVar)=c;
            tailleVar++;
        }
        else{
            printError("Variable incorrecte");
        }
        compteur++;
        c=*(ligne+compteur);
    }

    // récupération de notre pointeur de structure si la variable existe
    tmpVar=getVariable(var);

    if(tmpVar!=NULL){ // cas ou la variable existe
        char* tmpVarValeur=unbounded_int2string(tmpVar->valeur);
        stringOutput=malloc(sizeof(char)*(strlen(tmpVarValeur)+strlen(tmpVar->nom)+4));
        // concaténation du string à écrire
        sprintf(stringOutput,"%s = %s\n",var,tmpVarValeur);
        fputs(stringOutput,output);
    }
    else if(tailleVar!=0){ // on affiche dans le fichier que la variable vaut 0 sinon
        stringOutput=malloc(sizeof(char)* strlen(var)+5);
        testMalloc(stringOutput);
        // concaténation du string à écrire
        sprintf(stringOutput,"%s = 0\n",var);
        fputs(stringOutput,output);
    }
}


// appelle les fonctions dépendant de l'opérateur et change la valeur de la variable leftVar
void effectuerOperation(variable* leftVar,unbounded_int firstVar,char operateur,unbounded_int secondVar){
    switch (operateur) {
        case '+':
            leftVar->valeur= unbounded_int_somme(firstVar,secondVar);
            break;
        case '-':
            leftVar->valeur= unbounded_int_difference(firstVar,secondVar);
            break;
        case '*':
            leftVar->valeur= unbounded_int_produit(firstVar,secondVar);
            break;
        default:
            break;
    }
}


// détecte si rightVar est un nombre ou une variable et attribue sa valeur à leftVar
void attribuerValeurSansOp(char* leftVar,char* rightVar){
    variable* tmpLeftVar=getVariable(leftVar);
    variable* tmpRightVar;

    if(isalpha(*rightVar)){ // variable n'est pas un nombre
        tmpRightVar=getVariable(rightVar);
        tmpLeftVar->valeur=tmpRightVar->valeur;
    }
    else{ // variable est un nombre
        unbounded_int rightVarUI=string2unbounded_int(rightVar);
        tmpLeftVar->valeur=rightVarUI;
    }
}


// détecte si firstVar et secondVar sont des nombres ou des variables pour effectuer l'opération
void attribuerValeurAvecOp(char* leftVar,char* firstVar,char operateur,char* secondVar){
    variable* tmpLeftVar=getVariable(leftVar);
    variable* tmpFirstVar;
    variable* tmpSecondVar;
    unbounded_int firstVarUI;
    unbounded_int secondVarUI;

    if(isalpha(*firstVar)){ // première variable n'est pas un nombre
        tmpFirstVar=getVariable(firstVar);
        if(isalpha(*secondVar)){ // seconde variable n'est pas un nombre
            tmpSecondVar= getVariable(secondVar);
            effectuerOperation(tmpLeftVar,tmpFirstVar->valeur,operateur,tmpSecondVar->valeur);
        }
        else{ // seconde variable est un nombre
            secondVarUI=string2unbounded_int(secondVar);
            effectuerOperation(tmpLeftVar,tmpFirstVar->valeur,operateur,secondVarUI);
        }
    }
    else{ // première variable est un nombre
        firstVarUI=string2unbounded_int(firstVar);
        if(isalpha(*secondVar)){ // seconde variable n'est pas un nombre
            tmpSecondVar= getVariable(secondVar);
            effectuerOperation(tmpLeftVar,firstVarUI,operateur,tmpSecondVar->valeur);
        }
        else{ // seconde variable est un nombre
            secondVarUI=string2unbounded_int(secondVar);
            effectuerOperation(tmpLeftVar,firstVarUI,operateur,secondVarUI);
        }
    }
}


// récupère les variables de la ligne, détecte si elle contient une opération ou non
void attribuerValeur(char *var, const char *ligne) {
    size_t* tailleFirstMalloc=malloc(sizeof(size_t));
    testMalloc(tailleFirstMalloc);
    size_t* tailleSecondMalloc=malloc(sizeof(size_t));
    testMalloc(tailleSecondMalloc);
    *tailleFirstMalloc=1;
    *tailleSecondMalloc=1;
    int tailleFirstVar=0;
    int tailleSecondVar=0;
    int foundFirstSpace=0;
    int compteur=0;
    char* firstVar=malloc(sizeof(char)*(*tailleFirstMalloc));
    testMalloc(firstVar);
    char* secondVar=malloc(sizeof(char)*(*tailleSecondMalloc));
    testMalloc(secondVar);
    char operateur=' ';
    char c=*ligne;

    while(c==' '){ // on avance dans la ligne tant qu'il s'agit d'un espace
        compteur++;
        c=*(ligne+compteur);
    }

    while(c!=EOF && c!='\n' && c!='\0'){
        if(c==' '){ // cas d'un espace dans la ligne
            foundFirstSpace=1;
        }
        else if(c=='+' || c=='-' || c=='*' || c=='/') { // cas d'un opérateur
            if (*(ligne + compteur + 1) >= '0' && *(ligne + compteur + 1) <= '9' && (c=='+' || c=='-')){ // cas d'un entier signé
                if(operateur==' '){
                    // on vérifie la taille allouée du string
                    firstVar=checkStringSize(firstVar,tailleFirstVar,tailleFirstMalloc);
                    *(firstVar+tailleFirstVar)=c;
                    tailleFirstVar++;
                }
                else{
                    // on vérifie la taille allouée du string
                    secondVar=checkStringSize(secondVar,tailleSecondVar,tailleSecondMalloc);
                    *(secondVar+tailleSecondVar)=c;
                    tailleSecondVar++;
                }
            }
            else if(foundFirstSpace==0){ // vérifie que l'opérateur se trouve après une variable
                printError("Première variable manquante ou espace manquant autour de l'opérateur");
            }
            else if (*(ligne + compteur - 1) != ' ' || *(ligne + compteur + 1) != ' '){ // vérifie les espaces autout de l'opérateur
                printError("Espace manquant autour de l'opérateur, seconde variable manquante ou mauvais caractère");
            }
            else if (operateur != ' '){ // vérifie qu'il n'y a qu'un seul opérateur
                printError("Un seul opérateur est requis");
            }
            else {
                operateur = c;
                compteur++; // on saute l'espace après l'opérateur
            }
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isdigit(c)) { // cas d'une lettre dans la variable
            // si un espace a déjà été trouvé après un caractère, à l'encontre d'un nouveau caractère on provoque une erreur
            if (operateur==' ' && foundFirstSpace) {
                printError("Opérateur manquant");
            }

            if(operateur==' '){
                // agrandissement de la mémoire pour stocker le nom de la variable
                firstVar=checkStringSize(firstVar,tailleFirstVar,tailleFirstMalloc);
                *(firstVar+tailleFirstVar)=c;
                tailleFirstVar++;
            }
            else{
                // agrandissement de la mémoire pour stocker le nom de la variable
                secondVar=checkStringSize(secondVar,tailleSecondVar,tailleSecondMalloc);
                *(secondVar+tailleSecondVar)=c;
                tailleSecondVar++;
            }
        }
        else{
            printError("Caractère invalide");
        }
        compteur++;
        c=*(ligne+compteur);
    }

    if(tailleSecondVar==0 && operateur!=' '){ // si il y a un opérateur sans seconde variable on provoque une erreur
        printError("Seconde variable manquante");
    }

    if(tailleFirstVar!=0){
        if(tailleSecondVar!=0){
            // realloc des strings à leur taille réelle
            secondVar=reallocToSize(secondVar,tailleSecondVar);
            firstVar=reallocToSize(firstVar,tailleFirstVar);
            // vérification du format du string, composé seulement de lettres ou de chiffre
            verifyStringFormat(firstVar);
            verifyStringFormat(secondVar);
            // vérifie que les variables existent avant d'appeler les opérations
            checkVariablesAvecOp(var,firstVar,operateur,secondVar);
            attribuerValeurAvecOp(var,firstVar,operateur,secondVar);
        }
        else{
            // realloc des strings à leur taille réelle
            firstVar=reallocToSize(firstVar,tailleFirstVar);
            // vérification du format du string, composé seulement de lettres ou seulement de chiffre
            verifyStringFormat(firstVar);
            // vérifie que la variable existe avant d'appeler les opérations
            checkVariablesSansOp(var,firstVar);
            attribuerValeurSansOp(var,firstVar);
        }
    }
}



// traite la ligne pour savoir s'il s'agit d'un print ou d'une attribution, traite les erreurs de la ligne
void interpreterLineByLine(FILE *sortie, char *ligne) {
    int tailleVar= 0;
    size_t* tailleVarInitiale= malloc(sizeof(size_t));
    *tailleVarInitiale=5;
    int compteur=0;
    int foundSpace=0;
    char* var=malloc(sizeof(char)*(*tailleVarInitiale));
    testMalloc(var);
    char c=*ligne;
    while (c != EOF && c != '\n' && c!='\0' ){ // on itère lettre par lettre jusqu'a reconnaître une variable, un print ou une erreur
        if(tailleVar==0 && c==' '){ // cas des espaces avant la variable
            compteur++;
            c=*(ligne+compteur);
        }
        else{
            // vérification de la taille de la var
            if(tailleVar==0) { // cas de la premiere lettre
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                    *var=c;
                    tailleVar++;
                }
                else{
                    printError("La ligne ne commence pas par une lettre");
                }
            }
            else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) { // cas d'une lettre dans la variable
                if(foundSpace){
                    printError("Espace invalide");
                }
                checkStringSize(var,tailleVar,tailleVarInitiale);
                *(var+tailleVar)=c;
                tailleVar++;
            }
            else if(tailleVar==5){ // cas print
                if(strcmp(var, "print") == 0 && *(ligne+compteur)==' '){
                    free(var);
                    printValeur(ligne+compteur+1, sortie);
                    return;
                }
            }
            else if(c=='='){ // cas attribution
                // on realloc notre variable à sa taille réelle
                var=reallocToSize(var,tailleVar);
                attribuerValeur(var,ligne+compteur+1);
                return;
            }
            else if(c==' '){
                foundSpace=1;
            }
            else{
                printError("Caractère invalide");
            }
            compteur++;
            c=*(ligne+compteur);
        }
    }
}


void interpreter(FILE* source, FILE* sortie){
    char* ligne;
    char c;
    int x = 0;
    int y = 0;
    printf("test1\n");
    while(c != EOF){
        c = fgetc(source);
        printf("test2\n");
        while(c != EOF && c != '\n'){
            y += 1;
            c = fgetc(source);
        }
        printf("test3\n");

        ligne = malloc(sizeof(char) * (y - x + 1));
        testMalloc(ligne);
        printf("test4\n");

        fseek(source,x,SEEK_SET);
        printf("test5\n");

        fread(ligne,sizeof(char),y - x, source);
        printf("test6\n");

        ligne[y - x] = '\0'; //cas avec une ligne vide = erreur ?
        printf("test7");
        
        x = y;
        printf("test8\n");
        printf("%s\n", ligne);
        interpreterLineByLine(sortie, ligne);
        printf("test9\n");

    }
}

/*
 int i;
    while(c != EOF){
        ligne=malloc(sizeof(char) * len);
        testMalloc(ligne);
        c = (char)fgetc(source);
        for(i = 0; (c != '\n' && c != EOF); i++){
            if(i >= len - 1){
                len *= 2;
                ligne = realloc(ligne,len);
            }
            ligne[i] = c;
            c = (char)fgetc(source);
        }
        ligne=reallocToSize(ligne,i+1);
        interpreterLineByLine(sortie, ligne);
    }
 */



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
            fclose(fichierSortie);
        }
        else{
            interpreter(fichierEntree,stdout);
        }
        fclose(fichierEntree);
    }
    else if(argc>1 && strcmp(argv[1],"-o")==0){
        fichierSortie=fopen(argv[2],"w");
        interpreter(stdin,fichierSortie);
        fclose(fichierSortie);
    }
    else{
        interpreter(stdin,stdout);
    }


    /* TODO FREE ET FERMER LES FICHIERS A LA FIN DU MAIN ??  --------- fait ------------
     * TODO AJOUTER TESTS ERREUR MALLOC ET REALLOC ---------- fait -----------
     * TODO PRENDRE DES LIGNES PLUS GRANDE QUE 1024 -------- en cours -----------
     * TODO possible d'inverser -i et -o
     * TODO vérifier que toutes les fonctions auxiliaires sont static ------- fait -------
     * TODO RETIRER LES ZEROS INUTILES D'UN UI
     * TODO enlever la première ligne dans le readme pour le rendu
     * TODO enlever les warnings à la compilation ----- fait -----
     * TODO RETIRER LES TRUCS INUTILES DU MAIN DE TEST ------- fait -------
     */
}