#!/usr/local/bin/octave-cli
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% reimplementation of the standard pendulum example of onelab in octave      %%   
%% Alexander Krimm <alex@wirew0rm.de>, TEMF TU-Darmstadt, 2014                %%   
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

printf("Starting test solver for octavelab\n");

% load the onelab api
pkg load onelab;

% parameter parsing
for i = [1:nargin]
	if (i+2 > nargin)
		error("invoke with -onelab name server_addr");
	elseif (strcmp(argv(){i},"-onelab"))
		name = argv(){i+1};
		addr = argv(){i+2};
		break;
	endif
endfor

% Start the client
ol_client(name, addr);

% we're done if we are in the "check" phase
action = ol_getParameters([name "/Action"]);
if (!strcmp(action{1}.value, "compute"))
	printf("Nothing to do");
	ol_disconnect();
	exit;
endif

% change to directory
wdir = fileparts(mfilename('fullpath'));

% initialize onelab variables
meshfilepar.name = "Gmsh/MshFileName";
meshfilepar.value = [wdir "/LaplacianNeumann.msh"];
meshfilepar.type = "string";
meshfilepar.label = "Mesh File Name";
meshfilepar.help = "mesh to use";
ol_setParameter(meshfilepar);

par.name = "Input/param";
par.type = "number";
par.value = 1;
par.label = "par";
par.choices = cell();
ol_setParameter(par);

pointx.name = "ReadPoint/pointx";
pointx.type = "number";
pointx.value = 0.4;
pointx.label = "X-Wert";
pointx.choices = cell();
ol_setParameter(pointx);

pointy.name = "ReadPoint/pointy";
pointy.type = "number";
pointy.value = 0.6;
pointy.label = "Y-Wert";
pointy.choices = cell();
ol_setParameter(pointy);

dataname.name = "ReadPoint/dataname";
dataname.type = "string";
dataname.value = "Output/P1" 
dataname.label = "Speicherpunkt";
dataname.choices = cell();
ol_setParameter(dataname);

result.name = "Output/result";
result.type = "number";
result.value = 1;
result.label = "u(P1)*u(P2)";
result.choices = {};
ol_setParameter(result);

% Values to run solver for
ps = [1:10];

for p = ps
	% solve the problem
	par.value = p; ol_setParameter(par);
	ol_runBlockingSubClient("subclient", ["getdp " wdir "/LaplacianNeumann.pro -solve LaplacianNeumann -pos Map_u"]);
	% read Point P1
	pointx.value = 0.1; ol_setParameter(pointx);
	pointy.value = 0.3; ol_setParameter(pointy);
	dataname.value = "Output/P1"; ol_setParameter(dataname);
	ol_runBlockingSubClient("subclient", ["getdp " wdir "/ReadPoint.pro -res " wdir "/LaplacianNeumann.res -pre LaplacianNeumann -pos readPoint"]);
	p1 = ol_getParameters("Output/P1");
	% read Point P2
	pointx.value = 0.4; ol_setParameter(pointx);
	pointy.value = 0.8; ol_setParameter(pointy);
	dataname.value = "Output/P2"; ol_setParameter(dataname);
	ol_runBlockingSubClient("subclient", ["getdp " wdir "/ReadPoint.pro -res " wdir "/LaplacianNeumann.res -pre LaplacianNeumann -pos readPoint"]);
	p2 = ol_getParameters("Output/P2");
	% Postprocess data to P1 + P2
	result.value = p1{1}.value * p2{1}.value;
	result.choices = {result.choices{:} result.value};
	ol_setParameter(result);
endfor

ol_sendInfo('ende')
ol_disconnect();
