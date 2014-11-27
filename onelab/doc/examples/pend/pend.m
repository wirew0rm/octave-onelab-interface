#!/usr/bin/octave-cli
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% reimplementation of the standard pendulum example of onelab in octave                      %%   
%% Alexander Krimm <alexander_johannes.krimm@stud.tu-darmstadt.de>, TEMF TU-Darmstadt, 2014   %%   
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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

% get working correct dir
wdir = fileparts(mfilename('fullpath'));

function exportMsh(wdir, le1, le2)
	fid = fopen ([wdir '/' "pend.msh"], 'w');
	if (!fid)
		error("failed to open file");
		exit;
	endif
	fprintf(fid, '$MeshFormat\n2.2 0 8\n$EndMeshFormat\n');
	fprintf(fid, '$Nodes\n3\n1 0 0 0\n2 0 %f 0\n3 0 %f 0\n$EndNodes\n', -le1, -le1-le2);
	fprintf(fid, '$Elements\n3\n1 1 2 0 1 1 2\n2 1 2 0 1 2 3\n3 15 2 0 2 3\n$EndElements\n');
	fclose(fid);
endfunction

function exportMshOpt(wdir)
	fid = fopen ([wdir '/' "pend.msh.opt"], 'w');
	if (!fid)
		error("failed to open file");
		exit;
	endif
	fprintf(fid, 'n = PostProcessing.NbViews - 1;\n');
	fprintf(fid, 'If(n >= 0)\nView[n].ShowScale = 0;\nView[n].VectorType = 5;\n');
	fprintf(fid, 'View[n].ExternalView = 0;\nView[n].DisplacementFactor = 1 ;\n');
	fprintf(fid, 'View[n].PointType = 1;\nView[n].PointSize = 5;\n');
	fprintf(fid, 'View[n].LineWidth = 2;\nEndIf\n');
	fclose(fid);
endfunction

function exportIter(wdir, iter, t, x1, y1, x2, y2)
	fid = fopen ([wdir '/' "pend.msh"], 'a');
	if (!fid)
		error("failed to open file");
		exit;
	endif
	fprintf(fid, '$NodeData\n1\n"motion"\n1\n\t%f\n3\n\t%d\n3\n', t, iter);
	fprintf(fid, '\t3\n\t1 0 0 0\n\t2 %f %f 0\n\t3 %f %f 0\n$EndNodeData\n', x1, y1, x2, y2);
	fclose(fid);
endfunction

g = 9.8;
m = 0.3;

l.name = "Geom/Length";
l.type = "number";
l.value=1.0;
l.label = "Arm length[m]";
l.help = "Length of the arm";
ol_setParameter(l);

time.name = "Dyna/time";
time.type = "number";
time.value = 0.0;
time.label = "time [s]";
time.help = "elapsed time";
ol_setParameter(time);

dt.name = "Geom/dt";
dt.type = "number";
dt.value = 0.001;
dt.label = "time step [s]";
dt.help = "time step size";
ol_setParameter(dt);

tmax.name = "Geom/tmax";
tmax.type = "number";
tmax.value = 20;
tmax.label = "max time [s]";
tmax.help = "Maximum time";
ol_setParameter(tmax);

refresh.name = "Geom/refresh";
refresh.type = "number";
refresh.value = 0.05;
refresh.label = "refresh interval [s]";
ol_setParameter(refresh);

theta0.name = "Geom/theta";
theta0.type = "number";
theta0.value = 10;
theta0.label = "Initial theta angle[deg]";
ol_setParameter(theta0);

phi0.name = "Geom/phi";
phi0.type = "number";
phi0.value = 180;
phi0.label = "Initial Phi angle [deg]";
ol_setParameter(phi0);

printf("+++\n");
test = ol_toString
printf("+++\n");
% we're done if we are in the "check" phase
action = ol_getParameters([name "/Action"]);
if (isempty(action) || !strcmp(action{1}.value, "compute"))
	printf("Nothing to do");
	ol_disconnect();
	exit;
endif

l1 = l.value;
l2 = l.value;
m1 = m;
m2 = m;
theta.name = 'Solu/theta';
theta.choices = cell();
theta.type = "number";
theta.label = "theta";
theta.value = theta0.value / 180 * pi;
phi.name = 'Solu/phi';
phi.choices = cell();
phi.type = "number";
phi.label = "phi";
phi.value = phi0.value / 180 * pi;
theta_dot.name = 'Solu/theta_dot';
theta_dot.choices = cell();
theta_dot.type = "number";
theta_dot.label = "theta_dot";
theta_dot.value = 0.0;
phi_dot.name = 'Solu/phi_dot';
phi_dot.choices = cell();
phi_dot.type = "number";
phi_dot.label = "phi_dot";
phi_dot.value = 0.0;
refr = 0.0;
iter = 0;
time.value = 0.0;

while (time.value < tmax.value)
	delta = phi.value - theta.value;
	sdelta = sin(delta);
	cdelta = cos(delta);
	theta_dot_dot = ( m2*l1*(theta_dot.value**2.0)*sdelta*cdelta ...
										+ m2*g*sin(phi.value)*cdelta ...
										+ m2*l2*(phi_dot.value**2.0)*sdelta ...
										- (m1+m2)*g*sin(theta.value) );
	theta_dot_dot /= ( (m1+m2)*l1 - m2*l1*(cdelta)**2.0 );

	phi_dot_dot = ( -m2*l2*(phi_dot.value**2.0)*sdelta*cdelta ...
									 + (m1+m2)*(g*sin(theta.value)*cdelta ...
															- l1*(theta_dot.value**2.0)*sdelta ...
															- g*sin(phi.value)) );
	phi_dot_dot /= ( (m1+m2)*l2 - m2*l2*(cdelta)**2.0 );

	theta_dot.value = theta_dot.value + theta_dot_dot*dt.value;
	phi_dot.value = phi_dot.value + phi_dot_dot*dt.value;

	theta.value = theta.value + theta_dot.value*dt.value;
	phi.value = phi.value + phi_dot.value*dt.value;

	x1 =  l1*sin(theta.value);
	y1 = -l1*cos(theta.value);
	x2 =  l1*sin(theta.value) + l2*sin(phi.value);
	y2 = -l1*cos(theta.value) - l2*cos(phi.value);

	time.value += dt.value;
	refr += dt.value;

	exportMshOpt(wdir);

	if (refr >= refresh.value)
		refr = 0;
		ol_setParameter([name '/Progress'], "number", time.value, "Progress", "");
		ol_setParameter(time);
		phi.choices = {phi.choices{:}, phi.value};
		ol_setParameter(phi);
		theta.choices = {theta.choices{:}, theta.value};
		ol_setParameter(theta);
		phi_dot.choices = {phi_dot.choices{:}, phi_dot.value};
		ol_setParameter(phi_dot);
		theta_dot.choices = {theta_dot.choices{:}, theta_dot.value};
		ol_setParameter(theta_dot);

		% ask Gmsh to refresh
		ol_setParameter('Gmsh/Action', "string", 'refresh', "action", "");

		% stop if we are asked to (by Gmsh)
		action = ol_getParameters([name "/Action"]);
		if (strcmp(action{1}.value, "stop"))
			break;
		endif

		exportMsh(wdir, l1, l2);
		exportIter(wdir, iter, time.value, x1, y1+l1, x2, y2+l1+l2);
		ol_sendMergeFileRequest([wdir '/' 'pend.msh']);
		iter += 1;
	endif
endwhile
ol_disconnect();
