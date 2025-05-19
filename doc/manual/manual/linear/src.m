import py.codac4matlab.*

% [linear-1-beg]
A = Matrix({ ...
  { 1, 2, 0 }, ...
  { 3, 4, 1 }, ...
  { 0, 1, 0 }, ...
});

B = inverse_enclosure(A);
% B == [[     <1, 1> ,    <-0, 0> ,        <-2, -2> ]
%       [    <-0, 0> ,    <-0, 0> , [0.9999, 1.001] ]
%       [   <-3, -3> ,     <1, 1> ,  [1.999, 2.001] ]]

C = A*B;
i = C.contains(Matrix(3,3).eye(3,3));
% i == 1 (true)
% [linear-1-end]