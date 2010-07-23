#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct _linkedlist_node {
	void *data;
	struct _linkedlist_node *prev;
	struct _linkedlist_node *next;
} linkedlist_node;

typedef struct {
	int len;//Can be used for smarter get
	linkedlist_node *first;
	linkedlist_node *last;
} linkedlist;

extern linkedlist *linkedlist_create();
extern void linkedlist_add(linkedlist *,void *,int);
extern void linkedlist_add_first(linkedlist *,void *);
extern void linkedlist_add_last(linkedlist *,void *);
extern void *linkedlist_get(linkedlist *,int);
extern void *linkedlist_get_first(linkedlist *);
extern void *linkedlist_get_last(linkedlist *);
extern void linkedlist_remove(linkedlist *,int);//Index specific
extern void linkedlist_remove_object(linkedlist *,void *);//Memory specific
extern void linkedlist_remove_first(linkedlist *);
extern void linkedlist_remove_last(linkedlist *);

/*#define LINKEDLIST_CREATE(type) linkedlist_create
#define LINKEDLIST_ADD(list,obj,loc) linkedlist_add(list,obj,loc)
#define LINKEDLIST_ADD_FIRST(list,obj) linkedlist_add_first(list,obj)
#define LINKEDLIST_ADD_LAST(list,obj) linkedlist_add_last(list,obj)*/
#define LINKEDLIST_GET(list,loc,type) (type)linkedlist_get(list,loc)
#define LINKEDLIST_GET_FIRST(list,type) (type)linkedlist_get_first(list)
#define LINKEDLIST_GET_LAST(list,type) (type)linkedlist_get_last(list)
//These are here to provide simple casting 

#endif
