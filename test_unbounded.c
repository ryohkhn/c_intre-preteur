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
    //char* s="-4543676543298";
    char* s="123";
    unbounded_int test1=string2unbounded_int(s);
    //print_unbounded_int(&test1);
    printf("%s",unbounded_int2string(test1));
}
