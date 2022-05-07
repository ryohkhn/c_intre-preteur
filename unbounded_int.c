#include <stdlib.h>
#include <string.h>
#include "unbounded_int.h"

#include "stdio.h"

unbounded_int string2unbounded_int(const char* e){
    unsigned long stringLen=strlen(e);
    chiffre* premier=malloc(sizeof(chiffre));
    if(premier==NULL || stringLen==0){
        unbounded_int empty={.signe='*'};
        return empty;
    }
    chiffre* tmp;
    unbounded_int res={.premier=premier};
    int i=0;
    int signePresent=0;
    if(*(e)=='-' || *(e)=='+'){
        res.signe=*(e);
        res.len=stringLen-1;
        i++;
        signePresent=1;
    }
    else{
        res.signe='+';
        res.len=stringLen;
    }
    while(i<stringLen){
        if((*(e+i)-'0')<0 || (*(e+i)-'0')>9){
            unbounded_int empty={.signe='*'};
            return empty;
        }
        if((signePresent && i==1) || (!signePresent && i==0)){
            premier->c=*(e+i);
            tmp=premier;
        }
        else{
            chiffre* nouveau=malloc(sizeof(chiffre));
            nouveau->c=*(e+i);
            nouveau->precedent=tmp;
            tmp->suivant=nouveau;
            tmp=nouveau;
        }
        i++;
    }
    res.dernier=tmp;
    return res;
}

unbounded_int ll2unbounded_int(long long i){
    chiffre* premier = malloc(sizeof(chiffre));
    chiffre* dernier = malloc(sizeof(chiffre));
    chiffre* tmp = premier;
    if(premier == NULL || dernier == NULL){
        unbounded_int empty = {.signe='*'};
        return empty;
    }
    unbounded_int res={.premier=premier,.dernier=dernier};
    if(i < 0){
        res.signe = '-';
    }else{
        res.signe = '+';
    }
    i = llabs(i);
    long long x = i;
    do {
        chiffre* nouveau = malloc(sizeof(chiffre));
        if(x == i){ //first iteration
            res.dernier = nouveau;
        }
        nouveau->c = i%10+'0';
        nouveau->suivant = tmp;
        tmp->precedent = nouveau;
        tmp = nouveau;
        res.len += 1;
    }while(i /= 10);
    res.premier = tmp;
    return res;
}


char* unbounded_int2string(unbounded_int i){
    char* res=malloc(sizeof(char)*(i.len+2));
    *res=i.signe;
    int compt=1;
    chiffre* tmp=i.premier;
    while(tmp!=NULL){
        *(res+compt)=tmp->c;
        compt++;
        tmp=tmp->suivant;
    }
    *(res+compt)='\0';
    return res;
}

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b){
    if(a.signe == '+'){
        if(b.signe == '-') return 1;
        if(a.len > b.len) return 1;
        if(a.len < b.len) return -1;
    }else if(a.signe == '-'){
        if(b.signe == '+') return -1;
        if(a.len > b.len) return -1;
        if(a.len < b.len) return 1;
    }


    if(a.len > b.len && a.signe == '-') return -1;
    if(a.len < b.len && a.signe == '-') return +1;

    chiffre * tempa = a.premier;
    chiffre * tempb = b.premier;
    if(a.signe == '+'){
        while(tempa->suivant != NULL){
            if(tempa->c < tempb->c) return -1;
            if(tempa->c > tempb->c) return 1;
            tempa = tempa->suivant;
            tempb = tempb->suivant;
        }
    }else{
        while(tempa->suivant != NULL){
            if(tempa->c > tempb->c) return -1;
            if(tempa->c < tempb->c) return 1;
            tempa = tempa->suivant;
            tempb = tempb->suivant;
        }
    }

    return 0;
}

int unbounded_int_cmp_ll(unbounded_int a, long long b){
    long long bCopy=b;
    unsigned int bSize=1;
    while(bCopy/=10) bSize++;



    // test de signe et de longueur
    if(a.signe == '+'){
        if(b < 0) return 1;
        if(a.len > bSize) return 1;
        if(a.len < bSize) return -1;
    }else if(a.signe == '-'){
        if(b >= 0) return -1;
        if(a.len > bSize) return -1;
        if(a.len < bSize) return 1;
    }


    if(b < 0){
        b *= -1;
    }

    // transformation du long long en tableau de long long unité par unité
    long long * tab = malloc(sizeof(long long) * bSize);
    while(bSize--){
        tab[bSize]=b%10;
        b/=10;
    }

    chiffre* nextChiffre=a.premier;
    int compt=0;
    if(a.signe == '+') {
        while (nextChiffre != NULL) {
            if (tab[compt] > ((nextChiffre->c) - '0')) {
                printf("return n°1 avec %lld vs %d\n",tab[compt],((nextChiffre->c) - '0'));

                return -1;
            } else if (tab[compt] < ((nextChiffre->c) - '0')) {
                printf("return 2\n");

                return 1;
            }
            nextChiffre = nextChiffre->suivant;
            compt++;
        }
    }else{
        while (nextChiffre != NULL) {
            if (tab[compt] > ((nextChiffre->c) - '0')) {
                printf("return 3\n");

                return 1;
            } else if (tab[compt] < ((nextChiffre->c) - '0')) {
                printf("return 4\n");

                return -1;
            }
            nextChiffre = nextChiffre->suivant;
            compt++;
        }
    }
    return 0;
}

// addition de deux unbounded_int positifs
static unbounded_int unbounded_int_somme_a_b_positifs(unbounded_int a, unbounded_int b){
    chiffre * dernier = malloc(sizeof(chiffre));
    if(dernier == NULL){
        unbounded_int empty = {.signe ='*'};
        return empty;
    }
    // création des pointeurs de chiffre pour avancer dans la structure
    unbounded_int res = {.signe = '+', .dernier = dernier};
    chiffre * nextChiffreA = a.dernier;
    chiffre * nextChiffreB = b.dernier;
    chiffre * nextChiffre = malloc(sizeof(chiffre));
    nextChiffre->c = ((nextChiffreA->c-'0' + nextChiffreB->c-'0') % 10)+'0';
    int retenue = (nextChiffreA->c-'0' + nextChiffreB->c-'0')/10;
    res.dernier = nextChiffre ;
    nextChiffreA = nextChiffreA->precedent;
    nextChiffreB = nextChiffreB->precedent;
    res.len += 1;

    // boucle qui compare le chiffre A au B, sur toute la longueur de B
    while(nextChiffreB != NULL && nextChiffreA != NULL) {
        chiffre* next = malloc(sizeof(chiffre));

        int add = nextChiffreA->c-'0' + nextChiffreB->c-'0' + retenue;
        next->c = add % 10 + '0';
        retenue = add / 10;

        next->suivant = nextChiffre;
        nextChiffre->precedent = next;
        nextChiffre = next;

        nextChiffreA = nextChiffreA->precedent;
        nextChiffreB = nextChiffreB->precedent;

        res.len += 1;
    }
    while(nextChiffreB != NULL){
        chiffre* next = malloc(sizeof(chiffre));
        int add = nextChiffreB->c-'0' + retenue;
        next->c = add % 10+'0';
        retenue = add / 10;

        next->suivant = nextChiffre;
        nextChiffre->precedent = next;
        nextChiffre = nextChiffre->precedent;

        nextChiffreB = nextChiffreB->precedent;
        res.len += 1;
    }
    while(nextChiffreA != NULL){
        chiffre* next = malloc(sizeof(chiffre));
        int add = nextChiffreA->c-'0' + retenue;
        next->c = add % 10+'0';
        retenue = add / 10;

        next->suivant = nextChiffre;
        nextChiffre->precedent = next;
        nextChiffre = nextChiffre->precedent;

        nextChiffreA= nextChiffreA->precedent;
        res.len += 1;
    }
    res.premier = nextChiffre;
    return res;
}

/* renvoie la représentation de la somme de deux entiers représentés par a et b */
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b){
    //printf("somme\n");
    if(a.signe == '+' && b.signe == '+'){
        //printf("a et b >= 0\n");
        return unbounded_int_somme_a_b_positifs(a,b);
    }
    else if(a.signe == '-' && b.signe == '-'){
        //printf("a et b < 0\n");

        a.signe = '+';
        b.signe = '+';
        unbounded_int res = unbounded_int_somme_a_b_positifs(a,b);
        res.signe = '-';
        return res;
    }
    else if(a.signe == '+' && b.signe == '-'){
        //printf("a >= 0 et b < 0\n");
        b.signe = '+';
        return unbounded_int_difference(a,b);
    }
    else{ //if(a.signe == '-' && b.signe == '+'){
        //printf("a < 0 et b >= 0\n");

        a.signe = '+';
        return unbounded_int_difference(b,a);
    }
}

// soustraction a-b pour a et b positifs tels que a>=b
static unbounded_int unbounded_int_difference_a_b_positifs(unbounded_int a, unbounded_int b){
    chiffre* dernier=malloc(sizeof(chiffre));
    if(dernier==NULL){
        unbounded_int empty={.signe='*'};
        return empty;
    }
    // création des pointeurs de chiffre pour avancer dans la structure
    unbounded_int res={.signe='+',.dernier=dernier};
    int retenue=0;
    int count=0;
    chiffre* nextChiffre=res.dernier;
    chiffre* nextChiffreA=a.dernier;
    chiffre* nextChiffreB=b.dernier;
    // boucle qui compare le chiffre A au B, sur toute la longueur de B
    while(nextChiffreB!=NULL){
        if((nextChiffreA->c-'0')-(nextChiffreB->c-'0')+retenue>=0){
            nextChiffre->c=(char)(((nextChiffreA->c-'0')-(nextChiffreB->c-'0')+retenue)+'0');
            retenue=0;
        }
        else{
            nextChiffre->c=(char)(((nextChiffreA->c-'0')-(nextChiffreB->c-'0')+retenue+10)+'0');
            retenue=-1;
        }
        // on se déplace au chiffre précédent seulement si on n'est pas au dernier chiffre de la valeur
        if(count<b.len-1){
            chiffre* next=malloc(sizeof(chiffre));
            next->suivant=nextChiffre;
            nextChiffre->precedent=next;
            nextChiffre=nextChiffre->precedent;
        }
        // on va sur le chiffre précédent de A et de B
        nextChiffreB=nextChiffreB->precedent;
        nextChiffreA=nextChiffreA->precedent;
        res.len+=1;
        count++;
    }
    // boucle pour compléter le résultat, si A est plus grand que B
    while(nextChiffreA!=NULL){
        chiffre* next=malloc(sizeof(chiffre));
        if((nextChiffreA->c-'0')+retenue>=0){
            next->c=(char)(((nextChiffreA->c-'0')+retenue)+'0');
            retenue=0;
        }
        else{
            next->c=(char)(((nextChiffreA->c-'0')+retenue+10)+'0');
            retenue=-1;
        }
        next->suivant=nextChiffre;
        nextChiffre->precedent=next;
        nextChiffre=nextChiffre->precedent;
        nextChiffreA=nextChiffreA->precedent;
        res.len+=1;
        count++;
    }
    res.premier=nextChiffre;
    return res;
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b){
    //printf("difference\n");
    if(a.signe=='+' && b.signe=='+'){
        //printf("a et b >= 0\n");
        if(unbounded_int_cmp_unbounded_int(a,b)==-1){
            unbounded_int res=unbounded_int_difference_a_b_positifs(b,a);
            res.signe='-';
            return res;
        }else {
            return unbounded_int_difference_a_b_positifs(a, b);
        }
    }
    else if(a.signe=='-' && b.signe=='-') {
        //printf("a et b < 0\n");
        a.signe = '+';
        b.signe = '+';
        if (unbounded_int_cmp_unbounded_int(a, b) == -1) { // si a > b APRES inversion des signes (donc a < b AVANT)
            unbounded_int res = unbounded_int_difference_a_b_positifs(a, b);
            res.signe = '-';
            return res;

        } else {
            return unbounded_int_difference_a_b_positifs(b, a);
        }
    }
    else if(a.signe=='+' && b.signe=='-'){
        //printf("a >= 0 et b < 0\n");
        b.signe='+';
        return unbounded_int_somme_a_b_positifs(a,b);
    }
    else{  //if(a.signe=='-' && b.signe=='+'){
        //printf("a < 0 et b >= 0\n");
        a.signe='+';
        unbounded_int res = unbounded_int_somme_a_b_positifs(a,b);
        res.signe = '-';
        return res;
    }
}

/* fonction auxilliaire pour le produit. Renvoie le produit d'un unbounded_int par une valeur */
static unbounded_int produit_simple(unbounded_int a, int value){
    unbounded_int res = string2unbounded_int(unbounded_int2string(a));
    if(value == 0){
        chiffre * chif = malloc(sizeof(chiffre));
        chif->c = '0';
        unbounded_int zero = {.signe = '+', 1,chif,chif};
        return zero;
    }
    if(value == 1) return res;
    while(value > 9){
        value /= 10;
        chiffre * next = malloc(sizeof(chiffre));
        next->c = '0';
        next->precedent = res.dernier;
        res.dernier->suivant = next;
        res.dernier = next;
        res.len += 1;
    }
    int retenue = 0;
    chiffre * temp = res.dernier;
    while(temp != NULL){
        int calc = (((temp->c-'0') * value) + retenue) % 10;
        retenue =  (((temp->c-'0') * value) + retenue) / 10;
        temp->c = calc+'0' ;
        temp = temp->precedent;
    }
    if(retenue != 0){
        chiffre * first = malloc(sizeof(chiffre));
        first->c = retenue+'0';
        res.premier->precedent = first;
        first->suivant = res.premier;
        res.premier = first;
        res.len += 1;
    }
    return res;
}

/* renvoie le produit de a par b */
unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b) {
    chiffre * chiffreB = b.dernier;
    int mult = 1;
    chiffre * chif = malloc(sizeof(chiffre));
    chif->c = '0';
    unbounded_int res = {.signe = '+', 1,chif,chif};
    while(chiffreB != NULL){
        int value = (chiffreB->c-'0') * mult;
        unbounded_int res2 = produit_simple(a, value);
        res = unbounded_int_somme(res, res2);
        mult *= 10;
        chiffreB = chiffreB->precedent;
    }
    res.signe = (a.signe != b.signe)?'-':'+';
    return res;
}


