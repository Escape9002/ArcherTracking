% Einlesen der Daten
 data = readtable('ShootingQuatPelvis.txt','ReadVariableNames',true); 
 
 % Ausfall-Variable auslesen und speichern
 ausfall = data(1:end,2); 
 % Merkmale erstellen 
 merkmale = data(1:end,[3:23 25]); 
 
 % Auswahl der Merkmale für die Anzeige
 
 bsp_merkmale = [1 14 22]; 
    % Auswahl der Bsp.Ausgabe/Merkmale
    % editierbar
    % die maximale Merkmal-Nr. ist 22   
 
 disp('Ein Ausschnitt der ersten 5 Trainingsdaten')
 disp('sieht wie folgt aus:')
 
 % Bsp.Ausgabe der Daten erstellen
 out = [ausfall(1:5,1), merkmale(1:5,bsp_merkmale)]