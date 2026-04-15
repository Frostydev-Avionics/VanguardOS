%% WPI High Power Model Rocket MQP
% Author: Daniel Pearson
% Version: 2/16/2025

clear variables; close all; clc;

rawLPS22 = readtable('./data/DataLog_0045_17_2_2025.csv');

% Extract Timestamp
time = rawLPS22.timestamp / 1000;

% Sensor Readings
pressure = rawLPS22.pressure; % [hPa] Pressure
altitude = rawLPS22.altitude; % [m] Altitude

%% Plot Readings
figure('Name', 'Altitude');
plot(time, altitude, 'DisplayName', 'Altitude');
grid on;
title('Altitude');
xlabel('Time (s)');
ylabel('Altitude (m)');
legend()

figure('Name', 'Pressure');
plot(time, pressure, 'DisplayName', 'Pressure');
grid on;
title('Pressure');
xlabel('Time (s)');
ylabel('Pressure (hPa)');
legend();

%% Calculate Sensor Error
% Root Mean Squared Error (RMS)
RMS = std(pressure); % [hPa]
RMS_alt = std(altitude);

%% Gaussian Distribution
% Calculate Deviation
dev = pressure - mean(pressure);

% Fit Probability Density Function
pd = fitdist(dev,'Normal');

% Bounds
distRange = linspace(min(dev), max(dev), 100);

pd = pdf(pd, distRange);

% Plot Gaussian Distribution
figure('Name', 'LPS22 Error Distribution');
hold on;
histogram(dev, 'Normalization', 'pdf', 'FaceAlpha', 0.5, 'DisplayName', 'Pressure Error');
plot(distRange, pd, 'r', 'LineWidth', 2, 'DisplayName', 'Fit');
hold off;
grid on;
xlabel('Pressure Error (hPa)');
ylabel('Probability Density');
legend();
