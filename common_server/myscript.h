#ifndef myscript_H_H
#define myscript_H_H

#include "common/common.h"
#include "pyscript/scriptobject.h"
#include "pyscript/script.h"
#include "common/smartpointer.h"
#include "pyscript/pyobject_pointer.h"

using namespace KBEngine;
using namespace KBEngine::script;

namespace KBEngine{ namespace script{

class MyScript : public ScriptObject
{
	INSTANCE_SCRIPT_HREADER /*BASE_SCRIPT_HREADER*/(MyScript, ScriptObject)
public:
	MyScript();
	virtual ~MyScript();

	static PyObject* __py_test(PyObject* self, PyObject *args);

	static PyObject* tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

private:
	int m_iType;
};

}}

#endif