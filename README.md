# Newton In The Maze
Un hoț i-a furat merele lui Newton și le-a ascuns într-un labirint. Rolul tău este să-l ajuți pe Newton să-și recupereze merele.

### Cum se joacă?
Personajul pe care îl poți controla este marcat cu litera T. Trebuie să colectezi obiectele marcate cu roșu (merele) și după să 
te duci spre ieșirea din labirint (va fi marcată cu verde în momentul în care toate merele au fost adunate). Uneori hoțul a pus merele
în locuri imposibil de accesat. În această situație poți lua o bombă (obiectul albastru plasat deasupra labirintului) și să
o detonezi la poziția la care te afli (ce va fi în jurul caracterului va fi distrus). Poți lua o bombă chiar dacă nu ai nevoie de ea. Atunci
când iei o bombă se va genera alta nouă peste 5 secunde (bomba se păstrează chiar dacă a fost una nouă generată).  

Se recomandă consola să fie pe modul fullscreen.

### Dimensiunea labirintului
Inițial programul cere un numar care reprezintă dimensiunea labirintului. Acesta poate fi cuprins între 10 sau 30 din cauza limitărilor
de rendering.

### Controale
Space - iei un obiect (trebuie să te afli peste obiect ca să-l poți lua);<br>
W, A, S, D - te miști sus, stânga, jos, dreapta;<br>
F - arunci o bombă care va sparge cei 8 vecini din jurul tău;<br>
Q - părăsești jocul.

### Important!
Aveți voie cu cod generat de modele de limbaj la care nu ați contribuit semnificativ doar în folder-ul `generated`.
Codul generat pus "ca să fie"/pe care nu îl înțelegeți se punctează doar pentru puncte bonus, doar în contextul
în care oferă funcționalități ajutătoare și doar dacă are sens.

O cerință nu se consideră îndeplinită dacă este realizată doar ca o serie de apeluri proxy către cod generat.

### Cerințe modele de limbaj la nivel de semestru
- [ ] minim o funcționalitate majoritar doar cu LLM
- [ ] minim 2-3 funcționalități ~50% voi, ~50% LLM-ul
- [ ] minim 2-3 funcționalități suficient de complicate pe care un LLM nu le poate implementa

### Tema 0

- [ ✔ ] Nume proiect (poate fi schimbat ulterior)
- [ ✔ ] Scurtă descriere a temei alese, ce v-ați propus să implementați

## Tema 1

#### Cerințe
- [ ✔ ] definirea a minim **3-4 clase** folosind compunere cu clasele definite de voi
- [ ✔ ] constructori de inițializare
- [ ✔ ] pentru o aceeași (singură) clasă: constructor de copiere, `operator=` de copiere, destructor
<!-- - [ ] pentru o altă clasă: constructor de mutare, `operator=` de mutare, destructor -->
<!-- - [ ] pentru o altă clasă: toate cele 5 funcții membru speciale -->
- [ ✔ ] `operator<<` pentru toate clasele pentru afișare (std::ostream)
- [ ✔ ] cât mai multe `const` (unde este cazul)
- [ ✔ ] implementarea a minim 3 funcții membru publice pentru funcționalități specifice temei alese, dintre care cel puțin 1-2 funcții mai complexe
  - nu doar citiri/afișări sau adăugat/șters elemente într-un/dintr-un vector
- [ ✔ ] scenariu de utilizare a claselor definite:
  - crearea de obiecte și apelarea tuturor funcțiilor membru publice în main
  - vor fi adăugate în fișierul `tastatura.txt` DOAR exemple de date de intrare de la tastatură (dacă există); dacă aveți nevoie de date din fișiere, creați alte fișiere separat
- [ ] tag de `git`: de exemplu `v0.1`
- [ ] serviciu de integrare continuă (CI); exemplu: GitHub Actions

## Tema 2

#### Cerințe
- [ ] separarea codului din clase în `.h` (sau `.hpp`) și `.cpp`
- [ ] moșteniri:
  - minim o clasă de bază și **3 clase derivate**
  - ierarhia trebuie să fie cu bază proprie, nu derivată dintr-o clasă predefinită
  - [ ] clasă cu atribut de tip pointer la o clasă de bază cu derivate
  - [ ] funcții virtuale (pure) apelate prin pointeri de bază din clasa care conține atributul de tip pointer de bază
    - minim o funcție virtuală va fi **specifică temei** (e.g. nu simple citiri/afișări)
    - constructori virtuali (clone): sunt necesari, dar nu se consideră funcții specifice temei
  - [ ] apelarea constructorului din clasa de bază din constructori din derivate
  - [ ] smart pointers (recomandat, opțional)
  - [ ] `dynamic_cast`/`std::dynamic_pointer_cast` pentru downcast cu sens
- [ ] suprascris cc/op= pentru copieri/atribuiri corecte, copy and swap
- [ ] excepții
  - [ ] ierarhie proprie cu baza `std::exception` sau derivată din `std::exception`; minim **3** clase pentru erori specifice
  - [ ] utilizare cu sens: de exemplu, `throw` în constructor (sau funcție care întoarce un obiect), `try`/`catch` în `main`
- [ ] funcții și atribute `static`
- [ ] STL
- [ ] cât mai multe `const`
- [ ] la sfârșit: commit separat cu adăugarea unei noi clase derivate fără a modifica restul codului, **pe lângă cele 3 derivate deja adăugate**
  - noua derivată nu poate fi una existentă care a fost ștearsă și adăugată din nou
- [ ] tag de `git`: de exemplu `v0.2`

## Tema 3

#### Cerințe
- [ ] 2 șabloane de proiectare (design patterns)
- [ ] o clasă șablon cu sens; minim **2 instanțieri**
  - [ ] preferabil și o funcție șablon (template) cu sens; minim 2 instanțieri
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [ ] tag de `git`: de exemplu `v0.3` sau `v1.0`

## Resurse

- [digestpp](https://github.com/kerukuro/digestpp/tree/ebb699402c244e22c3aff61d2239bcb2e87b8ef8) (Unlicense)
- [random](https://github.com/effolkronium/random/tree/b9e79f352278f48948689dca74fd27e6821d24e8) (MIT)
- [rlutil](https://github.com/tapio/rlutil/tree/821fdca0191b314ee07b0fad2abe4ea973e45575) (WTFPL)
- [csv-parser](https://github.com/vincentlaucsb/csv-parser/tree/9d5f796a32c6cdecd83a2f778ca6db0500948d27) (MIT)
- [date](https://github.com/HowardHinnant/date/tree/88a3b1512661decb78c60841f819a14eb200423c) (MIT)
- adăugați trimiteri către resursele externe care v-au ajutat sau pe care le-ați folosit
