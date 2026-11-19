import py.codac4matlab.*

a=Interval(-1,2)
a.lb
a.ub

x=[1,2]
X=IntervalVector(x)
X.size
X(2)
X(2).init(Interval(-10,10))
X

x={{-5,5},{-4,4}}
X=IntervalVector(x)
X.size

M=[1,2;3,4;5,6]
C=arrayfun(@(i) num2cell(M(i,:)), 1:size(M,1), 'UniformOutput', false); % Inverse operation would be cell2mat(vertcat(C{:}))
A=IntervalMatrix(Matrix(C))
A.rows
A.cols
A(3,2)
A(3,2).init(Interval(-10,10))
A.set_item({1,2},Interval(11,12))
A

A_lb = Matrix({{-1,-2},{-3,-4},{-5,-6}});
A_ub = Matrix({{1,2},{3,4},{5,6}});
A=IntervalMatrix(A_lb,A_ub)
A.rows
A.cols
A(3,2)

A_ub.set_item({1,2},10)
A_ub

function C = Matrix2cell(M)
    import py.codac4matlab.*
    r = int32(M.rows);
    c = int32(M.cols);
    C = cell(1,r);
    for i=1:r
        rl = cell(1,c);
        for j=1:c
            rl{j} = double(M(i,j));
        end
        C{i} = rl;
    end
end

%M=Matrix({{1,2},{3,4},{5,6}})
%Matrix2cell(M)
%return

% It seems we cannot do A(i,j).init(x) if A is a Matrix... But we can do
% A.set_item({i,j},x)...
function B = setitemM(A,i,j,x)
%SETITEMM Set element (i,j) of a Codac Matrix.
%
% B = setitemM(A,i,j,x)
%
% A : Codac Matrix
% i : row index (1-based MATLAB indexing)
% j : column index (1-based MATLAB indexing)
% x : double
    
    import py.codac4matlab.*
    C = Matrix2cell(A);
    C{i}{j} = x;
    B = Matrix(C);
end

% Does the same as A(i,j).init(x), probably in a slower way...
function B = setitemIM(A,i,j,x)
%SETITEMIM Set element (i,j) of a Codac IntervalMatrix.
%
% B = setitemIM(A,i,j,x)
%
% A : Codac IntervalMatrix
% i : row index (1-based MATLAB indexing)
% j : column index (1-based MATLAB indexing)
% x : Codac Interval

    import py.codac4matlab.*
    C_lb = Matrix2cell(A.lb);
    C_lb{i}{j} = x.lb;
    C_ub = Matrix2cell(A.ub);
    C_ub{i}{j} = x.ub;
    B = IntervalMatrix(Matrix(C_lb),Matrix(C_ub));
end

A = Matrix({{1,2},{3,4},{5,6}});
A = setitemM(A,3,2,99)

A(3,2)
A

A_lb = Matrix({{-1,-2},{-3,-4},{-5,-6}});
A_ub = Matrix({{1,2},{3,4},{5,6}});
A = IntervalMatrix(A_lb,A_ub);

A = setitemIM(A,2,1,Interval(100,200));

A(2,1)
A
