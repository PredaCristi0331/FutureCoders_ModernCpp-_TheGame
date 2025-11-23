Jocul The Game reprezintă un joc cooperativ de cărți în care participanții colaborează pentru a plasa toate cele 98 de cărți numerotate de la 2 la 99 în patru teancuri distincte, respectând reguli stricte de ordonare: două teancuri în ordine crescătoare și două în ordine descrescătoare. Obiectivul colectiv constă în utilizarea tuturor cărților, situație în care echipa câștigă și „învinge” jocul.

Pornind de la conceptul jocului, se propune implementarea unei aplicații care să respecte următoarele reguli fundamentale:

Structura pachetului de cărți

Există două categorii de cărți:

4 cărți fundament (două având valoarea „1” și două având valoarea „100”), care constituie bazele celor patru teancuri și nu pot fi distribuite jucătorilor;

98 de cărți numerotate de la 2 la 99, care vor fi manipulate exclusiv de jucători.

Configurarea jocului

Un joc poate fi inițiat cu minimum 2 și maximum 5 jucători.

În faza de pregătire:

Cele patru cărți fundament sunt plasate vizibil, cu fața în sus.

Teancul celor 98 de cărți este amestecat, apoi se distribuie un număr de cărți fiecărui jucător, în funcție de numărul participanților:

2 jucători: câte 8 cărți/jucător

3 jucători: câte 7 cărți/jucător

4–5 jucători: câte 6 cărți/jucător

Cărțile rămase formează teancul de tragere, plasat cu fața în jos.

Desfășurarea jocului

Jocul se desfășoară în ture, astfel încât fiecare jucător joacă o dată pe rundă. În cadrul propriului tur:

Jucătorul trebuie să joace cel puțin două cărți (una singură după epuizarea teancului de tragere), plasându-le pe unul sau mai multe dintre cele patru teancuri.

Cărțile se plasează una câte una, respectând regulile de ordonare:

Teancuri ascendente (începând de la „1”): fiecare carte nouă trebuie să fie mai mare decât cea precedentă.

Teancuri descendente (începând de la „100”): fiecare carte nouă trebuie să fie mai mică decât cea precedentă.

Excepția „Backwards Trick” / „Trucul invers”

Se permite o deviere controlată de la regulile de ordine pentru a crește flexibilitatea:

Pe un teanc ascendent se poate plasa o carte cu exact 10 unități mai mică decât cartea curentă.

Pe un teanc descendent se poate plasa o carte cu exact 10 unități mai mare decât cartea curentă.

După plasarea cărților, jucătorul extrage din teancul de tragere un număr de cărți egal cu numărul celor jucate, restabilindu-și astfel mărimea inițială a mâinii. După epuizarea teancului de tragere, fiecare jucător este obligat să joace cel puțin o singură carte per tur.

Condiții de finalizare

Jocul se încheie în două moduri:

Eșec: Un jucător nu poate plasa numărul minim de cărți permise în turul său.

Victorie: Toate cele 98 de cărți au fost plasate în teancuri în mod valid.

Reguli de comunicare

Jucătorii pot comunica prin intermediul unui chat, însă fără a dezvălui valorile exacte ale cărților din mână. Comunicarea trebuie să fie indirectă, de exemplu:

„Evitați să jucați pe primul teanc.”

„Am câteva opțiuni bune pentru teancul doi.”

Profilul utilizatorului

Aplicația va include un profil de utilizator conținând:

numărul total de ore jucate,

un scor de performanță (1–5), calculat asupra raportului dintre jocurile jucate, cele câștigate și numărul de cărți rămase în mâna jucătorilor la finalul jocurilor pierdute,

alte elemente considerate relevante.

Cerințe tehnice de bază

Arhitectură client–server: aplicația trebuie să permită rularea a minimum două instanțe de client și o instanță de server, utilizând protocolul HTTP și biblioteca CROW.

Pagina de Login/Register: utilizatorii se pot autentifica sau își pot crea cont folosind un nume de utilizator unic (emailul/parola sunt opționale).

Pagina jocului: aplicația principală va funcționa în consolă, respectând integral regulile jocului.

Bază de date: datele vor fi gestionate utilizând biblioteca SQLite ORM.

Componente avansate (2 puncte)

Multigaming: suport pentru mai multe jocuri simultan. Jucătorii sunt distribuiți automat în jocuri în funcție de scor; un utilizator poate aștepta maximum 30 de secunde, după care jocul pornește dacă există cel puțin doi participanți.

Interfață grafică (GUI): implementarea unei interfețe grafice personalizate, diferită de versiunea originală a jocului.

Elemente opționale recomandate

Introducerea unor niveluri de dificultate (ușor, mediu, greu), selectabile la începutul jocului sau ajustate dinamic.

Implementarea tratării riguroase a excepțiilor și realizarea de teste unitare utilizând framework-uri dedicate (minim 40 de teste sau acoperire de cod ≥50%).
