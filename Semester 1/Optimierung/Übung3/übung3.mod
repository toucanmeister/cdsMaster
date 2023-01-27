/*
  min_x sum_{i=1}^{200} ||x-a^i||_1
= min_x sum_{i=1}^{200} sum_{j=1}^{20} |x_j-a_j^i|
= min_x sum_{i=1}^{200} sum_{j=1}^{20} y_ij
  s.t.  x_j - a_j^i <= y_ij
  s.t. -x_j + a_j^i <= y-ij
*/


param n;
# 200 in our example

param m;
# 20 in our example

param a{i in 1..m, j in 1..n};
# Data

var x{i in 1..m};

var y{i in 1..m, j in 1..n};

minimize err: sum{i in 1..m} sum{j in 1..n} y[i,j];

s.t. abs1{i in 1..m, j in 1..n}:  x[i] - a[i,j] <= y[i,j];
s.t. abs2{i in 1..m, j in 1..n}: -x[i] + a[i,j] <= y[i,j];

solve;

printf '###############\n' ;
printf 'Fehler: %d\n', err;
printf{i in 1..m}: 'Wert für x_%d: %f\n', i, x[i];

end;
