function isotp_com_test()
    clc;
    disp('--- UDS Read Data IDs via isotp_com ---');

    %% (A) Basic config: which CAN interface & 29-bit IDs
    canInterface = 'can0';        
    rxIdHex      = '18DAF101';    
    txIdHex      = '18DA01F1';    

    %% (B) Load Data ID list
    dataListFile = 'data_id_list.txt';
    disp(['Loading Data IDs from file: ', dataListFile]);
    dataIDBundles = loadDataIDList(dataListFile);
    if isempty(dataIDBundles)
        disp('No Data IDs found or file not readable. Exiting...');
        return;
    end
    disp('Data ID list loaded successfully.');

    % Ensure isoTpRequestMex MEX function is available
    if ~exist('isotp_mex_linux','file')
        error('isotp_mex file not found on the MATLAB path. Please compile isotp_mex_OS.cpp.');
    end

    %% (C) Loop over each Data ID
    disp('Starting ReadDataByIdentifier for each Data ID...');
    for k = 1:numel(dataIDBundles)
        dataID   = dataIDBundles(k).id;
        labelStr = dataIDBundles(k).label;
        typeStr  = strtrim(lower(dataIDBundles(k).type));  % Trim spaces & force lowercase
        
        fprintf('  -> DEBUG: Checking typeStr = "%s"\n', typeStr);  % Print for verification

        requestVec = uint8([0x22, bitshift(dataID, -8), bitand(dataID, 255)]);

        fprintf('\nReading "%s" (ID=%d, type=%s)...\n', labelStr, dataID, typeStr);

        %% (1) Send the request & get response
        resp = isotp_mex_linux(canInterface, rxIdHex, txIdHex, requestVec);

        if isempty(resp)
            fprintf('  -> No UDS response (timeout) for ID %d\n', dataID);
            continue;
        end

        %% (2) Parse the UDS response
        if resp(1) == hex2dec('62')
            hiResp = resp(2);
            loResp = resp(3);
            payload = resp(4:end);

            % Debug print of raw payload in hex:
            fprintf('  -> Positive response for 0x%02X%02X. Data bytes: [', hiResp, loResp);
            fprintf('%02X ', payload);
            fprintf(']\n');

            % Ensure the payload is long enough for specific types
            parsedVal = NaN;

            %% **FLOAT64 (double) Processing**
            if numel(payload) >= 8 && contains(typeStr, 'float64')
                fprintf('  -> DEBUG: Processing as double-precision float\n');
                fprintf('  -> Raw payload length: %d\n', numel(payload));
                fprintf('  -> Raw bytes before flip: [%s]\n', sprintf('%02X ', payload(1:8)));

                % Flip for big-endian conversion
                doubleBytes = flip(payload(1:8));  
                fprintf('  -> Bytes after flip: [%s]\n', sprintf('%02X ', doubleBytes));

                try
                    parsedVal = typecast(uint8(doubleBytes), 'double');
                    fprintf('  -> Interpreted double = %.6f %%\n', parsedVal);
                catch ME
                    fprintf('  -> ERROR in typecast (double): %s\n', ME.message);
                end

            %% **FLOAT32 (single) Processing**
            elseif numel(payload) >= 4 && contains(typeStr, 'single')
                fprintf('  -> DEBUG: Processing as single-precision float\n');
                singleBytes = flip(payload(1:4));
                fprintf('  -> Raw bytes before flip: [%s]\n', sprintf('%02X ', payload(1:4)));
                fprintf('  -> Bytes after flip: [%s]\n', sprintf('%02X ', singleBytes));

                try
                    parsedVal = typecast(uint8(singleBytes), 'single');
                    fprintf('  -> Interpreted float32 = %.3f %%\n', parsedVal);
                catch ME
                    fprintf('  -> ERROR in typecast (single): %s\n', ME.message);
                end

            %% **Other Integer Processing**
            elseif numel(payload) >= 4 && contains(typeStr, 'uint32')
                uintBytes = flip(payload(1:4));
                parsedVal = typecast(uint8(uintBytes), 'uint32');
                fprintf('  -> Interpreted uint32 = %d\n', parsedVal);

            elseif numel(payload) >= 4 && contains(typeStr, 'int32')
                intBytes = flip(payload(1:4));
                parsedVal = typecast(uint8(intBytes), 'int32');
                fprintf('  -> Interpreted int32 = %d\n', parsedVal);

            elseif numel(payload) >= 2 && contains(typeStr, 'uint16')
                uint16Bytes = flip(payload(1:2));
                parsedVal = typecast(uint8(uint16Bytes), 'uint16');
                fprintf('  -> Interpreted uint16 = %d\n', parsedVal);

            elseif numel(payload) >= 2 && contains(typeStr, 'int16')
                int16Bytes = flip(payload(1:2));
                parsedVal = typecast(uint8(int16Bytes), 'int16');
                fprintf('  -> Interpreted int16 = %d\n', parsedVal);

            else
                fprintf('  -> Unrecognized type, using raw hex interpretation.\n');
                parsedVal = parsePayloadByType(payload, typeStr);
            end

        elseif resp(1) == hex2dec('7F')
            % Handle negative response
            negativeService = resp(2);
            nrc = resp(3);
            fprintf('  -> Negative response: Service=0x%02X, NRC=0x%02X\n', negativeService, nrc);

        else
            disp('  -> Unknown/Unexpected response frame:');
            disp(resp);
        end
    end

    disp('Finished reading Data IDs via isoTpRequestMex.');
end
%% -----------------------
%% Helper: loadDataIDList
function dataBundles = loadDataIDList(fileName)
    dataBundles = [];
    if ~isfile(fileName)
        return;
    end
    
    fid = fopen(fileName, 'r');
    if fid < 0
        return;
    end
    
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
        
        dataBundles(idx).label = labelStr; %#ok<*AGROW>
        dataBundles(idx).id    = idVal;
        dataBundles(idx).type  = typeStr;
        idx = idx + 1;
    end
    fclose(fid);
end

%% -----------------------
%% Helper: parsePayloadByType
function parsedStr = parsePayloadByType(byteArray, typeStr)
    parsedStr = 'N/A';
    lowerType = lower(typeStr);
    
    try
        switch true
            case contains(lowerType, 'float64') || contains(lowerType, 'double')
                val = typecast(uint8(byteArray), 'double');
                parsedStr = num2str(val);
            case contains(lowerType, 'float32')
                val = typecast(uint8(byteArray), 'single');
                parsedStr = num2str(val);
            case contains(lowerType, 'int32')
                val = typecast(uint8(byteArray), 'int32');
                parsedStr = num2str(val);
            case contains(lowerType, 'uint32')
                val = typecast(uint8(byteArray), 'uint32');
                parsedStr = num2str(val);
            case contains(lowerType, 'int16')
                val = typecast(uint8(byteArray), 'int16');
                parsedStr = num2str(val);
            case contains(lowerType, 'uint16')
                val = typecast(uint8(byteArray), 'uint16');
                parsedStr = num2str(val);
            otherwise
                parsedStr = sprintf('%02X ', byteArray);
        end
    catch
        parsedStr = sprintf('%02X ', byteArray);
    end
end