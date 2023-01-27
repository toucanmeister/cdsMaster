set M;
# Maschinen

param n;
# Anzahl Quartale

set Q := 1..n;
# Quartale

param demand{q in Q};
# Bedarf am Produkt in Quartal q

param capacity{m in M, q in Q};
# Kapazitäten von Maschine m in Quartal q

param cost{m in M, q in Q};
# Produktionskosten pro produzierter Einheit von Maschine m in Quartal q

param storage_cost;
# Lagerkosten pro Einheit pro Quartal

var x{m in M, q in Q} >= 0;
# Durch Maschine m produzierte Einheiten in Quartal q

minimize costs: 
	(sum{m in M, q in Q} cost[m,q]*x[m,q]) + 
	(sum{q in Q} (sum{k in 1..q} ((sum{m in M} x[m,k]) - demand[k])))*storage_cost;

s.t. capacity_limit{m in M, q in Q}: x[m,q] <= capacity[m,q];
# Kann nicht mehr als Maschinenkapazität produzieren

s.t. demand_limit{q in Q}: sum{k in 1..q} ((sum{m in M} x[m,k]) - demand[k]) >= 0;
# Müssen in jedem Quartal mindestens Bedarf ausfüllen

solve;

printf '###############\n' ;
printf 'Minimierte Kosten: %d\n', costs;
printf{m in M, q in Q}: 'Maschine %s in Quartal %d: %d\n', m, q, x[m,q];

end;
