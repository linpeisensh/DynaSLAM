#include<iostream>
#include<python2.7/Python.h>
using namespace std;
int main()
{
	cout << "hello world!" << endl;
	Py_Initialize();

	PyRun_SimpleString("print 'Python Start'");

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");

	PyObject *pModule = PyImport_ImportModule("MaskRCNN");
	PyObject *pDict = PyModule_GetDict(pModule);

	PyObject *pClass = PyDict_GetItemString(pDict, "Mask");
	cout << "hello " << endl;
	PyObject *pInstance = PyInstance_New(pClass, NULL, NULL);
//	result = PyObject_CallMethod(pInstance, "GetDynSeg", "(s)", "Charity");
	cout << "hello world!" << endl;
//	char* name=NULL;
//	PyArg_Parse(result, "s", &name);
//	printf("%s\n", name);

	PyRun_SimpleString("print 'Python End'");

	Py_Finalize();
	getchar();
	return 0;
}