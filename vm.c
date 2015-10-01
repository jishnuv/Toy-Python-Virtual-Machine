#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "PyVm.h"

char *stringref[MAXSIZE];
int n_strref = 0;
int main(int argc, char** argv)
{
    FILE *fp;
    unsigned char buffer[8], c;
    code_object *cobj;
    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        perror("error");
        exit(1);
    }
    fread(buffer, 8, 1, fp);
    c = getc(fp);
    if (c != 0x63) {
        fprintf(stderr,"Expected a code object !\n");
        exit(1);
    }
    cobj = handle_code_object(fp);
    fclose(fp);
    run(cobj, cobj->names, cobj->name_value, cobj->n_name);
    return 0;
}

code_object *handle_code_object(FILE *fp)
{
    int size, c;
    code_object *code;
    code = (code_object *) malloc(sizeof (code_object));
    code->argcount = r_long(fp);
    code->nlocals = r_long(fp);
    code->stacksize = r_long(fp);
    code->flags = r_long(fp);
    c = getc(fp);
    if (c != 0x73) {
        fprintf(stderr, "Expected a string object !\n");
        exit(1);
    }
    code->code_size = r_long(fp);
    code->co_code = (unsigned char *) malloc(code->code_size);
    fread(code->co_code, code->code_size, 1, fp);

    c = getc(fp);
    if (c != 0x28) {
        fprintf(stderr,"Expected a tuple object !\n");
        exit(1);
    }
    code->constant = get_tuple(fp, &code->n_const);

    c = getc(fp);
    if (c != 0x28) {
        fprintf(stderr,"Expected a tuple object !\n");
        exit(1);
    }
    code->names = get_tuple(fp, &code->n_name);

    c = getc(fp);
    if (c != 0x28) {
        fprintf(stderr,"Expecting a tuple object !\n");
        exit(1);
    }
    code->varnames = get_tuple(fp, &code->n_varname);

    c = getc(fp);
    if (c != 0x28) {
        fprintf(stderr,"Expected a tuple object !\n");
        exit(1);
    }
    code->freevars = get_tuple(fp, &code->n_freevar);

    c = getc(fp);
    if (c != 0x28) {
        fprintf(stderr,"Expected a tuple object !\n");
        exit(1);
    }
    code->cellvars = get_tuple(fp, &code->n_cellvar);

    c = getc(fp);
    size = r_long(fp);
    code->file_name = get_strings(fp, size);

    c = getc(fp);
    size = r_long(fp);
    code->fun_name = get_strings(fp, size);
    if(c == TYPE_INTERNED)
        stringref[n_strref++] = code->fun_name;

    code->firstlno = (int) r_long(fp);

    c = getc(fp);
    size = r_long(fp);
    code->lnotab = get_strings(fp, size);
    return code;
}

PyObject *get_tuple(FILE *fp, int *size)
{
    PyObject *array;
    int c, i, n;
    *size = n = (int) r_long(fp);
    array = (PyObject *) malloc(n * sizeof (PyObject));
    for (i = 0; i < n; i++) {
        c = getc(fp);
        switch (c) {
        case TYPE_NULL:
            array[i].type = TYPE_NULL;
            break;
        case TYPE_NONE:
            array[i].type = TYPE_NONE;
            break;
        case TYPE_TRUE:
            array[i].type = TYPE_TRUE;
            break;
        case TYPE_FALSE:
            array[i].type = TYPE_FALSE;
            break;
        case TYPE_INT:
            array[i].type = TYPE_INT;
            array[i].data.i = r_long(fp);
            break;
        case TYPE_BINARY_FLOAT:
            array[i].type = TYPE_BINARY_FLOAT;
            array[i].data.f = r_float(fp);
            break;
        case TYPE_INTERNED:
        case TYPE_STRING:
            array[i].type = TYPE_STRING;
            array[i].size = r_long(fp);
            array[i].data.s = get_strings(fp, array[i].size);
            if(c == TYPE_INTERNED)
            {
                array[i].type = TYPE_INTERNED;
                stringref[n_strref++] = array[i].data.s;
            }
            break;
        case TYPE_TUPLE:
            array[i].type = TYPE_TUPLE;
            array[i].data.tple = get_tuple(fp, &array[i].size);
            break;
        case TYPE_CODE:
            array[i].type = TYPE_CODE;
            array[i].data.codeobj = handle_code_object(fp);
            break;
        case TYPE_STRINGREF:
            array[i].type = TYPE_STRINGREF;
            array[i].size = r_long(fp);
            array[i].data.s = stringref[array[i].size];
            break;
        default:
            break;
        }
    }
    return array;

}

long r_long(FILE *fp)
{
    long x;
    x = getc(fp);
    x |= (long) getc(fp) << 8;
    x |= (long) getc(fp) << 16;
    x |= (long) getc(fp) << 24;
    return x;
}

float r_float(FILE *fp)
{
    double f;
    unsigned char raw_input[8];
    raw_input[0]  = getc(fp);
    raw_input[1]  = getc(fp);
    raw_input[2]  = getc(fp);
    raw_input[3]  = getc(fp);
    raw_input[4]  = getc(fp);
    raw_input[5]  = getc(fp);
    raw_input[6]  = getc(fp);
    raw_input[7]  = getc(fp);
    memcpy(&f, raw_input, sizeof(f));
    return (float)f;
}

char *get_strings(FILE *fp, int size)
{
    char temp[100], *c;
    fread(temp, size, 1, fp);
    temp[size] = '\0';
    c = (char *) malloc(size + 1);
    strcpy(c, temp);
    return c;
}
