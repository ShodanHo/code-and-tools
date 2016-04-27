#include "Python.h"

static PyObject* py_fizzbuzz(PyObject* self, PyObject* args)
{
    int value;
    if (!PyArg_ParseTuple(args, "i", &value))
        return NULL;
    printf("value=%d\n", value);
    for (int i=1; i <= value; i++){
        if (i % 3 == 0 && i % 5 ==0){
            printf("fizzbuzz %d \n", i);
            }
        else if (i % 3 == 0){
            printf("fizz %d \n", i);
            }
        else if(i % 5 == 0){
            printf("buzz %d \n", i);
            }
        }

    // Return value.
    return Py_BuildValue("i", 0);

}

// Mapping between python and c function names. 
static PyMethodDef fizzbuzzModule_methods[] = {
    {"hello", py_fizzbuzz, METH_VARARGS},
    {NULL, NULL}
    };

//extern "C" {
//  void initlibhello(void);
//};

// Module initialisation routine.
void initlibhello(void)
{
    // Init module.
    (void) Py_InitModule("hello", fizzbuzzModule_methods);

}

