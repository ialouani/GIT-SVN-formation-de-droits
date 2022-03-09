#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//2 remarques primordiales:
//1->copiage via des char et retour du 1er pointeur meme type argument on parle de la meme identification-- donc change pas l'adresse donc retour pointe vers le debut et ajout d'affectation sur '\0' en elargissant le block alloue pour ok valgrind (pas de invalide write of size 1) puis on ajoute *s2='\0' une instruction pareille que dans la boucle: *s1=*s2 pour que s1 retournee pointe vers une vraie chaine donc la reconnaise de '\0' produit la bonne mecanique des choses afin d'eviter les caracteres BIZARRES.
//2->allouer un char* puis affecter une valeur n'est pas trop top d'ailleurs ca bloque un peu au niveau de gcc et deuxiement une adresse sur la pile pointant vers un segmen e donnees du tas ne pas se voir affecter un section .text partie des strings statiques,... donc *()++seulement..
//3->Conditional jump or move on unizialised values: SIGNIFIE CEST CLAIR ICI que lorqu'on affiche *s1 avec s1 sortant de la boucle verifiant !s1 et incremente encore donc adresse non valide si le block d'allocation est sur mesure (*(s1+1) avec *s1 pour conditional jump no) ;par ex:  //  printf("-------%c %d---------------------\n",*(s1+1),len); en la mettant en commentaires elle a supprime 4 erreurs de valgrind.
//1typeError: |free| --leak-check=full
//2eme..: en general, DEPASSEMENT D'INDICES OU BLOCK ALLOUE INSUFFISANT --track-origins=yes ou PARFOIS: --vgdb-error=1 puis alt+f2+gnome-terminal+cd $(pwd) + gdb ./a.out + target remote | /usr/lib/valgrind/../../vgdb/me--pid=.... .//

char* str_copy(char* dst, char* src){
  do{
    *dst=*src;//sinon (*) ca va poser probleme => dst tjrs via calloc si besoin.
    dst++;
    src++;//chaine de depart0(source) non vide bien evidemment.
  }while(*src);
  return dst-(int)strlen(src);
}

int str_len(char* s);

char* str_copy2(char* dst, char* src){
    while(( *dst++ = *src++ )){}
  return dst-(int)str_len(src);
}

int str_cmp(char* s, char* ss){
  return s-ss;
}

char* str_cat(char* s1, char* s2){
  char* s3=malloc(sizeof(char)*(strlen(s1)+strlen(s2)+1));
  int k=0;
  for(int a=0;a<str_len(s1);a++){
    s3[k]=s1[a];k++;}
  for(int b=0;b<str_len(s2);b++){
    s3[k]=s2[b];k++;}
  //free(s1);free(s2);
  s3[k]='\0';
  return s3;
}

/*void slice_str(const char * str, char * buffer, size_t start, size_t end)
{
    size_t j = 0;
    for ( size_t i = start; i <= end; ++i ) {
        buffer[j++] = str[i];
    }
    buffer[j] = 0;
    }*/
char* str_cat2(char* s1,char* s2){
  //s1: '?'..\0
  //s2 de meme.
  int i=0;int len=str_len(s1);
  while(i<len){
    //printf("%c %d\n",*s1,len);//(tres importante)
    s1++;
    i++;
  }
  /**s1=*s2;
  *(s1+1)=*(s2+1);
  *(s1+2)=*(s2+2);*/
  int len2=str_len(s2);int cpt=0;
  while(*(s2) && cpt<len2-1){
    *s1=*s2;
    s1++;s2++;
  }
  *s1=*s2;
  //  printf("-------%c %d---------------------\n",*(s1+1),len);
  //pas memoire tel que le processus n'as pas le droit d'y acceder.
  return s1;
  //printf("\n\n");
  //*s1 avant dernier charactere.(~~~)
  //printf("%s(pas c....)\n",*s1+'0'..);//(***)___4!(())///////////
}
  
//reecriture rapide!!:::::
char* str_cat22(char* s1, char* s2){
  while(*s1) s1++;
  while(*s2) *s1++=*s2++;
  *s1=*s2;//COPIAGE DU ZERO TERMINAL POUR EVITER LES AFFICHAGES BIZARRES..///
  return s1;
  //s1 n'as pas change mais ses valeurs pointees par incrementation ont change
  //bien sur! (2###)/////
}

int main(){
  char* s1="abcde";
  //size_t len=strlen(s1);
  char* s2=calloc(str_len(s1)+1,sizeof(char));//initialisation importante(*)
  str_copy(s2,s1);
  printf("%s\n",s2);
  printf("%d:%d\n\n",str_len(s1),str_len(s2));
  free(s2);
  char* ss="erftge";
  char* s2_2=calloc(str_len(ss)+1,sizeof(char));
  str_copy2(s2_2,ss);
  // printf("%d\t%d\t%s:%s\n",str_len(s2),str_len(ss),ss,s2_2);s2_2 pas s2.//
  printf("%d\t%d\t%c:%c\n",str_len(s2_2),str_len(ss),*ss,*s2_2);
  char* ss2=calloc(str_len(s2_2),sizeof(char));
  *ss2='a';*(ss2+1)='b';*(ss2+2)='c';*(ss2+3)='d';*(ss2+4)='e';//(**)___1
  //ss2="abcde";(1:-----------------a eviter pour des problemes
  //d'anti-optimisation par le compilateur couple au VALGRIND.....)
  //ss2=realloc(ss2,2*strlen(s2_2)+2);
  printf("%d: ss est apres s dans l'ordre lexicographique:\n",str_cmp(ss,s1));
  printf("%d      %d***************************************  :%s--%s\n",str_len(s2_2),str_len(ss2),ss2,s2_2);//(**)___2(/à: str_len(s2_2).) (juste attention)
  char* rslt=str_cat(ss2,s2_2);
  free(ss2);free(s2_2);//impossible de retourner un tableau!!!!!
  printf("%s: concatenation de abcde et erfgte\n",rslt);//(**)___3
  //ajouter le zero terminal pour que ca marche..
  //voir justification plus haut..///...
  printf("%c: concatenation de abcde et erfgte\n",*(rslt+7));
  //abcde et erftge avec un char* s3 (pile/retour --fonction);
  //ON a! char* s3 remplit l'overhead de s1 par les elements de s2
  //tout en s'incrementant ..probleme d'ordre: s3=s1+strlen(s1) permet de pointer vers le '\0' de s1 puis *s3=*s2 avec s3++ et s2++ tant QUE levaluation *s3=*s2 ou disons operation plutot retourne une valeur de $(echo $?)==v==1(###0..///);OK.
  /*char buffer1[str_len(ss2)];char buffer2[str_len(s2_2)];
  slice_str(rslt,buffer1,0,str_len(ss2));
  slice_str(rslt,buffer2,str_len(ss2)+1,str_len(ss2)+str_len(s2_2));
  free(buffer2);free(buffer1);*/
  free(rslt);
  //free(s2);//independant..(DEJA FAITE!!!)
  //free(s2_2);//independant..
  //free(ss2);
  char* sss1=malloc(sizeof(char)*7);
  char* sss2=malloc(sizeof(char)*3);
  *sss1='a';*(sss1+1)='b';*(sss1+2)='c';sss1[3]='\0';*sss2='d';
  *(sss2+1)='e';sss2[2]='\0';
  //char* rslt;
  //getchar(rslt);
  str_cat2(sss1,sss2);
  printf("%s\n",sss1);
  printf("%c\n",*(sss1+4));
  char* vsss1=malloc(sizeof(char)*7);
  char* vsss2=malloc(sizeof(char)*3);
  *vsss1='a';*(vsss1+1)='b';*(vsss1+2)='c';vsss1[3]='\0';*vsss2='d';
  *(vsss2+1)='e';vsss2[2]='\0';
  //char* rslt;
  //getchar(rslt);
  str_cat22(vsss1,vsss2);
  printf("%s\n",vsss1);
  printf("%c\n",*(vsss1+4));
  free(vsss1);
  free(sss1);
  free(vsss2);
  free(sss2);
  return 0;
}
  
int str_len(char* s){
  size_t len=0;
  while(*s++) len++;
  return len;
}



