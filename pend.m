#!/usr/bin/octave -qf
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% reimplementation of the standard pendulum example of onelab in octave      %%   
%% Alexander Krimm <alex@wirew0rm.de>, TEMF TU-Darmstadt, 2014                %%   
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

printf("Starting test solver for octavelab\n");

% load the onelab api the old way when it wasn't an octave package
%addpath('/home/alex/TEMF/onelab-python-demo/octavelab_cpp/src/')
%ol_autoload;

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

function exportMsh(le1, le2)
	fid = fopen ([pwd '/' "pend.msh"], 'w');
	if (!fid)
		error("failed to open file");
		exit;
	endif
	fprintf(fid, '$MeshFormat\n2.2 0 8\n$EndMeshFormat\n');
	fprintf(fid, '$Nodes\n3\n1 0 0 0\n2 0 %f 0\n3 0 %f 0\n$EndNodes\n', -le1, -le1-le2);
	fprintf(fid, '$Elements\n3\n1 1 2 0 1 1 2\n2 1 2 0 1 2 3\n3 15 2 0 2 3\n$EndElements\n');
	fclose(fid);
endfunction

function exportMshOpt()
	fid = fopen ([pwd '/' "pend.msh.opt"], 'w');
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

function exportIter(iter, t, x1, y1, x2, y2)
	fid = fopen ([pwd '/' "pend.msh"], 'a');
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

l=1.0;
ol_setParameter("Geom/Length", "number", l, "Arm length[m]", "Length of the arm");
time = 0.0;
ol_setParameter("Geom/time", "number", time, "time [s]", "elapsed time");
dt = 0.001;
ol_setParameter("Geom/dt", "number", dt, "time step [s]", "time step size");
tmax = 20;
ol_setParameter("Geom/tmax", "number", tmax, "max time [s]", "Maximum time");
refresh = 0.05;
ol_setParameter("Geom/refresh", "number", refresh, "refresh interval [s]", "");
theta0 = 10;
ol_setParameter("Geom/theta", "number", theta0, "Initial theta angle[deg]", "");
phi0 = 180;
ol_setParameter("Geom/phi", "number", phi0, "Initial Phi angle [deg]", "");

printf("+++\n");
test = ol_toString
printf("+++\n");
% we're done if we are in the "check" phase
if (!strcmp(ol_getParameters([name "/Action"]).value, "compute"))
	printf("Nothing to do");
	ol_disconnect();
	exit;
endif

l1 = l;
l2 = l;
m1 = m;
m2 = m;
theta = theta0 / 180 * pi;
phi = phi0 / 180 * pi;
theta_dot = 0.0;
phi_dot = 0.0;
refr = 0.0;
iter = 0;
time = 0.0;

while (time < tmax)
	delta = phi - theta;
	sdelta = sin(delta);
	cdelta = cos(delta);
	theta_dot_dot = ( m2*l1*(theta_dot**2.0)*sdelta*cdelta ...
										+ m2*g*sin(phi)*cdelta ...
										+ m2*l2*(phi_dot**2.0)*sdelta ...
										- (m1+m2)*g*sin(theta) );
	theta_dot_dot /= ( (m1+m2)*l1 - m2*l1*(cdelta)**2.0 );

	phi_dot_dot = ( -m2*l2*(phi_dot**2.0)*sdelta*cdelta ...
									 + (m1+m2)*(g*sin(theta)*cdelta ...
															- l1*(theta_dot**2.0)*sdelta ...
															- g*sin(phi)) );
	phi_dot_dot /= ( (m1+m2)*l2 - m2*l2*(cdelta)**2.0 );

	theta_dot = theta_dot + theta_dot_dot*dt;
	phi_dot = phi_dot + phi_dot_dot*dt;

	theta = theta + theta_dot*dt;
	phi = phi + phi_dot*dt;

	x1 =  l1*sin(theta);
	y1 = -l1*cos(theta);
	x2 =  l1*sin(theta) + l2*sin(phi);
	y2 = -l1*cos(theta) - l2*cos(phi);

	time += dt;
	refr += dt;

	exportMshOpt();

	if (refr >= refresh)
		refr = 0;
		ol_setParameter([name '/Progress'], "number", time, "Progress", "");
		ol_setParameter('Dyna/time', "number", time, "time", "");
		ol_setParameter('Solu/phi', "number", phi, "phi", "");
		% not implemented yet
		%ol_addNumberChoice('Solu/phi', phi)
		ol_setParameter('Solu/theta', "number", theta, "theta", "");
		%not implemented yet
		%ol_addNumberChoice('Solu/theta', theta)
		ol_setParameter('Solu/phi_dot', "number", phi_dot, "phi_dot", "");
		% not yet implemented
		%ol_addNumberChoice('Solu/phi_dot', phi_dot)
		ol_setParameter('Solu/theta_dot', "number", theta_dot, "theta_dot", "");
		%not yet implemented :(
		%ol_addNumberChoice('Solu/theta_dot', theta_dot)

		% ask Gmsh to refresh
		ol_setParameter('Gmsh/Action', "string", 'refresh', "action", "");

		% stop if we are asked to (by Gmsh)
		if (strcmp(ol_getParameters([name "/Action"]).value, "stop"))
			break;
		endif

		exportMsh(l1, l2);
		exportIter(iter, time, x1, y1+l1, x2, y2+l1+l2);
		ol_sendMergeFileRequest([pwd '/' 'pend.msh']);
		iter += 1;
	endif
endwhile
ol_disconnect();
