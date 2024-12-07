#include<stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"

void init_list(List* list) {
    list->head = NULL;
    list->tail = NULL;
}

void push_front(List* list, pair* value) {
    if (list->head == NULL) {
        list->head = (List_Node*)malloc(sizeof(List_Node));
        list->head->value.first   = value->first;
        list->head->value.second  = value->second;
        list->tail = list->head;
        list->tail->next = NULL;  
        list->tail->prev = NULL;
    }
    else {
     List_Node* tmp = list->head;
     list->head = (List_Node*)malloc(sizeof(List_Node));
     list->head->value.first = value->first;
     list->head->value.second = value->second;
     list->head->next = tmp;
     tmp->prev = list->head;
     list->head->prev = NULL;
    }
}

void print_list(List* hd) {
    List_Node* first = hd->head;
    while (first != NULL) {
        printf("%d->", first->value.first);
        printf("%d->", first->value.second);
        first = first->next;
    }
    fputs("NULL\n", stdout);
}

void move_front(List* list, List_Node* the_node) {
    if (the_node == list->head) return;

    else if (the_node == list->tail) {
        the_node->next = list->head;
        the_node->prev->next = NULL;
        list->head->prev = the_node;
        the_node->prev = NULL;
        list->head = the_node;
    }

    else {
      the_node->prev->next = the_node->next;
      the_node->next->prev = the_node->prev;
      the_node->next = list->head;
      the_node->prev = NULL;
      list->head->prev = the_node;
      list->head = the_node;
    }
}

void pop_back(List* list) {
    assert(list->tail != NULL && "list is empty"); 
    List_Node* tmp = list->tail;
    if (list-> tail->prev != NULL) { 
       list->tail = tmp->prev;
       list->tail->next = NULL;
    }
    else {
        list->tail = NULL;
        list->head = NULL;
    }
    free(tmp);
}

void erase_elem(List* list, List_Node* node) {
    int node_index = 0;
    List_Node* tmp = list->head;

    while(tmp != NULL) {
        if (tmp == node) break;
        ++node_index;
        tmp = tmp->next;
    }

    List_Node* rem_node = list->head;
    for(int i = 0; i < node_index; ++i) {
        rem_node = rem_node->next;
    }

    assert(rem_node != NULL && "Unable remove empty node");

    if (rem_node == list->head) {
        list->head = rem_node->next;
    }
    else if(rem_node == list->tail) {
        list->tail = rem_node->prev;
        list->tail->next = NULL;
    }
    else {
        rem_node->prev->next = rem_node->next;
        rem_node->next->prev = rem_node->prev;
    }
    free(rem_node);
}

List_Node* begin(List* list) {
    return list->head;
}
/*
int main() {
    List list = {
        .head = NULL,
        .tail = NULL,
    };
    pair value = {
        .first = 4,
        .second = 75
    };
    push_front(&list, &value);
    print_list(&list);
}
*/
