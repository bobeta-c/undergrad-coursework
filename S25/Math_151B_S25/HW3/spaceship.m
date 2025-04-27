%% HW3 Problem 3 - Simulation of the trajectory of a spaceship using RK4

%% Define "velocity" and initial values

% Assign velocity field to variable f 
f = @(t,y) velocity_field(t,y) ;

% Iniital value
y0 = [0.994; 0; 0 ; -2.00158510637908252240537862224] ;


%% Discretization of time

% Simulation time
T = 17.1 ; 

% Number of time steps (uncomment below and type in your choice)
% N = % your choice ;

% Time vector
tvec = linspace(0, T, N)' ;

% Step size
h = tvec(2) - tvec(1) ;


%% Time-stepping

% Initialize numerical solution by initial values
y = y0 ;

% Array for storing numerical solution in each time step
y_stored = zeros(4, N) ;

for n = 1:N

    % Get time
    t = tvec(n) ;
    
    %%%%%%%%%%%%%%%%%%% Uncomment below and complete %%%%%%%%%%%%%%%%%%%%%%

    % % 4-stage updates
    % k1 = % your syntax
    % k2 = % your syntax
    % k3 = % your syntax
    % k4 = % your syntax

    % % Update solution y
    % y = % your syntax

    %%%%%%%%%%%%%%%%%%%%% End of your edits for this part %%%%%%%%%%%%%%%%%

    % Save solution
    y_stored(:,n) = y ; 

end


%% Plot (uncomment below when you code is ready)

% % x-coordinates of planets
% mu = 0.012277471 ;
% mu0 = 1 - mu ;
% 
% figure(1) ; clf ;
% plot(mu, 0, 'ro', 'MarkerSize', 5, 'MarkerFaceColor', 'r', 'DisplayName', 'Planet A') ; hold on; 
% plot(mu0, 0, 'bo', 'MarkerSize', 5, 'MarkerFaceColor', 'b', 'DisplayName', 'Planet B') ;
% plot(y_stored(1,:), y_stored(3,:), 'k-', 'LineWidth', 1.5, 'DisplayName', 'Orbit') ;
% set(gca, 'FontSize', 15)
% leg = legend ; 
% set(leg, 'FontSize', 15) 
% xlabel('x') ; ylabel('y') ;
