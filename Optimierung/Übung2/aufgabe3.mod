set S;
# Studenten

set T;
# Termine

param prefs{t in T, s in S};
# Idee: Formen Daten um, sodass alle Termine in der Liste auftauchen. 
# Eintrag in dieser Liste sind die Strafpunkte, die dem Termin durch diesen Student zukommen.

var dates{t in T, s in S} >= 0;
# Hier soll später in jeder Spalte (für jeden Studenten) eine 1 an der Stelle des gewählten Termins stehen und sonst Nullen

minimize penalties: sum{t in T, s in S} prefs[t,s]*dates[t,s];

s.t. student_condition{s in S}: sum{t in T} dates[t,s] = 1;
s.t. date_condition{t in T}: sum{s in S} dates[t,s] = 1;

solve;

printf '###############\n' ;
printf 'Minimierte Strafpunkte: %d\n', penalties;
printf{t in T, s in S} 'Termin %s für Student %s: %f\n', t, s, dates[t,s];

end;
