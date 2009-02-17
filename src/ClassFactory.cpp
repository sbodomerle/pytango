static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/ClassFactory.cpp,v 1.4 2006/05/17 15:56:36 taurel Exp $";
//+=============================================================================
//
// file :        ClassFactory.cpp
//
// description : C++ source for the class_factory method of the DServer
//               device class. This method is responsible to create
//               all class singletin for a device server. It is called
//               at device server startup
//
// project :     Python TANGO Device Server
//
// $Author: taurel $
//
// $Revision: 1.4 $
//
// $Log: ClassFactory.cpp,v $
// Revision 1.4  2006/05/17 15:56:36  taurel
// - Changes due to the Windows port using XP and VC8 (Visual Studio 2005)
//
// Revision 1.3  2006/03/03 10:56:19  taurel
// - Add code to be able to run a Tango device server under Python debugger
//
// Revision 1.2  2006/02/06 14:14:12  taurel
// - Use a Python package in order to hide the TgFactory package from the user point of view
//
// Revision 1.1  2006/01/18 14:47:33  taurel
// - Added files when adding device server features in the Python binding
//
// Revision 1.5  2006/01/10 11:35:28  etaurel
// - Not using "using namespace Tango" anymore to be coherent with PyTango
//
// Revision 1.4  2006/01/10 09:38:54  etaurel
// - Change the way python lock is managed
//
// Revision 1.3  2005/12/20 16:40:49  etaurel
// - Many changes in locking mechanism
// - Fix bug in PyAttr ctor
// - Make some new methods known to python (via Boost)
//
// Revision 1.2  2005/12/12 10:44:10  etaurel
// - First release with Restart and CTRL-C running
//
// Revision 1.1.1.1  2005/12/02 13:44:34  etaurel
// Initial import
//
//
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//-=============================================================================

#include <boost/python.hpp>
#include <tango.h>

#include <py_ds.h>


using namespace boost::python;

//
// The per thread data storage key
//

#ifdef WIN32
__declspec(dllimport) omni_thread::key_t key_py_data;
#else
extern omni_thread::key_t key_py_data;
#endif


void Tango::DServer::class_factory()
{

//
// Get the Tango Util object
//

	Tango::Util *tg = Tango::Util::instance();
			
//
// Get the Python TgFactory module
//

	handle<> mod(borrowed(PyImport_AddModule("PyTango")));
	object mod_obj(mod);

//
// First, create CPP class if any. Their name is defined in a Python list
//

	PyObject *cpp_list = PyObject_GetAttrString(mod_obj.ptr(),"cpp_class_list");
	int cpp_class_num = PyList_Size(cpp_list);
	for (int loop = 0;loop < cpp_class_num;loop++)
	{
		PyObject *one_cpp_class = PyList_GetItem(cpp_list,loop);
		char *cl_name = PyString_AsString(PyTuple_GetItem(one_cpp_class,0));
		char *par_name = PyString_AsString(PyTuple_GetItem(one_cpp_class,1));
		create_cpp_class(cl_name,par_name);
	}
	Py_DECREF(cpp_list);

//
// Call the delete_class_list function in order to clear the global
// constructed class Python list. This is necessary only in case of
// ServerRestart command
//

	PyObject *del_cl_list = PyObject_GetAttrString(mod_obj.ptr(),"delete_class_list");
	call<void>(del_cl_list);
	Py_DECREF(del_cl_list);

//
// Create Python classes with a call to the class_factory Python function
//

	PyObject *cl_fact = PyObject_GetAttrString(mod_obj.ptr(),"class_factory");
	call<void>(cl_fact);
	Py_DECREF(cl_fact);

//
// Make all Python tango class(es) known to C++
//

	handle<> jj(PyObject_GetAttrString(mod_obj.ptr(),"constructed_class"));
	object jj_obj(jj);
	boost::python::list construc_list = extract<boost::python::list>(jj_obj.ptr());
			
	short construc_list_len = extract<short>(construc_list.attr("__len__")());

	for (short i = 0;i < construc_list_len;i++)
	{
		Tango::DeviceClass *dev_class = static_cast<Tango::DeviceClass *> (extract<CppDeviceClass *> (construc_list[i])());
		add_class(dev_class);
	}
	
//
// Init all DS classes with the Py interpreter object
//
	
	PyInterpreterState *inte = (PyInterpreterState *)tg->get_py_interp();

	for (short i = 0;i < construc_list_len;i++)
	{
		CppDeviceClass *cpp_dev_class = extract<CppDeviceClass *> (construc_list[i])();
		cpp_dev_class->set_py_interp(inte);
	}

}

