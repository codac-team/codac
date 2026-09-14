classdef MyCtc < handle

    properties
        M
    end

    methods
        function obj = MyCtc(M_)
            obj.M = M_;
        end

        function a = contract(obj, a)
            % [B-q2-beg]
            u = py.codac4matlab.IntervalVector().empty(int32(2));
            for i = 1:numel(obj.M)
                mi = obj.M{i};
                u.self_union(a.inter(mi));
            end
            a = u;
            % [B-q2-end]

            % Insert contraction formula here (question B.2)
            
        end
    end
end