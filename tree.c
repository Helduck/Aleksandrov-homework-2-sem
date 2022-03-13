#include <stdio.h>
#include <stdlib.h>
#define n 10

struct Elem {
    int val;
    struct Elem * lv;
    struct Elem * pr;
};

void tree_print (struct Elem *p) {
    if (p != NULL) {
        tree_print (p->lv);
        printf ("%d ", p->val);
        tree_print (p->pr);
    }
    return;
}

struct Elem * add_elem (struct Elem * p, int x) {
    if (p == NULL) {
        p = (struct Elem * ) malloc (sizeof (struct Elem));
        p->val = x;
        p->lv = NULL;
        p->pr = NULL;
        return p; 
    }
    if (p->val > x) {
        p->lv = add_elem (p->lv, x);
        return p;
    }
    if (p->val < x) {
        p->pr = add_elem (p->pr, x);
        return p;
    }
    return p;
}

struct Elem * find_elem (struct Elem * p, int x) {
    if (p == NULL) {
        printf ("The tree is empty");
        return NULL;
    }
    if (x != p->val) {
        if (p->pr != NULL) {
            p = find_elem (p->pr, x);
        }
        if (p->lv != NULL) {
            p = find_elem (p->lv, x);
        }
    }
    return p;
}

void tree_destroy (struct Elem * p) {
    if (p != NULL) {
        if (p->pr != NULL) {
            tree_destroy (p->pr);
        }
        if (p->lv != NULL) {
            tree_destroy (p->lv);
        }
        free (p);
    }
    return;
}

void lvl_add (struct Elem *p, int lvl[][n], int k) {
    if (p != NULL) {
        lvl[k][p->val] = p->val;
        lvl_add (p->lv, lvl, k + 1);
        lvl_add (p->pr, lvl, k + 1);
    }
    return;
}

int lvl_max (int lvl[][n], int maxl) {
    for (int i = 0; i < n; i++) {
        int b = 0;
        for (int j = 0; j < n; j++) {
            b+=lvl[i][j];
        }
        if (b == 0) {
            return maxl;
        }
        else {
            maxl++;
        }
    }
    return maxl;
}

void smart_print (int lvl[][n], int maxl, int k) {
    for (int i = 0; i < maxl; i++) {
        for (int j = 0; j < n; j++) {
            if (lvl[i][j] == 0) {
                printf (" ");
            }
            else {
                printf ("%d", lvl[i][j]);
            }
        }
        printf ("\n");
    }
}

void lvl_null (int lvl[][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            lvl[i][j] = 0;
        }
    }
    return;
}

struct Elem * par_search (struct Elem *p, int x) {
    if (p->pr != NULL) {
        if (p->pr->val != x) {
            par_search (p->pr, x);
        }
    }
    if (p->lv != NULL) {
        if (p->lv->val != x) {
            par_search (p->lv, x);
        }
    }
    return p;
}

struct Elem * search_right (struct Elem *p) {
    if (p->pr != NULL) {
        p = search_right (p->pr);
    }
    return p;
}

void del_elem (struct Elem *p, struct Elem *root, int x) {
    if (root == NULL) { //Если дерево пусто, то выводим тот факт, что оно пусто
        printf ("The tree is empty");
        return;
    }
    if ((root->pr == NULL) && (root->lv == NULL)) { //Если дерево состоит только из корня, зануляем указатель на него
        free (p);
        return;
    }
    p = find_elem (p, x); //Теперь ищем наш элемент
    struct Elem *q = par_search (root, x); //Ищем родительский элемент
    if ((p->lv == NULL) && (p->pr == NULL)) { //Проверяем случай отсутствия потомков
        if (q->pr == p) { //Зануляем указатель родительского элемента
            q->pr == NULL;
        }
        else {
            q->lv == NULL;
        }
        free (p);
        return;
    }

    if (p->lv == NULL) { //Смотрим случай, когда у нашего элемента нет левых потомков
        if (p == q->lv) { //Изменяем родительский указатель
            q->lv = p->pr;
        }
        else {
            q->pr = p->pr;
        }
        free (p);
        return;
    }

    if (p->lv != NULL) { //Смотрим случай, когда у элемента есть левые потомки
        struct Elem *r = search_right (p->lv); //Ищем самый правый элемент его левого поддерева
        if (p->lv == r) { //Если родитель самого правого элемента левого поддерева и есть удаляемый элемент, то изменяем указатель родителя и освобождаем память
            if (q->pr == p) {
                q->pr = r;
            }
            else {
                q->lv = r;
            }
            r->pr = p->pr;
            free (p);
            return;
        }
        else { //Если родитель самого правого элемента левого поддерева не является удаляемым элементом, то
            struct Elem *u = par_search (r, x); //Ищем родительский элемент для него
            u->pr = r->lv; //Отдаём родительскому элементу его левых потомков
            r->pr = p->pr;
            r->lv = p->lv; //Отдаём потомков удаляемого элемента самому правому
            if  (q->lv == p) {
                q->lv = r;
            }
            else {
                q->pr = r;
            }
            free (p);
            return;
        }
    }
    return;
}

int main () {
    struct Elem * root = NULL;
    int lvl[n][n];
    lvl_null (lvl);
	root = add_elem (root, 7);
	root = add_elem (root, 5);
	root = add_elem (root, 8); 
	root = add_elem (root, 6); 
	root = add_elem (root, 3); 
	root = add_elem (root, 1); 
	root = add_elem (root, 4); 
	root = add_elem (root, 9);
    printf ("Наше дерево выглядит так:\n");
    tree_print (root);
    printf ("\n");
    lvl_add (root, lvl, 0);
    int maxl = 0;
    maxl = lvl_max (lvl, maxl);
    printf ("Вывод дерева по уровням:\n");
    smart_print (lvl, maxl, n);
    int a = 0;
    printf ("Введите значение элемента, который хотите удалить:\n");
    scanf ("%d", &a);
    del_elem (root, root, a);
    lvl_null (lvl);
    lvl_add (root, lvl, 0);
    //maxl = lvl_max (lvl, maxl);
    printf ("А вот так выглядит дерево с удалённым элементом:\n");
    smart_print (lvl, maxl, n);
    tree_destroy (root);
    return 0;
}

/*void smart_print (struct Elem * q, struct Elem * r) {
    while (p != NULL) {
        struct Elem * q = p->lv;
        struct Elem * r = p->pr;
        print_children (q);
        print_children (r);
        printf ("\n");  
    }
}*/

/*void print_children (struct Elem * p) {
    if (p != NULL) {
        if (p->lv != NULL) {
            printf ("%d ", p->lv->val);
        }
        if (p->pr != NULL) {
            printf ("%d ", p->pr->val);
        }   
    }
    return;
}*/