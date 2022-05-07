#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "unbounded_int.c"
#include "calc_unbounded_int.c"


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

static void testPrintLong(long long a){
	long long copy=a;
	unsigned int ret=1;
	while (copy/=10) ret++;
	long long tab[ret];
	while(ret--){
		tab[ret]=a%10;
		a/=10;
	}
	int j=0;
	while(j<sizeof(tab)/ sizeof(tab[0])){
		printf("%lld",*(tab+j));
		j++;
	}
    printf("\n");
}
static char * randString(int length){
    //srand ( time(NULL) );
    char * dictionnary = "0123456789";
    char * randomString = malloc(sizeof(char) * (length +2));
    if(randomString == NULL) {
        exit(-1);
    }
    int key = rand() % 2;
    randomString[0] = (key == 1)?'+':'-';
    for (int n = 1; n < length; n++) {
        key = rand() % 10;
        randomString[n] = dictionnary[key];
    }
    return randomString;
}

static long long randomll(length){
    //srand ( time(NULL) );
    if(length > 64){
        length = 64;
    }
    long long res = 0;
    for(int n = 0; n < length; n++){
        long long key = rand() % 10;
        for (int i = 1; i < n; i++) {
            key *= 10;
        }
        res += key;
    }
    int key = rand() % 2;
    if(key == 0){
        res *= -1;
    }
    return res;
}

static void testS2UI2S(){
    int grade = 0;
    for(int i = 1; i <= 100; i++){
        for(int j = 1; j <= 100; j++){
            char * s = randString(i);
            unbounded_int to_unbounded_int = string2unbounded_int(s);
            char * to_string = unbounded_int2string(to_unbounded_int);
            if(strcmp(s,to_string) == 0){
                grade += 1;
            }
        }
    }
    printf(" test String to unbounded_int to String : grade of %d / 10000 \n", grade);
}

static void testll2UI(){
    int grade = 0;
    for(int i = 1; i <= 19; i++){ // 19 because it's the length of a long long in base 10.
        for(int j = 1; j <= 100; j++){
            long long ll = randomll(i);
            unbounded_int to_unbounded_int = ll2unbounded_int(ll);
            char * to_string = unbounded_int2string(to_unbounded_int);
            long long to_ll = atoll(to_string);
            if(ll == to_ll){
                grade += 1;
            }
        }
    }
    printf(" test long long to unbounded_int : grade of %d / 1900 \n", grade);
}

static void testUIcmpUI(){
    int gradeTrue = 0;
    int gradeFalse = 0;
    int gradeSign = 0;

    int grade_ui_llTrue = 0;
    int grade_ui_llFalse = 0;
    int grade_ui_llSign = 0;
    for(int i = 1; i <= 10; i++){
        for(int j = 0; j < 10; j++){
            long long base = randomll(i);
            long long base2 = -1 * base;
            long long base3 = randomll(i);

            if(base3 == base){
                base3 += 1;
            }
            unbounded_int ui1 = ll2unbounded_int(base);
            unbounded_int ui2 = ll2unbounded_int(base);
            unbounded_int ui3 = ll2unbounded_int(base2);
            unbounded_int ui4 = ll2unbounded_int(base3);

            //test equals unbounded int
            if(unbounded_int_cmp_unbounded_int(ui1,ui2) == 0) {
                gradeTrue += 1;
            }

            // test equals unbounded int and long long
            if(unbounded_int_cmp_ll(ui1, base) == 0){
                grade_ui_llTrue += 1;
            }else{
                print_unbounded_int(&ui1);
                printf("%lld\n",base);
                printf("comp = %d\n\n",unbounded_int_cmp_ll(ui1, base));
            }

            // test sign unbounded int
            int result_cmp_sign = unbounded_int_cmp_unbounded_int(ui1,ui3);
            if(result_cmp_sign != 0){
                if(base > base2 && result_cmp_sign == 1){
                    gradeSign += 1;
                }else if(base < base2 && result_cmp_sign == -1){
                    gradeSign += 1;
                }
            }else{
                if(base == 0) gradeSign += 1;
            }


            // test sign long long
            int result_cmp_sign_ui_ll = unbounded_int_cmp_ll(ui1,base2);
            if(result_cmp_sign_ui_ll != 0){
                if(base > base2 && result_cmp_sign_ui_ll == 1){
                    grade_ui_llSign += 1;
                }else if(base < base2 && result_cmp_sign_ui_ll == -1){
                    grade_ui_llSign += 1;
                }
            }else{
                if(base == 0) grade_ui_llSign += 1;
            }

            // test non equals unbounded int
            int result_cmp = unbounded_int_cmp_unbounded_int(ui1,ui4);
            if(result_cmp != 0){
                if(base > base3 && result_cmp == 1){
                    gradeFalse += 1;
                }else if(base < base3 && result_cmp == -1){
                    gradeFalse += 1;
                }
            }else{
                if(base == base3) gradeFalse += 1;
            }

            // test non equals unbounded int and long long
            int result_cmp_ui_ll = unbounded_int_cmp_ll(ui1,base3);
            if(result_cmp_ui_ll != 0){
                if(base > base3 && result_cmp_ui_ll == 1){
                    grade_ui_llFalse += 1;
                }else if(base < base3 && result_cmp_ui_ll == -1){
                    grade_ui_llFalse += 1;
                }
            }else{
                if(base == base3) grade_ui_llFalse += 1;
            }
        }
    }
    printf("test comparison unbounded_int and unbounded_int : \n    "
           "- comparison between two identical unbounded_int  %d/1000\n    "
           "- comparison with only the sign varying %d/1000\n   "
           "- comparison with two different unbounded_int %d/1000\n"
           ,gradeTrue,gradeSign,gradeFalse);
    printf("test comparison unbounded_int and long long : \n    "
           "- comparison between two identical numbers  %d/1000\n    "
           "- comparison with only the sign varying %d/1000\n   "
           "- comparison with two different numbers %d/1000\n"
            ,grade_ui_llTrue,grade_ui_llSign,grade_ui_llFalse);
}

int main(void) {
/*
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
    print_unbounded_int(&test1);
    print_unbounded_int(&test2);
    printf("%d",unbounded_int_cmp_unbounded_int(test1,test2));

	//chiffre * chif = malloc(sizeof(chiffre));
	//chif->c = '0';
	//unbounded_int test3= {.signe = '+', 1,chif,chif};
	//unbounded_int res2 = unbounded_int_somme(test2,test3);
	//printf("res2 = ");
	//print_unbounded_int(&res2);
    */

    testS2UI2S();
    testll2UI();
    testUIcmpUI();
}
