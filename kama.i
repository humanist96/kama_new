%module kama
%include exception.i
%{
#include "kama.h"
#include <Python.h>
%}


%exception {
  try { $action }
  catch (char *e) { SWIG_exception (SWIG_RuntimeError, e); }
  catch (const char *e) { SWIG_exception (SWIG_RuntimeError, (char*)e); }
}

%include <std_vector.i>
%include <std_string.i>
%include <std_list.i>
%include <std_shared_ptr.i>

%shared_ptr(std::vector<Kama::Node>)
%shared_ptr(std::list<const char*>)


%typemap(out) std::list<std::string> %{
    $result = PyList_New(0);
    for(std::list<std::string>::const_iterator i = $1.begin(); i != $1.end(); i++){
        PyObject *token = PyString_FromString(i->c_str());
        PyList_Append($result, token);
        Py_DECREF(token);
    }
%}


%typemap(out) std::vector<Kama::Node> %{
    //$result = PyList_New($1.size()); // Create outer Python list of correct size
    $result = PyList_New(0);
    for(std::vector<Kama::Node>::const_iterator i = $1.begin(); i != $1.end(); i++){
        PyObject* node = PyDict_New();
        PyObject* nbest = PyInt_FromLong(i->nbest);
        PyObject* id = PyInt_FromLong(i->id);
        PyObject* surface = PyString_FromString(i->surface);
        PyObject* feature = PyString_FromString(i->feature);
        PyDict_SetItemString(node, "nbest", nbest);
        PyDict_SetItemString(node, "id",  id); 
        PyDict_SetItemString(node, "surface", surface); 
        PyDict_SetItemString(node, "feature", feature); 
        PyList_Append($result, node);
        Py_DECREF(nbest);
        Py_DECREF(id);
        Py_DECREF(surface);
        Py_DECREF(feature);
        Py_DECREF(node);
    }
%}

%typemap(in) std::list<const char*> %{
    if(PyList_Check($input)){
        std::list<const char*> strList;
        for(int i = 0; i < PyList_Size($input); i++){
           PyObject* pyItem = PyList_GetItem($input, i);
           PyObject* str = PyUnicode_AsEncodedString(pyItem, "utf-8", "strict");
           const char* tmpStr = PyBytes_AsString(str);
           strList.push_back(tmpStr);
           Py_DECREF(pyItem);
           Py_DECREF(str);
           Py_DECREF(tmpStr);
        }
        $1 = strList;
    } else {
        PyErr_SetString(PyExc_TypeError,"Wrong argument type, list expected");
        return NULL;
    }
%}

%include version.h
%include kama.h


