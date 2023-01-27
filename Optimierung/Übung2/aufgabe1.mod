/*
max 150*a + 100*b
bzgl.
 12*a + 6*b <= 252
 4*a + 12*b <= 168
 a >= 0,
 b >= 0
*/

set K;
# Kabel

set R;
# Rohstoffe

param g{k in K};
# Gewinn für Kabel k

param s{r in R};
# Vorrat an Rohstoff r

param p{k in K, r in R};
# Verbrauch an Rohstoff r für Kabel k

var x{k in K} >= 0;
# Hunderte Meter von Kabel k, die hergestellt werden

maximize profit: sum{k in K} g[k]*x[k];

s.t. supply_limits{r in R}: sum{k in K} x[k]*p[k,r] <= s[r];

solve;

printf '###############\n' ;
printf 'Maximierter Gewinn: %d\n', profit;
printf{k in K}: 'Hunderte Meter %s hergestellt: %d\n', k, x[k];

end;
