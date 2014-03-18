////////////////////////////////////////////////////////////////////////////////
// Onelab interface for octave. Exposes all necessary calls from the C++      //
// bindings to octave.                                                        //
// Alexander Krimm <alex@wirew0rm.de>, TEMF TU-Darmstadt, 2014                //
////////////////////////////////////////////////////////////////////////////////
// PKG_ADD: ollib = which("onelab.oct");
// PKG_DEL: ollib = which("onelab.oct");

#include <gmsh/onelab.h>
// undefine Symbols as octave uses the same symbols
#undef HAVE_OPENGL
#undef HAVE_FLTK
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <oct.h>
#include <octave/ov-struct.h>

#ifndef DEFCONST
#define DEFCONST(name, defn, doc) DEFCONST_INTERNAL(name, defn, doc)
#endif

// define to string to not depend on C++11
template <typename T>
std::string to_string(T const& value) {
	std::stringstream sstr;
	sstr << value;
	return sstr.str();
}

onelab::remoteNetworkClient *c = NULL;

// PKG_ADD: autoload("ol_client", ollib);
// PKG_DEL: try; autoload ("ol_client", ollib, "remove"); catch; end;
DEFUN_DLD (ol_client, args, ,
		"-*- texinfo -*-\n\
		@deftypefn {Loadable Funktion} {} ol_client (@var{name}, @var{adress})\n\
		Connect to the onelab server listening on @var{adress} with the solver\n\
		name @var{name}\n\
		@end deftypefn") {
	// check if there already is a connection
	if (c != NULL) {
		error("Already connected");
		return octave_value_list ();
	}
	// check input arguments
	int nargin = args.length();
	if (nargin != 2) {
		print_usage ();
		error("call ol_client with solver name and server adress");
		return octave_value_list ();
	}
	std::string name, address;
	name = args(0).string_value();
	address = args(1).string_value();
	// establish connection to the server
	c = new onelab::remoteNetworkClient(name, address);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_disconnect", ollib);
// PKG_DEL: try; autoload ("ol_disconnect", ollib, "remove"); catch; end;
DEFUN_DLD (ol_disconnect, args, ,
		"disconnect from the server and free the resources") {
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input arguments
	int nargin = args.length();
	if (nargin != 0) {
		print_usage ();
		error("too many arguments");
		return octave_value_list ();
	}
	// Delete the client object
	delete c;
	c = NULL;
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_getName", ollib);
// PKG_DEL: try; autoload ("ol_getName", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getName, args, ,
		           "get client name")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// checking input arguments
	int nargin = args.length();
	if (nargin != 0) {
		print_usage ();
		error("getName has no arguments");
		return octave_value_list ();
	}
	// return name
	std::string name;
	name = c->getName();
	return octave_value_list(name);
}


// PKG_ADD: autoload("ol_setId", ollib);
// PKG_DEL: try; autoload ("ol_setId", ollib, "remove"); catch; end;
DEFUN_DLD (ol_setId, args, ,
		           "set client id")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// checking input arguments
	int nargin = args.length();
	if (nargin != 1) {
		print_usage();
		error("setID needs argument id");
		return octave_value_list ();
	}
	// Set the ID
	int id = args(0).int_value();
	c->setId(id);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_getId", ollib);
// PKG_DEL: try; autoload ("ol_getId", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getId, args, ,
		           "get client id")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// checking input parameters
	int nargin = args.length();
	if (nargin != 0) {
		print_usage();
		error("getId has no arguments");
		return octave_value_list ();
	}
	// Get the ID
	int id;
	id = c->getId();
	return octave_value_list(id);
}


// PKG_ADD: autoload("ol_setIndex", ollib);
// PKG_DEL: try; autoload ("ol_setIndex", ollib, "remove"); catch; end;
DEFUN_DLD (ol_setIndex, args, ,
		           "set client index")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input arguments
	int nargin = args.length();
	if (nargin != 1) {
		print_usage();
		error("setIndex needs argument index");
		return octave_value_list ();
	}
	// Set the index
	int index = args(0).int_value();
	c->setIndex(index);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_getIndex", ollib);
// PKG_DEL: try; autoload ("ol_getIndex", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getIndex, args, ,
		           "get client index")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin != 0) { 
		print_usage();
		error("getIndex has no arguments");
		return octave_value_list ();
	}
	// get Index
	int index;
	index = c->getIndex();
	return octave_value_list(index);
}


// PKG_ADD: autoload("ol_run", ollib);
// PKG_DEL: try; autoload ("ol_run", ollib, "remove"); catch; end;
DEFUN_DLD (ol_run, args, ,
		           "run client")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// checking input parameters
	int nargin = args.length();
	if (nargin != 0) {
		print_usage();
		error("run has no arguments");
		return octave_value_list ();
	}
	// run it and return values
	return octave_value_list(c->run());
}


// PKG_ADD: autoload("ol_isNetworkClient", ollib);
// PKG_DEL: try; autoload ("ol_isNetworkClient", ollib, "remove"); catch; end;
DEFUN_DLD (ol_isNetworkClient, args, ,
		           "check if client is network client")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin != 0) {
		print_usage();
		error("isNetworkClient has no arguments");
		return octave_value_list ();
	}
	// check if the client is a network client
	return octave_value_list(c->isNetworkClient());
}


// PKG_ADD: autoload("ol_kill", ollib);
// PKG_DEL: try; autoload ("ol_kill", ollib, "remove"); catch; end;
DEFUN_DLD (ol_kill, args, ,
		           "kill client")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin != 0) {
		print_usage();
		error("kill has no arguments");
		return octave_value_list ();
	}
	// Kill client process
	return octave_value_list(c->kill());
}


// PKG_ADD: autoload("ol_sendInfo", ollib);
// PKG_DEL: try; autoload ("ol_sendInfo", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendInfo, args, ,
		           "send info")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input arguments
	int nargin = args.length();
	if (nargin != 1) {
		print_usage();
		error("call ol_sendInfo with msg");
		return octave_value_list ();
	}
	std::string msg = args(0).string_value();
	//TODO check conversion to string
	// send Information message to onelab server
	c->sendInfo(msg);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendWarning", ollib);
// PKG_DEL: try; autoload ("ol_sendWarning", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendWarning, args, ,
		           "send Warning")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin != 1) {
		print_usage();
		error("call ol_sendWarning with msg");
		return octave_value_list ();
	}
	std::string msg = args(0).string_value();
	//TODO check conversion to string
	// send Warning message to onelab server
	c->sendWarning(msg);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendError", ollib);
// PKG_DEL: try; autoload ("ol_sendError", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendError, args, ,
		           "send error")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin != 1) {
		print_usage();
		error("call ol_sendError with msg");
		return octave_value_list ();
	}
	std::string msg = args(0).string_value();
	//TODO check conversion to string
	// Sent Error message to the onelab server
	c->sendError(msg);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendProgress", ollib);
// PKG_DEL: try; autoload ("ol_sendProgress", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendProgress, args, ,
		           "send progress")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin != 1) {
		print_usage();
		error("call ol_sendProgress with msg");
		return octave_value_list ();
	}
	std::string msg = args(0).string_value();
	//TODO: Check conversion to string
	// update Client Progress Status (shown in gmsh status bar)
	c->sendProgress(msg);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendMergeFileRequest", ollib);
// PKG_DEL: try; autoload ("ol_sendMergeFileRequest", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendMergeFileRequest, args, ,
		           "send merge file request")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin != 1) {
		print_usage();
		error("call ol_sendMergeFileRequest with msg");
		return octave_value_list ();
	}
	std::string msg = args(0).string_value();
	//TODO: Check to string conversion
	// Make onelab load files into gmsh
	c->sendMergeFileRequest(msg);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendParseStringRequest", ollib);
// PKG_DEL: try; autoload ("ol_sendParseStringRequest", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendParseStringRequest, args, ,
		           "send parse string request")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	int nargin = args.length();
	if (nargin != 1) {
		print_usage();
		error("call ol_sendParseStringRequest with msg");
		return octave_value_list ();
	}
	std::string msg = args(0).string_value();
	//TODO: check to string conversion
	// send Parse String Request
	c->sendParseStringRequest(msg);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendVertexArray", ollib);
// PKG_DEL: try; autoload ("ol_sendVertexArray", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendVertexArray, args, ,
		           "send vertex array")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin != 1) {
		print_usage();
		error("call ol_sendVertexArray with msg");
		return octave_value_list ();
	}
	std::string msg = args(0).string_value();
	//TODO: check conversion to string
	// Send Vertex array
	c->sendVertexArray(msg);
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_setNumber", ollib);
// PKG_DEL: try; autoload ("ol_setNumber", ollib, "remove"); catch; end;
DEFUN_DLD (ol_setNumber, args, ,
		           "create or update a number")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin != 4) 
		error("call ol_setNumber with name, value, label and helpstring");
	else {
		std::string name = args(0).string_value();
		double value = args(1).double_value();
		std::string label = args(2).string_value();
		std::string help = args(3).string_value();

		onelab::number n = onelab::number(name, value, label, help);
		c->set(n);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_setString", ollib);
// PKG_DEL: try; autoload ("ol_setString", ollib, "remove"); catch; end;
DEFUN_DLD (ol_setString, args, ,
		           "create or update a string")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	int nargin = args.length();
	if (nargin != 4) 
		error("call ol_setNumber with name, value, label and helpstring");
	else {
		std::string name = args(0).string_value();
		std::string value = args(1).string_value();
		std::string label = args(2).string_value();
		std::string help = args(3).string_value();

		onelab::string n = onelab::string(name, value, label, help);
		c->set(n);
	}
	return octave_value_list ();
}


octave_value_list ol2oct(std::vector<onelab::parameter> parameterlist){
	octave_value_list result;
	for (std::vector<onelab::parameter>::iterator i = parameterlist.begin(); i != parameterlist.end(); ++i){
		octave_scalar_map st;
		// general parameter values
		st.assign(std::string("label"), i->getLabel());
		st.assign(std::string("help"), i->getHelp());
		st.assign(std::string("type"),i->getType());
		st.assign(std::string("Path"),i->getPath());
		st.assign(std::string("changed"),i->getChanged());
		st.assign(std::string("neverChanged"),i->getNeverChanged());
		st.assign(std::string("visible"), i->getVisible());
		st.assign(std::string("readOnly"), i->getReadOnly());
		//getAttributes
		octave_scalar_map attributes;
		std::map<std::string,std::string> attrlst = i->getAttributes();
		for (std::map<std::string,std::string>::iterator j = attrlst.begin(); j != attrlst.end(); ++j) {
			attributes.assign(j->first,j->second);
		}
		st.assign(std::string("attributes"),attributes);

		std::string type = i->getType();
		if (type == "number") {
			onelab::number num = (onelab::number)*i;
			st.assign(std::string("value"), num.getValue());
			st.assign(std::string("min"), num.getMin());
			st.assign(std::string("max"), num.getMax());
			st.assign(std::string("step"), num.getStep());
			st.assign(std::string("index"), num.getIndex());
			// Choices
			octave_scalar_map choices;
			std::vector<double> c = num.getChoices();
			for (std::vector<double>::iterator j = c.begin(); j != c.end(); ++j)
				choices.assign(to_string(*j),std::string("")); //TODO: find out if choice labels can be read
			st.assign(std::string("Choices"), choices);
			// Value Labels
			octave_scalar_map vlabels;
			std::map<double, std::string> vl = num.getValueLabels();
			for (std::map<double, std::string>::iterator j = vl.begin(); j != vl.end(); ++j)
				vlabels.assign(to_string(j->first),j->second);
			st.assign(std::string("Labels"), vlabels);
		} else if (type == "string") {
			onelab::string s = (onelab::string)*i;
			st.assign(std::string("value"), s->getValue());
		} else if (type == "region") {
			onelab::region r = (onelab::region)*i;
			st.assign(std::string("value"), r->getValue());
			//TODO: Implement rest of the region class
		} else if (type =="expression") {
			onelab::expression exp = (onelab::expression)*i;
			st.assign(std::string("value"), exp->getValue());
			//TODO: Implement rest of the function class
		} else {
			// unknown parameter type
			error("Unknown parameter type" + type);
		}
		result.append(st);
	}
	return result;
}

// PKG_ADD: autoload("ol_getNumber", ollib);
// PKG_DEL: try; autoload ("ol_getNumber", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getParameters, args, ,
		           "Parameters from onelab")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin > 1) {
		print_usage();
		error("call ol_Parameters with name or without parameter");
		return octave_value_list ();
	}
	std::string name = args(0).string_value();
	// Retrieve Parameters
	std::vector<onelab::number> n;
	c->get(n, name);
	std::vector<onelab::string> s;
	c->get(s, name);

	octave_value_list result;
	result = ol2oct(n);
	return result;
}

// WILL BE DELETED
// PKG_ADD: autoload("ol_getString", ollib);
// PKG_DEL: try; autoload ("ol_getString", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getString, args, ,
		           "get a String from onelab")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	int nargin = args.length();
	if (nargin != 1) {
		error("call ol_getString with name");
		return octave_value_list ();
	}
	std::string name = args(0).string_value();

	std::vector<onelab::string> s;
	octave_value_list result;
	if (c->get(s, name)){
		for (std::vector<onelab::string>::iterator si = s.begin(); si != s.end(); ++si){
			octave_scalar_map st;
			st.assign("value", si->getValue());
			result.append(st);
		}
	}
	return result;
}


// PKG_ADD: autoload("ol_toString", ollib);
// PKG_DEL: try; autoload ("ol_toString", ollib, "remove"); catch; end;
DEFUN_DLD (ol_toString, args, ,
		"output all Parameters as strings") {
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	int nargin = args.length();
	if (nargin != 0) 
		error("toStrin has no arguments");
	else {
		octave_value_list result;
		std::vector<std::string> v = c->toChar();
		for (std::vector<std::string>::iterator vi = v.begin(); vi != v.end(); ++vi){
			result.append(*vi);
		}
		return result;
	}
	return octave_value_list ();
}


/* TODO: TESTCASES

%!test 
%! assert(1,1)

*/
