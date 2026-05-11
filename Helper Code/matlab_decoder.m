clc;
clear;

% ============================================================
% RAW HEX PACKET FROM REALTERM
% ============================================================
% Paste your packet exactly as copied from RealTerm
% MATLAB will clean it automatically

rawHex = [ ...
'AA 10 60 B1 AD 03 00 32 30 30 30 2F 30 31 2F 30 31 20 30 30 3A 35 39 3A 31 38 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 CD CC B4 41 CD  CC 46 42 00 80 8E 41 FA 36 75 41 DA AD 6F 41 00 00 00 00 CD CC CC 3D 00 00 00 00 E1 7A 14 3F 5C 8F C2 BE 33 33 93 3F CD CC 6C 40 9A 99 01 C1 33 33 C3 41 08 03']; 



% ============================================================
% CLEAN INPUT STRING
% ============================================================

% Remove accidental non-hex characters
rawHex = regexprep(rawHex, '[^0-9A-Fa-f ]', '');

% Split into individual hex byte strings
hexCells = strsplit(strtrim(rawHex));

% Convert to uint8 byte array
packet = uint8(hex2dec(hexCells));

fprintf('Total packet bytes: %d\n\n', length(packet));

% ============================================================
% BASIC PACKET VALIDATION
% ============================================================

START_BYTE = hex2dec('AA');
END_BYTE   = hex2dec('03');

if packet(1) ~= START_BYTE
    error('Invalid START BYTE');
end

if packet(end) ~= END_BYTE
    error('Invalid END BYTE');
end

fprintf('START BYTE VALID\n');
fprintf('END BYTE VALID\n\n');

% ============================================================
% EXTRACT HEADER
% ============================================================

msgType      = packet(2);
payloadLength = packet(3);

fprintf('Message Type   : 0x%02X\n', msgType);
fprintf('Payload Length : %d bytes\n\n', payloadLength);

% ============================================================ 
% EXTRACT PAYLOAD 
% ============================================================
payload = packet(4 : 3 + payloadLength);
if length(payload) ~= payloadLength 
    error('Payload length mismatch'); 
end

receivedChecksum = packet(end-1);

% ============================================================
% VERIFY CHECKSUM
% ============================================================

calculatedChecksum = uint8(0);

for i = 1:length(payload)
    calculatedChecksum = bitxor(calculatedChecksum, payload(i));
end

fprintf('Received Checksum   : 0x%02X\n', receivedChecksum);
fprintf('Calculated Checksum : 0x%02X\n\n', calculatedChecksum);

if calculatedChecksum == receivedChecksum
    fprintf('CHECKSUM VALID\n\n');
else
    fprintf('CHECKSUM INVALID\n\n');
end

% ============================================================
% TELEMETRY DECODER
% ============================================================

index = 1;

% Helper functions
readUint32 = @(bytes) typecast(uint8(bytes), 'uint32');
readFloat  = @(bytes) typecast(uint8(bytes), 'single');


% ------------------------------------------------------------
% SYSTEM
% ------------------------------------------------------------

timestamp_ms = readUint32(payload(index:index+3)); 
index = index + 4; 
rtcTimestamp = char(payload(index:index+31));  
index = index + 32;
systemHealthFlags = readUint32(payload(index:index+3)); index = index + 4;
git status
% ------------------------------------------------------------
% ENVIRONMENTAL
% ------------------------------------------------------------

ambientTemp_C = readFloat(payload(index:index+3));
index = index + 4;

humidity_percent = readFloat(payload(index:index+3));
index = index + 4;

% ------------------------------------------------------------
% THERMAL
% ------------------------------------------------------------

ds18b20Temp_C = readFloat(payload(index:index+3));
index = index + 4;

ntc1Temp_C = readFloat(payload(index:index+3));
index = index + 4;

ntc2Temp_C = readFloat(payload(index:index+3));
index = index + 4;

% ------------------------------------------------------------
% POWER
% ------------------------------------------------------------

busVoltage_V = readFloat(payload(index:index+3));
index = index + 4;

current_mA = readFloat(payload(index:index+3));
index = index + 4;

power_mW = readFloat(payload(index:index+3));
index = index + 4;

% ------------------------------------------------------------
% IMU ACCEL
% ------------------------------------------------------------

accelX_g = readFloat(payload(index:index+3));
index = index + 4;

accelY_g = readFloat(payload(index:index+3));
index = index + 4;

accelZ_g = readFloat(payload(index:index+3));
index = index + 4;

% ------------------------------------------------------------
% IMU GYRO
% ------------------------------------------------------------

gyroX_dps = readFloat(payload(index:index+3));
index = index + 4;

gyroY_dps = readFloat(payload(index:index+3));
index = index + 4;

gyroZ_dps = readFloat(payload(index:index+3));
index = index + 4;

% ============================================================
% DISPLAY RESULTS
% ============================================================

fprintf('==================================================\n');
fprintf('DECODED TELEMETRY\n');
fprintf('==================================================\n\n');

fprintf('Timestamp             : %u ms\n', timestamp_ms);
fprintf('RTC Timestamp : %s\n', rtcTimestamp);

fprintf('Health Flags          : 0x%08X\n\n', systemHealthFlags);

fprintf('Ambient Temp          : %.2f C\n', ambientTemp_C);
fprintf('Humidity              : %.2f %%\n\n', humidity_percent);

fprintf('DS18B20 Temp          : %.2f C\n', ds18b20Temp_C);
fprintf('NTC1 Temp             : %.2f C\n', ntc1Temp_C);
fprintf('NTC2 Temp             : %.2f C\n\n', ntc2Temp_C);

fprintf('Bus Voltage           : %.2f V\n', busVoltage_V);
fprintf('Current               : %.2f mA\n', current_mA);
fprintf('Power                 : %.2f mW\n\n', power_mW);

fprintf('Accel X               : %.3f g\n', accelX_g);
fprintf('Accel Y               : %.3f g\n', accelY_g);
fprintf('Accel Z               : %.3f g\n\n', accelZ_g);

fprintf('Gyro X                : %.3f dps\n', gyroX_dps);
fprintf('Gyro Y                : %.3f dps\n', gyroY_dps);
fprintf('Gyro Z                : %.3f dps\n\n', gyroZ_dps);


% ============================================================
% HEALTH FLAG DECODER
% ============================================================

fprintf('==================================================\n');
fprintf('HEALTH FLAGS\n');
fprintf('==================================================\n\n');

if bitget(systemHealthFlags, 1)
    fprintf('DS18B20 FAULT\n');
end

if bitget(systemHealthFlags, 2)
    fprintf('NTC1 FAULT\n');
end

if bitget(systemHealthFlags, 3)
    fprintf('NTC2 FAULT\n');
end

if bitget(systemHealthFlags, 4)
    fprintf('SHTC3 FAULT\n');
end

if bitget(systemHealthFlags, 5)
    fprintf('INA219 FAULT\n');
end

if bitget(systemHealthFlags, 6)
    fprintf('MPU6050 FAULT\n');
end

if bitget(systemHealthFlags, 7)
    fprintf('RTC FAULT\n');
end

if systemHealthFlags == 0
    fprintf('NO FAULTS DETECTED\n');
end
