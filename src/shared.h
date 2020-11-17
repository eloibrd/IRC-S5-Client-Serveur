#ifndef __SHARED_H__
#define __SHARED_H__

typedef struct Json_object Json_object;
struct Json_object{
  char code[30];
  char valeur[100];
};

int removeChar(char * s, char c);
int JSONformat(char * s);
int StringToJSON(char * json_string, Json_object * json_obj);
int JSONToString(char * json_string, Json_object * json_obj);

#endif
