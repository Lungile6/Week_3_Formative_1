#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h> // For rand()
#include <time.h>   // For time()

// This is the C function that will be called from Python
static PyObject* get_sensor_readings(PyObject* self, PyObject* args) {
    // Simulate sensor readings
    double temp = 20.0 + (rand() % 100) / 10.0 - 5.0;     // 15.0 to 25.0
    double humidity = 60.0 + (rand() % 200) / 10.0 - 10.0; // 50.0 to 70.0
    double battery = 90.0 - (rand() % 200) / 10.0;        // 70.0 to 90.0

    // Check for battery depletion simulation
    if (battery < 70.0) battery = 70.0; 

    // Build the return value: a Python tuple (temp, humidity, battery)
    // "ddd" format code means: (double, double, double)
    return Py_BuildValue("(ddd)", temp, humidity, battery);
}

// Method definition table: maps Python function names to C functions
static PyMethodDef SensorMethods[] = {
    {
        "get_readings",        // Python function name
        get_sensor_readings,   // C function to call
        METH_NOARGS,           // Takes no arguments
        "Get simulated sensor readings (temp, humidity, battery)" // Docstring
    },
    {NULL, NULL, 0, NULL} // Sentinel to mark the end of the table
};

// Module definition structure
static struct PyModuleDef sensormodule = {
    PyModuleDef_HEAD_INIT,
    "sensormodule", // Module name
    "A module to simulate IoT sensor data", // Module docstring
    -1,
    SensorMethods
};

// Module initialization function (called when Python imports the module)
PyMODINIT_FUNC PyInit_sensormodule(void) {
    // Initialize random seed
    srand((unsigned int)time(NULL)); 
    
    return PyModule_Create(&sensormodule);
}