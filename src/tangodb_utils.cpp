static const char *RcsId = "$Header: /cvsroot/tango-cs/tango/bindings/python/src/tangodb_utils.cpp,v 1.5 2007/03/06 09:55:14 tiagocoutinho Exp $";
//+=============================================================================
//
// file :         tangodb_utils.cpp
//
// description :  C++ source for the Database related utility methods used in the 
//                other source files.
//
// project :      Python Binding
//
// $Author: tiagocoutinho $
//
// $Revision: 1.5 $
//
// $Log: tangodb_utils.cpp,v $
// Revision 1.5  2007/03/06 09:55:14  tiagocoutinho
// - fixed problem preventing methods 'delete_device_attribute_property' and 'delete_class_attribute_property' from working correctly
//
// Revision 1.4  2006/01/18 14:26:57  taurel
// - Add a "typename" definition in order to remove compilation warning
// - Change include file order to also remove compiler warnings
// - Now call device server module wrapped stuff
//
// Revision 1.3  2005/03/24 13:20:20  taurel
// - Added Vincenzo changes for the Tango V5 port
// - Fix bug on Linux in the client_request function (default  argument)
//
// Revision 1.2  2004/01/27 09:59:56  ounsy
// Added asynchronous related methods
//
// Revision 1.1  2003/10/23 13:50:55  ounsy
//
//
// Added Python binding for TANGO
//
//
// copyleft :     Synchrotron SOLEIL
//			L'Orme des Merisiers, Saint-Aubin BP 48
//			91192 GIF SUR YVETTE Cedex
//
//-=============================================================================

#include <tangodb.h>
#include <templates.cpp>


void
Database::translate_list_to_nonvalued_dbdata(boost::python::list list_prop,Tango::DbData &db_data)
{
    std::string prop_name;
    int p_len = boost::python::extract<int>(list_prop.attr("__len__")()) ;
    for (int i = 0 ; i < p_len; i++ )
    {
        get_scalar_value<std::string>(prop_name,"String",list_prop[i]);
        Tango::DbDatum db_datum(prop_name);
        db_data.push_back(db_datum);    
    }
}

void
Database::translate_dict_to_valued_dbdata(boost::python::dict py_list_prop,Tango::DbData &db_data)
{
    std::string prop_name;
    boost::python::list list_prop = py_list_prop.keys();
    int p_len = boost::python::extract<int>(list_prop.attr("__len__")()) ;
    for (int i = 0 ; i < p_len; i++ )
    {
        get_scalar_value<std::string>(prop_name,"String",list_prop[i]);
        Tango::DbDatum db_datum(prop_name);
        get_array_value<std::string>(db_datum.value_string,"String list",py_list_prop[prop_name]);
        db_data.push_back(db_datum);    
    }
}

void
Database::translate_attr_dict_to_valued_dbdata(boost::python::dict py_list_attr_prop,Tango::DbData &db_data)
{
    std::string attr_name;
    std::string prop_name;
    std::string prop_val;
    boost::python::list list_attr = py_list_attr_prop.keys();
    int i_len = boost::python::extract<int>(list_attr.attr("__len__")()) ;
    for (int i = 0 ; i < i_len; i++ )
    {
        get_scalar_value<std::string>(attr_name,"String",list_attr[i]);
		boost::python::dict py_list_prop = (boost::python::dict)(py_list_attr_prop[ attr_name ]);
		boost::python::list list_prop = py_list_prop.keys();
		short j_len = boost::python::extract<short>(list_prop.attr("__len__")()) ;
        Tango::DbDatum db_datum(attr_name);
		db_datum << j_len;
        db_data.push_back(db_datum);    
        for (int j=0 ; j < j_len ; j++ )
		{
			get_scalar_value<std::string>(prop_name,"String",list_prop[j]);
			Tango::DbDatum db_datum(prop_name);
			get_array_value<std::string>(db_datum.value_string,"String list",py_list_prop[prop_name]);
//			get_scalar_value<std::string>(prop_val,"String",py_list_prop[prop_name]);
//			db_datum.value_string.push_back(prop_val);
			db_data.push_back(db_datum);
		}
    }
//     Tango::DbDatum db_datum;
//     db_data.push_back(db_datum);    
}

void
Database::translate_attr_dict_to_str_valued_dbdata(boost::python::dict py_list_attr_prop,Tango::DbData &db_data)
{
    std::string attr_name;
    std::string prop_name;
    std::string prop_val;
    boost::python::list list_attr = py_list_attr_prop.keys();
    int i_len = boost::python::extract<int>(list_attr.attr("__len__")()) ;
    for (int i = 0 ; i < i_len; i++ )
    {
        get_scalar_value<std::string>(attr_name,"String",list_attr[i]);
		boost::python::list list_prop = (boost::python::list)(py_list_attr_prop[ attr_name ]);
		short j_len = boost::python::extract<short>(list_prop.attr("__len__")()) ;
		db_data.push_back(Tango::DbDatum(attr_name));
        for (int j=0 ; j < j_len ; j++ )
		{
			get_scalar_value<std::string>(prop_name,"String",list_prop[j]);
			db_data.push_back(Tango::DbDatum(prop_name));
		}
    }
}

boost::python::dict 
Database::translate_dbdata_to_property_dict(Tango::DbData &db_data,boost::python::list list_prop)
{
    std::vector<std::string> vstr_array;
    get_array_value<std::string>(vstr_array,"String",
		(boost::python::object)list_prop);
    std::vector<Tango::DbDatum>::iterator i;
    boost::python::dict py_list_prop;
    int k;
    for ( k=0,i=db_data.begin() ; i < db_data.end() ; k++,i++)
    {
		boost::python::list py_list_val = get_list_from_vector<std::string>(i->value_string);
		py_list_prop[ vstr_array[k] ] = py_list_val;
	}
    return py_list_prop;
}

boost::python::dict 
Database::translate_dbdata_to_attribute_property_dict(Tango::DbData &db_data,boost::python::list list_attr)
{
    std::vector<std::string> vstr_array;
    get_array_value<std::string>(vstr_array,"String",
		(boost::python::object)list_attr);
    boost::python::dict py_list_attr_prop;
	long n_attr_props;
	int index=-1;
	int p_len = boost::python::extract<int>(list_attr.attr("__len__")()) ;
    for (int i = 0 ; i < p_len; i++ )
    {
		boost::python::dict py_list_prop;
		db_data[index+1] >> n_attr_props;
		for (int j=index+2; j < index + 2 + n_attr_props; j++)
		{
			boost::python::list py_list_val = get_list_from_vector<std::string>(db_data[j].value_string);
			py_list_prop[ db_data[j].name ] = py_list_val;
		}
		py_list_attr_prop[ vstr_array[i] ] = py_list_prop;
		index += n_attr_props + 1;
	}
    return py_list_attr_prop;
}

