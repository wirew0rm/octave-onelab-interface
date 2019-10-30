//////////////////////////////////////////////////////////////////////////////////////////////
// Onelab interface for octave. Exposes all necessary calls from the C++                    //
// bindings to octave.                                                                      //
// Alexander Krimm <alexander_johannes.krimm@stud.tu-darmstadt.de>, TEMF TU-Darmstadt, 2014 //
//////////////////////////////////////////////////////////////////////////////////////////////
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
#include <octave/ov-cell.h>

#ifndef DEFCONST
#define DEFCONST(name, defn, doc) DEFCONST_INTERNAL(name, defn, doc)
#endif

// define to_string() to not depend on C++11
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
		return octave_value_list();
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


// PKG_ADD: autoload("ol_runNonBlockingSubClient", ollib);
// PKG_DEL: try; autoload ("ol_runNonBlockingSubClient", ollib, "remove"); catch; end;
DEFUN_DLD (ol_runNonBlockingSubClient, args, ,
		           "run client")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// checking input parameters
	int nargin = args.length();
	if (nargin != 2) {
		print_usage();
		error("runNonBlockingSubClient(name, command)");
		return octave_value_list ();
	}
	std::string name = args(0).string_value();
	std::string command = args(1).string_value();
	//TODO check conversion to string
	// run it and return values
	c->runNonBlockingSubClient(name, command);
	return octave_value_list();
}

// PKG_ADD: autoload("ol_waitOnSubClients", ollib);
// PKG_DEL: try; autoload ("ol_waitOnSubClients", ollib, "remove"); catch; end;
DEFUN_DLD (ol_waitOnSubClients, args, ,
		           "wait for subclients to finish")
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
		error("ol_waitOnSubClients expects no arguments");
		return octave_value_list ();
	}
	// run it and return values
	c->waitOnSubClients();
	return octave_value_list();
}

// PKG_ADD: autoload("ol_runBlockingSubClient", ollib);
// PKG_DEL: try; autoload ("ol_runBlockingSubClient", ollib, "remove"); catch; end;
DEFUN_DLD (ol_runBlockingSubClient, args, ,
		           "run client and wait for it to finish before proceeding")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// checking input parameters
	int nargin = args.length();
	if (nargin != 2) {
		print_usage();
		error("runBlockingSubClient(name, command)");
		return octave_value_list ();
	}
	std::string name = args(0).string_value();
	std::string command = args(1).string_value();
	//TODO check conversion to string
	// run it and return values
	c->runNonBlockingSubClient(name, command);
	c->waitOnSubClients();
	return octave_value_list();
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


// PKG_ADD: autoload("ol_setParameter", ollib);
// PKG_DEL: try; autoload ("ol_setParameter", ollib, "remove"); catch; end;
DEFUN_DLD (ol_setParameter, args, ,
		           "create or update a parameter")
{
	// check if connected
	if (c == NULL) {
		error("Not connected, doing nothing");
		return octave_value_list ();
	}
	// check input parameters
	int nargin = args.length();
	if (nargin < 1 || nargin > 5 || nargin == 2) {	
		error("call ol_setNumber with name, type, value, [label] and [helpstring] or parameter struct");
		return octave_value_list();
	}
	// if it is not already a parameter struct, create one
	octave_scalar_map parameter;
	if (nargin == 1)
		parameter = args(0).scalar_map_value();
	if (nargin > 2) {
		parameter.assign("name", args(0).string_value());
		if (parameter.contents("name").string_value() == "") {
			error("empty parameter name");
			return octave_value_list(0);
		 }
		std::string type = args(1).string_value();
		if (type != "string" && type != "number") {
			error("parameter type not known");
			return octave_value_list(0);
		}
		parameter.assign("type", type);
		if (type == std::string("number")) {
				parameter.assign("value", args(2).float_value());
		} else if (type == std::string("string")) {
				parameter.assign("value", args(2).string_value());
		}
		parameter.assign("label", "");
		parameter.assign("help", "");
	}
	if (nargin > 3)
		parameter.assign("label", args(3).string_value());
	if (nargin > 4)
		parameter.assign("help", args(4).string_value());
	// update/create/delete Parameters
	std::string name = parameter.contents("name").string_value();
	std::string label;
	if (parameter.contains("label")) {
		label = parameter.contents("label").string_value();
	} else {
		label = "";
	}
	std::string help;
	if (parameter.contains("help")) {
		std::string help = parameter.contents("help").string_value();
	} else {
		help = "";
	}
	if (parameter.contents("type").string_value() == "number"){
		double value = parameter.contents("value").float_value();
		onelab::number n = onelab::number(name, value, label, help);
		if (parameter.contains("changed"))
			n.setChanged(parameter.contents("changed").bool_value());
		if (parameter.contains("visible"))
			n.setVisible(parameter.contents("visible").bool_value());
		if (parameter.contains("readonly"))
			n.setReadOnly(parameter.contents("readonly").bool_value());
		//number stuff
		if (parameter.contains("min"))
			n.setMin(parameter.contents("min").float_value());
		if (parameter.contains("max"))
			n.setMax(parameter.contents("max").float_value());
		if (parameter.contains("step"))
			n.setStep(parameter.contents("step").float_value());
		if (parameter.contains("index"))
			n.setIndex(parameter.contents("index").int_value());
		//choices
		if (parameter.contains("choices")){
			std::vector<double> choices;
			octave_value_list octchoices = parameter.contents("choices").list_value();
			for (int i = 0; i != octchoices.length(); ++i) {
				choices.push_back(octchoices(i).float_value());
			}
			n.setChoices(choices);
			// Choice labels are ignored as they are messed up
		}
		c->set(n);
	} else if (parameter.contents("type").string_value() == "string") {
		std::string value = parameter.contents("value").string_value();
		onelab::string s = onelab::string(name, value, label, help);
		if (parameter.contains("changed"))
			s.setChanged(parameter.contents("changed").bool_value());
		if (parameter.contains("visible"))
			s.setVisible(parameter.contents("visible").bool_value());
		if (parameter.contains("readonly"))
			s.setReadOnly(parameter.contents("readonly").bool_value());
		//string stuff
		if (parameter.contains("kind"))
			s.setKind(parameter.contents("kind").string_value());
		//choices
		if (parameter.contains("choices")){
			std::vector<std::string> choices;
			octave_value_list octchoices = parameter.contents("choices").list_value();
			for (int i = 0; i != octchoices.length(); ++i) {
				choices.push_back(octchoices(i).string_value());
			}
			s.setChoices(choices);
			// Choice labels are ignored as they are messed up
		}
		c->set(s);
	} else {
		error("missing or invalid parameter type information");
		return octave_value_list(0);
	}
	return octave_value_list(1); //success
}

void ol2oct(std::vector<onelab::number> parameterlist, octave_value_list &result){
	for (std::vector<onelab::number>::iterator i = parameterlist.begin(); i != parameterlist.end(); ++i){
		octave_scalar_map st;
		// general parameter values
		st.assign(std::string("name"), i->getName());
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

		// number specific
		st.assign(std::string("value"), i->getValue());
		st.assign(std::string("min"), i->getMin());
		st.assign(std::string("max"), i->getMax());
		st.assign(std::string("step"), i->getStep());
		st.assign(std::string("index"), i->getIndex());
		// Choices
		std::vector<double> c = i->getChoices();
		octave_value_list choices;
		for (size_t j = 0; j < c.size(); ++j){
			choices(j) = c.at(j);
		}
		st.assign(std::string("choices"), choices.cell_value());
		// Value Labels are ignored because they are messed up
		result.append(st);
	}
	return;
}

void ol2oct(std::vector<onelab::string> parameterlist, octave_value_list &result){
	for (std::vector<onelab::string>::iterator i = parameterlist.begin(); i != parameterlist.end(); ++i){
		octave_scalar_map st;
		// general parameter values
		st.assign(std::string("name"), i->getName());
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
		// string specific
		st.assign(std::string("value"), i->getValue());
		st.assign(std::string("kind"), i->getKind());
		// Choices
		std::vector<std::string> c = i->getChoices();
		octave_value_list choices;
		for (size_t j = 0; j < c.size(); ++j){
			choices(j) = c.at(j);
		}
		st.assign(std::string("choices"), choices);
		result.append(st);
	}
	return;
}

// TODO implement Region and Function Parameter Types
//void ol2oct(std::vector<onelab::region> parameterlist, octave_value_list &result){
//	for (std::vector<onelab::region>::iterator i = parameterlist.begin(); i != parameterlist.end(); ++i){
//		octave_scalar_map st;
//		// general parameter values
//		st.assign(std::string("name"), i->getName());
//		st.assign(std::string("label"), i->getLabel());
//		st.assign(std::string("help"), i->getHelp());
//		st.assign(std::string("type"),i->getType());
//		st.assign(std::string("Path"),i->getPath());
//		st.assign(std::string("changed"),i->getChanged());
//		st.assign(std::string("neverChanged"),i->getNeverChanged());
//		st.assign(std::string("visible"), i->getVisible());
//		st.assign(std::string("readOnly"), i->getReadOnly());
//		//getAttributes
//		octave_scalar_map attributes;
//		std::map<std::string,std::string> attrlst = i->getAttributes();
//		for (std::map<std::string,std::string>::iterator j = attrlst.begin(); j != attrlst.end(); ++j) {
//			attributes.assign(j->first,j->second);
//		}
//		st.assign(std::string("attributes"),attributes);
//		// region specific
//		octave_value_list value;
//		std::set<std::string> val = i->getValue();
//		for (std::set<std::string>::iterator j = val.begin(); j != val.end(); ++j)
//			value.append(*j);
//		st.assign(std::string("value"), value);
//		st.assign(std::string("dimension"), i->getDimension());
//		// Choices
//		octave_value_list choices;
//		std::vector<std::map<std::string> > c = i->getChoices();
//		for (std::vector<std::map<std::string> >::iterator j = c.begin(); j != c.end(); ++j){
//			octave_value_list choicelist;
//			for (std::map<std::string>::iterator k = j->begin(); k != j->end(); ++k)
//				choicelist.append(*k);
//			map
//			choices.append(*j); //TODO: find out if choice labels can be read
//		}
//		st.assign(std::string("Choices"), choices);
//		result.append(st);
//	}
//	return;
//}
//
//void ol2oct(std::vector<onelab::function> parameterlist, octave_value_list &result){
//	for (std::vector<onelab::function>::iterator i = parameterlist.begin(); i != parameterlist.end(); ++i){
//		octave_scalar_map st;
//		// general parameter values
//		st.assign(std::string("name"), i->getName());
//		st.assign(std::string("label"), i->getLabel());
//		st.assign(std::string("help"), i->getHelp());
//		st.assign(std::string("type"),i->getType());
//		st.assign(std::string("Path"),i->getPath());
//		st.assign(std::string("changed"),i->getChanged());
//		st.assign(std::string("neverChanged"),i->getNeverChanged());
//		st.assign(std::string("visible"), i->getVisible());
//		st.assign(std::string("readOnly"), i->getReadOnly());
//		//getAttributes
//		octave_scalar_map attributes;
//		std::map<std::string,std::string> attrlst = i->getAttributes();
//		for (std::map<std::string,std::string>::iterator j = attrlst.begin(); j != attrlst.end(); ++j) {
//			attributes.assign(j->first,j->second);
//		}
//		st.assign(std::string("attributes"),attributes);
//		// string specific
//		st.assign(std::string("value"), i->getValue());
//		st.assign(std::string("kind"), i->getKind());
//		// Choices
//		octave_scalar_map choices;
//		std::vector<std::string> c = i->getChoices();
//		for (std::vector<std::string>::iterator j = c.begin(); j != c.end(); ++j)
//			choices.assign(*j,std::string("")); //TODO: find out if choice labels can be read
//		st.assign(std::string("Choices"), choices);
//		result.append(st);
//	}
//	return;
//}

// PKG_ADD: autoload("ol_getParameters", ollib);
// PKG_DEL: try; autoload ("ol_getParameters", ollib, "remove"); catch; end;
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
	std::string name = std::string("");
	if (nargin==1) name = args(0).string_value();
	// Retrieve Parameters
	octave_value_list result;
	std::vector<onelab::number> n;
	c->get(n, name);
	ol2oct(n, result);
	std::vector<onelab::string> s;
	c->get(s, name);
	ol2oct(s, result);
// TODO: implement Region and function parameters
//	std::vector<onelab::region> r;
//	c->get(r, name);
//	ol2oct(r, result);
//	std::vector<onelab::function> f;
//	c->get(f, name);
//	ol2oct(f, result);

//	Return a list of parameters
	octave_value_list resultwrap;
	resultwrap(0) = result.cell_value();
	return octave_value(resultwrap);
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
		std::stringstream result;
		std::vector<std::string> v = c->toChar();
		for (std::vector<std::string>::iterator vi = v.begin(); vi != v.end(); ++vi){
			result << *vi << std::endl;
		}
		return octave_value_list(result.str());
	}
	return octave_value_list ();
}


/* TODO: TESTCASES

%!test 
%! assert(1,1)

*/
