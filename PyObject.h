#define MAXSIZE 1000

struct PyObject {
    int type;
    int size;

    union {
        int i;
        float f;
        char *s;
        struct PyObject *tple;
        struct code_object *codeobj;
    } data;
};
typedef struct PyObject PyObject;

struct code_object {
    int argcount;
    int nlocals;
    int stacksize;
    int flags;

    int code_size;
    unsigned char *co_code;

    int n_const;
    PyObject *constant;

    int n_name;
    PyObject *names;
    PyObject *name_value[MAXSIZE];

    int n_varname;
    PyObject *varnames;
    PyObject *varref[MAXSIZE];

    int n_freevar;
    PyObject *freevars;

    int n_cellvar;
    PyObject *cellvars;

    char *file_name;
    char *fun_name;
    int firstlno;
    char *lnotab;
};
typedef struct code_object code_object;
