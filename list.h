#ifndef SING_LIST
#define SING_LIST

typedef struct {
    int first;
    int second;
}pair;

typedef struct List_Node{
    pair value;
    struct List_Node* next;
    struct List_Node* prev;
}List_Node;


typedef struct List {
    List_Node* head;
    List_Node* tail;
}List; 

void push_front(List* obj, pair* value);
void move_front(List* list, List_Node* the_node);
void pop_back(List* list);

#endif
