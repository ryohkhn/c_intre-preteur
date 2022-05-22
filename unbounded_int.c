#include <stdlib.h>
#include <string.h>
#include "unbounded_int.h"


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
        else{
            nouveau->suivant = tmp;
        }
        nouveau->c =(char)((i%10)+'0');
        tmp->precedent = nouveau;
        tmp = nouveau;
        res.len += 1;
    }
    while(i /= 10);
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
        if(b.signe == '-'){
            if(a.premier->c == '0' && b.premier->c == '0') return 0;
            return 1;
        }
        if(a.len > b.len) return 1;
        if(a.len < b.len) return -1;
    }
    else if(a.signe == '-') {
        if (b.signe == '+') {
            if (a.premier->c == '0' && b.premier->c == '0') return 0;
            return -1;
        }
        if(a.len > b.len) return -1;
        if(a.len < b.len) return 1;
    }


    if(a.len > b.len && a.signe == '-') return -1;
    if(a.len < b.len && a.signe == '-') return +1;

    chiffre * tempa = a.premier;
    chiffre * tempb = b.premier;
    if(a.signe == '+'){
        while(tempa!= NULL){
            if(tempa->c < tempb->c) return -1;
            if(tempa->c > tempb->c) return 1;
            tempa = tempa->suivant;
            tempb = tempb->suivant;
        }
    }else{
        while(tempa!= NULL){
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
    // calcul de la taille de b
    while(bCopy/=10) bSize++;

    // test de signe et de longueur
    if(a.signe == '+'){
        if(b < 0) return 1;
        if(a.len > bSize) return 1;
        if(a.len < bSize) return -1;
    }
    else if(a.signe == '-'){
        if(b > 0) return -1;
        if(a.len > bSize) return -1;
        if(a.len < bSize) return 1;
    }

    // on met le long long en positif s'il est négatif pour permettre la comparaison chiffre par chiffre
    if(b<0){
        b*=-1;
    }

    // transformation du long long en tableau de long long unité par unité
    long long tab[bSize];
    while(bSize--){
        tab[bSize]=b%10;
        b/=10;
    }

    chiffre* nextChiffre=a.premier;
    int compt=0;
    while(nextChiffre!=NULL){
        if(a.signe=='+'){
            if(*(tab+compt)>(nextChiffre->c)-'0'){
                return -1;
            }
            else if(*(tab+compt)<(nextChiffre->c)-'0'){
                return 1;
            }
        }
        else{
            if(*(tab+compt)>(nextChiffre->c)-'0'){
                return 1;
            }
            else if(*(tab+compt)<(nextChiffre->c)-'0'){
                return -1;
            }
        }
        nextChiffre=nextChiffre->suivant;
        compt++;
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
    chiffre* next;
    int add=nextChiffreA->c-'0' + nextChiffreB->c-'0';
    nextChiffre->c = (char)((add % 10)+'0');
    int retenue = add/10;
    res.dernier = nextChiffre;
    nextChiffreA = nextChiffreA->precedent;
    nextChiffreB = nextChiffreB->precedent;
    res.len+=1;

    // boucle qui compare le chiffre A au B
    while(nextChiffreB != NULL && nextChiffreA != NULL) {
        next = malloc(sizeof(chiffre));
        add = nextChiffreA->c-'0' + nextChiffreB->c-'0' + retenue;
        next->c = (char)((add % 10)+'0');
        retenue = add / 10;

        next->suivant = nextChiffre;
        nextChiffre->precedent = next;
        nextChiffre = next;

        nextChiffreA = nextChiffreA->precedent;
        nextChiffreB = nextChiffreB->precedent;

        res.len += 1;
    }
    while(nextChiffreB != NULL){
        next = malloc(sizeof(chiffre));
        add = nextChiffreB->c-'0' + retenue;
        next->c =(char)((add % 10)+'0');
        retenue = add / 10;

        next->suivant = nextChiffre;
        nextChiffre->precedent = next;
        nextChiffre = next;

        nextChiffreB = nextChiffreB->precedent;
        res.len += 1;
    }
    while(nextChiffreA != NULL){
        next = malloc(sizeof(chiffre));
        add = nextChiffreA->c-'0' + retenue;
        next->c =(char)(( add % 10)+'0');
        retenue = add / 10;

        next->suivant = nextChiffre;
        nextChiffre->precedent = next;
        nextChiffre = next;

        nextChiffreA= nextChiffreA->precedent;
        res.len += 1;
    }
    if(retenue!=0){
        next = malloc(sizeof(chiffre));
        next->c=(char)((retenue)+'0');
        next->suivant = nextChiffre;
        nextChiffre->precedent = next;
        nextChiffre = next;
    }
    res.premier = nextChiffre;
    return res;
}

/* renvoie la représentation de la somme de deux entiers représentés par a et b */
unbounded_int unbounded_int_somme(unbounded_int a, unbounded_int b){
    if(a.signe == '+' && b.signe == '+'){
        return unbounded_int_somme_a_b_positifs(a,b);
    }
    else if(a.signe == '-' && b.signe == '-'){
        a.signe = '+';
        b.signe = '+';
        unbounded_int res = unbounded_int_somme_a_b_positifs(a,b);
        res.signe = '-';
        return res;
    }
    else if(a.signe == '+' && b.signe == '-'){
        b.signe = '+';
        return unbounded_int_difference(a,b);
    }
    else{
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
    if(a.signe=='+' && b.signe=='+'){
        if(unbounded_int_cmp_unbounded_int(a,b)==-1){
            unbounded_int res=unbounded_int_difference_a_b_positifs(b,a);
            res.signe='-';
            return res;
        }else {
            return unbounded_int_difference_a_b_positifs(a, b);
        }
    }
    else if(a.signe=='-' && b.signe=='-') {
        a.signe = '+';
        b.signe = '+';
        if (unbounded_int_cmp_unbounded_int(a, b) == -1) { // si a < b APRES inversion des signes (donc a > b AVANT)
            unbounded_int res = unbounded_int_difference_a_b_positifs(b, a);
            return res;

        } else {
            unbounded_int res = unbounded_int_difference_a_b_positifs(a, b);
            res.signe = '-';
            return res;
        }
    }
    else if(a.signe=='+' && b.signe=='-'){
        b.signe='+';
        return unbounded_int_somme_a_b_positifs(a,b);
    }
    else{
        a.signe='+';
        unbounded_int res = unbounded_int_somme_a_b_positifs(a,b);
        res.signe = '-';
        return res;
    }
}


/* renvoie la représentation de leur produit */
static unbounded_int unbounded_int_produit_aux(unbounded_int a, unbounded_int b){
    unbounded_int res={.signe='+',.len=0};
    chiffre* nextChiffreA;
    chiffre* nextChiffreB=b.dernier;
    chiffre* newChiffre;
    chiffre* chiffreZero;
    int retenue;
    int val;
    int val2;
    int decalage=0;

    while (nextChiffreB != NULL) {
        unbounded_int tmpRes = {.signe='+', .len=0};
        nextChiffreA = a.dernier;
        retenue = 0;
        val = (nextChiffreB->c) - '0';

        // si la valeur est à 0 on ne fait pas de mutiplication
        if(val==0){
            nextChiffreB=nextChiffreB->precedent;
            decalage++;
            continue;
        }

        // ajoute les 0 du décalage à l'UI
        for(int i=0;i<decalage;i++){
            chiffreZero=malloc(sizeof(chiffre));
            chiffreZero->c='0';
            if(tmpRes.len==0){
                tmpRes.premier=chiffreZero;
                tmpRes.dernier=chiffreZero;
            }
            else{
                chiffreZero->suivant=tmpRes.premier;
                tmpRes.premier->precedent=chiffreZero;
                tmpRes.premier=chiffreZero;
            }
            tmpRes.len++;
        }

        while(nextChiffreA!=NULL){
            newChiffre=malloc(sizeof(chiffre));
            val2=(nextChiffreA->c)-'0';

            newChiffre->c=(char)(((val*val2+retenue)%10)+'0');
            if(tmpRes.len==0){
                tmpRes.premier=newChiffre;
                tmpRes.dernier=newChiffre;
            }
            else{
                newChiffre->suivant=tmpRes.premier;
                tmpRes.premier->precedent=newChiffre;
                tmpRes.premier=newChiffre;
            }
            tmpRes.len++;

            retenue = (val * val2 + retenue) / 10;
            nextChiffreA = nextChiffreA->precedent;
        }

        // s'il reste une retenue après avoir fait les premières opérations on l'ajoute au bout de l'UI
        if(retenue>0){
            newChiffre= malloc(sizeof(chiffre));
            newChiffre->c=(char)((retenue)+'0');
            newChiffre->suivant = tmpRes.premier;
            tmpRes.premier->precedent = newChiffre;
            tmpRes.premier = newChiffre;
            tmpRes.len++;
        }

        if (res.len != 0) {
            res = unbounded_int_somme(res, tmpRes);
        } else {
            res = tmpRes;
        }
        decalage++;
        nextChiffreB = nextChiffreB->precedent;
    }
    return res;
}


unbounded_int unbounded_int_produit(unbounded_int a, unbounded_int b) {
    if(unbounded_int_cmp_ll(a,0)==0){
        return a;
    }
    if(unbounded_int_cmp_ll(b,0)==0){
        return b;
    }
    if(a.signe=='*'){
        return b;
    }
    if(b.signe=='*'){
        return a;
    }
    if((a.signe=='+' && b.signe=='-') || (a.signe=='-' && b.signe=='+')){
        unbounded_int res=unbounded_int_produit_aux(a,b);
        if(unbounded_int_cmp_ll(res,0)!=0){
            res.signe='-';
        }
        return res;
    }
    // cas ou les deux unbounded sont positifs ou les deux sont négatifs
    return unbounded_int_produit_aux(a,b);
}

/*
static unbounded_int UI_to_Binary(unbounded_int ui){
    chiffre * chiffre1 = malloc(sizeof(chiffre));
    chiffre1->c = '2';
    unbounded_int multiplieur = {.signe = '+', 1, chiffre1,chiffre1};

    chiffre * chiffre2 = malloc(sizeof(chiffre));
    chiffre2->c = '1';
    unbounded_int diviseur_binaire = {.signe = '+', 1, chiffre2,chiffre2};
    int compteur_puissance = 0;

    while(unbounded_int_cmp_unbounded_int(ui,diviseur_binaire) != -1){
        diviseur_binaire = unbounded_int_produit(diviseur_binaire,multiplieur);
        compteur_puissance += 1;
    }
    //cas avec 0 ??

 */


