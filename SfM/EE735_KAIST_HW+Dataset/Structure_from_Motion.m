%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This is the skeleton code of PA2 in EE735 Computer Vision.              %
% It will help you to implement the Structure-from-Motion method easily.  %
% Using this skeleton is recommended, but it's not necessary.             %
% You can freely modify it or you can implement your own program.         %
% If you have a question, please send me an email to hwha@rcv.kaist.ac.kr %
%                                                           TA. Hyowon Ha %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

close all;
clear all;

%% Define constants and parameters
% Constants ( need to be set )
number_of_pictures                  = 0;     % number of input pictures
number_of_iterations_for_5_point    = 0;
number_of_iterations_for_3_point    = 0;
number_of_iterations_for_LM         = 0;

% Thresholds ( need to be set )
threshold_of_distance = 0; 

% Assumptions ( need to be set )
max_number_of_features  = 0; % maximum number of features in 1 image
max_number_of_points    = 0; % maximum number of reconstructed points

% Matrices
K               = [  3397.747510 -0.000687 1943.499541;
                    0.000000 3397.759329 1295.497655;
                    0.000000 0.000000 1.000000 ];                                                
                % = intrinsic parameter matrix. 3 x 3
num_Feature     = zeros(1, number_of_pictures);                         
                % = number of features in each images. 1 x (# picture)
Feature         = zeros(4, max_number_of_features, number_of_pictures);     
                % = SIFT features in each images. 4 x (# feature) x (# picture) containing [ x y s th ]
Descriptor      = zeros(128, max_number_of_features, number_of_pictures);
                % = SIFT descriptors in each images. 128 x (# feature) x (# picture)
num_Match       = zeros(number_of_pictures, number_of_pictures);          
                % = number of matches. (# picture) x (# picture)
Match           = zeros(number_of_pictures, max_number_of_features, number_of_pictures);
                % = corresponding feature index in second picture for a feature of first picture. (# picture) x (# feature) x (# picture)
X               = zeros(6, max_number_of_points);                                 
                % = reconstructed 3D points. 6 x (# point) containing [ x y z r g b ] (float)
X_exist         = zeros(1, max_number_of_points);                           
                % = 1 : X is exist. 0 : not. 1 x (# point)
Feature2X       = zeros(number_of_pictures, max_number_of_features);
                % = feature to 3D pointindex. (# picture) x (# feature)
X2Feature       = zeros(max_number_of_points, number_of_pictures);        
                % = 3D point to feature index. (# point) x (# picture)
Image_selected  = zeros(1, number_of_pictures);                      
                % = 1 : image is selected, 0 : not. (# picture)
R               = zeros(3, 3, number_of_pictures);                                
                % = Camera rotation R of images. 3 x 3 x (# picture)
T               = zeros(3, number_of_pictures);                      
                % = Camera translation t of images. 3 x (# picture)

% ADVICE : These matrices seem very difficult, but you will need sum data structures like them.

%% Feature extraction and matching
% Load images and extract features and find correspondences.
% Fill num_Feature, Feature, Descriptor, num_Match and Match
% hints : use vl_sift to extract features and get the descriptors.
%        use vl_ubcmatch to find corresponding matches between two feature sets.


%% Initialization step (for 2 views)
% Find the best reference image. 
% It is reasonable to use the image which has the largest sum of # correspondences to other images.
% Let's call it 'ref'
ref; % find out
Image_selected(ref)=1;

% Give ref's R|T = I|0
R(:, :, ref) = eye(3);
T(:, ref) = zeros(3, 1);

% Find the best pair of reference image.
% It will be the one that has the largest # correspondences to the reference image.
% Let's call it 'pair'
pair; % find out
Image_selected(pair)=1;


% Estimate E using 8,7-point algorithm or calibrated 5-point algorithm and RANSAC
E; % find out

% Decompose E into [Rp, Tp]
Rp; % find out
Tp; % find out

% (Optional) Optimize Rp and Tp

% Give pair's R|T = Rp|Tp
R(:, :, pair) = Rp;
T(:, pair) = Tp;

% Reconstruct 3D points using triangulation
X; % find out
X_exist; % find out

% Save 3D points to PLY
filename = sprintf('%02dviews.ply', 2);
SavePLY(filename, X, X_exist);

%% Growing step ( for more than 2 views )
% If you have done the initialization step, then do this step.

for picture = 3 : number_of_pictures
    % Find the best image that has the largest sum of # correspondences to previously selected images. 
    new; % find out
    old=find(selected==1); % previously selected images
    
    % Find the 2D-to-3D correspondences between features of 'new' and features of 'old's.
    
    
    % Estimate pose R|T using 6-point DLT or 3-point algorithm.
    
    
    % (Optional) Optimize R|T
    
    
    % Reconstruct 3D points using triangulation
    
    
    % (Optional) Optimize all R|T and all 3D points
    
    
    % Reduce duplicate points
    
    
    % Save 3D points to PLY
    filename = sprintf('%02dviews.ply', picture);
    SavePLY(filename, X, X_exist);
end

% ADVICE : This code only gives you an outline of the Structure-from-Motion algorithm.
%          You should implement the key (omitted) algorithms by yourself.