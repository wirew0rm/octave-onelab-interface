////////////////////////////////////////////////////////////////////////////////
// Onelab interface for octave. Exposes all necessary calls from the C++      //
// bindings to octave.                                                        //
// Alexander Krimm <alex@wirew0rm.de>, TEMF TU-Darmstadt, 2014                //
////////////////////////////////////////////////////////////////////////////////
// PKG_ADD: ollib = which("onelab.oct");

#include <gmsh/onelab.h>
// undefine Symbols as octave uses the same symbols
#undef HAVE_OPENGL
#undef HAVE_FLTK
#include <stdio.h>
#include <iostream>
#include <oct.h>
#include <octave/ov-struct.h>

#ifndef DEFCONST
#define DEFCONST(name, defn, doc) DEFCONST_INTERNAL(name, defn, doc)
#endif

onelab::remoteNetworkClient *c = NULL;

// PKG_ADD: autoload("ol_client", ollib);
// PKG_DEL: try; autoload ("ol_client", ollib, "remove"); catch; end;
DEFUN_DLD (ol_client, args, ,
		"-*- texinfo -*-\n\
		@deftypefn {Loadable Funktion} {} ol_client (@var{name}, @var{adress})\n\
		Connect to the onelab server listening on @var{adress} with the solver\n\
		name @var{name}\n\
		@end deftypefn") {
	int nargin = args.length();
	if (nargin != 2) 
		print_usage ();
		error("call ol_client with solver name and server adress");
	else {
		std::string name, address;
		name = args(0).string_value();
		address = args(1).string_value();

		if (c != NULL) delete c;
		c = new onelab::remoteNetworkClient(name, address);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_disconnect", ollib);
// PKG_DEL: try; autoload ("ol_disconnect", ollib, "remove"); catch; end;
DEFUN_DLD (ol_disconnect, args, ,
		"disconnect from the server and free the resources") {
	if (c != NULL) {
		delete c;
		c = NULL;
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_getName", ollib);
// PKG_DEL: try; autoload ("ol_getName", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getName, args, ,
		           "get client name")
{
	int nargin = args.length();
	if (nargin != 0) 
		error("getName has no arguments");
	else {
		std::string name;
		name = c->getName();
		return octave_value_list(name);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_setId", ollib);
// PKG_DEL: try; autoload ("ol_setId", ollib, "remove"); catch; end;
DEFUN_DLD (ol_setId, args, ,
		           "set client id")
{
	int nargin = args.length();
	if (nargin != 1) 
		error("setID needs argument id");
	else {
		int id = args(0).int_value();
		c->setId(id);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_getId", ollib);
// PKG_DEL: try; autoload ("ol_getId", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getId, args, ,
		           "get client id")
{
	int nargin = args.length();
	if (nargin != 0) 
		error("getId has no arguments");
	else {
		int id;
		id = c->getId();
		return octave_value_list(id);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_setIndex", ollib);
// PKG_DEL: try; autoload ("ol_setIndex", ollib, "remove"); catch; end;
DEFUN_DLD (ol_setIndex, args, ,
		           "set client index")
{
	int nargin = args.length();
	if (nargin != 1) 
		error("setIndex needs argument index");
	else {
		int index = args(0).int_value();
		c->setIndex(index);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_getIndex", ollib);
// PKG_DEL: try; autoload ("ol_getIndex", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getIndex, args, ,
		           "get client id")
{
	int nargin = args.length();
	if (nargin != 0) 
		error("getIndex has no arguments");
	else {
		int index;
		index = c->getIndex();
		return octave_value_list(index);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_run", ollib);
// PKG_DEL: try; autoload ("ol_run", ollib, "remove"); catch; end;
DEFUN_DLD (ol_run, args, ,
		           "run client")
{
	int nargin = args.length();
	if (nargin != 0) 
		error("run has no arguments");
	else {
		return octave_value_list(c->run());
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_isNetworkClient", ollib);
// PKG_DEL: try; autoload ("ol_isNetworkClient", ollib, "remove"); catch; end;
DEFUN_DLD (ol_isNetworkClient, args, ,
		           "check if client is network client")
{
	int nargin = args.length();
	if (nargin != 0) 
		error("isNetworkClient has no arguments");
	else {
		return octave_value_list(c->isNetworkClient());
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_kill", ollib);
// PKG_DEL: try; autoload ("ol_kill", ollib, "remove"); catch; end;
DEFUN_DLD (ol_kill, args, ,
		           "kill client")
{
	int nargin = args.length();
	if (nargin != 0) 
		error("kill has no arguments");
	else {
		return octave_value_list(c->kill());
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendInfo", ollib);
// PKG_DEL: try; autoload ("ol_sendInfo", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendInfo, args, ,
		           "send info")
{
	int nargin = args.length();
	if (nargin != 1) 
		error("call ol_sendInfo with msg");
	else {
		std::string msg = args(0).string_value();
		c->sendInfo(msg);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendWarning", ollib);
// PKG_DEL: try; autoload ("ol_sendWarning", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendWarning, args, ,
		           "send Warning")
{
	int nargin = args.length();
	if (nargin != 1) 
		error("call ol_sendWarning with msg");
	else {
		std::string msg = args(0).string_value();
		c->sendWarning(msg);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendError", ollib);
// PKG_DEL: try; autoload ("ol_sendError", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendError, args, ,
		           "send error")
{
	int nargin = args.length();
	if (nargin != 1) 
		error("call ol_sendError with msg");
	else {
		std::string msg = args(0).string_value();
		c->sendError(msg);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendProgress", ollib);
// PKG_DEL: try; autoload ("ol_sendProgress", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendProgress, args, ,
		           "send progress")
{
	int nargin = args.length();
	if (nargin != 1) 
		error("call ol_sendProgress with msg");
	else {
		std::string msg = args(0).string_value();
		c->sendProgress(msg);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendMergeFileRequest", ollib);
// PKG_DEL: try; autoload ("ol_sendMergeFileRequest", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendMergeFileRequest, args, ,
		           "send merge file request")
{
	int nargin = args.length();
	if (nargin != 1) 
		error("call ol_sendMergeFileRequest with msg");
	else {
		std::string msg = args(0).string_value();
		c->sendMergeFileRequest(msg);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendParseStringRequest", ollib);
// PKG_DEL: try; autoload ("ol_sendParseStringRequest", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendParseStringRequest, args, ,
		           "send parse string request")
{
	int nargin = args.length();
	if (nargin != 1) 
		error("call ol_sendParseStringRequest with msg");
	else {
		std::string msg = args(0).string_value();
		c->sendParseStringRequest(msg);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_sendVertexArray", ollib);
// PKG_DEL: try; autoload ("ol_sendVertexArray", ollib, "remove"); catch; end;
DEFUN_DLD (ol_sendVertexArray, args, ,
		           "send vertex array")
{
	int nargin = args.length();
	if (nargin != 1) 
		error("call ol_sendVertexArray with msg");
	else {
		std::string msg = args(0).string_value();
		c->sendVertexArray(msg);
	}
	return octave_value_list ();
}


// PKG_ADD: autoload("ol_setNumber", ollib);
// PKG_DEL: try; autoload ("ol_setNumber", ollib, "remove"); catch; end;
DEFUN_DLD (ol_setNumber, args, ,
		           "create or update a number")
{
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


octave_value_list ol2oct(std::vector<onelab::number> parameterlist){
	octave_value_list result;
	for (std::vector<onelab::number>::iterator i = parameterlist.begin(); i != parameterlist.end(); ++i){
		octave_scalar_map st;
		// general parameter values
		st.assign("label", i->getLabel());
		st.assign("help", i->getHelp());
		st.assign("type"),i->getType();
		st.assign("Path"),i->getPath();
		st.assign("changed"),i->getChanged();
		st.assign("neverChanged"),i->getNeverChanged();
		st.assign("visible", i->getVisible());
		st.assign("readOnly", i->getReadOnly());
		//getAttributes
		//octave_scalar_map attributes;
		st.assign("attributes", i->getAttributes());

		//number specific
		st.assign("value", i->getValue());

		result.append(st);
	}
	return result;
}

// PKG_ADD: autoload("ol_getNumber", ollib);
// PKG_DEL: try; autoload ("ol_getNumber", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getNumber, args, ,
		           "get a Number from onelab")
{
	int nargin = args.length();
	octave_value_list result;
	if (nargin != 1) 
		error("call ol_getNumber with name");
	else {
		std::vector<onelab::number> n;
		std::string name = args(0).string_value();
		c->get(n, name);
	 	result = ol2oct(n);
	}
	return result;
}


// PKG_ADD: autoload("ol_getString", ollib);
// PKG_DEL: try; autoload ("ol_getString", ollib, "remove"); catch; end;
DEFUN_DLD (ol_getString, args, ,
		           "get a String from onelab")
{
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
