#include "linkedlist.h"
#include <stdlib.h>

linkedlist *linkedlist_create() {
	linkedlist *cur = malloc(sizeof(linkedlist));
	cur->len = 0;
	cur->first = 0;
	cur->last = 0;
	return cur;
}

void linkedlist_add(linkedlist *l,void *v,int loc) {
	if (loc == l->len) {
		linkedlist_add_last(l,v);
		return;
	} else if (loc == 0) {
		linkedlist_add_first(l,v);
		return;
	} else if (loc > l->len) {
		return;
	}
	linkedlist_node *n = malloc(sizeof(linkedlist_node));
	int a;
	linkedlist_node *next = l->first;
	for (a = 0;a < loc;a++)
		next = next->next;
	n->prev = next->prev;
	n->next = next;
	next->prev->next = n;
	next->prev = n;
	n->data = v;
	l->len++;
}
void linkedlist_add_first(linkedlist *l,void *v) {
	linkedlist_node *n = malloc(sizeof(linkedlist_node));
	n->next = l->first;
	l->first->prev = n;
	n->prev = 0;
	n->data = v;
	l->first = n;
	l->len++;
	if (!l->last)
		l->last = n;
}
void linkedlist_add_last(linkedlist *l,void *v) {
	linkedlist_node *n = malloc(sizeof(linkedlist_node));
	n->prev = l->first;
	l->first->prev = n;
	n->next = 0;
	n->data = v;
	l->last = n;
	l->len++;
	if (!l->first)
		l->first = n;
}
void *linkedlist_get(linkedlist *l,int loc) {
	if (loc == l->len) {
		return linkedlist_get_last(l);
	} else if (loc == 0) {
		return linkedlist_get_first(l);
	} else if (loc > l->len) {
		return 0;
	}
	linkedlist_node *next = l->first;
	int a;
	for (a = 0;a < loc;a++)
		next = next->next;
	return next->data;
}
void *linkedlist_get_first(linkedlist *l) {
	if (l->len == 0) return 0;
	return l->first->data;
}
void *linkedlist_get_last(linkedlist *l) {
	if (l->len == 0) return 0;
	return l->last->data;
}
void linkedlist_remove(linkedlist *l,int loc) {
	if (loc == l->len) {
		linkedlist_remove_last(l);
		return;
	} else if (loc == 0) {
		linkedlist_remove_first(l);
		return;
	} else if (loc > l->len) {
		return;
	}
	linkedlist_node *next = l->first;
	int a;
	for (a = 0;a < loc;a++)
		next = next->next;
	next->prev->next = next->next;
	next->next->prev = next->prev;
}
void linkedlist_remove_first(linkedlist *l) {
	if (l->len == 0) return;
	l->first = l->first->next;
}
void linkedlist_remove_last(linkedlist *l) {
	if (l->len == 0) return;
	l->last = l->last->prev;
}
void linkedlist_remove_object(linkedlist *l,void *v) {
	int loc = l->len;
	linkedlist_node *next = l->first;
	int a;
	for (a = 0;a < loc;a++) {
		if (next->data == v) {
			break;
		}
		next = next->next;
	}
	if (next->next)
		next->next->prev = next->prev;
	else
		l->last = next->prev;
	if (next->prev)
		next->prev->next = next->next;
	else
		l->first = next->next;
}
