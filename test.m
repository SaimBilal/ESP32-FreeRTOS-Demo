% MATLAB TCP Client for ESP32-S3 Temperature Streaming
% Ensure the ESP32-S3 is running in AP mode with SSID "ESP32-S3_AP" and password "password123".

% ESP32-S3 AP IP address and port
esp32_ip = '192.168.4.1'; % Default AP IP address
esp32_port = 3333;        % TCP server port

% Create a TCP/IP object
try
    tcp_client = tcpclient(esp32_ip, esp32_port);
    fprintf('Connected to ESP32-S3 at %s:%d\n', esp32_ip, esp32_port);
catch ME
    error('Failed to connect to ESP32-S3: %s', ME.message);
end

% Configure the client
tcp_client.Timeout = 10; % Timeout in seconds
tcp_client.InputBufferSize = 4096; % Buffer size for receiving data

% Receive and process temperature data
try
    while true
        % Check if data is available
        if tcp_client.BytesAvailable > 0
            % Read 1024 floats (4 KB) of data
            data = read(tcp_client, 512, 'single'); % 'single' for float (32-bit)

            % Convert fixed-point data to temperature values
            temperatures = double(data); % No scaling needed (already in float)

            % Display the received temperatures
            fprintf('Received temperatures:\n');
            disp(temperatures');

            % Plot the temperatures (optional)
            plot(temperatures);
            xlabel('Sample Index');
            ylabel('Temperature (°C)');
            title('Temperature Data from ESP32-S3');
            drawnow;
        else
            pause(0.1); % Wait for more data
        end
    end
catch ME
    % Handle errors (e.g., disconnection)
    fprintf('Error: %s\n', ME.message);
end

% Clean up
clear tcp_client;
fprintf('TCP client disconnected.\n');