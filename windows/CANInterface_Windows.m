function varargout = CANInterface_Windows(func, varargin)
    % CANInterface_Windows - Consolidated Windows CAN interface functions.
    % Usage:
    %   response = CANInterface_Windows('tranceiveCAN', canIf, rxHex, txHex, requestVec);
    %   isAvailable = CANInterface_Windows('checkCANInterface', canInterface);
    %   availableInterfaces = CANInterface_Windows('detect');
    %   CANInterface_Windows('cleanup');
    
    switch func
        case 'tranceiveCAN'
            % Now uses ISO-TP for automatic segmentation and reassembly.
            varargout{1} = tranceiveCAN_ISOTP_windows(varargin{:});
        case 'checkCANInterface'
            varargout{1} = checkCANInterface_windows(varargin{:});
        case 'cleanup'
            cleanup_windows();
        case 'detect'
            varargout{1} = detectCANInterface_windows();
        otherwise
            error('Unsupported function: %s', func);
    end
end

%% === ISO-TP based send and receive function === %
function response = tranceiveCAN_ISOTP_windows(canIf, rxHex, txHex, requestVec)   
    persistent isISOTPInitialized;
    if isempty(isISOTPInitialized)
        isISOTPInitialized = false;
    end

    if ~libisloaded('PCAN_ISOTP')
        %loadlibrary('PCAN-ISO-TP.dll', 'PCAN_ISOTP.h');
        loadlibrary('PCAN-ISO-TP.dll');
    end

    % Get the channel number from the user-selected interface.
    channel = getPCANChannelFromInterface(canIf);
    PCAN_BAUD_500K = uint16(6);
    
    % If the ISO-TP layer has not been initialized, initialize it.
    if ~isISOTPInitialized
        statusCAN = calllib('PCANBasic', 'CAN_Uninitialize', chanInfo.channel);
        statusISOTP = calllib('PCAN_ISOTP', 'TP_Initialize', channel, PCAN_BAUD_500K, 0, 0, 0);
        if statusCAN && statusISOTP ~= 0
            error('Failed to initialize PCAN ISO-TP interface %s', canIf);
        end
        isISOTPInitialized = true;
    end

    % Convert the transmit and receive hex IDs to numeric values.
    msgID_tx = parseHexID(txHex);
    msgID_rx = parseHexID(rxHex);
    
    % Send the request using the ISO-TP DLL.
    status = calllib('PCAN_ISOTP', 'TP_Send', channel, msgID_tx, requestVec, uint8(length(requestVec)));
    if status ~= 0
        error('Failed to send ISO-TP message on %s', canIf);
    end

    % Prepare a buffer to receive the complete response.
    maxBufferSize = 64;  % Adjust as needed.
    responseBuffer = zeros(1, maxBufferSize, 'uint8');
    lenPtr = libpointer('uint8Ptr', uint8(0));
    
    % Read the reassembled response using the expected receive ID.
    status = calllib('PCAN_ISOTP', 'TP_Read', channel, msgID_rx, responseBuffer, uint32(maxBufferSize), lenPtr);
    if status ~= 0
        error('Failed to read ISO-TP message on %s', canIf);
    end

    responseLength = double(lenPtr.Value);
    response = responseBuffer(1:responseLength);
end

%% Function to detect CAN interfaces
function availableInterfaces = detectCANInterface_windows()
    % Ensure the PCANBasic DLL is loaded.
    if ~libisloaded('PCANBasic')
        %loadlibrary('PCANBasic.dll', 'PCANBasic.h');
        loadlibrary('PCANBasic.dll');
    end

    possibleChannels = {...
        struct('name', 'can0', 'channel', uint16(0x51)), ...
        struct('name', 'can1', 'channel', uint16(0x52)) ...
    };

    availableInterfaces = {};
    PCAN_BAUD_500K = uint16(6);

    for k = 1:length(possibleChannels)
        chanInfo = possibleChannels{k};
        status = calllib('PCANBasic', 'CAN_Initialize', chanInfo.channel, PCAN_BAUD_500K, 0, 0, 0);
        if status == 0
            availableInterfaces{end+1} = chanInfo.name; %#ok<AGROW>
        end
    end
end


%% Function to check CAN interface availability
function isAvailable = checkCANInterface_windows(canInterface)
    % Load the PCANBasic DLL if not already loaded.
    if ~libisloaded('PCANBasic')
        %loadlibrary('PCANBasic.dll', 'PCANBasic.h');
        loadlibrary('PCANBasic.dll');
    end

    % Define constants from the PCAN API.
    PUDS_PARAMETER_CHANNEL_CONDITION = uint32(42); % Replace with the actual constant value.
    PUDS_CHANNEL_UNAVAILABLE = uint8(0);
    PUDS_CHANNEL_AVAILABLE = uint8(1);
    PUDS_CHANNEL_OCCUPIED = uint8(2);

    % Convert the CAN interface name to a channel handle.
    channel = getPCANChannelFromInterface(canInterface);

    % Check the channel condition.
    channelCondition = uint8(0); % Initialize.
    status = calllib('PCANBasic', 'UDS_GetParameter', channel, PUDS_PARAMETER_CHANNEL_CONDITION, channelCondition);

    if status ~= 0
        error('Failed to check CAN interface condition');
    end

    % Determine availability.
    isAvailable = (channelCondition == PUDS_CHANNEL_AVAILABLE);
end

%% Helper function to map CAN interface to PCAN channel
function channel = getPCANChannelFromInterface(canInterface)
    switch canInterface
        case 'can0'
            channel = uint16(0x51); % PCAN_USBBUS1
        case 'can1'
            channel = uint16(0x52); % PCAN_USBBUS2
        otherwise
            error('Unsupported CAN interface: %s', canInterface);
    end
end

%% Helper function to parse hex ID
function val = parseHexID(hexStr)
    val = hex2dec(hexStr);
end

%% Windows-specific cleanup function
function cleanup_windows()
    if libisloaded('PCANBasic')
        calllib('PCANBasic', 'CAN_Uninitialize', chanInfo.channel);
        unloadlibrary('PCANBasic');
    end
    if libisloaded('PCAN_ISOTP')
        calllib('PCAN_ISOTP', 'TP_Uninitialize', channel);
        unloadlibrary('PCAN_ISOTP');
    end
end
