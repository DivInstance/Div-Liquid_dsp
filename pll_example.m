% pll_example.m : simple Octave script to plot the results of pll_example.c

clear all;
close all;

% load data into single vector
v = load('pll_example.dat');
index = v(:,1);             % index
x     = v(:,2) + 1i*v(:,3); % input sinusoid
y     = v(:,4) + 1i*v(:,5); % output sinusoid
dphi  = v(:,6);             % phase error

figure;

% plot real components
subplot(3,1,1),
  plot(index, real(x), real(y));
  xlabel('Sample Index');
  ylabel('real');
  grid on;

% plot imaginary components
subplot(3,1,2),
  plot(index, imag(x), imag(y));
  xlabel('Sample Index');
  ylabel('imag');
  grid on;

% plot phase error
subplot(3,1,3),
  plot(index, dphi);
  xlabel('Sample Index');
  ylabel('phase error');
  grid on;

