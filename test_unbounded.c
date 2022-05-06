#include <stdio.h>
#include "unbounded_int.c"

static void print_unbounded_int(const unbounded_int* s){
	printf("%c",s->signe);
	int i=0;
	chiffre* tmp=s->premier;
	while(i<s->len){
        printf("%c",tmp->c);
        if(tmp->suivant!=NULL){
			tmp=tmp->suivant;
		}
		i++;
	}
    printf("\n");
}


int main(void) {

    char* s="126";
    //char* s="157863980";
	char* s2="2149";
	//char* s="82345";
	unbounded_int test1=string2unbounded_int(s);
	//char* s2="8973882";
	unbounded_int test2=string2unbounded_int(s2);
    printf("a = ");
    print_unbounded_int(&test1);
    printf("b = ");
	print_unbounded_int(&test2);

    //print_unbounded_int(&test1);
    //printf("%s",unbounded_int2string(string2unbounded_int("-4543676543298")));
    //printf("%d\n",unbounded_int_cmp_ll(test1,1111));
	//unbounded_int res=unbounded_int_difference(test1,test2);
    //printf("%c test1.dernier.val\n", test1.dernier->c);
    //printf("%c test1.premier.val\n", test1.premier->c);
    unbounded_int res = unbounded_int_produit(test1,test2);
    printf("res = ");
	print_unbounded_int(&res);
    /*
    print_unbounded_int(&test1);
    print_unbounded_int(&test2);
    printf("%d",unbounded_int_cmp_unbounded_int(test1,test2));
*/

	//chiffre * chif = malloc(sizeof(chiffre));
	//chif->c = '0';
	//unbounded_int test3= {.signe = '+', 1,chif,chif};
	//unbounded_int res2 = unbounded_int_somme(test2,test3);
	//printf("res2 = ");
	//print_unbounded_int(&res2);
}
