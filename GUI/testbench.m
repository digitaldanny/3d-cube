tb = gui_be();
val = tb.connectToComPort( 6, 9600 );
%delete(instrfind);
tb.comWrite( 1 );