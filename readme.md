# Implementimi i Algoritmit Prioritet Ndalues (Preemptive Priority Scheduling)

Ky projekt permban implementimin ne C++ te algoritmit te CPU scheduling **Prioritet Ndalues (Preemptive Priority Scheduling)**, si pjese e Detyres 2 (Praktike 2) per lenden Sistemet Operative. Qellimi i ketij kodi eshte te simuloje funksionimin e algoritmit Preemptive Priority Scheduling. Ky algoritem perdoret nga sistemet operative per te menaxhuar ekzekutimin e proceseve te ndryshme ne CPU, duke u dhene perparesi proceseve me **priority** me te larte dhe duke lejuar nderprerjen (preemption) e proceseve me prioritet me te ulet.

Gjuha e perdorur per implementim eshte **C++**.

## Pershkrimi i Algoritmit (Prioritet Ndalues - Preemptive Priority)

Algoritmi Preemptive Priority Scheduling funksionon sipas ketyre parimeve:

1.  **Priority:** cdo procesi i caktohet nje numer prioriteti. Ne kete implementim, **nje numer me i vogel tregon nje prioritet me te larte**.
2.  **Zgjedhja (Selection):** Nga te gjitha proceset qe jane gati per t'u ekzekutuar (kane mberritur - `arrivalTime` <= koha aktuale) dhe nuk kane perfunduar ende, zgjidhet ai me prioritetin me te larte (numrin me te vogel).
3.  **Ndalimi/Nderprerja (Preemption):** Nese nje proces i ri arrin ne sistem (ose behet gati) dhe ka nje **priority me te larte** se procesi qe aktualisht po ekzekutohet ne CPU, procesi aktual nderpritet menjehere (preempted). Procesi i nderprere kthehet ne listen e proceseve gati (ready queue), dhe procesi i ri me prioritet me te larte fillon ekzekutimin.
4.  **Prioritete te Barabarta:** Nese ka me shume se nje proces me prioritetin me te larte ne ready queue, ato zakonisht trajtohen sipas parimit First-Come, First-Served (FCFS) bazuar ne `arrivalTime`. Ky implementim zgjedh procesin qe gjendet me pare gjate kerkimit ose qe ka `arrivalTime` me te hershme ne rast barazie prioriteti.

## Detajet e Implementimit

- **Struktura e te Dhenave (`struct Procesi`):** Perdoret nje `struct` me emrin `Procesi` per te ruajtur informacionin relevant per cdo proces:
  - `id`: Identifikues unik.
  - `arrivalTime`: Koha e mberritjes.
  - `burstTime`: Koha totale e nevojshme e CPU (origjinale).
  - `priority`: Prioriteti i procesit.
  - `remainingBurstTime`: Koha e CPU qe i ka mbetur.
  - `completionTime`: Koha kur procesi perfundon.
  - `turnaroundTime`: Koha totale ne sistem (`completionTime - arrivalTime`).
  - `waitingTime`: Koha totale e pritjes (`turnaroundTime - burstTime`).
  - `isCompleted`: Nje boolean qe tregon nese procesi ka perfunduar.
- **Logjika Kryesore:**
  - Programi mban gjurme te kohes aktuale (`currentTime`).
  - Ne cdo hap kohor, programi kerkon per procesin me prioritetin me te larte (numrin me te vogel) midis atyre qe kane mberritur (`arrivalTime <= currentTime`) dhe nuk kane perfunduar (`isCompleted == false`).
  - Nese gjendet nje proces i tille, `remainingBurstTime` i tij zvogelohet me 1.
  - Nese `remainingBurstTime` behet 0, procesi shenohet si i perfunduar dhe llogariten `completionTime`, `turnaroundTime`, dhe `waitingTime`.
  - Nese asnje proces nuk eshte gati, CPU konsiderohet `Idle` (bosh) dhe koha `currentTime` thjesht rritet.
  - Mekanizmi **preemptive** realizohet sepse ne _cdo_ hap kohor, behet ri-evaluimi se cili proces ka prioritetin me te larte midis _te gjithe_ proceseve te gatshme.
- **Input:** Programi kerkon nga perdoruesi numrin total te proceseve dhe me pas, per secilin proces, kerkon `Arrival Time`, `Burst Time`, dhe `Priority`.
- **Output:** Programi printon:
  - Nje tabele permbledhese me te gjitha metrikat per cdo proces (`PID`, `Arrival`, `Burst`, `Priority`, `Completion`, `Turnaround`, `Waiting`).
  - Average `Waiting Time` dhe Average `Turnaround Time` per te gjitha proceset.
  - Nje paraqitje vizuale te ekzekutimit ne kohe (**Gantt Chart**).

## Shembull

**Input Shembull:**

Vendosni numrin total te proceseve: 5
Vendosni detajet per secilin proces (Arrival Time, Burst Time, Priority):
(Shenim: Prioritet me i larte = numer me i vogel)
Procesi P1: 0 4 2
Procesi P2: 1 3 1
Procesi P3: 2 1 4
Procesi P4: 3 5 3
Procesi P5: 4 2 5

**Output i Pritshem (mund te ndryshoje pak formatimi):**

--- Rezultatet e Scheduling ---
PID Arrival Burst Priority Completion Turnaround Waiting

---

    1         0      4         2            7             7           3
    2         1      3         1            4             3           0
    3         2      1         4           10             8           7
    4         3      5         3           15            12           7
    5         4      2         5           17            13          11

---

Average Waiting Time (Koha Mesatare e Pritjes): 5.60
Average Turnaround Time (Koha Mesatare e Qendrimit): 8.60

--- Gantt Chart ---
| P1 (1) | P2 (4) | P1 (7) | P4 (10) | P3 (11) | P4 (15) | P5 (17) |
0 1 4 7 10 11 15 17

_(Shenim: Gantt Chart-i i mesiperm eshte nje interpretim. Rezultati aktual i programit mund te varioje pak ne paraqitje, por duhet te reflektoje te njejten sekuence dhe kohe perfundimi per segmentet)._
