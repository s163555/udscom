function [varargout] = CANInterface_Windows(cmd,varargin)% vendorName, deviceOrEmpty, rxIdHex, txIdHex, requestVec)
% CANINTERFACE_WINDOWS  Single-frame TX, multi-frame RX (naive ISO-TP),
%                       with MULTIPLE flow-control frames after FirstFrame.
%
%  Usage:
%    resp = CANInterface_Windows('tranceiveCAN', ...
%       'PEAK-System', [], '18DAF101', '18DA01F1', uint8([0x22, 0xF1, 0x90]));
%
%  If 'deviceOrEmpty' is empty, it auto-detects the first PEAK-System device
%  from canHWInfo() and uses that. This example forcibly sends three (3) flow
%  control frames to mimic a Linux SocketCAN log where repeated FC frames
%  were observed.


    switch cmd
        case 'tranceiveCAN'
            % Now uses ISO-TP for automatic segmentation and reassembly.
            varargout{1} = tranceive(varargin{:});
        case 'setup'
            varargout{1} = setup(varargin{:});
        case 'cleanup'
            cleanup(varargin{:});
        case 'detect'
            [varargout{1}, varargout{2}] = detect();
        otherwise
            error('Unsupported function: %s', cmd);
    end

end
function [ifName,handle] = detect()

 %------------------------------------------------------------
    % 1) If the device is empty, auto-detect the first PEAK-System entry
    %------------------------------------------------------------
    vendorName = 'PEAK-System';
    
    hwInfo = canHWInfo();  % older MATLAB returns a struct array or table
    % The displayed table typically has fields: Vendor, Device, Channel, etc.

    % We need to find rows with Vendor == "PEAK-System".
    allVendors = {hwInfo(:).VendorInfo.VendorName};  % cell array of vendors
    peakMask   = strcmp(allVendors, vendorName);
    peakIdx    = find(peakMask, 1, 'first');  % index of first match
    if isempty(peakIdx)
        error('No PEAK-System device found in canHWInfo().');
    end

    deviceName  = hwInfo.VendorInfo(1,peakIdx).ChannelInfo.Device;  % e.g. 'PCAN_USBBUS1'
    fprintf('[INFO] Auto-detected PEAK device="%s"', ...
        deviceName);
    canCh = canChannel('PEAK-System', deviceName);
    ifName = deviceName;
    handle = canCh;
    
end
function canOK = setup(canCh)
    %------------------------------------------------------------
    % 2) Create CAN channel
    %------------------------------------------------------------
    
    % If needed, set the bus speed:
    stop(canCh);
    configBusSpeed(canCh, 500000);

    %------------------------------------------------------------
    % 3) Start the channel
    %------------------------------------------------------------
    start(canCh);
    canOK = true;
end
    
function respData = tranceive(canCh,rxIdHex, txIdHex, requestVec)
    

    %------------------------------------------------------------
    % 4) Parse the hex IDs
    %------------------------------------------------------------
    rxId = hex2dec(rxIdHex);
    txId = hex2dec(txIdHex);
    rxIsExtended = (rxId > 2047);
    txIsExtended = (txId > 2047);

    % Request must be single-frame (<= 8 bytes)
    if numel(requestVec) > 8
        error('Request has more than 8 bytes. This code only does single-frame TX.');
    end
    %------------------------------------------------------------
    % 5) Send single-frame request
    %------------------------------------------------------------
    msgTx = canMessage(txId, txIsExtended, numel(requestVec));
    msgTx.Data = requestVec;
    transmit(canCh, msgTx);

    %------------------------------------------------------------
    % 6) Multi-frame RX with repeated flow-controls
    %------------------------------------------------------------
    respData = isoTpReceive(canCh, rxId,txId, rxIsExtended,txIsExtended);
end


%====================================================================
% Subfunction: isoTpReceive
%====================================================================
function respData = isoTpReceive(canCh, rxId,txId, rxIsExtended,txIsExtended)
    respData = uint8([]);
    tStart = tic;
    timeoutSec = 0.5;

    gotFirstFrame = false;
    totalLen = 0;

    while toc(tStart) < timeoutSec
        msgs = receive(canCh, Inf);  % get all queued frames
        if isempty(msgs)
            pause(0.01);
            continue;
        end

        for m = 1:numel(msgs)
            msg = msgs(m);
            % Must match our response ID (rxId, with correct Extended bit)
            if msg.ID ~= rxId || msg.Extended ~= rxIsExtended
                continue;
            end
            raw = msg.Data;
            if isempty(raw), continue; end
            pci = raw(1);
            frameType = bitshift(pci, -4);  % top nibble
            if frameType == 0
                lengthNib =bitand(pci, 15);
            else
                lengthNib = raw(2);%
            end
            switch frameType
                case 0 % SINGLE FRAME
                    sfLen = lengthNib;
                    if sfLen <= (numel(raw) - 2)
                        respData = raw(2 : 1 + sfLen);
                    end
                    return;

                case 1 % FIRST FRAME
                    if numel(raw) < 3
                        continue; % malformed
                    end
                    %ffLen = lengthNib * 256 + raw(2);
                    totalLen = lengthNib;
                    ffPayload = raw(3:end);
                    respData = ffPayload(:).';
                    gotFirstFrame = true;

                    % === REPLICATE LINUX: SEND MULTIPLE FLOW CONTROLS ===
                    % In your log, we see three "30 00 00" frames repeated
                    for i = 1:3
                        sendFlowControl(canCh, txId, txIsExtended);
                    end

                    % If the first frame has all data (rare)
                    if numel(respData) >= totalLen
                        respData = respData(1:totalLen);
                        return;
                    end

                case 2 % CONSECUTIVE FRAME
                    if ~gotFirstFrame, continue; end
                    cfPayload = raw(2:end);
                    respData = [respData, cfPayload(:).'];

                    if numel(respData) >= totalLen
                        respData = respData(1:totalLen);
                        return;
                    end

                case 3 % FLOW CONTROL (not expected from ECU->us)
                    continue;
            end
        end
        pause(0.01);
    end
end


%====================================================================
% Subfunction: sendFlowControl
%====================================================================
function sendFlowControl(canCh, txId, txIsExtended)
    % "30 00 00" => FlowControl(ClearToSend), BlockSize=0, STmin=0
    fc = canMessage(txId, txIsExtended, 3);
    fc.Data = uint8([0x30, 0x00, 0x00]);
    transmit(canCh, fc);
end

%% Windows-specific cleanup function
function cleanup(canCh)

    %------------------------------------------------------------
    % 7) Stop & return
    %------------------------------------------------------------
    stop(canCh);
    delete(canCh);
end
