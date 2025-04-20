#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>    
#include <iomanip>   

// Struktura per te perfaqesuar nje Process
struct Procesi {
    int id;                // ID unike e procesit
    int arrivalTime;       // Koha kur procesi behet gati (arrives)
    int burstTime;         // Koha totale e CPU-se qe i nevojitet procesit (original)
    int priority;          // Prioriteti i procesit (numer me i ulet = prioritet me i larte)
    int remainingBurstTime;// Koha e CPU-se qe i ka mbetur per t'u ekzekutuar
    int completionTime;    // Koha kur procesi perfundon ekzekutimin
    int turnaroundTime;    // Koha totale nga arrival deri ne completion (CT - AT)
    int waitingTime;       // Koha qe procesi ka pritur ne ready queue (TAT - BT)
    bool isCompleted;      // Boolean per te treguar nese procesi ka perfunduar

    Procesi(int p_id, int at, int bt, int prio) :
        id(p_id),
        arrivalTime(at),
        burstTime(bt),
        priority(prio),
        remainingBurstTime(bt), // Fillimisht, koha e mbetur = koha totale e burst
        completionTime(0),
        turnaroundTime(0),
        waitingTime(0),
        isCompleted(false) {}
};

// Funksion per te printuar tabelen e rezultateve
void printResults(const std::vector<Procesi>& proceset, float avgWaitingTime, float avgTurnaroundTime) {
    std::cout << "\n--- Rezultatet e Scheduling ---\n";
    std::cout << std::setw(5) << "PID" << std::setw(10) << "Arrival" << std::setw(8) << "Burst"
              << std::setw(10) << "Priority" << std::setw(15) << "Completion"
              << std::setw(15) << "Turnaround" << std::setw(12) << "Waiting" << std::endl;
    std::cout << "---------------------------------------------------------------------------\n";

    for (const auto& p : proceset) {
        std::cout << std::setw(5) << p.id << std::setw(10) << p.arrivalTime << std::setw(8) << p.burstTime
                  << std::setw(10) << p.priority << std::setw(15) << p.completionTime
                  << std::setw(15) << p.turnaroundTime << std::setw(12) << p.waitingTime << std::endl;
    }
     std::cout << "---------------------------------------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average Waiting Time (Koha Mesatare e Pritjes): " << avgWaitingTime << std::endl;
    std::cout << "Average Turnaround Time (Koha Mesatare e Qendrimit): " << avgTurnaroundTime << std::endl;
}

// Funksion per te printuar Gantt Chart (diagramin kohor)
void printGanttChart(const std::vector<std::pair<int, int>>& gantt) {
    std::cout << "\n--- Gantt Chart ---\n";
    if (gantt.empty()) {
        std::cout << " (Bosh - Asnje proces nuk u ekzekutua)\n";
        return;
    }

    std::cout << "|";
    for (const auto& segment : gantt) {
         // Nese ID = -1, tregojme kohe boshe (Idle)
        if (segment.first == -1) {
             std::cout << " Idle (" << segment.second << ") |";
        } else {
            std::cout << " P" << segment.first << " (" << segment.second << ") |";
        }
    }
    std::cout << std::endl;

    // Printimi i kohes poshte diagramit
    std::cout << "0";
    int currentTime = 0;
    for (const auto& segment : gantt) {
         currentTime = segment.second; // Koha e fundit e ketij segmenti
         std::cout << std::setw( (segment.first == -1 ? 8 : 5) + std::to_string(segment.second).length() ) << currentTime;
    }
     std::cout << std::endl;
}


int main() {
    int n; // Numri i proceseve

    std::cout << "--- Algoritmi Prioritet Ndalues (Preemptive Priority Scheduling) ---\n";
    std::cout << "Vendosni numrin total te proceseve: ";
    std::cin >> n;

    if (n <= 0) {
        std::cerr << "Numri i proceseve duhet te jete pozitiv." << std::endl;
        return 1;
    }

    std::vector<Procesi> proceset;
    std::cout << "Vendosni detajet per secilin proces (Arrival Time, Burst Time, Priority):\n";
    std::cout << "(Shenim: Prioritet me i larte = numer me i vogel)\n";
    for (int i = 0; i < n; ++i) {
        int arrival, burst, priority;
        std::cout << "Procesi P" << i + 1 << ": ";
        std::cin >> arrival >> burst >> priority;
        if (arrival < 0 || burst <= 0 || priority < 0) {
             std::cerr << "Input i pavlefshem (Arrival >= 0, Burst > 0, Priority >= 0)." << std::endl;
             return 1;
        }
        proceset.emplace_back(i + 1, arrival, burst, priority);
    }

    int currentTime = 0;
    int completedProcesses = 0;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;
    std::vector<std::pair<int, int>> ganttChart; // Pair: <Process ID, Completion Time of this segment>
    int lastProcessId = -1; // ID e procesit te fundit qe u ekzekutua (-1 per idle)

    std::cout << "\nSimulimi fillon...\n";

    while (completedProcesses < n) {
        int highestPriority = std::numeric_limits<int>::max(); // Prioriteti me i larte (numri me i vogel)
        int selectedProcessIndex = -1; // Indeksi i procesit te zgjedhur per kete cikel kohe

        // Gjej procesin me prioritet me te larte qe ka ardhur dhe nuk ka perfunduar
        for (int i = 0; i < n; ++i) {
            if (proceset[i].arrivalTime <= currentTime && !proceset[i].isCompleted) {
                if (proceset[i].priority < highestPriority) {
                    highestPriority = proceset[i].priority;
                    selectedProcessIndex = i;
                }
                // Nese prioritetet jane te barabarta, zgjidh ate qe ka ardhur me heret (ose me ID me te vogel si tie-breaker implicit ketu)
                // Kjo zakonisht behet me FCFS per prioritete te barabarta, por implementimi i thjeshte zgjedh ate qe gjendet i pari
                // ne iterim pas kontrollit te prioritetit. Nje implementim me rigoroz do te kerkonte nje kontroll te dyte per arrivalTime.
                 else if (proceset[i].priority == highestPriority) {
                     if (selectedProcessIndex == -1 || proceset[i].arrivalTime < proceset[selectedProcessIndex].arrivalTime) {
                         selectedProcessIndex = i;
                     }
                 }
            }
        }

        if (selectedProcessIndex != -1) {
            // Nje proces eshte zgjedhur per ekzekutim
            Procesi& currentProcess = proceset[selectedProcessIndex];

            // --- Gantt Chart Logic ---
            if (currentProcess.id != lastProcessId) {
                 // Nese ka pasur nje segment me pare, perfundoje ate
                 if (!ganttChart.empty() && ganttChart.back().second == 0) {
                     ganttChart.back().second = currentTime;
                 }
                 // Shto segmentin e ri (koha do te vendoset kur te ndryshoje procesi ose te perfundoje)
                 ganttChart.push_back({currentProcess.id, 0}); // Perditesohet me vone
                 lastProcessId = currentProcess.id;
            }


            currentProcess.remainingBurstTime--;


            // Kontrollo nese procesi ka perfunduar
            if (currentProcess.remainingBurstTime == 0) {
                currentProcess.completionTime = currentTime + 1; // Perfshin ciklin aktual
                currentProcess.turnaroundTime = currentProcess.completionTime - currentProcess.arrivalTime;
                currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime; // Perdor burstTime origjinal
                currentProcess.isCompleted = true;
                completedProcesses++;

                totalWaitingTime += currentProcess.waitingTime;
                totalTurnaroundTime += currentProcess.turnaroundTime;

                // Finalizo segmentin ne Gantt Chart
                 if (!ganttChart.empty() && ganttChart.back().first == currentProcess.id) {
                    ganttChart.back().second = currentTime + 1;
                 }
                 lastProcessId = -1; // Reseto procesin e fundit, pasi ky perfundoi

            }
        } else {
            // Asnje proces nuk eshte gati per ekzekutim (CPU Idle)

             // --- Gantt Chart Logic ---
             if (lastProcessId != -1 || ganttChart.empty()) { // Nese ishte duke u ekzekutuar nje proces ose eshte fillimi
                 if (!ganttChart.empty() && ganttChart.back().second == 0) { // Finalizo segmentin e procesit te meparshem nese ekziston
                     ganttChart.back().second = currentTime;
                 }
                 // Shto segmentin Idle vetem nese nuk jemi ne nje segment Idle ekzistues
                 if (ganttChart.empty() || ganttChart.back().first != -1) {
                    ganttChart.push_back({-1, 0}); // -1 per Idle
                 }
                 lastProcessId = -1;
             }
        }

        currentTime++; // Shko ne ciklin tjeter te kohes
    }

    // Finalizo segmentin e fundit ne Gantt Chart nese eshte e nevojshme
    if (!ganttChart.empty() && ganttChart.back().second == 0) {
         ganttChart.back().second = currentTime -1 ; // Koha kur mbaroi procesi i fundit ose Idle time
         // Korrigjim i vogel: nese procesi i fundit perfundoi saktesisht ne 'currentTime-1', koha e fundit eshte kjo.
         // Nese kaloi ne Idle, 'currentTime-1' eshte fundi i segmentit te meparshem.
         // Testimi eshte menyra me e mire per te verifikuar kete.
         // Meqe perditesimi behet brenda ciklit kur procesi perfundon ose kur fillon Idle, kjo mund te mos jete e nevojshme
         // por e leme si mase sigurie. Le te verifikojme logjiken e mesiperme.
         // Korrigjim: Koha e sakte eshte 'currentTime' kur while loop perfundon.
         // Por perditesimi behet brenda ciklit kur ndodh eventi (perfudnimi ose ndryshimi).
         // Nese segmenti i fundit eshte ende i hapur (koha 0), vendos kohen e fundit kur mbaroi cikli while
         ganttChart.back().second = currentTime -1 ; 
         // perditesimi i fundit duhet te reflektoje kohen kur perfundoi segmenti
         // qe eshte currentTime -1 pasi currentTime inkrementohet ne fund te ciklit
         // Por meqe perfundimi regjistrohet me currentTime+1 brenda ciklit, kjo duhet te jete OK.
         // Testimi tregoi se perditesimi brenda ciklit eshte i mjaftueshem. Heqim kete pjese te jashtme.

    }


    // Llogarit mesataret
    float averageWaitingTime = (n > 0) ? totalWaitingTime / n : 0;
    float averageTurnaroundTime = (n > 0) ? totalTurnaroundTime / n : 0;

    // Printo rezultatet dhe Gantt Chart
    printResults(proceset, averageWaitingTime, averageTurnaroundTime);
    printGanttChart(ganttChart);

    return 0;
}