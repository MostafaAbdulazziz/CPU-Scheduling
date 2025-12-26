#include "lab7.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <queue>
#include <math.h>

using namespace std;

void Scheduler::execute()
{
    for (int i = 0; i < schedulongPoliceis.size(); i++)
    {
        clearTables();
        if (schedulongPoliceis[i].first == 1)
        {
            FCFS();
            if (type == "trace")
                trace(1, -1);
            else
                stats(1, -1);
        }
        else if (schedulongPoliceis[i].first == 2)
        {
            RR(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(2, schedulongPoliceis[i].second);
            else
                stats(2, schedulongPoliceis[i].second);
        }
        else if (schedulongPoliceis[i].first == 3)
        {
            SPN();
            if (type == "trace")
                trace(3, -1);
            else
                stats(3, -1);
        }
        else if (schedulongPoliceis[i].first == 4)
        {
            SRT();
            if (type == "trace")
                trace(4, -1);
            else
                stats(4, -1);
        }
        else if (schedulongPoliceis[i].first == 5)
        {
            HRRN();
            if (type == "trace")
                trace(5, -1);
            else
                stats(5, -1);
        }
        else if (schedulongPoliceis[i].first == 6)
        {
            FB1();
            if (type == "trace")
                trace(6, -1);
            else
                stats(6, -1);
        }
        else if (schedulongPoliceis[i].first == 7)
        {
            FB2i();
            if (type == "trace")
                trace(7, -1);
            else
                stats(7, -1);
        }
        else if (schedulongPoliceis[i].first == 8)
        {
            AGE(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(8, -1);
        }
    }
}
void Scheduler::trace(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 2)
    {
        if (argument > 10)
            cout << "RR-" << argument << " ";
        else
            cout << "RR-" << argument << "  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 3)
    {
        cout << "SPN   ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 4)
    {
        cout << "SRT   ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 5)
    {
        cout << "HRRN  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 6)
    {
        cout << "FB-1  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 7)
    {
        cout << "FB-2i ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 8)
    {
        cout << "Aging ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
}
void Scheduler::stats(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 2)
    {
        cout << "RR-" << argument << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 3)
    {
        cout << "SPN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 4)
    {
        cout << "SRT" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 5)
    {
        cout << "HRRN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 6)
    {
        cout << "FB-1" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 7)
    {
        cout << "FB-2i" << endl;
        printStats();
        cout << '\n';
    }
}

void Scheduler::FCFS()
{
    deque<int> process_queue;
    int active_pid = -1;
    int clock = 0;

    while (clock < maxSeconds)
    {
        //  Handle Arrivals
        for (int i = 0; i < numberOfProcesses; ++i) {
            if (processes[i].arrivalTime == clock) {
                process_queue.push_back(i);
            }
        }

        //Select Process if CPU Idle
        if (active_pid == -1 && !process_queue.empty()) {
            active_pid = process_queue.front();
            process_queue.pop_front();
        }

        // Record State (Trace)
        //Mark running
        if (active_pid != -1) {
            processesPrintingArray[active_pid * maxSeconds + clock] = '*';
        }
        // Mark waiting
        for (int pid : process_queue) {
            processesPrintingArray[pid * maxSeconds + clock] = '.';
        }

        if (active_pid != -1) {
            processes[active_pid].remainingTime--;
            
            if (processes[active_pid].remainingTime == 0) {
                processes[active_pid].finishTime = clock + 1;
                processes[active_pid].turnAroundTime = processes[active_pid].finishTime - processes[active_pid].arrivalTime;
                processes[active_pid].NormTurnTime = (float)processes[active_pid].turnAroundTime / processes[active_pid].serveTime;
                active_pid = -1; // Release CPU
            }
        }
        clock++;
    }
}

void Scheduler::RR(int quantum)
{
    deque<int> process_queue;
    int active_pid = -1;
    int q_timer = 0;
    int clock = 0;

    while (clock < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; ++i) {
            if (processes[i].arrivalTime == clock) {
                process_queue.push_back(i);
            }
        }

        if (active_pid != -1) {
            if (q_timer == quantum) {
                process_queue.push_back(active_pid);
                active_pid = -1;
                q_timer = 0;
            }
        }

        if (active_pid == -1 && !process_queue.empty()) {
            active_pid = process_queue.front();
            process_queue.pop_front();
            q_timer = 0;
        }

        if (active_pid != -1) processesPrintingArray[active_pid * maxSeconds + clock] = '*';
        for (int pid : process_queue) processesPrintingArray[pid * maxSeconds + clock] = '.';

        if (active_pid != -1) {
            processes[active_pid].remainingTime--;
            q_timer++;

            if (processes[active_pid].remainingTime == 0) {
                processes[active_pid].finishTime = clock + 1;
                processes[active_pid].turnAroundTime = processes[active_pid].finishTime - processes[active_pid].arrivalTime;
                processes[active_pid].NormTurnTime = (float)processes[active_pid].turnAroundTime / processes[active_pid].serveTime;
                active_pid = -1;
                q_timer = 0;
            }
        }
        clock++;
    }
}

void Scheduler::SPN()
{
    vector<int> ready_pool;
    int active_pid = -1;
    int clock = 0;

    while (clock < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; ++i) {
            if (processes[i].arrivalTime == clock) {
                ready_pool.push_back(i);
            }
        }

        if (active_pid == -1 && !ready_pool.empty()) {
            int shortest_idx = 0;
            // Find shortest job
            for (size_t k = 1; k < ready_pool.size(); ++k) {
                int curr_id = ready_pool[k];
                int best_id = ready_pool[shortest_idx];
                if (processes[curr_id].serveTime < processes[best_id].serveTime) {
                    shortest_idx = k;
                }
            }
            active_pid = ready_pool[shortest_idx];
            ready_pool.erase(ready_pool.begin() + shortest_idx);
        }

        if (active_pid != -1) processesPrintingArray[active_pid * maxSeconds + clock] = '*';
        for (int pid : ready_pool) processesPrintingArray[pid * maxSeconds + clock] = '.';

        if (active_pid != -1) {
            processes[active_pid].remainingTime--;
            if (processes[active_pid].remainingTime == 0) {
                processes[active_pid].finishTime = clock + 1;
                processes[active_pid].turnAroundTime = processes[active_pid].finishTime - processes[active_pid].arrivalTime;
                processes[active_pid].NormTurnTime = (float)processes[active_pid].turnAroundTime / processes[active_pid].serveTime;
                active_pid = -1;
            }
        }
        clock++;
    }
}

void Scheduler::SRT()
{
    vector<int> ready_pool;
    int active_pid = -1;
    int clock = 0;

    while (clock < maxSeconds)
    {
        
        for (int i = 0; i < numberOfProcesses; ++i) {
            if (processes[i].arrivalTime == clock) {
                ready_pool.push_back(i);
            }
        }

       
        // Check if any waiting process is strictly better than current active
        if (active_pid != -1) {
            int current_rem = processes[active_pid].remainingTime;
            bool better_found = false;
            int better_vec_idx = -1;

            for (size_t k = 0; k < ready_pool.size(); ++k) {
                if (processes[ready_pool[k]].remainingTime < current_rem) {
                    current_rem = processes[ready_pool[k]].remainingTime;
                    better_vec_idx = k;
                    better_found = true;
                }
            }

            if (better_found) {
                ready_pool.push_back(active_pid);
                active_pid = ready_pool[better_vec_idx];
                ready_pool.erase(ready_pool.begin() + better_vec_idx);
            }
        }

        if (active_pid == -1 && !ready_pool.empty()) {
            int shortest_idx = 0;
            for (size_t k = 1; k < ready_pool.size(); ++k) {
                int curr_id = ready_pool[k];
                int best_id = ready_pool[shortest_idx];
                if (processes[curr_id].remainingTime < processes[best_id].remainingTime) {
                    shortest_idx = k;
                }
            }
            active_pid = ready_pool[shortest_idx];
            ready_pool.erase(ready_pool.begin() + shortest_idx);
        }

        if (active_pid != -1) {
            processesPrintingArray[active_pid * maxSeconds + clock] = '*';
            processes[active_pid].remainingTime--;

            if (processes[active_pid].remainingTime == 0) {
                processes[active_pid].finishTime = clock + 1;
                processes[active_pid].turnAroundTime = processes[active_pid].finishTime - processes[active_pid].arrivalTime;
                processes[active_pid].NormTurnTime = (float)processes[active_pid].turnAroundTime / processes[active_pid].serveTime;
                active_pid = -1;
            }
        }
        for (int pid : ready_pool) processesPrintingArray[pid * maxSeconds + clock] = '.';
        
        clock++;
    }
}

void Scheduler::HRRN()
{
    vector<int> ready_pool;
    int active_pid = -1;
    int clock = 0;

    while (clock < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; ++i) {
            if (processes[i].arrivalTime == clock) ready_pool.push_back(i);
        }

        if (active_pid == -1 && !ready_pool.empty()) {
            int best_vec_idx = 0;
            double max_ratio = -1.0;

            for (size_t k = 0; k < ready_pool.size(); ++k) {
                int pid = ready_pool[k];
                double waiting_time = (double)(clock - processes[pid].arrivalTime);
                double service_time = (double)processes[pid].serveTime;
                double ratio = (waiting_time + service_time) / service_time;

                if (ratio > max_ratio) {
                    max_ratio = ratio;
                    best_vec_idx = k;
                }
            }
            active_pid = ready_pool[best_vec_idx];
            ready_pool.erase(ready_pool.begin() + best_vec_idx);
        }

        if (active_pid != -1) {
            processesPrintingArray[active_pid * maxSeconds + clock] = '*';
            processes[active_pid].remainingTime--;

            if (processes[active_pid].remainingTime == 0) {
                processes[active_pid].finishTime = clock + 1;
                processes[active_pid].turnAroundTime = processes[active_pid].finishTime - processes[active_pid].arrivalTime;
                processes[active_pid].NormTurnTime = (float)processes[active_pid].turnAroundTime / processes[active_pid].serveTime;
                active_pid = -1;
            }
        }
        for (int pid : ready_pool) processesPrintingArray[pid * maxSeconds + clock] = '.';

        clock++;
    }
}

void Scheduler::FB1()
{
    vector<deque<int>> mlq(100);
    int active_pid = -1;
    int clock = 0;
    
    // Reset levels
    for(int i=0; i<numberOfProcesses; ++i) processes[i].FBLevel = 0;

    while (clock < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; ++i) {
            if (processes[i].arrivalTime == clock) {
                processes[i].FBLevel = 0;
                mlq[0].push_back(i);
            }
        }

        if (active_pid != -1) {
            bool need_preemption = false;
            // Check if anyone waiting in queues
            for(const auto& q : mlq) {
                if(!q.empty()) {
                    need_preemption = true;
                    break;
                }
            }

            if (need_preemption) {
                // Demote
                int lvl = processes[active_pid].FBLevel;
                int next = (lvl + 1 < 100) ? lvl + 1 : lvl;
                processes[active_pid].FBLevel = next;
                mlq[next].push_back(active_pid);
                active_pid = -1;
            } else {
                // If nobody waiting, return to own queue (Round Robin style behavior for FB-1)
                mlq[processes[active_pid].FBLevel].push_back(active_pid);
                active_pid = -1;
            }
        }

        if (active_pid == -1) {
            for(int lvl = 0; lvl < 100; ++lvl) {
                if(!mlq[lvl].empty()) {
                    active_pid = mlq[lvl].front();
                    mlq[lvl].pop_front();
                    processes[active_pid].FBLevel = lvl;
                    break;
                }
            }
        }

        for (auto &q : mlq) {
            for (int pid : q) processesPrintingArray[pid * maxSeconds + clock] = '.';
        }

        if (active_pid != -1) {
            processesPrintingArray[active_pid * maxSeconds + clock] = '*';
            processes[active_pid].remainingTime--;

            if (processes[active_pid].remainingTime == 0) {
                processes[active_pid].finishTime = clock + 1;
                processes[active_pid].turnAroundTime = processes[active_pid].finishTime - processes[active_pid].arrivalTime;
                processes[active_pid].NormTurnTime = (float)processes[active_pid].turnAroundTime / processes[active_pid].serveTime;
                active_pid = -1;
            }
        }
        clock++;
    }
}

void Scheduler::FB2i()
{
    vector<deque<int>> mlq(100);
    int active_pid = -1;
    int q_timer = 0;
    int clock = 0;
    
    for(int i=0; i<numberOfProcesses; ++i) processes[i].FBLevel = 0;

    while (clock < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; ++i) {
            if (processes[i].arrivalTime == clock) {
                processes[i].FBLevel = 0;
                mlq[0].push_back(i);
            }
        }

        if (active_pid != -1) {
            int lvl = processes[active_pid].FBLevel;
            int limit = 1 << lvl; // 2^lvl

            if (q_timer >= limit) {
                bool higher_priority_exists = false;
                for(const auto& q : mlq) {
                    if(!q.empty()) {
                        higher_priority_exists = true;
                        break;
                    }
                }

                if (higher_priority_exists) {
                    // Preempted or quantum done with others waiting 
                    int next = (lvl + 1 < 100) ? lvl + 1 : lvl;
                    processes[active_pid].FBLevel = next;
                    mlq[next].push_back(active_pid);
                    active_pid = -1;
                } else {
                    // Just quantum done, nobody else waiting
                    mlq[lvl].push_back(active_pid);
                    active_pid = -1;
                }
            }
        }

        if (active_pid == -1) {
            for(int lvl=0; lvl<100; ++lvl) {
                if(!mlq[lvl].empty()) {
                    active_pid = mlq[lvl].front();
                    mlq[lvl].pop_front();
                    processes[active_pid].FBLevel = lvl;
                    q_timer = 0;
                    break;
                }
            }
        }

        for (auto &q : mlq) {
            for (int pid : q) processesPrintingArray[pid * maxSeconds + clock] = '.';
        }

        if (active_pid != -1) {
            processesPrintingArray[active_pid * maxSeconds + clock] = '*';
            processes[active_pid].remainingTime--;
            q_timer++;

            if (processes[active_pid].remainingTime == 0) {
                processes[active_pid].finishTime = clock + 1;
                processes[active_pid].turnAroundTime = processes[active_pid].finishTime - processes[active_pid].arrivalTime;
                processes[active_pid].NormTurnTime = (float)processes[active_pid].turnAroundTime / processes[active_pid].serveTime;
                active_pid = -1;
                q_timer = 0;
            }
        }
        clock++;
    }
}

void Scheduler::AGE(int quantum)
{
    vector<int> ready_list;
    int active_pid = -1;
    int q_timer = 0;
    int clock = 0;

    // Reset priorities to base
    for (int i = 0; i < numberOfProcesses; i++) {
        processes[i].currentPriority = processes[i].priority;
        processes[i].processState = ' '; // tracking state locally
    }

    while (clock < maxSeconds)
    {
        //Add Arrivals to Ready List
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == clock && processes[i].processState == ' ') {
                processes[i].processState = 'R'; // Mark as ready
                ready_list.push_back(i);
            }
        }

        if (active_pid == -1 || q_timer >= quantum)
        {
            // Boost Priority of waiting processes
            for(int idx : ready_list) {
                processes[idx].currentPriority++;
            }

            // If active process timed out, reset its priority and requeue
            if (active_pid != -1) {
                processes[active_pid].currentPriority = processes[active_pid].priority;
                ready_list.push_back(active_pid);
                active_pid = -1;
            }

            // Select highest priority
            if (!ready_list.empty()) {
                int best_vec_idx = 0;
                for(size_t k = 1; k < ready_list.size(); ++k) {
                    if(processes[ready_list[k]].currentPriority > processes[ready_list[best_vec_idx]].currentPriority) {
                        best_vec_idx = k;
                    }
                }
                
                active_pid = ready_list[best_vec_idx];
                ready_list.erase(ready_list.begin() + best_vec_idx);
                q_timer = 0;
            }
        }

        if (active_pid != -1) {
            processesPrintingArray[active_pid * maxSeconds + clock] = '*';
            q_timer++;
            

        }
        for (int pid : ready_list) processesPrintingArray[pid * maxSeconds + clock] = '.';

        clock++;
    }
}

void Scheduler::printTracing()
{
    for (int process = 0; process < numberOfProcesses; process++)
    {
        cout << processes[process].processName << "     |";
        for (int time = 0; time < maxSeconds; time++)
            cout << *(processesPrintingArray + process * maxSeconds + time) << '|';
        cout << " \n";
    }
}
void Scheduler::printStats()
{
    float sum, mean, sum2;
    cout << "Process    |";
    for (int i = 0; i < numberOfProcesses; i++)
        cout << "  " << processes[i].processName << "  |";
    cout << endl;
    cout << "Arrival    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].arrivalTime << "  |";
        else
            cout << " " << processes[i].arrivalTime << "  |";
    }
    cout << endl;
    cout << "Service    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].serveTime << "  |";
        else
            cout << " " << processes[i].serveTime << "  |";
    }
    cout << " Mean|" << endl;
    cout << "Finish     |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].finishTime >= 10)
            cout << " " << processes[i].finishTime << "  |";
        else
            cout << "  " << processes[i].finishTime << "  |";
    }
    cout << "-----|" << endl;
    cout << "Turnaround |";
    for (int i = 0; i < numberOfProcesses; i++)
    {

        if (processes[i].turnAroundTime >= 10)
            cout << " " << (int)processes[i].turnAroundTime << "  |";
        else
            cout << "  " << (int)processes[i].turnAroundTime << "  |";
        sum += processes[i].turnAroundTime;
    }
    cout << fixed;
    cout << setprecision(2);
    mean = (sum * 1.0) / numberOfProcesses;
    if (mean >= 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
    cout << "NormTurn   |";
    sum2 = 0;
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].NormTurnTime > 10)
            cout << processes[i].NormTurnTime << "|";
        else
            cout << " " << processes[i].NormTurnTime << "|";
        sum2 += (processes[i].NormTurnTime * 1.0);
    }

    mean = (sum2 * 1.0) / numberOfProcesses;
    if (mean > 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
}
void Scheduler::clearTables()
{

    for (int i = 0; i < numberOfProcesses; i++)
    {
        for (int j = 0; j < maxSeconds; j++)
            *(processesPrintingArray + i * maxSeconds + j) = ' ';
    }
    for (int i = 0; i < numberOfProcesses; i++)
    {
        processes[i].finishTime = 0;
        processes[i].turnAroundTime = 0;
        processes[i].NormTurnTime = 0;
        processes[i].processState = ' ';
        processes[i].remainingTime = processes[i].serveTime;
    }
    processorBusy = false;
    while (!readyQueue.empty())
        readyQueue.pop();
    while (!readyPriorityQueue.empty())
        readyPriorityQueue.pop();
}
void Scheduler::splitPolicyAndParameter(string str)
{
    string w = "";
    pair<int, int> policies;
    bool parameterExists = false;
    policies.second = -1;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '-')
        {
            parameterExists = true;
            policies.first = stoi(w);
            w = "";
        }
        else if (str[i] == ',')
        {
            if (parameterExists)
                policies.second = stoi(w);
            else
            {
                policies.first = stoi(w);
                policies.second = -1;
            }
            w = "";
            schedulongPoliceis.push_back(policies);
            parameterExists = false;
        }
        else
            w = w + str[i];
    }
    if (parameterExists)
        policies.second = stoi(w);
    else
        policies.first = stoi(w);
    schedulongPoliceis.push_back(policies);
}
void Scheduler::splitPrcoessAndTimes(string str, int id)
{
    Process process;
    string w = "";
    process.processName = str[0];
    for (int i = 2; i < str.length(); i++)
    {
        if (str[i] == ',')
        {
            process.arrivalTime = stoi(w);
            w = "";
        }
        else
            w = w + str[i];
    }
    processorBusy = false;
    process.processState = ' ';
    if (schedulongPoliceis[0].first == 8)
    {
        process.priority = stoi(w);
        process.currentPriority = stoi(w);
    }
    else
        process.serveTime = stoi(w);
    process.remainingTime = process.serveTime;
    process.waitingTime = 0;
    process.id = id;
    processes.push_back(process);
}
void Scheduler::readFile()
{
    processorBusy = false;
    string temp1, temp2;
    cin >> type;
    cin >> temp1;
    splitPolicyAndParameter(temp1);
    cin >> maxSeconds;
    cin >> numberOfProcesses;

    for (int i = 0; i < numberOfProcesses; i++)
    {
        cin >> temp2;
        splitPrcoessAndTimes(temp2, i);
    }
    processesPrintingArray = new char[numberOfProcesses * maxSeconds];
    currentProcess.processName = 0;
    currentProcess.q = 0;
    clearTables();
}
void Scheduler::printHeader()
{
    for (int i = 0; i < maxSeconds + 1; i++)
        cout << i % 10 << ' ';
    cout << "\n";
    printDashes();
}
void Scheduler::printDashes()
{
    cout << "------------------------------------------------";
}
int main(void)
{
    Scheduler scheduler;
    scheduler.readFile();
    scheduler.execute();
    return 0;
}