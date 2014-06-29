#!/usr/bin/octave-cli -qf
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Interactive octave promt to interact with onelab                                         %%   
%% Alexander Krimm <alexander_johannes.krimm@stud.tu-darmstadt.de>, TEMF TU-Darmstadt, 2014 %%   
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

printf("Starting interactive interface for octavelab\n");

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

system(["xterm -e \"octave --persist --eval \\\"name=\'" name "\';addr=\'" addr "\';pkg load onelab; ol_client(name, addr)\\\"\""], 0, "async");
