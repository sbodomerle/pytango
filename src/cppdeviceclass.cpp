static const char *RcsId = "$Id: cppdeviceclass.cpp,v 1.10 2007/03/09 14:54:16 tiagocoutinho Exp $\n$Name:  $";

//+============================================================================
//
// file :               cppdeviceclass.cpp
//
// description :        
//
// project :            TANGO
//
// author(s) :          E.Taurel
//
// $Revision: 1.10 $
//
// $Log: cppdeviceclass.cpp,v $
// Revision 1.10  2007/03/09 14:54:16  tiagocoutinho
// - Added 64 bits data types for 64 bits computer...
//
// Revision 1.9  2006/12/05 08:24:47  taurel
// - Automatic management of reference count increase for BOOST
// - min_warning and max-waarning now supported
//
// Revision 1.8  2006/11/21 10:36:04  taurel
// Several bug fixes:
//   - Added INIT state
//   - Some memory leak
//   - Bug in asynchronous calls
//   - Abort in case of exception thrown by asynchrnous call-back
//   - Bug with Image attribute and is_allowed method
//   - Display level instead of display type
//
// Revision 1.7  2006/02/27 10:07:42  taurel
// - Add a check on the init_device method existence
//
// Revision 1.6  2006/02/06 14:14:12  taurel
// - Use a Python package in order to hide the TgFactory package from the user point of view
//
// Revision 1.5  2006/01/27 12:52:51  taurel
// - Remove C++ vector used on client side between CORBA sequences and Python list
// - Replace list by tuple on server side for DevVarLongStringArray and DevVarDoubleStringArray
// - Add defualt command case in Py DS
//
// Revision 1.4  2006/01/20 16:38:19  taurel
// - Add the new stuff for memorized attribute but without being hardware
//   applied introduced in Tango 5.4
//
// Revision 1.3  2006/01/19 17:12:11  taurel
// - Remove some printings
//
// Revision 1.2  2006/01/19 11:54:04  taurel
// - Small changes to keep compiler quiet (some warning)
//
// Revision 1.1  2006/01/18 14:47:33  taurel
// - Added files when adding device server features in the Python binding
//
// Revision 1.7  2006/01/10 11:35:28  etaurel
// - Not using "using namespace Tango" anymore to be coherent with PyTango
//
// Revision 1.6  2006/01/04 12:59:52  etaurel
// - Other changes to add the add_attribute feature
//
// Revision 1.5  2005/12/20 16:40:49  etaurel
// - Many changes in locking mechanism
// - Fix bug in PyAttr ctor
// - Make some new methods known to python (via Boost)
//
// Revision 1.4  2005/12/14 15:44:07  etaurel
// - Now the AttrWrite test sequence is running well
//
// Revision 1.3  2005/12/12 11:11:14  etaurel
// Move out python_init from main.py file
//
// Revision 1.2  2005/12/12 10:44:10  etaurel
// - First release with Restart and CTRL-C running
//
// Revision 1.1.1.1  2005/12/02 13:44:34  etaurel
// Initial import
//
//
//
// copyleft :           CELLS - ALBA
//			Campus Universitari de Bellaterra. Universitat 
//			Aut�noma de Barcelona
//			08193 Bellaterra, Barcelona
//		  	SPAIN			
//
//-============================================================================


#include <cppdeviceclass.h>
#include <sstream>


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClass::command_factory 
// 
// description : 	Create command(s) for Python Tango device server
//			from the content of the "cmd_list" python
//			dictionnary
//
//--------------------------------------------------------------------------

void CppDeviceClass::command_factory()
{
//
// Find the "constructed_class" Python list and the
// "class_list" list in the Python TgFactory module
// which is made available through PyTango (package stuff)
//
					 
	object mod_obj((handle<>(borrowed(PyImport_AddModule("PyTango")))));	
	
	boost::python::list constructed((handle<>(PyObject_GetAttrString(mod_obj.ptr(),"constructed_class"))));
	short constructed_len = extract<short>(constructed.attr("__len__")());

	boost::python::list cl_list((handle<>(PyObject_GetAttrString(mod_obj.ptr(),"class_list"))));

//
// Search our class in the constructed_class list
//
			
	for (short i = 0;i < constructed_len;i++)
	{
		
		Tango::DeviceClass *dev_class = static_cast<Tango::DeviceClass *> (extract<CppDeviceClass *> (constructed[i])());
		if (dev_class->get_name() == name)
		{
			object cl(constructed[i]);
			

//
// Search for the cmd_list python dictionnary in out class object
//

			boost::python::dict cmd_dict((handle<>(PyObject_GetAttrString(cl.ptr(),"cmd_list"))));
			short cmd_num = extract<short>(cmd_dict.attr("__len__")());

			boost::python::list keys_list(cmd_dict.keys());
			
			tuple tup(cl_list[i]);
			object obj1(tup[1]);

//
// First check if a init_device method is defined
//

			bool met_defined = false;
			PyObject *meth = PyObject_GetAttrString(obj1.ptr(),"init_device");
			if (meth != NULL)
			{
				int mod = PyObject_HasAttrString(meth,"__module__");
				if (mod == 1)
				{
					if (PyMethod_Check(meth) == false)
						met_defined = false;
					else
						met_defined = true;
				}
				else
					met_defined = false;				
				Py_DECREF(meth);
			}
			else
				met_defined = false;
			
			if (met_defined == false)
			{
				PyErr_Clear();
				TangoSys_OMemStream o;
				o << "Wrong definition of class " << name;
				o << "\nThe init_device() method does not exist!" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       					o.str(),
			       					(const char *)"CppDeviceClass::CppDeviceClass()");
			}						
//
// then, create each command
//

			for (short cmd_loop = 0;cmd_loop < cmd_num;cmd_loop++)
			{
				create_cmd(cmd_dict,keys_list,cmd_loop,obj1.ptr());
			}
			break;							
		}
	}
}


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClass::create_cmd
// 
// description : 	Create one instance of the PyCmd class used for
//			Python Tango device server
//
// argin : - cmd_dict : The python cmd_list dictionnary
//	   - keys_list : The command python dictionnary keys list
//	   - cmd_loop : The index within the command python dictionnary
//	   - tg_ptr : Ptr to the PyObject for the Tango object class
//
//--------------------------------------------------------------------------


void CppDeviceClass::create_cmd(boost::python::dict &cmd_dict,
				boost::python::list &keys_list,
				short cmd_loop,
				PyObject *tg_ptr)
{

//
// Get command name
//

	string cmd_name = extract<std::string>(keys_list[cmd_loop]);

//
// Get command parameters and check their numbers
//

	boost::python::list cmd_param(cmd_dict[keys_list[cmd_loop]]);
	short cmd_param_num = extract<short>(cmd_param.attr("__len__")());

	if ((cmd_param_num < 2) || (cmd_param_num > 3))
	{
		TangoSys_OMemStream o;
		o << "Wrong number of argument for describing command " << cmd_name;
		o << " in class " << name;
		o << "\nMust be a list with 2 or 3 elements" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_cmd()");
	}

	Tango::CmdArgType cmd_in_type = Tango::DEV_VOID;
	Tango::CmdArgType cmd_out_type = Tango::DEV_VOID;
	std::string cmd_in_desc,cmd_out_desc;
	long period = 0;
	bool period_defined = false;
	Tango::DispLevel cmd_disp = Tango::OPERATOR;
	bool disp_defined = false;
	bool default_cmd = false;

//
// Get the command input parameters and check their number
//

	extract<boost::python::list>  ext_input(cmd_param[0]);
	if (ext_input.check() == false)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in command argument for command " << cmd_name;
		o << " in class " << name;
		o << "\nCmd input type (first args) must be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_cmd()");
	}
	boost::python::list input = ext_input();

	short input_param_num = extract<short>(input.attr("__len__")());

	if (input_param_num > 2)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in command argument for command " << cmd_name;
		o << " in class " << name;
		o << "\nList describing cmd input type has too many elements (2 max)" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_cmd()");
	}	
	
//
// Get command input data type
//
	
	extract<Tango::CmdArgType> py_cmd_in_type(input[0]);
	if (py_cmd_in_type.check() == false)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in command argument for command " << cmd_name;
		o << " in class " << name;
		o << "\nCmd input type (first arg in first list) must be a Tango type definition" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_cmd()");
	}
	else
		cmd_in_type = py_cmd_in_type();
	
//
// Get command input data description if defined
//

	if (input_param_num == 2)
	{
		extract<std::string> py_cmd_in_desc(input[1]);
		if (py_cmd_in_desc.check() == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong data type in command argument for command " << cmd_name;
			o << " in class " << name;
			o << "\nCmd input parameter description (second args) must be a string" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
				       o.str(),
				       (const char *)"CppDeviceClass::create_cmd()");
		}
		else
			cmd_in_desc = py_cmd_in_desc();
	}
	else
		cmd_in_desc = "";

//
// Get the command output parameters and check their number
//
		
	extract<boost::python::list>  ext_output(cmd_param[1]);
	if (ext_output.check() == false)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in command argument for command " << cmd_name;
		o << " in class " << name;
		o << "\nCmd output type (second args) must be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_cmd()");
	}	

	boost::python::list output = ext_output();
	short output_param_num = extract<short>(output.attr("__len__")());
	
	if (output_param_num > 2)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in command argument for command " << cmd_name;
		o << " in class " << name;
		o << "\nList describing cmd output type has too many elements (2 max)" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_cmd()");
	}
		
//
// Get command output data type
//

	extract<Tango::CmdArgType> py_cmd_out_type(output[0]);
	if (py_cmd_out_type.check() == false)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in command argument for command " << cmd_name;
		o << " in class " << name;
		o << "\nCmd output type (first arg in second list) must be a command type definition" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_cmd()");
	}
	else
		cmd_out_type = py_cmd_out_type();

//
// Get command output data description if defined
//
								
	if (output_param_num == 2)
	{
		extract<std::string> py_cmd_out_desc(output[1]);
		if (py_cmd_out_desc.check() == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong data type in command argument for command " << cmd_name;
			o << " in class " << name;
			o << "\nCmd output parameter description (forth args) must be a string" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
				       o.str(),
				       (const char *)"CppDeviceClass::create_cmd()");
		}
		else
			cmd_out_desc = py_cmd_out_desc();
	}
	else
		cmd_out_desc = "";

//
// If it is defined, get addictional dictionnary
// used for optional parameters
//

	if (cmd_param_num == 3)
	{
		boost::python::dict cmd_extra_dict(cmd_param[2]);
		short extra_cmd_param = extract<short>(cmd_extra_dict.attr("__len__")());

		if (extra_cmd_param > 3)
		{
			TangoSys_OMemStream o;
			o << "Wrong number of argument for command " << cmd_name;
			o << " in class " << name;
			o << "\nThe optional directtory can not have more than three elements" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       			o.str(),
			       			(const char *)"CppDeviceClass::create_cmd()");
		}

//
// Extract optional parameters from additional
// dictionary
//
			
		boost::python::list keys_list(cmd_extra_dict.keys());
		for (int ctr = 0;ctr < extra_cmd_param;ctr++)
		{
			string key_str;
			extract<std::string> key(keys_list[ctr]);
			if (key.check() == false)
			{
				TangoSys_OMemStream o;
				o << "Wrong data type in command argument for command " << cmd_name;
				o << " in class " << name;
				o << "\nCmd additional parameters dictionnary key is not a string" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
					       o.str(),
					       (const char *)"CppDeviceClass::create_cmd()");
			}
			else
			{
				key_str = key();
				transform(key_str.begin(),key_str.end(),key_str.begin(),::tolower);
				if (key_str == DISPLAY_LEVEL)
				{
				
//
// The display level optional parameter
//

					object disp_type(cmd_extra_dict[keys_list[ctr]]);
					if (PyInt_Check(disp_type.ptr()) == false)
					{
						TangoSys_OMemStream o;
						o << "Wrong data type in command argument for command " << cmd_name;
						o << " in class " << name;
						o << "\nCmd additional parameter for display level is not a number" << ends;

						Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
							       o.str(),
							       (const char *)"CppDeviceClass::create_cmd()");
					}
					
					cmd_disp = extract<Tango::DispLevel>(disp_type);
					disp_defined = true;
				}
				else if (key_str == DEFAULT_CMD)
				{
				
//
// Is it a default command ?
//

					object def_cmd(cmd_extra_dict[keys_list[ctr]]);
					if (PyString_Check(def_cmd.ptr()) == false)
					{
						TangoSys_OMemStream o;
						o << "Wrong data type in command argument for command " << cmd_name;
						o << " in class " << name;
						o << "\nCmd additional parameter for default command is not a string" << ends;

						Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
							       o.str(),
							       (const char *)"CppDeviceClass::analyse_opt()");
					}
					char *tmp_ptr = PyString_AsString(def_cmd.ptr());	
					string memo_str = tmp_ptr;
					transform(memo_str.begin(),memo_str.end(),memo_str.begin(),::tolower);
					if (memo_str == "true")
						default_cmd = true;
					else if (memo_str == "false")
						default_cmd = false;
					else
					{
						TangoSys_OMemStream o;
						o << "Wrong data type in command argument for command " << cmd_name;
						o << " in class " << name;
						o << "\nCmd additional parameter for default command has a wrong syntax" << ends;

						Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
							       o.str(),
							       (const char *)"CppDeviceClass::create_cmd()");
					}
				}
				else if (key_str == POLL_PERIOD)
				{
				
//
// The polling period optional parameter
//

					object poll_per(cmd_extra_dict[keys_list[ctr]]);
					if (PyInt_Check(poll_per.ptr()) == false)
					{
						TangoSys_OMemStream o;
						o << "Wrong data type in command argument for command " << cmd_name;
						o << " in class " << name;
						o << "\nCmd additional parameter for polling period is not a number" << ends;

						Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
							       o.str(),
							       (const char *)"CppDeviceClass::create_cmd()");
					}
					period = extract<long>(poll_per);
					period_defined = true;
				}
				else
				{
				
//
// Throw exception in case of unknown optional
// parameter
//

					TangoSys_OMemStream o;
					o << "Wrong data type in command argument for command " << cmd_name;
					o << " in class " << name;
					o << "\nCmd additional parameters dictionnary has a unknown key";
					o << "\nAllowed keys are : \"Display type\" or \"Polling period\"" << ends;

					Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
						       o.str(),
						       (const char *)"CppDeviceClass::create_cmd()");
				}
			}			
		}

	}

//
// Before creating the command, check that the method executing
// the command exist and that it is callable
//

	PyObject *meth = PyObject_GetAttrString(tg_ptr,const_cast<char *>(cmd_name.c_str()));
	if (meth == NULL)
	{
		PyErr_Clear();
		TangoSys_OMemStream o;
		o << "Wrong definition of command " << cmd_name;
		o << " in class " << name;
		o << "\nThe command method does not exist!" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       			o.str(),
			       			(const char *)"CppDeviceClass::create_cmd()");
	}
	else
	{
		if (PyMethod_Check(meth) == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong definition of command " << cmd_name;
			o << " in class " << name;
			o << "\nThe object " << cmd_name << " exist in class " << name;
			o << " but is not a Python method" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       				o.str(),
			       			       (const char *)"CppDeviceClass::create_cmd()");
		}
		else
			Py_DECREF(meth);
	}
							
//
// Create the command objet
//

	PyCmd *cmd_ptr = new PyCmd(cmd_name,cmd_in_type,cmd_out_type,
			 	   cmd_in_desc,cmd_out_desc,Tango::OPERATOR);
					 
//
// Check if there is a is_allowed method for this command
// 

	string is_allo("is_");
	is_allo += cmd_name;
	is_allo += "_allowed";

	PyObject *allo_meth = PyObject_GetAttrString(tg_ptr,const_cast<char *>(is_allo.c_str()));
	if (allo_meth == NULL)
	{
		PyErr_Clear();
	}
	else
	{
		if (PyMethod_Check(allo_meth) == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong definition of command " << cmd_name;
			o << " in class " << name;
			o << "\nThe object " << is_allo << " exist in class " << name;
			o << " but is not a Python method" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       				o.str(),
			       			       (const char *)"CppDeviceClass::create_cmd()");
		}
		else
		{
			cmd_ptr->set_allowed(is_allo);
			Py_DECREF(allo_meth);
		}
	}
	
//
// Set the comand type 
//

	if (cmd_in_type == Tango::DEV_VOID)
	{
		if (cmd_out_type == Tango::DEV_VOID)
			cmd_ptr->set_cmd_type(VOID_VOID);
		else
			cmd_ptr->set_cmd_type(VOID_OUT);
	}
	else
	{
		if (cmd_out_type == Tango::DEV_VOID)
			cmd_ptr->set_cmd_type(IN_VOID);
		else
			cmd_ptr->set_cmd_type(IN_OUT);
	}
	
//
// Install command in Tango and add set optional stuff
// if defined
//

	if (period_defined)
		cmd_ptr->set_polling_period(period);
	if (disp_defined)
		cmd_ptr->set_disp_level(cmd_disp);
		
	if (default_cmd == true)
		set_default_command(cmd_ptr);
	else
		command_list.push_back(cmd_ptr);
			
}


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClass::attribute_factory 
// 
// description : 	Create attribute(s) for Python Tango device server
//			from the content of the "attr_list" python
//			dictionnary
//
//--------------------------------------------------------------------------


void CppDeviceClass::attribute_factory(vector<Tango::Attr *> &att_list)
{

//
// Find the "constructed_class" Python list and the
// "class_list" list in the Python TgFactory module
// which is made available through PyTango (package stuff)
//
					 
	object mod_obj((handle<>(borrowed(PyImport_AddModule("PyTango")))));	
	
	boost::python::list constructed((handle<>(PyObject_GetAttrString(mod_obj.ptr(),"constructed_class"))));
	short constructed_len = extract<short>(constructed.attr("__len__")());

	boost::python::list cl_list((handle<>(PyObject_GetAttrString(mod_obj.ptr(),"class_list"))));

//
// Search the our class in the constructed_class list
//
			
	for (short i = 0;i < constructed_len;i++)
	{
		
		Tango::DeviceClass *dev_class = static_cast<Tango::DeviceClass *> (extract<CppDeviceClass *> (constructed[i])());
		if (dev_class->get_name() == name)
		{
			object cl(constructed[i]);

//
// Search for the attr_list python dictionnary in out class object
//

			boost::python::dict attr_dict((handle<>(PyObject_GetAttrString(cl.ptr(),"attr_list"))));
			short attr_num = extract<short>(attr_dict.attr("__len__")());

//			cout << "Class " << name << " has " << attr_num << " attribute(s)" << endl;
			boost::python::list keys_list(attr_dict.keys());
			
			tuple tup(cl_list[i]);
			object obj1(tup[1]);
			
//
// Create each attribute
//

			for (short attr_loop = 0;attr_loop < attr_num;attr_loop++)
			{
				create_attr(attr_dict,keys_list,attr_loop,obj1.ptr(),att_list);
			}
			break;							
		}
	}
}


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClass::create_attr
// 
// description : 	Create one instance of the PyAtt class used for
//			Python Tango device server attribute
//
// argin : - attr_dict : The python attr_list dictionnary
//	   - keys_list : The command python dictionnary keys list
//	   - attr_loop : The index within the command python dictionnary
//	   - tg_ptr : Ptr to the PyObject for the Tango object class
//	   - att_list : The class attribute list
//
//--------------------------------------------------------------------------


void CppDeviceClass::create_attr(boost::python::dict &attr_dict,
				 boost::python::list &keys_list,
				 short attr_loop,
				 PyObject *tg_ptr,
				 vector<Tango::Attr *> &att_list)
{
	string attr_name = extract<std::string>(keys_list[attr_loop]);

//
// Get attribute parameters and check their numbers
//

	boost::python::list att_param(attr_dict[keys_list[attr_loop]]);
	short att_param_num = extract<short>(att_param.attr("__len__")());

	if ((att_param_num < 1) || (att_param_num > 2))
	{
		TangoSys_OMemStream o;
		o << "Wrong number of argument for descrbing attribute " << attr_name;
		o << " in class " << name;
		o << "\nMust be a list with 1 or 2 elements" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr)");
	}
	
//
// Get the attribute mandatory parameters and check their number
//

	extract<boost::python::list>  ext_mandat(att_param[0]);
	if (ext_mandat.check() == false)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nAttribute mandatory parameters must be a Python list" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr()");
	}
	boost::python::list mandat = ext_mandat();

	short mandat_param_num = extract<short>(mandat.attr("__len__")());

	if (mandat_param_num < 3)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nList describing mandatory attribute parameters does not have enought elements";
		o << "\n(3 min: Data type,Data format and R/W type)" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr()");
	}	
	if (mandat_param_num > 5)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nList describing mandatory attribute parameters has too many elements (5 max)" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr()");
	}

// Don't worry about the init. here. It's only to keep compiler quiet

	Tango::CmdArgType attr_type = Tango::DEV_VOID;
	Tango::AttrDataFormat attr_format = Tango::SCALAR;
	Tango::AttrWriteType attr_write = Tango::READ;
				
//
// Get attribute data type
//
		
	extract<Tango::CmdArgType> py_attr_type(mandat[0]);
	if (py_attr_type.check() == false)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nAttribute data type (first arg in list) must be a Tango type definition" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr()");
	}
	else
		attr_type = py_attr_type();
			
//
// Get attribute data format
//

	extract<Tango::AttrDataFormat> py_attr_format(mandat[1]);
	if (py_attr_format.check() == false)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nAttribute data format (second arg in list) must be a Tango data format value" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr()");
	}
	else
		attr_format = py_attr_format();

//
// Now that we know data format, check for the exact param number
// SCALAR --> 3, SPECTRUM --> 4, IMAGE --> 5

	if ((attr_format == Tango::SCALAR) && (mandat_param_num != 3))
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nList describing mandatory attribute parameters for scalar attribute";
		o << " must have 3 elements" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr()");
	}
	
	if ((attr_format == Tango::SPECTRUM) && (mandat_param_num != 4))
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nList describing mandatory attribute parameters for spectrum attribute";
		o << " must have 4 elements" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr()");
	}
	
	if ((attr_format == Tango::IMAGE) && (mandat_param_num != 5))
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nList describing mandatory attribute parameters for image attribute";
		o << " must have 5 elements" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr()");
	}
			
//
// Get attribute write type
//

	extract<Tango::AttrWriteType> py_attr_write(mandat[2]);
	if (py_attr_write.check() == false)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nAttribute write type (third arg in list) must be a Tango attribute write type" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::create_attr()");
	}
	else
		attr_write = py_attr_write();
		
//
// Get X dimension for SPECTRUM and IMAGE attribute
//

	long x = 0;
	long y = 0;
	
	if ((attr_format == Tango::SPECTRUM) || (attr_format == Tango::IMAGE))
	{
		extract<long> py_x(mandat[3]);
		if (py_x.check() == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong data type in attribute argument for attribute " << attr_name;
			o << " in class " << name;
			o << "\nAttribute X dimension (forth arg in list) must be an integer" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
				       o.str(),
				       (const char *)"CppDeviceClass::create_attr()");
		}
		else
			x = py_x();
	}
	
	
//
// Get Y dimension for Image attribute
//

	if (attr_format == Tango::IMAGE)
	{
		extract<long> py_y(mandat[4]);
		if (py_y.check() == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong data type in attribute argument for attribute " << attr_name;
			o << " in class " << name;
			o << "\nAttribute Y dimension (fifth arg in list) must be an integer" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
				       o.str(),
				       (const char *)"CppDeviceClass::create_attr()");
		}
		else
			y = py_y();
	}

//
// If a dictionnary for additional parameters is defined
//

	Tango::UserDefaultAttrProp attr_prop;
	disp_poll dp;
	
	if (att_param_num == 2)
	{
		extract<boost::python::dict>  ext_opt(att_param[1]);
		if (ext_opt.check() == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong data type in attribute argument for attribute " << attr_name;
			o << " in class " << name;
			o << "\nAttribute optional parameters must be a Python dictionary" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongAttributeDefinition",
				       o.str(),
				       (const char *)"CppDeviceClass::create_attr()");
		}
		boost::python::dict opt = ext_opt();
		
		analyse_opt(opt,attr_prop,dp,attr_name);
	}
		
//
// Before creating the attribute, check that the method to read and/or to write
// the attributes exist and that they are callable
//

	string read_name_met("read_");
	read_name_met += attr_name;

	string write_name_met("write_");
	write_name_met += attr_name;
		
	switch (attr_write)
	{
	case Tango::READ:
		met_exist_callable(read_name_met,attr_name,tg_ptr);
		break;
		
	case Tango::WRITE:
		met_exist_callable(write_name_met,attr_name,tg_ptr);
		break;
		
	case Tango::READ_WRITE:
		met_exist_callable(read_name_met,attr_name,tg_ptr);
		met_exist_callable(write_name_met,attr_name,tg_ptr);
		break;
		
	default:
	
//
// Do we support READ_WITH_WRITE ?
//

		break;
	}

//
// Create the attribute objet according to attribute format
//

	PyScaAttr *sca_attr_ptr = NULL;
	PySpecAttr *spec_attr_ptr = NULL;
	PyImaAttr *ima_attr_ptr = NULL;

	switch (attr_format)
	{
	case Tango::SCALAR:
		sca_attr_ptr = new PyScaAttr(attr_name.c_str(),attr_type,attr_write);
		sca_attr_ptr->set_read_name(read_name_met);
		sca_attr_ptr->set_write_name(write_name_met);
		break;
		
	case Tango::SPECTRUM:
		spec_attr_ptr = new PySpecAttr(attr_name.c_str(),attr_type,attr_write,x);
		spec_attr_ptr->set_read_name(read_name_met);
		spec_attr_ptr->set_write_name(write_name_met);
		break;
		
	case Tango::IMAGE:
		ima_attr_ptr = new PyImaAttr(attr_name.c_str(),attr_type,attr_write,x,y);
		ima_attr_ptr->set_read_name(read_name_met);
		ima_attr_ptr->set_write_name(write_name_met);
		break;
	}
	
//
// Check if there is a is_allowed method for this attribute
// 

	string is_allo("is_");
	is_allo += attr_name;
	is_allo += "_allowed";

	PyObject *allo_meth = PyObject_GetAttrString(tg_ptr,const_cast<char *>(is_allo.c_str()));
	if (allo_meth == NULL)
	{
		PyErr_Clear();
	}
	else
	{
		if (PyMethod_Check(allo_meth) == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong definition of attribute " << attr_name;
			o << " in class " << name;
			o << "\nThe object " << is_allo << " exist in class " << name;
			o << " but is not a Python method" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       				o.str(),
			       			       (const char *)"CppDeviceClass::create_attr()");
		}
		else
		{
			switch (attr_format)
			{
			case Tango::SCALAR:
				sca_attr_ptr ->set_allowed(is_allo);
				break;

			case Tango::SPECTRUM:
				spec_attr_ptr->set_allowed(is_allo);
				break;

			case Tango::IMAGE:
				ima_attr_ptr->set_allowed(is_allo);
				break;
			}
			Py_DECREF(allo_meth);
		}
	}
					   
//
// Install attribute in Tango. Set its properties value.
// Also set its polling period and display type if defined
//

	switch (attr_format)
	{
	case Tango::SCALAR:
		att_list.push_back(sca_attr_ptr);
		break;
		
	case Tango::SPECTRUM:
		att_list.push_back(spec_attr_ptr);
		break;
		
	case Tango::IMAGE:
		att_list.push_back(ima_attr_ptr);
		break;
	}
	
	att_list.back()->set_default_properties(attr_prop);
	if (dp.period_defined)
		att_list.back()->set_polling_period(dp.period);
	if (dp.disp_defined)
		att_list.back()->set_disp_level(dp.attr_disp);
	if (dp.memorized_defined)
	{
		att_list.back()->set_memorized();
		if (dp.with_hard_applied == false)
			att_list.back()->set_memorized_init(false);
	}
}

//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClass::met_exist_callable
// 
// description : 	Create one instance of the PyAtt class used for
//			Python Tango device server attribute
//
// argin : - met_name : The method name
//	   - attr_name : The attribute name
//	   - tg_ptr : Ptr to the PyObject for the Tango object class
//
//--------------------------------------------------------------------------


void CppDeviceClass::met_exist_callable(string &met_name,string &attr_name,PyObject *tg_ptr)
{
		
	PyObject *meth = PyObject_GetAttrString(tg_ptr,const_cast<char *>(met_name.c_str()));
	if (meth == NULL)
	{
		PyErr_Clear();
		TangoSys_OMemStream o;
		o << "Wrong definition of attribute " << attr_name;
		o << " in class " << name;
		o << "\nThe attribute method " << met_name << " does not exist!" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       			o.str(),
			       			(const char *)"CppDeviceClass::met_exist_callable()");
	}
	else
	{
		if (PyMethod_Check(meth) == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong definition of attribute " << attr_name;
			o << " in class " << name;
			o << "\nThe object " << met_name << " exist in class " << name;
			o << " but is not a Python method" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       				o.str(),
			       			       (const char *)"CppDeviceClass::not_exist_callable()");
		}
		else
			Py_DECREF(meth);
	}
}
	

//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClass::analyse_opt
// 
// description : 	Analyse the dictionary used to set attribute 
//			optional parameters
//
// argin : - opt : The Python dictionary
//	   - att_prop : The attribute property structure
//	   - dp : The structure used to pass polling and display type info
//	   - attr_name : The attribute name
//
//--------------------------------------------------------------------------


void CppDeviceClass::analyse_opt(dict &opt,Tango::UserDefaultAttrProp &att_prop,disp_poll &dp,string &attr_name)
{

//
// First, a brief check on how many parameters are defined in this dictionary
//

	short extra_attr_param = extract<short>(opt.attr("__len__")());

	if (extra_attr_param > MAX_ATTR_OPT)
	{
		TangoSys_OMemStream o;
		o << "Wrong number of argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nThe optional directory can not have more than two elements" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       		o.str(),
			       		(const char *)"CppDeviceClass::analyse_opt()");
	}

//
// A loop for each optional parameters
//
	
	boost::python::list keys_list(opt.keys());
	for (int ctr = 0;ctr < extra_attr_param;ctr++)
	{
		string key_str;
		extract<std::string> key(keys_list[ctr]);
		if (key.check() == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong data type in attribute argument for attribute " << attr_name;
			o << " in class " << name;
			o << "\nAttr additional parameters dictionnary key is not a string" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
				       o.str(),
				       (const char *)"CppDeviceClass::analyse_opt()");
		}
		else
		{
			key_str = key();
			transform(key_str.begin(),key_str.end(),key_str.begin(),::tolower);
			
			string prop_number;
			string prop_string;
			
			if (key_str == DISPLAY_LEVEL)
			{
				
//
// The display type optional parameter
//

				object disp_type(opt[keys_list[ctr]]);
				if (PyInt_Check(disp_type.ptr()) == false)
				{
					TangoSys_OMemStream o;
					o << "Wrong data type in attribute argument for attribute " << attr_name;
					o << " in class " << name;
					o << "\nAttr additional parameter for display level is not a number" << ends;

					Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
						       o.str(),
						       (const char *)"CppDeviceClass::analyse_opt()");
				}

				dp.attr_disp = extract<Tango::DispLevel>(disp_type);
				dp.disp_defined = true;
			}
			else if (key_str == POLL_PERIOD)
			{
				
//
// The polling period optional parameter
//

				object poll_per(opt[keys_list[ctr]]);
				if (PyInt_Check(poll_per.ptr()) == false)
				{
					TangoSys_OMemStream o;
					o << "Wrong data type in attribute argument for attribute " << attr_name;
					o << " in class " << name;
					o << "\nAttr additional parameter for polling period is not a number" << ends;

					Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
						       o.str(),
						       (const char *)"CppDeviceClass::analyse_opt()");
				}
				dp.period = extract<long>(poll_per);
				dp.period_defined = true;
			}
			else if (key_str == MEMORIZED)
			{
				
//
// The memorized optional parameter
//

				object memo(opt[keys_list[ctr]]);
				if (PyString_Check(memo.ptr()) == false)
				{
					TangoSys_OMemStream o;
					o << "Wrong data type in attribute argument for attribute " << attr_name;
					o << " in class " << name;
					o << "\nAttr additional parameter for memorized feature is not a string" << ends;

					Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
						       o.str(),
						       (const char *)"CppDeviceClass::analyse_opt()");
				}
				char *tmp_ptr = PyString_AsString(memo.ptr());	
				string memo_str = tmp_ptr;
				transform(memo_str.begin(),memo_str.end(),memo_str.begin(),::tolower);
				if (memo_str == "true")
					dp.memorized_defined = true;
				else if (memo_str == "true_without_hard_applied")
				{
					dp.memorized_defined = true;
					dp.with_hard_applied = false;
				}
				else
					dp.memorized_defined = false;
			}
			else if (key_str == LABEL_PROP)
			{
				
//
// The label optional parameter
//

				object label(opt[keys_list[ctr]]);

				check_string_type(label.ptr(),attr_name,LABEL_PROP,prop_string);
				att_prop.set_label(prop_string.c_str());
			}
			else if (key_str == DESC_PROP)
			{
				
//
// The description optional parameter
//

				object desc(opt[keys_list[ctr]]);

				check_string_type(desc.ptr(),attr_name,DESC_PROP,prop_string);
				att_prop.set_description(prop_string.c_str());
			}
			else if (key_str == UNIT_PROP)
			{
				
//
// The unit optional parameter
//

				object unit(opt[keys_list[ctr]]);

				check_string_type(unit.ptr(),attr_name,UNIT_PROP,prop_string);
				att_prop.set_unit(prop_string.c_str());
			}
			else if (key_str == STD_UNIT_PROP)
			{
				
//
// The standard unit optional parameter
//

				object std_unit(opt[keys_list[ctr]]);
				
				check_number_type(std_unit.ptr(),attr_name,STD_UNIT_PROP,prop_number);
				att_prop.set_standard_unit(prop_number.c_str());

			}
			else if (key_str == DISPLAY_UNIT_PROP)
			{
				
//
// The display unit optional parameter
//

				object disp_unit(opt[keys_list[ctr]]);
				
				check_string_type(disp_unit.ptr(),attr_name,DISPLAY_UNIT_PROP,prop_string);
				att_prop.set_display_unit(prop_string.c_str());
			}
			else if (key_str == FORMAT_PROP)
			{
				
//
// The format optional parameter
//

				object format(opt[keys_list[ctr]]);

				check_string_type(format.ptr(),attr_name,FORMAT_PROP,prop_string);
				att_prop.set_format(prop_string.c_str());
			}
			else if (key_str == MAX_VALUE_PROP)
			{
				
//
// The max_value optional parameter
//

				object max_value(opt[keys_list[ctr]]);
				
				check_number_type(max_value.ptr(),attr_name,MAX_VALUE_PROP,prop_number);
				att_prop.set_max_value(prop_number.c_str());
			}
			else if (key_str == MIN_VALUE_PROP)
			{
				
//
// The min_value optional parameter
//

				object min_value(opt[keys_list[ctr]]);
				
				check_number_type(min_value.ptr(),attr_name,MIN_VALUE_PROP,prop_number);
				att_prop.set_min_value(prop_number.c_str());
			}
			else if (key_str == MAX_ALARM_PROP)
			{
				
//
// The max_alarm optional parameter
//

				object max_alarm(opt[keys_list[ctr]]);

				check_number_type(max_alarm.ptr(),attr_name,MAX_ALARM_PROP,prop_number);
				att_prop.set_max_alarm(prop_number.c_str());
			}
			else if (key_str == MIN_ALARM_PROP)
			{
				
//
// The min_alarm optional parameter
//

				object min_alarm(opt[keys_list[ctr]]);

				check_number_type(min_alarm.ptr(),attr_name,MIN_ALARM_PROP,prop_number);
				att_prop.set_min_alarm(prop_number.c_str());
			}
			else if (key_str == MAX_WARNING_PROP)
			{
				
//
// The max_warning optional parameter
//

				object max_warning(opt[keys_list[ctr]]);
				
				check_number_type(max_warning.ptr(),attr_name,MAX_WARNING_PROP,prop_number);
				att_prop.set_max_warning(prop_number.c_str());
			}
			else if (key_str == MIN_WARNING_PROP)
			{
				
//
// The min_warning optional parameter
//

				object min_warning(opt[keys_list[ctr]]);
				
				check_number_type(min_warning.ptr(),attr_name,MIN_WARNING_PROP,prop_number);
				att_prop.set_min_warning(prop_number.c_str());
			}
			else if (key_str == DELTA_T_PROP)
			{
				
//
// The delta_t optional parameter
//

				object delta_t(opt[keys_list[ctr]]);

				check_number_type(delta_t.ptr(),attr_name,DELTA_T_PROP,prop_number);
				att_prop.set_delta_t(prop_number.c_str());
			}
			else if (key_str == DELTA_VAL_PROP)
			{
				
//
// The delta_val optional parameter
//

				object delta_val(opt[keys_list[ctr]]);

				check_number_type(delta_val.ptr(),attr_name,STD_UNIT_PROP,prop_number);
				att_prop.set_delta_val(prop_number.c_str());
			}
			else
			{
				
//
// Throw exception in case of unknown optional
// parameter
//

				TangoSys_OMemStream o;
				o << "Wrong data type in attribute argument for attribute " << attr_name;
				o << " in class " << name;
				o << "\nAttr additional parameters dictionnary has a unknown key";
				o << " (" << key_str << ")" << ends;

				Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
					       o.str(),
					       (const char *)"CppDeviceClass::analyse_opt()");
			}
		}			
	}
		
}


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClass::check_number_type
// 
// description : 	Check if a Python object is a number (Python int
//			or float). If it is, convert it to a string
//
// argin : - py_ptr : The Python object
//	   - attr_name : The attribute name
//	   - prop_name : The property name
//	   - num_as_string : The number value converted to a string
//
//--------------------------------------------------------------------------


void CppDeviceClass::check_number_type(PyObject *py_ptr,string &attr_name,const char *prop_name,string &num_as_string)
{
	stringstream st;
	
	if (PyInt_Check(py_ptr) == false)
	{
		if (PyFloat_Check(py_ptr) == false)
		{
			TangoSys_OMemStream o;
			o << "Wrong data type in attribute argument for attribute " << attr_name;
			o << " in class " << name;
			o << "\nAttr additional parameter for " << prop_name << " is not a number" << ends;

			Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
				       o.str(),
				       (const char *)"CppDeviceClass::check_number_type()");
		}
		else
		{
			double the_value = PyFloat_AsDouble(py_ptr);
			st << the_value;
		}
	}
	else
	{
		long the_value = PyInt_AsLong(py_ptr);
		st << the_value;
	}
	
	num_as_string = st.str();
}


//+-------------------------------------------------------------------------
//
// method : 		CppDeviceClass::check_string_type
// 
// description : 	Check if a Python object is a string
//
// argin : - py_ptr : The Python object
//	   - attr_name : The attribute name
//	   - prop_name : The property name
//	   - num_as_string : The number value converted to a string
//
//--------------------------------------------------------------------------


void CppDeviceClass::check_string_type(PyObject *py_ptr,string &attr_name,const char *prop_name,string &prop_as_string)
{	
	if (PyString_Check(py_ptr) == false)
	{
		TangoSys_OMemStream o;
		o << "Wrong data type in attribute argument for attribute " << attr_name;
		o << " in class " << name;
		o << "\nAttr additional parameter for " << prop_name << " is not a string" << ends;

		Tango::Except::throw_exception((const char *)"PyDs_WrongCommandDefinition",
			       o.str(),
			       (const char *)"CppDeviceClass::check_string_type()");
	}

	char *tmp_ptr = PyString_AsString(py_ptr);	
	prop_as_string = tmp_ptr;
}

//+-------------------------------------------------------------------------
//
// method : 		PyScaAttr::PyScaAttr
// 
// description : 	A ctor of the PyScaAttr class
//
// argin : - na : The attribute name
//	   - type : The attribute data type
//	   - w : The attribute writable type
//	   - user_prop : The attribute user default properties
//
//--------------------------------------------------------------------------

PyScaAttr::PyScaAttr(const char *na,long type,Tango::AttrWriteType w,vector<Tango::AttrProperty> &user_prop):Tango::Attr(na,type,w)
{
	if (user_prop.size() != 0)
	{
		Tango::UserDefaultAttrProp def_prop;

		set_user_prop(user_prop,def_prop);
		set_default_properties(def_prop);
	}
}


//+-------------------------------------------------------------------------
//
// method : 		PySpecAttr::PySpecAttr
// 
// description : 	A ctor of the PySpecAttr class
//
// argin : - na : The attribute name
//	   - type : The attribute data type
//	   - w : The attribute writable type
//	   - xx : The attribute max dim x
//	   - user_prop : The attribute user default properties
//
//--------------------------------------------------------------------------

PySpecAttr::PySpecAttr(const char *na,long type,Tango::AttrWriteType w,long xx,vector<Tango::AttrProperty> &user_prop):SpectrumAttr(na,type,w,xx)
{
	if (user_prop.size() != 0)
	{
		Tango::UserDefaultAttrProp def_prop;

		set_user_prop(user_prop,def_prop);
		set_default_properties(def_prop);
	}
}


//+-------------------------------------------------------------------------
//
// method : 		PyImaAttr::PyImaAttr
// 
// description : 	A ctor of the PyImaAttr class
//
// argin : - na : The attribute name
//	   - type : The attribute data type
//	   - w : The attribute writable type
//	   - xx : The attribute max dim x
//	   - yy : The attribute max dim y
//	   - user_prop : The attribute user default properties
//
//--------------------------------------------------------------------------

PyImaAttr::PyImaAttr(const char *na,long type,Tango::AttrWriteType w,long xx,long yy,vector<Tango::AttrProperty> &user_prop):ImageAttr(na,type,w,xx,yy)
{
	if (user_prop.size() != 0)
	{
		Tango::UserDefaultAttrProp def_prop;

		set_user_prop(user_prop,def_prop);
		set_default_properties(def_prop);
	}
}
