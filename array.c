#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum ArrayData_T {
    _NONE,
    _STRING,
    _INT,
    _LONG,
    _cSTRING,
    _cARRAY,
    _ELEMENT,
    _cJSON,
    _FIELD
};

typedef struct Element {
    enum ArrayData_T    typ;
    void                *value;
} Element;

typedef struct Array {
    Element             **arr;
    long                idx;

    void                (*Set)          (struct Array *a, enum ArrayData_T dt, void *value);
    void *              (*Get)          (struct Array *a, int idx);
    char *              (*GetAsString)  (struct Array *s, int idx);
} Array;

void set(Array *a, enum ArrayData_T dt, void *value);
void *get(Array *a, int idx);
char *get_as_str(Array *a, int idx);

Array *new_mixed_array() {
    Array *a = (Array *)malloc(sizeof(Array));
    a->arr = (Element **)malloc(sizeof(Element *) * 1);
    memset(a->arr, '\0', sizeof(Element *) * 1);
    a->idx = 0;

    a->Set          = set;
    a->Get          = get;
    a->GetAsString  = get_as_str;
    return a;
}

int main() {
    Array *a = new_mixed_array();

    char *test_str = "Test";
    int test_int = 4;
    long test_long = 3000000000L;

    /* Add element(s) to the array ( In this example/test, 3 different type(s) ) */
    a->Set(a, _STRING, (void *)test_str);
    a->Set(a, _INT, (void *)&test_int);
    a->Set(a, _LONG, (void *)&test_long);

    // If you want the actual value in its type, you'd do something like this approach
    char *data = (char *)a->Get(a, 0);
    printf("%s\n", data);
    
    int num = *(int *)a->Get(a, 1);
    printf("%d\n", num);

    for(int i = 0; i < a->idx; i++) {
        // One Way To Get Element Value
        char *val = (char *)a->GetAsString(a, i);
        printf("VALUE: %s\n", val);

        // Second Way To Get Element Info
        Element *e = a->arr[i];
        switch (e->typ) {
            case _STRING:
                printf("[%d] %s\n", i, (char *)e->value);
                break;
            case _INT:
                printf("[%d] %d\n", i, *(int *)e->value);
                break;
            case _LONG:
                printf("[%d] %ld\n", i, *(long *)e->value);
                break;
            default:
                continue;
        }
    }

    return 0;
}

void set(Array *a, enum ArrayData_T dt, void *value) {
    if(a == NULL) return;
    a->arr              = (Element **)realloc(a->arr, sizeof(Element *) * (a->idx + 1));
    Element *e          = (Element *)malloc(sizeof(Element));
    e->value            = value;
    e->typ              = dt;
    a->arr[a->idx++]    = e;
}

void *get(Array *a, int idx) {
    if (idx >= a->idx || idx < 0) return NULL; // Check bounds
    return a->arr[idx]->value;
}

char *get_as_str(Array *a, int idx) {
    if (idx >= a->idx || idx < 0) return NULL; // Check bounds

    Element *e = a->arr[idx];
    if(e->typ == _STRING)
        return (char *)e->value;

    char *n = (char *)malloc(100);
    if (n == NULL)
        return NULL;

    switch (e->typ) {
        case _INT:
            snprintf(n, 100, "%d", *(int *)e->value);
            break;
        case _LONG:
            snprintf(n, 100, "%ld", *(long *)e->value);
            break;
        default:
            free(n);
            return NULL;
    }

    return n;
}
