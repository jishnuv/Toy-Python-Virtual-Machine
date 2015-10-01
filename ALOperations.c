#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "PyVm.h"

PyObject *ALOperation(PyObject *op1, PyObject *op2, int operation)
{
    int type1, type2, i;
    char *op;
    type1 = op1->type;
    type2 = op2->type;
    PyObject *result = (PyObject *) malloc(sizeof (PyObject));

    if (type1 == TYPE_INT && type2 == TYPE_INT) {
        result->type = TYPE_INT;
        switch (operation) {
        case BINARY_ADD:
            result->data.i = op1->data.i + op2->data.i;
            break;
        case BINARY_SUBTRACT:
            result->data.i = op1->data.i - op2->data.i;
            break;
        case BINARY_MULTIPLY:
            result->data.i = op1->data.i * op2->data.i;
            break;
        case BINARY_DIVIDE:
            if (op2->data.i == 0) {
                result->type = TYPE_NONE;
                fprintf(stderr,"Divide by zero !\n");
                break;
            }
            result->data.i = op1->data.i / op2->data.i;
            break;
        case BINARY_MODULO:
            if (op2->data.i == 0) {
                result->type = TYPE_NONE;
                fprintf(stderr,"Divide by zero !\n");
                break;
            }
            result->data.i = (op1->data.i) % (op2->data.i);
            break;
        default:
            fprintf(stderr,"Unknown operation\n");
            break;
        }
    } else if (type1 == TYPE_INT && type2 == TYPE_BINARY_FLOAT) {
        result->type = TYPE_BINARY_FLOAT;
        switch (operation) {
        case BINARY_ADD:
            result->data.f = (float) op1->data.i + op2->data.f;
            break;
        case BINARY_SUBTRACT:
            result->data.f = (float) op1->data.i - op2->data.f;
            break;
        case BINARY_MULTIPLY:
            result->data.f = (float) op1->data.i * op2->data.f;
            break;
        case BINARY_DIVIDE:
            if (op2->data.f == 0.0) {
                result->type = TYPE_NONE;
                fprintf(stderr,"Divide by zero !\n");
                break;
            }
            result->data.f = (float) op1->data.i / op2->data.f;
            break;
        default:
            fprintf(stderr,"Unknown operation\n");
            break;
        }
    } else if (type1 == TYPE_BINARY_FLOAT && type2 == TYPE_INT) {
        result->type = TYPE_BINARY_FLOAT;
        switch (operation) {
        case BINARY_ADD:
            result->data.f = (float) op1->data.f + op2->data.i;
            break;
        case BINARY_SUBTRACT:
            result->data.f = (float) op1->data.f - op2->data.i;
            break;
        case BINARY_MULTIPLY:
            result->data.f = (float) op1->data.f * op2->data.i;
            break;
        case BINARY_DIVIDE:
            if (op2->data.i == 0) {
                result->type = TYPE_NONE;
                fprintf(stderr,"Divide by zero !\n");
                break;
            }
            result->data.f = (float) op1->data.f / op2->data.i;
            break;
        default:
            fprintf(stderr,"Unknown operation\n");
            break;
        }
    } else if (type1 == TYPE_BINARY_FLOAT && type2 == TYPE_BINARY_FLOAT) {
        result->type = TYPE_BINARY_FLOAT;
        switch (operation) {
        case BINARY_ADD:
            result->data.f = (float) op1->data.f + op2->data.f;
            break;
        case BINARY_SUBTRACT:
            result->data.f = (float) op1->data.f - op2->data.f;
            break;
        case BINARY_MULTIPLY:
            result->data.f = (float) op1->data.f * op2->data.f;
            break;
        case BINARY_DIVIDE:
            if (op2->data.f == 0) {
                result->type = TYPE_NONE;
                fprintf(stderr,"Divide by zero !\n");
                break;
            }
            result->data.f = (float) op1->data.f / op2->data.f;
            break;
        default:
            fprintf(stderr,"Unknown operation\n");
            break;
        }
    } else if ((type1 == TYPE_STRING || type1 == TYPE_INTERNED || type1 == TYPE_STRINGREF) && (type2 == TYPE_STRING || type2 == TYPE_INTERNED || type2 == TYPE_STRINGREF)) {
        result->type = TYPE_STRING;
        switch (operation) {
        case BINARY_ADD:
            op = (char *)malloc((op1->size + op2->size)*sizeof(char)+1);
            strcpy(op, op1->data.s);
            strcat(op, op2->data.s);
            result->data.s = op;
            break;
        default:
            fprintf(stderr,"Unknown operation\n");
            break;
        }
    } else if (type1 == TYPE_INT && (type2 == TYPE_STRING || type2 == TYPE_INTERNED || type2 == TYPE_STRINGREF)) {
        result->type = TYPE_STRING;
        switch (operation) {
        case BINARY_MULTIPLY:
            op = (char *)malloc((op1->data.i)*(op2->size)*sizeof(char)+1);
            strcpy(op, op2->data.s);
            for(i = 1;i<op1->data.i;i++)
            	strcat(op, op2->data.s);
            result->data.s = op;
            break;
        default:
            fprintf(stderr,"Unknown operation\n");
            break;
        }
    } else if ((type1 == TYPE_STRING || type1 == TYPE_INTERNED || type1 == TYPE_STRINGREF) && type2 == TYPE_INT) {
        result->type = TYPE_STRING;
        switch (operation) {
        case BINARY_MULTIPLY:
            op = (char *)malloc((op2->data.i)*(op1->size)*sizeof(char)+1);
            strcpy(op, op1->data.s);
            for(i = 1;i<op2->data.i;i++)
            	strcat(op, op1->data.s);
            result->data.s = op;
            break;
         case BINARY_SUBSCR:
            op = (char *)malloc(sizeof(char)+1);
            op = (op1->data.s) + op2->data.i;
            op[1] = '\0';
            result->data.s = op;
            break;
        default:
            fprintf(stderr,"Unknown operation\n");
            break;
        }
    } else if (type1 == TYPE_TUPLE && type2 == TYPE_INT) {
        switch (operation) {
         case BINARY_SUBSCR:
            result = op1->data.tple+op2->data.i;
            break;
        default:
            fprintf(stderr,"Unknown operation\n");
            break;
        }
    } else
        fprintf(stderr,"Unknown operation\n");

    return result;
}

PyObject *compare_op(int operation)
{
    int a, b;
    PyObject *temp1, *temp2, *temp3;
    temp2 = pop();
    temp1 = pop();
    temp3 = ALOperation(temp2, temp1, BINARY_SUBTRACT);
    a = temp1->data.i;
    b = temp2->data.i;
    switch (operation) {
    case 0:
        if (a < b)
            temp3->type = TYPE_TRUE;
        else
            temp3->type = TYPE_FALSE;
        break;
    case 1:
        if (a <= b)
            temp3->type = TYPE_TRUE;
        else
            temp3->type = TYPE_FALSE;
        break;
    case 2:
        if (a == b)
            temp3->type = TYPE_TRUE;
        else
            temp3->type = TYPE_FALSE;
        break;
    case 3:
        if (a != b)
            temp3->type = TYPE_TRUE;
        else
            temp3->type = TYPE_FALSE;
        break;
    case 4:
        if (a > b)
            temp3->type = TYPE_TRUE;
        else
            temp3->type = TYPE_FALSE;
        break;
    case 5:
        if (a >= b)
            temp3->type = TYPE_TRUE;
        else
            temp3->type = TYPE_FALSE;
        break;
    default:
        temp3->type = TYPE_FALSE;
        break;
    }
    return temp3;
}

void print(PyObject *obj)
{
    int i;
    switch (obj->type) {
    case TYPE_INT:
        printf("%d", obj->data.i);
        break;
    case TYPE_BINARY_FLOAT:
        printf("%f", obj->data.f);
        break;
    case TYPE_INTERNED:
    case TYPE_STRINGREF:
    case TYPE_STRING:
        printf("%s", obj->data.s);
        break;
    case TYPE_TRUE:
        printf("True");
        break;
    case TYPE_FALSE:
        printf("False");
        break;
    case TYPE_TUPLE:
    	printf("(");
    	for(i = 0;i<(obj->size)-1;i++) {
    		print(obj->data.tple+i);
    		printf(", ");
    	}
    	print(obj->data.tple+i);
    	printf(")");
    	break;
    default:
        printf(" cannot print \n");
        break;
    }
}

bool pop_jump_if_false()
{
    if ((pop()->type) == TYPE_TRUE)
        return true;
    else
        return false;
}
