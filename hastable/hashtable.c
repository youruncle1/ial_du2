/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  
  if(!table){
    return;
  }

  for(int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL;
  } 
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  
  if(table){
    ht_item_t *table_item;
    int hash_index = get_hash(key);
    table_item = (*table)[hash_index];

    while(table_item){
      if(table_item->key == key){
        return table_item;
      }
      table_item = table_item->next;
    }
  }

  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  
  if(!table){
    return;
  }

  ht_item_t *table_item;
  table_item = ht_search(table, key);
  if(table_item){
    table_item->value = value;
  }
  else{
    int hash_index = get_hash(key);
    
    table_item = malloc(sizeof(struct ht_item));
    if(!table_item){
      exit(-1);
    }

    table_item->value = value;
    table_item->key = key;
    table_item->next = (*table)[hash_index];

    (*table)[hash_index] = table_item;
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  
  if(!table){
    return NULL;
  }
  
  ht_item_t *table_item;
  table_item = ht_search(table, key);
  if (table_item == NULL){
    return NULL;
  }
  return &table_item->value;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    
    if(!table){
      return;
    }
    
    //ht_item_t *del_item;
    //del_item = ht_search(table, key);
    //if (!del_item){
    //  return NULL;
    //}
    ht_item_t *item;
    ht_item_t *item_prev;
    int hash_index = get_hash(key);

    item = (*table)[hash_index];
    item_prev = NULL;

    while(item){
      //if((item->key == key) && (!item_prev)){
      if(item->key == key){
        if(!item_prev){
          (*table)[hash_index] = item->next;
        }
        else{
          item_prev->next = item->next;
        }
        free(item);
        break;
      }
      item_prev = item;
      item = item->next;
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  if(!table){
    return;
  }
  
  ht_item_t *next_item;

  for(int index = 0; index < HT_SIZE; index++){
    next_item = (*table)[index];
    while(next_item){
      ht_delete(table, next_item->key);
      next_item = next_item->next;
    }
    (*table)[index] = NULL;
  }
}
