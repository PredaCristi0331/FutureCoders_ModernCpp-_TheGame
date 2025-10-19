Jocul The Game  este un joc cooperativ de cărți, în care jucătorii colaborează cu scopul plasării tuturor celor 98 de cărți numerotate (de la 2 la 99) în patru teancuri, conform unor reguli de ordine (două în ordine crescătoare, două în ordine descrescătoare). Dacă reușesc, echipa câștigă și jocul este învins. Pornind de ideea acestui joc să se implementeze o aplicație care respectă următoarele reguli:
Există două categorii de cărți:
4 cărți fundament (două cu valoarea “1” și două cu valoarea “100”) — acestea sunt baze pentru cele patru teancuri ce vor trebui formate de către jucători și nu pot fi jucate către jucători
98 cărți numerotate de la “2” la “99” care vor fi manipulate de către jucători
Pentru desfășurarea unui joc este necesară prezența a cel puțin 2 și cel mult 5 jucători.
În partea de pregătire a jocului:
vor fi așezate cele patru cărți fundament în spațiul vizibil de joc, cu fața în sus.
se va amesteca teancul cu cele 98 de cărți și din acesta se distribuie cărți fiecărui jucător, în funcție de numărul de jucători:
2 jucători: câte 8 cărți fiecare
3 jucători: câte 7 cărți fiecare
4 sau 5 jucători: câte 6 cărți fiecare
cărțile rămase (după distribuire) formează un teanc de tragere, plasat cu fața în jos
Odată pregătite cărțile se începe desfășurarea jocului. Se va juca pe rând astfel încât la finalul unei runde toți jucătorii să fi jucat o dată, realizând următoarele acțiuni:
Fiecare jucător trebuie să joace minim 2 cărți din mână, plasându-le în unul sau mai multe din cele 4 teancuri  (care inițial conțin cărțile de 1 și 100). 
Cărțile se joacă una câte una, respectând regulile de ordine pentru fiecare tip de teanc:
Teancuri ascendente (cele pornite de la “1”): fiecare carte plasată trebuie să fie mai mare decât cartea de deasupra (ultima carte plasată) a acelui teanc 
Teancuri descendente (cele pornite de la “100”): fiecare carte plasată trebuie să fie mai mică decât cartea de deasupra acelui teanc
Excepția („Backwards Trick” / „Trucul invers”)
 Aceasta este o regulă specială care permite jucătorului, ocazional, să „încalce” temporar regula de ordine pentru a da mai multă flexibilitate:
Pe un teanc ascendent, dacă ai o carte care este exact cu 10 unități mai mică decât cartea de deasupra, poți să o plasezi – adică invers, ceea ce scade valoarea teancului (de exemplu dacă în teancul ascendent ultima carte plasată a fost “45”, se poate adăuga cartea “35”, astfel oferind din nou șansa de a adăuga cărți din intervalul [36, 98])
Pe un teanc descendent, dacă ai o carte care este exact cu 10 unități mai mare decât cartea de deasupra, o poți plasa, inițializând puțin posibilitatea de a juca ulterior. 
După ce jucătorul a terminat plasarea cărților din mână, jucătorul extrage cărți din teancul de tragere un număr de cărți egal cu numărul de cărți plasate, pentru a readuce mâna la numărul inițial 
După ce teancul de retragere se epuizează, fiecare jucător poate plasa minim o carte (nu 2 cum era până acum) și jocul continuă doar cu cărțile rămase în mâini.
Jocul se încheie dacă:
cel puțin un jucător nu poate plasa numărul minim de cărți în turul său (2 cărți sau 1 dacă teancul de tragere e gol). În acest caz, echipa pierde.
toate cele 98 de cărți pe cele patru teancuri, astfel jocul este câștigat
Jucătorii pot comunica prin intermediul unui chat astfel încât să ofere indicații cu privire la următoare mutare a coechipierilor, dar  jucătorii nu au voie să spună valorile exacte ale cărților din mână (de ex. „am 47”), comunicarea trebuie să fie vagă, indicând, de exemplu:
„Nu juca pe primul teanc”
„Am câteva cărți bune pentru al doilea teanc”
Se va crea un profil de utilizator care să conțină:
numărul de ore jucate
un scor de performanță (valoare între 1 și 5), calculat pe baza raportului dintre numărul de jocuri jucate, cele câștigate și a numărului de cărți pe care le aveau în mână jucătorii la final de joc pierdut
orice alte elemente le considerați relevante
Cerințe de bază
➡ Rețelistică: implementarea aplicației respectând arhitectura client-server(aplicația trebuie să asigure posibilitatea creării a minim 2 instanțe de Client + 1 aplicație server care vor comunica prin rețea). Se va utiliza protocolul HTTP (NU ALTUL) împreună cu biblioteca CROW.
➡ Pagină de Login/Register: la pornire, unui utilizator i se oferă posibilitatea de a se loga în contul său sau își poate crea un cont. Logarea/înregistrarea presupune introducerea numelui de utilizator. Atenție: numele de utilizator trebuie să fie unic (Atenție! utilizarea email-ului sau a parole este optională și nu va fi punctată suplimentar)
➡ Pagina jocului: va fi o aplicație consolă, care respectă regulile jocului enunțate anterior.
➡ Bază de date: Pentru a vă organiza datele, puteți să le stocați într-o bază de date. Se va folosi biblioteca de SQLite SQLite ORM (NU ALTA). Se poate instala într-un proiect de Visual Studio folosind Microsoft vcpkg (vedeți aici și la curs).
Componente avansate (împreuna, cele doua componente valorează 2 puncte)
➡ Multigaming : Suportarea mai multor jocuri în același timp. Jucătorii se pot conecta oricând și aceștia vor fi distribuiți în jocuri diferite, în funcție de scorul acestora. Un utilizator poate aștepta cel mult 30 de secunde. După trecerea acestui timp jocul va porni dacă există cel puțin 2 doritori indiferent de scorul acestora.
➡ GUI - Să se implementeze o interfață grafică a aplicației, respectând cerințele de mai sus. Aceasta trebuie să fie personalizată și să NU se asemene cu versiunea originală a jocului. 
Idei pentru elementul custom
➡ Definirea unui nivel de dificultate: ușor, mediu sau greu. Valoarea acestuia poate fi aleasă la începutul unui joc sau poate crește progresiv pe măsură ce jocul avansează (rămâne la alegerea voastră să alegeți modul în care jocul surprinde jucătorii). 
➡ Tratarea excepțiilor și implementarea unit testelor (se vor utiliza framework-uri specifice, de ex. Visual Studio Enterprise Edition), code coverage 50% SAU minim 40 de teste diverse (funcționalitate, backend, frontend, etc.)
