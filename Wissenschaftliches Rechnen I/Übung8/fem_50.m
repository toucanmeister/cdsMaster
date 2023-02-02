function fem_50 ( )
    clear
    load coordinates.dat;
    load elements3.dat;
    eval ( 'load neumann.dat;', 'neumann=[];' );
    load dirichlet.dat;
    A = sparse ( size(coordinates,1), size(coordinates,1) );
    b = sparse ( size(coordinates,1), 1 );
    for j = 1 : size(elements3,1)
        A(elements3(j,:),elements3(j,:)) = A(elements3(j,:),elements3(j,:)) ...
            + stima3(coordinates(elements3(j,:),:));
    end
    for j = 1 : size(elements3,1)
        b(elements3(j,:)) = b(elements3(j,:)) ...
            + det( [1,1,1; coordinates(elements3(j,:),:)'] ) * ...
            f(sum(coordinates(elements3(j,:),:))/3)/6;
    end
    if ( ~isempty(neumann) )
        for j = 1 : size(neumann,1)
            b(neumann(j,:)) = b(neumann(j,:)) + ...
                norm(coordinates(neumann(j,1),:) - coordinates(neumann(j,2),:)) * ...
                g(sum(coordinates(neumann(j,:),:))/2)/2;
        end
    end
    u = sparse ( size(coordinates,1), 1 );
    BoundNodes = unique ( dirichlet );
    u(BoundNodes) = u_d ( coordinates(BoundNodes,:) );
    b = b - A * u;
    FreeNodes = setdiff ( 1:size(coordinates,1), BoundNodes );
    u(FreeNodes) = A(FreeNodes,FreeNodes) \ b(FreeNodes);
    show ( elements3, coordinates, full ( u ) );
    return
end
function M = stima3 ( vertices )
    d = size ( vertices, 2 );
    D_eta = [ ones(1,d+1); vertices' ] \ [ zeros(1,d); eye(d) ];
    M = det ( [ ones(1,d+1); vertices' ] ) * D_eta * D_eta' / prod ( 1:d );
    return
end
function show (elements3, coordinates, u)
    trisurf ( elements3, coordinates(:,1), coordinates(:,2), u' );
    view ( -67.5, 30 );title ( 'Solution to the Problem' )
    return
end