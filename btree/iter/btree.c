/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL){
    return false;
  }

  while (tree != NULL)
  {
    if(tree->key == key){
      *value = tree->value;
      return true;
    }
    else{
      if(tree == NULL){
        return false;
      }
      if(tree->key > key){
        tree = tree->left;
      }
      else{
        tree = tree->right;
      }
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

bst_node_t *rootTrail = *tree;
bst_node_t *where = NULL;
bool found = false;

while(rootTrail){

  where = rootTrail;

  if(rootTrail->key > key){
      rootTrail = rootTrail->left;
    } 
    else{
      if(rootTrail->key < key){
        rootTrail = rootTrail->right; 
      }
      else{
        found = true; 
        break;
      }
    }
  }
  
  if(!found){

    bst_node_t *insertNode = malloc(sizeof(struct bst_node)); 

    if (insertNode == NULL){
      exit(-1);
    }

    insertNode->value = value;
    insertNode->key = key;   
    insertNode->left = NULL;
    insertNode->right = NULL;
    if(*tree == NULL){
      (*tree) = insertNode;
    }
    else{
      if(where->key > key){
        where->left = insertNode; 
      } 
      else{
        if(where->key < key){
          where->right = insertNode;
        }   
      }
    }
  }
  else{
    where->value = value;
  }
}


/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  while((*tree)->right){
    tree = &(*tree)->right;
  }

  bst_node_t *leftSubtree = NULL;
  leftSubtree = (*tree)->left; //can be null,<
  
  target->value = (*tree)->value;
  target->key = (*tree)->key;

  free(*tree);
  *tree = leftSubtree;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */


void bst_delete(bst_node_t **tree, char key) {
  if(!(*tree)){
    return;
  }

  bst_node_t *delete = *tree;
  bst_node_t *pDelete = NULL;
  while(delete){
    if(delete->key == key){
      
      if(!(delete->left) && !(delete->right)){
        if(!(pDelete)){
          *tree = NULL;
        }
        else if(pDelete->left == delete){
          pDelete->left = NULL;
        }
        else{
          pDelete->right = NULL;
        }
        free(delete);
        break;
      }

      if((delete->left) && !(delete->right)){
        if(!(pDelete)){
          *tree = delete->left;
        }
        else if(pDelete->left == delete){
          pDelete->left = delete->left;
        }
        else{
          pDelete->right = delete->left;
        }
        free(delete);
        break;
      }

      if(!(delete->left) && (delete->right)){
        if(!(pDelete)){
          *tree = delete->right;
        }
        else if(pDelete->left == delete){
          pDelete->left = delete->right;
        }
        else{
          pDelete->right = delete->right;
        }
        free(delete);
        break;
      }
      if((delete->left) && (delete->right)){
        bst_replace_by_rightmost(delete, &delete->left);
        break;
      }

    }
    pDelete = delete;
    if (delete->key < key) {
      delete = delete->right;
    } 
    else {
      delete = delete->left;
    }
  }
}
/*
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t *current_node = *tree;
    bst_node_t *parent_node = NULL;
    while (current_node != NULL){
        if (current_node->key == key){
            if (current_node->left == NULL && current_node->right == NULL){
                if (parent_node == NULL){
                    *tree = NULL;
                } else if (parent_node->left == current_node){
                    parent_node->left = NULL;
                } else {
                    parent_node->right = NULL;
                }
                free(current_node);
                return;
            } else if (current_node->left == NULL) {
                if (parent_node == NULL){
                    *tree = current_node->right;
                } else if (parent_node->left == current_node) {
                    parent_node->left = current_node->right;
                } else {
                    parent_node->right = current_node->right;
                }
                free(current_node);
                return;
            } else if (current_node->right == NULL){
                if (parent_node == NULL) {
                    *tree = current_node->left;
                } else if (parent_node->left == current_node) {
                    parent_node->left = current_node->left;
                } else {
                    parent_node->right = current_node->left;
                }
                free(current_node);
                return;
            } else {
                bst_replace_by_rightmost(current_node, &current_node->left);
                return;
            }
        }
        parent_node = current_node;
        if (current_node->key > key) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }
}

*/


/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t s1;
  stack_bst_init(&s1);

  do {
    if (*tree == NULL){
      if (!stack_bst_empty(&s1)){
        *tree = stack_bst_top(&s1);
        stack_bst_pop(&s1);
      }
    }
    else{
      if ((*tree)->right != NULL){
        stack_bst_push(&s1, (*tree)->right);
      }
      bst_node_t *auxTree = *tree;
      *tree = (*tree)->left;
      free(auxTree);
    }
  }
  while (((*tree) != NULL)||(!stack_bst_empty(&s1)));
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t s1;
  stack_bst_init(&s1);
  bst_leftmost_preorder(tree, &s1);
  
  while(!stack_bst_empty(&s1)){
    tree = stack_bst_top(&s1);
    stack_bst_pop(&s1);
    bst_leftmost_preorder(tree->right, &s1);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t s1;
  stack_bst_init(&s1);
  bst_leftmost_inorder(tree, &s1);
    
  while(!stack_bst_empty(&s1)){
    tree = stack_bst_top(&s1);
    stack_bst_pop(&s1);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, &s1);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  while (tree != NULL){
    stack_bst_push(to_visit, tree); //!!pointer stack
    stack_bool_push(first_visit, true); //!!boolean stack
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  bool fromLeft;
  stack_bst_t s1; //pointer stack
  stack_bool_t sb1; //boolean stack
  
  stack_bst_init(&s1);
  stack_bool_init(&sb1);

  bst_leftmost_postorder(tree, &s1, &sb1);

  while(!stack_bst_empty(&s1)){
    tree = stack_bst_top(&s1);
    fromLeft = stack_bool_top(&sb1);
    stack_bool_pop(&sb1);

    if(fromLeft){
      stack_bool_push(&sb1, false);
      bst_leftmost_postorder(tree->right, &s1, &sb1);
    }
    else{
      stack_bst_pop(&s1);
      bst_print_node(tree);
    }
  }
}
