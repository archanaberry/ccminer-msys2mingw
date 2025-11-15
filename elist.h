#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#include <stdint.h>   /* DITAMBAHKAN untuk uintptr_t */

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/* Insert a new entry between two known consecutive entries. */
static __inline void __list_add(struct list_head *lnew,
                              struct list_head *prev,
                              struct list_head *next)
{
    next->prev = lnew;
    lnew->next = next;
    lnew->prev = prev;
    prev->next = lnew;
}

static __inline void list_add(struct list_head *lnew, struct list_head *head)
{
    __list_add(lnew, head, head->next);
}

static __inline void list_add_tail(struct list_head *lnew, struct list_head *head)
{
    __list_add(lnew, head->prev, head);
}

static __inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

static __inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = (struct list_head *) 0;
    entry->prev = (struct list_head *) 0;
}

static __inline void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

static __inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}

static __inline void list_move_tail(struct list_head *list,
                                  struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}

static __inline int list_empty(struct list_head *head)
{
    return head->next == head;
}

static __inline void __list_splice(struct list_head *list,
                                 struct list_head *head)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;
    struct list_head *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

static __inline void list_splice(struct list_head *list, struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head);
}

static __inline void list_splice_init(struct list_head *list,
                                    struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head);
        INIT_LIST_HEAD(list);
    }
}

/*
 * FIX: gunakan uintptr_t supaya tidak terjadi
 * "cast loses precision" di MinGW64 / Windows
 */
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(uintptr_t)(&((type *)0)->member)))

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); \
            pos = pos->next)

#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); \
            pos = pos->prev)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

#define list_for_each_entry(pos, head, member, tpos)                \
    for (pos = list_entry((head)->next, tpos, member);  \
         &pos->member != (head);                   \
         pos = list_entry(pos->member.next, tpos, member))

#define list_for_each_entry_safe(pos, n, head, member, tpos, tn)            \
    for (pos = list_entry((head)->next, tpos, member),  \
        n = list_entry(pos->member.next, tpos, member); \
         &pos->member != (head);                     \
         pos = n, n = list_entry(n->member.next, tn, member))

#define list_for_each_entry_continue(pos, head, member, tpos)           \
    for (pos = list_entry(pos->member.next, tpos, member),  \
             prefetch(pos->member.next);            \
         &pos->member != (head);                    \
         pos = list_entry(pos->member.next, tpos, member), \
             prefetch(pos->member.next))

#endif