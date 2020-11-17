#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shared.h"

int removeChar(char * s, char c){   
    int j, n = strlen(s); 
    for (int i=j=0; i<n; i++) {
       if (s[i] != c) 
          s[j++] = s[i]; 
    } 
    s[j] = '\0'; 
    return 1;
} 

int JSONformat(char * s){
    char result[100]="\"";
    char tampon[100]="";
    strcpy(tampon, s);
    removeChar(tampon,'\n');
    char separator = ',';
    char * sep_at = strchr(tampon, separator);
    while(sep_at != NULL){
        *sep_at = '\0';
        strcat(result, tampon);
        strcat(result, "\",\"");
        strcpy(tampon, sep_at + 1);
        sep_at = strchr(tampon, separator);
    }
    strcat(result, tampon);
    strcat(result, "\"");
    strcpy(s,result);
    return 1;
}

int StringToJSON(char * json_string, Json_object * json_obj){
    char value[100]="";
    char code_value[30]="";
    sscanf(json_string, "{ \"code\" : %s , \"valeurs\" : [%[^]]] }",code_value,value);
    strcpy((*json_obj).code,code_value);
    removeChar(value,'"');
    strcpy((*json_obj).valeur,value);
    return 0;
}

int JSONToString(char * json_string, Json_object * json_obj){
    memset(json_string, 0, sizeof(json_string));
    char value[100]="";
    strcpy(value,(*json_obj).valeur);
    JSONformat(value);
    char code_value[30]="";
    strcpy(code_value,(*json_obj).code);
    char result[1024]="";
    snprintf(result,sizeof(result),"{ \"code\" : \"%s\" , \"valeurs\" : [%s] }",code_value,value);
    strcpy(json_string,result);
}
