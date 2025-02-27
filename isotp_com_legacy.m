function isotp_com_legacy()
    clc; close all;
    disp('--- ISO-TP COM (Legacy UI) ---');

    % Create a classical figure window (no uifigure).
    % Pass src into cleanupOnExit via an anonymous function.
    fig = figure('Name', 'ISO-TP COM (Legacy)', ...
                 'NumberTitle', 'off', ...
                 'Position', [100, 100, 600, 500], ...
                 'MenuBar', 'none', ...
                 'ToolBar', 'none', ...
                 'CloseRequestFcn', @(src,evt)cleanupOnExit(src));
    
    % Determine OS platform, load libraries
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
    
    % Store some default user data
    fig.UserData.rxIdHex      = '18DAF101';  % MCU -> PC
    fig.UserData.txIdHex      = '18DA01F1';  % PC -> MCU
    fig.UserData.isClosing    = false;
    fig.UserData.isPolling    = false;
    fig.UserData.displayMode  = 'Decimal'; % default
    fig.UserData.canInterface = availableInterfaces{1}; 
    disp(['Interfaces detected: ' strjoin(availableInterfaces, ', ')]);
    
    %--------------------- TOP CONTROLS LAYOUT ---------------------%
    % We'll create a row of controls (manual positioning).
    % y-level for top row
    topRowY = 460;       % slightly lower than figure top (500) so the X button is accessible
    rowHeight = 25;
    
    % Start with a label for "CAN IF:"
    labelWidth  = 50;
    controlWidth = 120;
    spacing = 10;
    
    % Positions will be incrementally increased as we add each control
    xPos = 10;  % Start at 10 px from left edge

    % 1) Label "CAN IF:"
    uicontrol('Parent', fig, ...
        'Style', 'text', ...
        'String', 'CAN IF:', ...
        'Position', [xPos, topRowY, labelWidth, rowHeight], ...
        'HorizontalAlignment', 'right');
    xPos = xPos + labelWidth + spacing;

    % 2) CAN Interface pop-up
    fig.UserData.dd = uicontrol('Parent', fig, ...
        'Style', 'popupmenu', ...
        'String', availableInterfaces, ...
        'Value', 1, ... % default to first
        'Position', [xPos, topRowY, controlWidth, rowHeight], ...
        'Callback', @(src, evt)setInterface(fig, src));
    xPos = xPos + controlWidth + spacing;

    % 3) Refresh button
    refreshButtonWidth = 40;
    uicontrol('Parent', fig, ...
        'Style', 'pushbutton', ...
        'String', '🔄', ...
        'Position', [xPos, topRowY, refreshButtonWidth, rowHeight], ...
        'Callback', @(btn,evt)refreshCANInterfaces(fig, platform));
    xPos = xPos + refreshButtonWidth + spacing;

    % 4) Load Data button
    loadBtnWidth = 80;
    uicontrol('Parent', fig, ...
        'Style', 'pushbutton', ...
        'String', 'Load Data', ...
        'Position', [xPos, topRowY, loadBtnWidth, rowHeight], ...
        'Callback', @(btn,evt)loadDataFile(fig));
    xPos = xPos + loadBtnWidth + spacing;

    % 5) Start/Stop
    toggleWidth = 60;
    fig.UserData.toggleButton = uicontrol('Parent', fig, ...
        'Style', 'pushbutton', ...
        'String', 'Start', ...
        'Position', [xPos, topRowY, toggleWidth, rowHeight], ...
        'Callback', @(btn,evt)togglePolling(platform, fig, btn));
    xPos = xPos + toggleWidth + spacing;

    % 6) Display Mode (Decimal/Hex/Bin)
    modeWidth = 100;
    fig.UserData.repDD = uicontrol('Parent', fig, ...
        'Style', 'popupmenu', ...
        'String', {'Decimal','Hexadecimal','Binary'}, ...
        'Value', 1, ...
        'Position', [xPos, topRowY, modeWidth, rowHeight], ...
        'Callback', @(src,evt)setDisplayMode(fig, src));
    
    %--------------------- MESSAGE DISPLAY ---------------------%
    % We'll place a static text below for user messages.
    fig.UserData.messageText = uicontrol('Parent', fig, ...
        'Style', 'text', ...
        'String', 'User Message', ...
        'Position', [10, 420, 580, 30], ...
        'HorizontalAlignment', 'left');

    %--------------------- DYNAMIC PANEL ---------------------%
    % We'll place a panel at the bottom for data ID fields.
    fig.UserData.dynamicPanel = uipanel('Parent', fig, ...
        'Title', 'Data IDs', ...
        'Units', 'pixels', ...
        'Position', [10, 10, 580, 400]);  % from y=10 up to y=410
    
    % If no device found, show a warning. Otherwise show success message.
    if strcmp(availableInterfaces{1}, 'N/A')
        userMessage(fig, 'No CAN devices found.');
    else
        userMessage(fig, ['Interfaces detected: ' strjoin(availableInterfaces, ', ')]);
    end
end

%========================================================
%    CALLBACKS AND FUNCTIONS
%========================================================

function setInterface(fig, src)
    list = get(src, 'String');
    idx  = get(src, 'Value');
    selectedIF = list{idx};
    
    fig.UserData.canInterface = selectedIF;
    disp(['CAN interface set to: ' selectedIF]);
    userMessage(fig, sprintf('CAN interface set to: %s', selectedIF));
end

function setDisplayMode(fig, src)
    modes = get(src, 'String');
    idx   = get(src, 'Value');
    newMode = modes{idx};
    fig.UserData.displayMode = newMode;
    disp(['Display mode set to: ' newMode]);
    userMessage(fig, ['Display mode set to: ' newMode]);
end

function refreshCANInterfaces(fig, platform)
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

    set(fig.UserData.dd, 'String', availableInterfaces, 'Value', 1);
    selectedIF = availableInterfaces{1};
    fig.UserData.canInterface = selectedIF;
    
    newMsg = ['Interfaces detected: ' strjoin(availableInterfaces, ', ') '. Selected: ' selectedIF];
    userMessage(fig, newMsg);
    disp(newMsg);
end

function loadDataFile(fig)
    [file, path] = uigetfile('*.txt', 'Select Data ID List File');
    if isequal(file, 0)
        return; % user canceled
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

    % We'll place rows of label/edit in dynamicPanel
    panelPos = get(fig.UserData.dynamicPanel, 'Position');  
    rowHeight = 25;
    spacing   = 5;
    labelWidth = 150;
    fieldWidth = 120;
    % Subtract panel's title bar (~20 px)
    titleBarOffset = 20;

    startY  = panelPos(4) - rowHeight - spacing - titleBarOffset;
    for k = 1:numIDs
        yPos = startY - (k-1)*(rowHeight + spacing);

        fig.UserData.dataLabels{k} = uicontrol('Parent', fig.UserData.dynamicPanel, ...
            'Style', 'text', ...
            'String', dataIDBundles(k).label, ...
            'FontName','FixedWidth', ...
            'Units','pixels', ...
            'Position', [10, yPos, labelWidth, rowHeight], ...
            'HorizontalAlignment','left');

        fig.UserData.dataFields{k} = uicontrol('Parent', fig.UserData.dynamicPanel, ...
            'Style','edit', ...
            'String','', ...
            'FontName','FixedWidth', ...
            'Enable','inactive', ...
            'Units','pixels', ...
            'Position',[20+labelWidth, yPos, fieldWidth, rowHeight]);
    end
end

function togglePolling(platform, fig, btn)
    if fig.UserData.isPolling
        stopPolling(fig);
    else
        if ~isfield(fig.UserData, 'dataIDBundles') || isempty(fig.UserData.dataIDBundles)
            disp('No data IDs loaded. Please load a data file first.');
            userMessage(fig, 'Cannot start polling. No data list selected.');
            set(btn,'String','Start');
            return;
        end

        if ~checkCANInterface(platform, fig)
            disp('CAN interface not available.');
            set(btn,'String','Start');
            return;
        end

        set(btn, 'String','Stop');  
        drawnow;  
        pollingLoop(platform, fig);
    end
end

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
    
    globalTimeout = 5;
    lastResponseTime = tic;

    try
        while fig.UserData.isPolling
            if ~isvalid(fig)
                disp('Figure closed. Stopping.');
                fig.UserData.isPolling = false;
                return;
            end

            for k = 1:numel(fig.UserData.dataIDBundles)
                dataID  = fig.UserData.dataIDBundles(k).id;
                typeStr = fig.UserData.dataIDBundles(k).type;
                
                if toc(lastResponseTime) > globalTimeout
                    disp('Request timeout. Stopping polling.');
                    userMessage(fig, 'Request timeout. Stopping polling.');
                    stopPolling(fig);
                    return;
                end
                val = requestCANData(platform, fig, dataID, typeStr);
                if ~isnan(val)
                    lastResponseTime = tic;
                end
                if isfield(fig.UserData,'displayMode')
                    dispMode = fig.UserData.displayMode;
                else
                    dispMode = 'Decimal';
                end
                strVal = formatValue(val, dispMode);

                if isvalid(fig) && k <= numel(fig.UserData.dataFields) ...
                        && isgraphics(fig.UserData.dataFields{k})
                    set(fig.UserData.dataFields{k}, 'String', strVal);
                end
            end
            pause(0.1);
        end
    catch ME
        if strcmp(ME.identifier, 'MATLAB:class:InvalidHandle')
            disp('Polling loop stopped: GUI closed.');
        else
            rethrow(ME);
        end
    end
end

function stopPolling(fig)
    if isfield(fig.UserData, 'isPolling') && fig.UserData.isPolling
        fig.UserData.isPolling = false;
        set(fig.UserData.toggleButton, 'String','Start');
        disp('Polling stopped.');
        userMessage(fig, 'Polling stopped.');
    end
end

function userMessage(fig, newMessage)
    if isfield(fig.UserData, 'messageText') && isgraphics(fig.UserData.messageText)
        set(fig.UserData.messageText, 'String', newMessage);
    end
end

function cleanupOnExit(figHandle)
    disp('Cleaning up resources...');
    if isfield(figHandle.UserData, 'isPolling') && figHandle.UserData.isPolling
        stopPolling(figHandle);
    end

    platform = getPlatform();
    switch platform
        case 'linux'
            clear isotp_mex_linux;
            system('pkill -f can_isotp');
        case 'windows'
            CANInterface_Windows('cleanup');
    end

    delete(figHandle);
end

% =========== HELPER FUNCTIONS (unchanged) ===================

function platform = getPlatform()
    if ispc
        platform = 'windows';
    elseif isunix
        platform = 'linux';
    else
        error('Unsupported operating system');
    end
end

function availableInterfaces = detectAvailablePCANInterfaces_Windows()
    availableInterfaces = CANInterface_Windows('detect');
end

function availableInterfaces = detectAvailablePCANInterfaces_Linux()
    availableInterfaces = {};
    disp('Detecting CAN interfaces.');
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
    hiDID = bitshift(dataID, -8);
    loDID = bitand(dataID, 255);
    requestVec = uint8([0x22, hiDID, loDID]);

    try
        resp = sendReceiveCAN(platform, fig.UserData.canInterface, ...
                              fig.UserData.rxIdHex, fig.UserData.txIdHex, requestVec);
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
        parsedVal = double(typecast(uint8(uintBytes), 'uint32'));
    elseif numel(payload) >= 4 && contains(typeStr, 'int32')
        intBytes = flip(payload(1:4));
        parsedVal = double(typecast(uint8(intBytes), 'int32'));
    elseif numel(payload) >= 2 && contains(typeStr, 'uint16')
        uint16Bytes = flip(payload(1:2));
        parsedVal = double(typecast(uint8(uint16Bytes), 'uint16'));
    elseif numel(payload) >= 2 && contains(typeStr, 'int16')
        int16Bytes = flip(payload(1:2));
        parsedVal = double(typecast(uint8(int16Bytes), 'int16'));
    elseif numel(payload) >= 1 && contains(typeStr, 'uint8')
        parsedVal = double(payload(1));
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

function response = sendReceiveCAN(platform, canIf, rxHex, txHex, requestVec)
    switch platform
        case 'windows'
            response = CANInterface_Windows('tranceiveCAN', canIf, rxHex, txHex, requestVec);
        case 'linux'
            response = isotp_mex_linux(canIf, rxHex, txHex, requestVec);
        otherwise
            response = [];
    end
end

function strVal = formatValue(val, displayMode)
    if isnan(val), strVal = 'NaN'; return; end
    switch displayMode
        case 'Decimal'
            strVal = num2str(val);
        case 'Hexadecimal'
            strVal = sprintf('0x%02X', round(val));
        case 'Binary'
            strVal = ['0b' dec2bin(round(val),8)];
        otherwise
            strVal = num2str(val);
    end
end
