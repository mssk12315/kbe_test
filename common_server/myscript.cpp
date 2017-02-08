#include "myscript.h"

namespace KBEngine{ namespace script{

SCRIPT_METHOD_DECLARE_BEGIN(MyScript)
SCRIPT_METHOD_DECLARE("test", test, METH_VARARGS, 0)
SCRIPT_METHOD_DECLARE_END()

SCRIPT_MEMBER_DECLARE_BEGIN(MyScript)
SCRIPT_MEMBER_DECLARE("type", m_iType, T_INT, 0, 0)
SCRIPT_MEMBER_DECLARE_END()

SCRIPT_GETSET_DECLARE_BEGIN(MyScript)
SCRIPT_GETSET_DECLARE_END()
/*BASE_SCRIPT_INIT*/ SCRIPT_INIT(MyScript, 0, 0, 0, 0, 0)

MyScript::MyScript()
:ScriptObject(getScriptType(), false)
{
	m_iType = 0;
}

MyScript::~MyScript()
{

}

PyObject* MyScript::__py_test(PyObject* self, PyObject *args)
{
	PyObject* obj = NULL;

	if (!PyArg_ParseTuple(args, "O", &obj))
	{
		ERROR_MSG("ScriptStdErr::write: Bad args\n");
		return NULL;
	}

	Py_ssize_t size = 0;
	wchar_t* PyUnicode_AsWideCharStringRet0 = PyUnicode_AsWideCharString(obj, &size);
	// static_cast<ScriptStdErr*>(self)->pScriptStdOutErr()->error_msg(PyUnicode_AsWideCharStringRet0, size);
	char *pArgs = strutil::wchar2char(PyUnicode_AsWideCharStringRet0);
	ERROR_MSG(fmt::format("test arg is {} \n", pArgs));
	free(pArgs);
	PyMem_Free(PyUnicode_AsWideCharStringRet0);
	S_Return;
}

PyObject* MyScript::tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
	MyScript* v = new MyScript();

	if (PyTuple_Size(args) > 0)
	{
// 		PyObject* pyResult = v->__py_pySet((PyObject*)v, args);
// 
// 		if (pyResult)
// 			Py_DECREF(pyResult);
// 		else
// 			return NULL;
	}

	return v;
}

}}