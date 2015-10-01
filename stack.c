#include<stdio.h>
#include "PyObject.h"
#define MAXVAL 2000

int sp = 0;
PyObject *val[MAXVAL];

void push(PyObject *pyobj)
{

    if (sp < MAXVAL) {
        val[sp++] = pyobj;
    } else
        fprintf(stderr,"error: stack full, can't push");
}

/* pop: pop and return top value from stack */
PyObject *pop(void)
{
    if (sp > 0) {
        return val[--sp];
    } else {
       fprintf(stderr,"error: stack empty\n");
        return 0;
    }
}

PyObject *getelement(int position)
{
    if (sp - position - 1 >= 0) {
        return val[sp - position - 1];
    } else {
        fprintf(stderr,"error\n");
        return 0;
    }
}
