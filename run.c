#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "PyVm.h"
extern char *stringref[1000];
extern int n_strref;
PyObject *run(code_object *cobj, PyObject *glob_name, PyObject *global_nvalue[], int n_global)
{
    int ip = 0, operand, c;
    code_object *co_temp;
    PyObject *temp1, *temp2, *temp3, *fun;

    while (ip < cobj->code_size) {

        c = *(cobj->co_code + ip);
        // printf("c = %d %x \n",c,c);
        switch (c) {
        case LOAD_CONST:
            operand = get_operand(cobj->co_code + ip);
            push((cobj->constant) + operand);
            ip = ip + 3;
            break;
        case STORE_NAME:
            operand = get_operand(cobj->co_code + ip);
            cobj->name_value[operand] = pop();
            ip = ip + 3;
            break;
        case LOAD_NAME:
            operand = get_operand(cobj->co_code + ip);
            push(cobj->name_value[operand]);
            ip = ip + 3;
            break;
        case BINARY_ADD:
        case BINARY_SUBTRACT:
        case BINARY_MULTIPLY:
        case BINARY_DIVIDE:
        case BINARY_MODULO:
            temp1 = pop();
            temp2 = pop();
            temp3 = ALOperation(temp2, temp1, c);
            push(temp3);
            ip = ip + 1;
            break;
        case PRINT_ITEM:
            print(pop());
            ip = ip + 1;
            break;
        case PRINT_NEWLINE:
            printf("\n");
            ip = ip + 1;
            break;
        case COMPARE_OP:
            operand = get_operand(cobj->co_code + ip);
            temp3 = compare_op(operand);
            push(temp3);
            ip = ip + 3;
            break;
        case POP_JUMP_IF_FALSE:
            if (!pop_jump_if_false()) {
                operand = get_operand(cobj->co_code + ip);
                ip = operand;
            } else
                ip = ip + 3;
            break;
        case JUMP_FORWARD:
            operand = get_operand(cobj->co_code + ip);
            ip = ip + operand + 3;
            break;
        case JUMP_ABSOLUTE:
            operand = get_operand(cobj->co_code + ip);
            ip = operand;
            break;
        case CALL_FUNCTION:
            operand = get_operand(cobj->co_code + ip);
            co_temp = (code_object *) malloc(sizeof (code_object));
            fun = getelement(operand);
            memcpy(co_temp, fun->data.codeobj, sizeof (code_object));
            putvar_ref(co_temp);
            temp3 = run(co_temp, glob_name, global_nvalue, n_global);
            pop();
            push(temp3);
            free(co_temp);
            ip = ip + 3;
            break;
        case MAKE_FUNCTION:
            operand = get_operand(cobj->co_code + ip);
            fun = pop();
            push(fun);
            ip = ip + 3;
            break;
        case RETURN_VALUE:
            return pop();
            break;
        case 1:
            pop();
            ip = ip + 1;
            break;
        case LOAD_FAST:
            operand = get_operand(cobj->co_code + ip);
            push(cobj->varref[operand]);
            ip = ip + 3;
            break;
        case LOAD_GLOBAL:
            operand = get_operand(cobj->co_code + ip);
            temp3 = load_global((cobj->names+operand),glob_name, global_nvalue, n_global);
            push(temp3);
            ip = ip + 3;
            break;
        case STORE_FAST:
            operand = get_operand(cobj->co_code + ip);
            cobj->varref[operand] = pop();
            ip = ip + 3;
            break;
        default:
            if (c < 90)
                ip = ip + 1;
            else
                ip = ip + 3;
            break;
        }
    }
    return 0;
}

int get_operand(unsigned char *s)
{
    int x;
    x = *(s + 1);
    x |= *(s + 2) << 8;
    return x;
}

void putvar_ref(code_object *cob)
{
    int argcount, i;
    argcount = cob->argcount;
    PyObject *ob;
    for (i = argcount - 1; i >= 0; i--) {
        ob = pop();
        cob->varref[i] = ob;
    }
}

PyObject *load_global(PyObject *name, PyObject *global_names, PyObject *global_nvalue[], int n_name)
{
    int i=0,t;
    for(i = 0; i < n_name; i++)
    {
        t = strcmp(name->data.s, global_names[i].data.s);
        if(t == 0)
            return global_nvalue[i];

    }
    return global_nvalue[0];
}
