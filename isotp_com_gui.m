function isotp_com_gui()
    clc; close all;
    disp('--- ISO-TP COM ---');

    % Create window
    fig = uifigure('Name', 'ISO-TP COM', 'Position', [100, 100, 400, 500]);

    platform = getPlatform();
    switch platform
        case 'linux'
            libraryPath = fullfile(fileparts(mfilename('fullpath')), 'linux');
            addpath(libraryPath);
            disp('Using Linux socketCAN.');
            availableInterfaces = detectAvailablePCANInterfaces_Linux();
        case 'windows'
            libraryPath = fullfile(fileparts(mfilename('fullpath')), 'windows');
            addpath(libraryPath);
            disp('Using Windows PCAN.');
            availableInterfaces = detectAvailablePCANInterfaces_Windows();
        otherwise
            error('Unsupported platform');
    end
    if isempty(availableInterfaces)
        availableInterfaces = {'N/A'};
    end
    mainLayout = uigridlayout(fig, [3, 1]);
    mainLayout.RowHeight = {'fit', 'fit', '1x'};
    mainLayout.Padding = [10 10 10 10];
    mainLayout.RowSpacing = 10;
    staticLayout = uigridlayout(mainLayout, [1, 5]);
    staticLayout.Layout.Row = 1;
    staticLayout.Layout.Column = 1;
    staticLayout.ColumnWidth = {'1x', 'fit', '1x', '1x', '1x'};
    staticLayout.RowHeight = {'fit'};
    staticLayout.ColumnSpacing = 10;

    fig.UserData.rxIdHex = '18DAF101';  % MCU -> PC
    fig.UserData.txIdHex = '18DA01F1';  % PC -> MCU

    fig.UserData.isClosing = false;     % Initialize the closing flag
    fig.CloseRequestFcn = @(~,~) cleanupOnExit(platform, fig);
    % --- Row 1: Static User Controls --- %
    dd = uidropdown(staticLayout, ...
        'Items', availableInterfaces, ...
        'Value', availableInterfaces{1}, ...
        'ValueChangedFcn', @(src,event) setInterface(fig, src.Value));
    fig.UserData.canInterface = dd.Value;
    refreshBtn = uibutton(staticLayout, 'Text', '🔄', ...
        'ButtonPushedFcn', @(btn,~) refreshCANInterfaces(fig, dd, platform));
    fig.UserData.canInterface = dd.Value;
    uibutton(staticLayout, 'Text', 'Load Data File', ...
        'ButtonPushedFcn', @(~,~) loadDataFile(fig));
    fig.UserData.isPolling = false;
    fig.UserData.toggleButton = uibutton(staticLayout, 'Text', 'Start', ...
        'ButtonPushedFcn', @(btn,~) togglePolling(platform, fig, btn));
    repDD = uidropdown(staticLayout, ...
    'Items', {'Decimal', 'Hexadecimal', 'Binary'}, ...
    'Value', 'Decimal', ...  % Default
    'ValueChangedFcn', @(src,~) setDisplayMode(fig, src.Value));
    % --- Row 2: Message Display --- %
    messageLabel = uilabel(mainLayout, 'Text', 'User Message');
    messageLabel.Layout.Row = 2;
    messageLabel.Layout.Column = 1;
    fig.UserData.messageLabel = messageLabel;
    % --- Row 3: Data ID Panel --- %
    dynamicPanel = uipanel(mainLayout, 'Title', 'Data IDs');
    dynamicPanel.Layout.Row = 3;
    dynamicPanel.Layout.Column = 1;

    if strcmp(availableInterfaces,'N/A')
        
        userMessage(fig, 'No CAN devices found.');
    else
        userMessage(fig, ['Interfaces detected: ' strjoin(availableInterfaces, ', ')]);
        disp(['Interfaces detected: ' strjoin(availableInterfaces, ', ')]);
    end
    fig.UserData.displayMode = 'Decimal';
    fig.UserData.dynamicPanel = dynamicPanel;
end

function setInterface(fig, selectedInterface)
    fig.UserData.canInterface = selectedInterface;
    disp(['CAN interface set to: ' selectedInterface]);
    userMessage(fig, sprintf('CAN interface set to: %s', selectedInterface));
    drawnow;
end


function loadDataFile(fig)
    [file, path] = uigetfile('*.txt', 'Select Data ID List File');
    if file == 0
        return; % User canceled
    end

    if isfield(fig.UserData, 'isPolling') && fig.UserData.isPolling
        stopPolling(fig);
    end

    dataFile = fullfile(path, file);
    fig.UserData.dataFile = dataFile;
    userMessage(fig, ['Loaded file: ', file]);
    disp(['Loaded file: ', file]);
    if isfield(fig.UserData, 'dataFields') && ~isempty(fig.UserData.dataFields)
        for i = 1:numel(fig.UserData.dataFields)
            if isgraphics(fig.UserData.dataFields{i})
                delete(fig.UserData.dataFields{i});
            end
        end
    end
    if isfield(fig.UserData, 'dataLabels') && ~isempty(fig.UserData.dataLabels)
        for i = 1:numel(fig.UserData.dataLabels)
            if isgraphics(fig.UserData.dataLabels{i})
                delete(fig.UserData.dataLabels{i});
            end
        end
    end

    fig.UserData.dataFields = {};
    fig.UserData.dataLabels = {};

    dataIDBundles = loadDataIDList(dataFile);
    fig.UserData.dataIDBundles = dataIDBundles;
    numIDs = numel(dataIDBundles);

    dLayout = uigridlayout(fig.UserData.dynamicPanel, [numIDs, 2]);
    dLayout.Padding = [10 10 10 10];
    dLayout.RowSpacing = 5;
    dLayout.ColumnSpacing = 10;
    dLayout.ColumnWidth = {'1x', '1x'};
    dLayout.RowHeight = repmat({'fit'}, 1, numIDs);

    for k = 1:numIDs
        label = uilabel(dLayout, ...
            'Text', dataIDBundles(k).label, ...
            'FontName', 'FixedWidth');
    
        field = uieditfield(dLayout, 'text', ...
            'Editable', 'off', ...
            'FontName', 'FixedWidth');

        fig.UserData.dataLabels{k} = label;
        fig.UserData.dataFields{k} = field;
    end
end

function togglePolling(platform, fig, btn)
    if fig.UserData.isPolling
        stopPolling(fig);
    else
        if ~isfield(fig.UserData, 'dataIDBundles') || isempty(fig.UserData.dataIDBundles)
            disp('No data IDs loaded. Please load a data file first.');
            userMessage(fig, 'Cannot start polling. No data list selected.');
            btn.Text = 'Start';
            return;
        end

        if ~checkCANInterface(platform, fig)
            disp('CAN interface is not available. Ensure it is up and running.');
            btn.Text = 'Start';
            return;
        end

        btn.Text = 'Stop';  
        drawnow;  % Ensure UI updates before entering polling loop
        pollingLoop(platform, fig);
    end
end
%% Polling loop function
function pollingLoop(platform, fig)
    if ~isfield(fig.UserData, 'dataIDBundles') || isempty(fig.UserData.dataIDBundles)
        disp('No data IDs loaded. Please load a data file first.');
        return;
    end

    if ~checkCANInterface(platform, fig)
        disp('Error: CAN interface not available.');
        return;
    end

    fig.UserData.isPolling = true;
    disp('Polling started.');
    userMessage(fig, 'Polling started.');
    % Global timeout settings
    globalTimeout = 5;
    lastResponseTime = tic;

    try
        while fig.UserData.isPolling
            if ~isvalid(fig)
                disp('GUI closed during polling. Stopping.');
                fig.UserData.isPolling = false; % Stop polling
                return;
            end

            for k = 1:numel(fig.UserData.dataIDBundles)
                dataID   = fig.UserData.dataIDBundles(k).id;
                typeStr  = fig.UserData.dataIDBundles(k).type;
                if toc(lastResponseTime) > globalTimeout
                    disp('Request timeout. Stopping polling.');
                    userMessage(fig, 'Request timeout. Stopping polling.');
                    stopPolling(fig)
                    return;
                end
                val = requestCANData(platform, fig, dataID, typeStr);
                if ~isnan(val)
                    lastResponseTime = tic;
                    if isfield(fig.UserData, 'displayMode')
                        dispMode = fig.UserData.displayMode;
                    else
                        dispMode = 'Decimal'; % default if not set
                    end

                end
                % Check if the figure and field are still valid before updating
                if isvalid(fig) && isgraphics(fig.UserData.dataFields{k})
                    formatted_val = formatValue(val, dispMode);
                    fig.UserData.dataFields{k}.Value = num2str(formatted_val);
                end
            end
            pause(0.1); % Faster polling rate
        end
    catch ME
        % Handle errors gracefully
        if strcmp(ME.identifier, 'MATLAB:class:InvalidHandle')
            disp('Polling loop stopped due to GUI closure.');
        else
            rethrow(ME);
        end
    end
end

function stopPolling(fig)
    if isfield(fig.UserData, 'isPolling') && fig.UserData.isPolling
        fig.UserData.isPolling = false;
        fig.UserData.toggleButton.Text = 'Start';
        disp('Polling stopped.');
        userMessage(fig, 'Polling stopped.');
    end
end

function dataBundles = loadDataIDList(fileName)
    dataBundles = [];
    if ~isfile(fileName), return; end
    fid = fopen(fileName, 'r');
    if fid < 0, return; end

    idx = 1;
    while ~feof(fid)
        line = fgetl(fid);
        if ~ischar(line), break; end
        parts = strsplit(strtrim(line), ',');
        if numel(parts) < 3, continue; end

        labelStr = strtrim(parts{1});
        idVal    = str2double(parts{2});
        typeStr  = strtrim(parts{3});

        if isnan(idVal), continue; end

        dataBundles(idx).label = labelStr;
        dataBundles(idx).id    = idVal;
        dataBundles(idx).type  = typeStr;
        idx = idx + 1;
    end
    fclose(fid);
end

function val = requestCANData(platform, fig, dataID, typeStr)
    if ~checkCANInterface(platform, fig)
        val = NaN;
        return;
    end

    % Build UDS ReadDataByIdentifier request: [0x22 hiDID loDID]
    hiDID = bitshift(dataID, -8);
    loDID = bitand(dataID, 255);
    requestVec = uint8([0x22, hiDID, loDID]);

    try
        resp = sendReceiveCAN(platform, fig.UserData.canInterface, fig.UserData.rxIdHex, fig.UserData.txIdHex, requestVec);

        if isempty(resp)
            fprintf('  -> Timeout for ID %d\n', dataID);
            val = NaN;
            return;
        end

        if resp(1) == hex2dec('62')
            payload = resp(4:end);
            val = parsePayload(payload, typeStr);
        else
            fprintf('  -> Unexpected UDS response for ID %d: ', dataID);
            disp(resp);
            val = NaN;
        end
    catch ME
        fprintf('  -> Error during CAN request: %s\n', ME.message);
        val = NaN;
    end
end

function parsedVal = parsePayload(payload, typeStr)
    parsedVal = NaN; % Default if parsing fails

    if numel(payload) >= 8 && contains(typeStr, 'float64')
        doubleBytes = flip(payload(1:8));  
        parsedVal = typecast(uint8(doubleBytes), 'double');

    elseif numel(payload) >= 4 && contains(typeStr, 'single')
        singleBytes = flip(payload(1:4));
        parsedVal = typecast(uint8(singleBytes), 'single');

    elseif numel(payload) >= 4 && contains(typeStr, 'uint32')
        uintBytes = flip(payload(1:4));
        parsedVal = double(typecast(uint8(uintBytes), 'uint32')); % Ensure decimal

    elseif numel(payload) >= 4 && contains(typeStr, 'int32')
        intBytes = flip(payload(1:4));
        parsedVal = double(typecast(uint8(intBytes), 'int32')); % Ensure decimal

    elseif numel(payload) >= 2 && contains(typeStr, 'uint16')
        uint16Bytes = flip(payload(1:2));
        parsedVal = double(typecast(uint8(uint16Bytes), 'uint16')); % Ensure decimal

    elseif numel(payload) >= 2 && contains(typeStr, 'int16')
        int16Bytes = flip(payload(1:2));
        parsedVal = double(typecast(uint8(int16Bytes), 'int16')); % Ensure decimal

    elseif numel(payload) >= 1 && contains(typeStr, 'uint8')
        parsedVal = double(payload(1));  % read the first byte, convert to double for display

    elseif numel(payload) >= 1 && contains(typeStr, 'int8')
        parsedVal = double(typecast(uint8(payload(1)), 'int8'));
    else
        fprintf('  -> Unknown data type, raw hex output: [%s]\n', sprintf('%02X ', payload));
        parsedVal = NaN;
    end
end

function isAvailable = checkCANInterface(platform, fig)
    canInterface = fig.UserData.canInterface;

    switch platform
        case 'windows'
            isAvailable = CANInterface_Windows('checkCANInterface', canInterface);
        case 'linux'
            [status, result] = system(['ip link show ', canInterface]);
            isAvailable = (status == 0 && contains(result, 'state UP'));
        otherwise
            isAvailable = false;
    end

    if ~isAvailable
        disp('Error: CAN interface is down or not found. Please check connection.');
        userMessage(fig, 'Error: CAN interface is down or not found. Please check connection.');
    end
end

function cleanupOnExit(platform, fig)
    disp('Cleaning up resources...');
    if isfield(fig.UserData, 'isPolling') && fig.UserData.isPolling
        stopPolling(fig);
    end

    switch platform
        case 'linux'
            clear isotp_mex_linux;
            system('pkill -f can_isotp');
        case 'windows'
            CANInterface_Windows('cleanup');
    end

    delete(fig);
end

function platform = getPlatform()
    if ispc
        platform = 'windows';
    elseif isunix
        platform = 'linux';
    else
        error('Unsupported operating system');
    end
end

function response = sendReceiveCAN(platform, canIf, rxHex, txHex, requestVec)
    switch platform
        case 'windows'
            response = CANInterface_Windows('tranceiveCAN', canIf, rxHex, txHex, requestVec);
        case 'linux'
            response = isotp_mex_linux(canIf, rxHex, txHex, requestVec);
    end
end

function availableInterfaces = detectAvailablePCANInterfaces_Windows()
    availableInterfaces = CANInterface_Windows('detect');
end

function availableInterfaces = detectAvailablePCANInterfaces_Linux()
    availableInterfaces = {};
    disp('Detecting CAN interfaces.')
    [status, output] = system('ip -o link show | awk -F": " ''{print $2}''');
    if status == 0
        lines = strsplit(strtrim(output), '\n');
        for i = 1:length(lines)
            if startsWith(lines{i}, 'can')
                availableInterfaces{end+1} = lines{i}; %#ok<AGROW>
            end
        end
    end
end

%% Function to display user messages in UI
function userMessage(fig, newMessage)
    fig.UserData.userMsg = newMessage;
    if isfield(fig.UserData, 'messageLabel')
        if isgraphics(fig.UserData.messageLabel)
            fig.UserData.messageLabel.Text = newMessage;
        else
            disp('Warning: The stored messageLabel is not a valid graphics object.');
        end
    else
        disp('Warning: No messageLabel handle found in fig.UserData.');
    end
end

%% Function to refresh can interfaces
function refreshCANInterfaces(fig, dd, platform)
    if isfield(fig.UserData, 'isPolling') && fig.UserData.isPolling
        stopPolling(fig);
    end
    switch platform
        case 'linux'
            availableInterfaces = detectAvailablePCANInterfaces_Linux();
        case 'windows'
            availableInterfaces = detectAvailablePCANInterfaces_Windows();
        otherwise
            error('Unsupported platform');
    end

    if isempty(availableInterfaces)
        availableInterfaces = {'No CAN device detected.'};
    end

    dd.Items = availableInterfaces;

    currentValue = dd.Value;
    idx = find(strcmp(availableInterfaces, currentValue));

    if isempty(idx)
        idx = 1;
    else
        idx = mod(idx, numel(availableInterfaces)) + 1;
    end

    dd.Value = availableInterfaces{idx};
    fig.UserData.canInterface = dd.Value;
    newMsg = ['Interfaces detected: ' strjoin(availableInterfaces, ', ') '. Selected: ' dd.Value];
    userMessage(fig, newMsg);
    disp(newMsg);
end
%% Function to set displaymode
function setDisplayMode(fig, newMode)
    fig.UserData.displayMode = newMode;
    disp(['Display mode set to: ' newMode]);
    userMessage(fig, ['Display mode set to: ' newMode]);
end
%% Function to format numerical repsentation
function strVal = formatValue(val, displayMode)
    if isnan(val), strVal = 'NaN'; return; end
    switch displayMode
        case 'Decimal'
            strVal = num2str(val);
        case 'Hexadecimal'
            % assume val is integral for simplicity
            strVal = sprintf('0x%02X', round(val));
        case 'Binary'
            % assume val is integral
            strVal = ['0b' dec2bin(round(val),8)];
        otherwise
            strVal = num2str(val);
    end
end