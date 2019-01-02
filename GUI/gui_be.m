classdef gui_be < handle
    %GUI back end properties and methods. Connects to GUI front end through
    %app designer 
    
    properties
        
        comNum = 0;
        baudRate = 0;
        ser;
        
    end
    
    methods
        
        % initialize a serial connection to COM port
        % defined by the user in FE.
        % OUTPUT: Returns "true" if connected and "false" if no connection
        function status = connectToComPort( obj, com, baud )
          
            obj.comNum = com;       % update class properties
            obj.baudRate = baud;    % update class properties
            
            % configure UART module for communication to processor
            temp = sprintf( 'COM%d', com ); % concatenate COM and com port number ( exp: 'COM' + 6 = 'COM6' )
            obj.ser = serial(temp);         % user defined COM port number from device manager
            
            % configure the serial port
            obj.ser.BaudRate = baud;        % user defined baud rate, default = 9600
            obj.ser.Parity = 'odd';         % odd parity bit - REMOVE LATER
            obj.ser.DataBits = 8;           % 8 data bits per message
            obj.ser.OutputBufferSize = 10;  % 1 byte for processor message, 1 byte frame #, 8 bytes for pattern data
            
            % connect to serial port
            try
                fopen(obj.ser);             % open COM port communication
                status = true;              % returns true if there is a COM port connection
            catch
                status = false;             % returns false if unable to connect to COM port
            end
        end
        
    end
    
end

