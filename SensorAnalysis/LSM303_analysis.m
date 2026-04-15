%% WPI High Power Model Rocket MQP
% Author: Daniel Pearson
% Version: 2/15/2025

clear variables; close all; clc;

rawLSM303 = readtable('./data/DataLog_0045_17_2_2025.csv');

% Extract Timestamp
time = rawLSM303.timestamp / 1000;

% Read Acceleration Values
accelX = rawLSM303.accelX; % [mg]
accelY = rawLSM303.accelY; % [mg]
accelZ = rawLSM303.accelZ; % [mg]

% Read Magnetometer Values
magX = rawLSM303.magX; % [mG]
magY = rawLSM303.magY; % [mG]
magZ = rawLSM303.magZ; % [mG]

%% Plot Readings

figure('Name', 'Acceleration Readings');
plot(time, accelX, 'DisplayName', 'Accel X');
hold on;
plot(time, accelY, 'DisplayName', 'Accel Y');
plot(time, accelZ, 'DisplayName', 'Accel Z');
hold off;
grid on;
title('Raw Acceleration Readings');
xlabel('Time (s)');
ylabel('Acceleration (mg)');
legend()

figure('Name', 'Magnetometer Readings');
plot(time, magX, 'DisplayName', 'Mag X');
hold on;
plot(time, magY, 'DisplayName', 'Mag Y');
plot(time, magZ, 'DisplayName', 'Mag Z');
hold off;
grid on;
title('Raw Magnetometer Readings');
xlabel('Time (s)');
ylabel('Magnetic Field (mG)');
legend()

%% Calculate Sensor Error
% Root Mean Squared Error (RMS)
RMS_accel_X = std(accelX); % [mg]
RMS_accel_Y = std(accelY); % [mg]
RMS_accel_Z = std(accelZ); % [mg] 

RMS_mag_X = std(magX);
RMS_mag_Y = std(magY);
RMS_mag_Z = std(magZ);

% Velocity Random Walk (VRW)
dt = mean(diff(time));
fs = 1 / dt;

VRW_x = RMS_accel_X / sqrt(fs);
VRW_y = RMS_accel_Y / sqrt(fs);
VRW_z = RMS_accel_Z / sqrt(fs);

%% Gaussian Distribution
% Calculate deviation
dev_accelX = accelX - mean(accelX);
dev_accelY = accelY - mean(accelY);
dev_accelZ = accelZ - mean(accelZ);

dev_magX = magX - mean(magX);
dev_magY = magY - mean(magY);
dev_magZ = magZ - mean(magZ);

% Fit probability density function
pd_accelX = fitdist(dev_accelX, 'Normal');
pd_accelY = fitdist(dev_accelY, 'Normal');
pd_accelZ = fitdist(dev_accelZ, 'Normal');

pd_magX = fitdist(dev_magX, 'Normal');
pd_magY = fitdist(dev_magY, 'Normal');
pd_magZ = fitdist(dev_magZ, 'Normal');

% 100 Point resolution for distribution x bounds
distRange_accel = linspace(min([dev_accelX; dev_accelY; dev_accelZ]), max([dev_accelX; dev_accelY; dev_accelZ]), 100);

distRange_mag = linspace(min([dev_magX; dev_magY; dev_magZ]), max([dev_magX; dev_magY; dev_magZ]), 100);

pd_accelX = pdf(pd_accelX, distRange_accel);
pd_accelY = pdf(pd_accelY, distRange_accel);
pd_accelZ = pdf(pd_accelZ, distRange_accel);

pd_magX = pdf(pd_magX, distRange_mag);
pd_magY = pdf(pd_magY, distRange_mag);
pd_magZ = pdf(pd_magZ, distRange_mag);

% Plot Gaussian Distribution
figure('Name', 'LSM303 Accelerometer Error Distribution');

% Acceleration X Error
subplot(3,1,1);
hold on;
histogram(dev_accelX, 'Normalization', 'pdf', 'FaceAlpha', 0.5, 'DisplayName', 'Accel X Error');
plot(distRange_accel, pd_accelX, 'r', 'LineWidth', 2, 'DisplayName', 'X Fit');
hold off;
grid on;
xlabel('Acceleration Error (mg)');
ylabel('Probability Density');
title('Acceleration X Error Distribution');
legend();

% Acceleration Y Error
subplot(3,1,2);
hold on;
histogram(dev_accelY, 'Normalization', 'pdf', 'FaceAlpha', 0.5, 'DisplayName', 'Accel Y Error');
plot(distRange_accel, pd_accelY, 'g', 'LineWidth', 2, 'DisplayName', 'Y Fit');
hold off;
grid on;
xlabel('Acceleration Error (mg)');
ylabel('Probability Density');
title('Acceleration Y Error Distribution');
legend();

% Acceleration Z Error
subplot(3,1,3);
hold on;
histogram(dev_accelZ, 'Normalization', 'pdf', 'FaceAlpha', 0.5, 'DisplayName', 'Accel Z Error');
plot(distRange_accel, pd_accelZ, 'b', 'LineWidth', 2, 'DisplayName', 'Z Fit');
hold off;
grid on;
xlabel('Acceleration Error (mg)');
ylabel('Probability Density');
title('Acceleration Z Error Distribution');
legend();

% Plot Gaussian Distribution
figure('Name', 'LSM303 Magnetometer Error Distribution');

% Mag X Error
subplot(3,1,1);
hold on;
histogram(dev_magX, 'Normalization', 'pdf', 'FaceAlpha', 0.5, 'DisplayName', 'Magnetometer X Error');
plot(distRange_mag, pd_magX, 'r', 'LineWidth', 2, 'DisplayName', 'X Fit');
hold off;
grid on;
xlabel('Magnetic Field Error (mg)');
ylabel('Probability Density');
title('Magnetic Field X Error Distribution');
legend();

% Mag Y Error
subplot(3,1,2);
hold on;
histogram(dev_magY, 'Normalization', 'pdf', 'FaceAlpha', 0.5, 'DisplayName', 'Magnetometer Y Error');
plot(distRange_mag, pd_magY, 'g', 'LineWidth', 2, 'DisplayName', 'Y Fit');
hold off;
grid on;
xlabel('Magnetic Field Error (mg)');
ylabel('Probability Density');
title('Magnetic Field Y Error Distribution');
legend();

% Mag Z Error
subplot(3,1,3);
hold on;
histogram(dev_magZ, 'Normalization', 'pdf', 'FaceAlpha', 0.5, 'DisplayName', 'Magnetometer Z Error');
plot(distRange_mag, pd_magZ, 'b', 'LineWidth', 2, 'DisplayName', 'Z Fit');
hold off;
grid on;
xlabel('Magnetic Field Error (mG)');
ylabel('Probability Density');
title('Magnetic Field Z Error Distribution');
legend();
