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
    queue<int> readyQ;
    int currentProcessIdx = -1;

    for (int time = 0; time < maxSeconds; time++)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                readyQ.push(i);
            }
        }

        if (currentProcessIdx == -1 && !readyQ.empty()) {
            currentProcessIdx = readyQ.front();
            readyQ.pop();
        }

        if (currentProcessIdx != -1) {
            *(processesPrintingArray + currentProcessIdx * maxSeconds + time) = '*';
            processes[currentProcessIdx].remainingTime--;

            if (processes[currentProcessIdx].remainingTime == 0) {
                processes[currentProcessIdx].finishTime = time + 1;
                processes[currentProcessIdx].turnAroundTime = processes[currentProcessIdx].finishTime - processes[currentProcessIdx].arrivalTime;
                processes[currentProcessIdx].NormTurnTime = processes[currentProcessIdx].turnAroundTime / processes[currentProcessIdx].serveTime;
                currentProcessIdx = -1;
            }
        }
        
        queue<int> tempQ = readyQ;
        while(!tempQ.empty()) {
            int idx = tempQ.front(); tempQ.pop();
            *(processesPrintingArray + idx * maxSeconds + time) = '.';
        }
    }
}

void Scheduler::RR(int quantum)
{
    queue<int> readyQ;
    int currentProcessIdx = -1;
    int timeInQuantum = 0;

    for (int time = 0; time < maxSeconds; time++)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                readyQ.push(i);
            }
        }

        if (currentProcessIdx != -1) {
            if (timeInQuantum == quantum) {
                readyQ.push(currentProcessIdx);
                currentProcessIdx = -1;
                timeInQuantum = 0;
            }
        }

        if (currentProcessIdx == -1 && !readyQ.empty()) {
            currentProcessIdx = readyQ.front();
            readyQ.pop();
            timeInQuantum = 0;
        }

        if (currentProcessIdx != -1) {
            *(processesPrintingArray + currentProcessIdx * maxSeconds + time) = '*';
            processes[currentProcessIdx].remainingTime--;
            timeInQuantum++;

            if (processes[currentProcessIdx].remainingTime == 0) {
                processes[currentProcessIdx].finishTime = time + 1;
                processes[currentProcessIdx].turnAroundTime = processes[currentProcessIdx].finishTime - processes[currentProcessIdx].arrivalTime;
                processes[currentProcessIdx].NormTurnTime = processes[currentProcessIdx].turnAroundTime / processes[currentProcessIdx].serveTime;
                currentProcessIdx = -1;
                timeInQuantum = 0;
            }
        }

        queue<int> tempQ = readyQ;
        while (!tempQ.empty()) {
            int idx = tempQ.front(); tempQ.pop();
            *(processesPrintingArray + idx * maxSeconds + time) = '.';
        }
    }
}

void Scheduler::SPN()
{
    vector<int> readyList;
    int currentProcessIdx = -1;

    for (int time = 0; time < maxSeconds; time++)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                readyList.push_back(i);
            }
        }

        if (currentProcessIdx == -1 && !readyList.empty()) {
            int bestIdx = 0;
            for (int i = 1; i < readyList.size(); i++) {
                if (processes[readyList[i]].serveTime < processes[readyList[bestIdx]].serveTime) {
                    bestIdx = i;
                }
            }
            currentProcessIdx = readyList[bestIdx];
            readyList.erase(readyList.begin() + bestIdx);
        }

        if (currentProcessIdx != -1) {
            *(processesPrintingArray + currentProcessIdx * maxSeconds + time) = '*';
            processes[currentProcessIdx].remainingTime--;

            if (processes[currentProcessIdx].remainingTime == 0) {
                processes[currentProcessIdx].finishTime = time + 1;
                processes[currentProcessIdx].turnAroundTime = processes[currentProcessIdx].finishTime - processes[currentProcessIdx].arrivalTime;
                processes[currentProcessIdx].NormTurnTime = processes[currentProcessIdx].turnAroundTime / processes[currentProcessIdx].serveTime;
                currentProcessIdx = -1;
            }
        }

        for(int idx : readyList) {
            *(processesPrintingArray + idx * maxSeconds + time) = '.';
        }
    }
}

void Scheduler::SRT()
{
    vector<int> readyList;
    int currentProcessIdx = -1;

    for (int time = 0; time < maxSeconds; time++)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                readyList.push_back(i);
            }
        }

        if (currentProcessIdx != -1) {
            bool preempt = false;
            int bestReadyIdx = -1;
            int minRem = processes[currentProcessIdx].remainingTime;
            
            for(int i=0; i<readyList.size(); i++) {
                if (processes[readyList[i]].remainingTime < minRem) {
                    minRem = processes[readyList[i]].remainingTime;
                    bestReadyIdx = i;
                    preempt = true;
                }
            }
            
            if (preempt) {
                readyList.push_back(currentProcessIdx);
                currentProcessIdx = readyList[bestReadyIdx];
                readyList.erase(readyList.begin() + bestReadyIdx);
            }
        } else if (!readyList.empty()) {
            int bestIdx = 0;
            for (int i = 1; i < readyList.size(); i++) {
                if (processes[readyList[i]].remainingTime < processes[readyList[bestIdx]].remainingTime) {
                    bestIdx = i;
                }
            }
            currentProcessIdx = readyList[bestIdx];
            readyList.erase(readyList.begin() + bestIdx);
        }

        if (currentProcessIdx != -1) {
            *(processesPrintingArray + currentProcessIdx * maxSeconds + time) = '*';
            processes[currentProcessIdx].remainingTime--;

            if (processes[currentProcessIdx].remainingTime == 0) {
                processes[currentProcessIdx].finishTime = time + 1;
                processes[currentProcessIdx].turnAroundTime = processes[currentProcessIdx].finishTime - processes[currentProcessIdx].arrivalTime;
                processes[currentProcessIdx].NormTurnTime = processes[currentProcessIdx].turnAroundTime / processes[currentProcessIdx].serveTime;
                currentProcessIdx = -1;
            }
        }

        for(int idx : readyList) {
            *(processesPrintingArray + idx * maxSeconds + time) = '.';
        }
    }
}

void Scheduler::HRRN()
{
    vector<int> readyList;
    int currentProcessIdx = -1;

    for (int time = 0; time < maxSeconds; time++)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                readyList.push_back(i);
            }
        }

        if (currentProcessIdx == -1 && !readyList.empty()) {
            int bestIdx = 0;
            double maxRatio = -1.0;

            for (int i = 0; i < readyList.size(); i++) {
                int pIdx = readyList[i];
                double W = (double)(time - processes[pIdx].arrivalTime);
                double S = (double)processes[pIdx].serveTime;
                double ratio = (W + S) / S;

                if (ratio > maxRatio) {
                    maxRatio = ratio;
                    bestIdx = i;
                }
            }
            currentProcessIdx = readyList[bestIdx];
            readyList.erase(readyList.begin() + bestIdx);
        }

        if (currentProcessIdx != -1) {
            *(processesPrintingArray + currentProcessIdx * maxSeconds + time) = '*';
            processes[currentProcessIdx].remainingTime--;

            if (processes[currentProcessIdx].remainingTime == 0) {
                processes[currentProcessIdx].finishTime = time + 1;
                processes[currentProcessIdx].turnAroundTime = processes[currentProcessIdx].finishTime - processes[currentProcessIdx].arrivalTime;
                processes[currentProcessIdx].NormTurnTime = processes[currentProcessIdx].turnAroundTime / processes[currentProcessIdx].serveTime;
                currentProcessIdx = -1;
            }
        }

        for(int idx : readyList) {
            *(processesPrintingArray + idx * maxSeconds + time) = '.';
        }
    }
}

void Scheduler::FB1()
{
    vector<queue<int>> queues(numberOfProcesses + 5);
    int currentProcessIdx = -1;
    int currentQLevel = 0;
    int timeInQuantum = 0;

    for (int time = 0; time < maxSeconds; time++)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                queues[0].push(i);
            }
        }

        if (currentProcessIdx != -1) {
            if (timeInQuantum == 1) {
                int nextLevel = currentQLevel + 1;
                if (nextLevel >= numberOfProcesses) nextLevel = numberOfProcesses;
                queues[nextLevel].push(currentProcessIdx);
                currentProcessIdx = -1;
                timeInQuantum = 0;
            }
        }
        
        if (currentProcessIdx == -1) {
            for (int k = 0; k < queues.size(); k++) {
                if (!queues[k].empty()) {
                    currentProcessIdx = queues[k].front();
                    queues[k].pop();
                    currentQLevel = k;
                    timeInQuantum = 0;
                    break;
                }
            }
        }

        if (currentProcessIdx != -1) {
            *(processesPrintingArray + currentProcessIdx * maxSeconds + time) = '*';
            processes[currentProcessIdx].remainingTime--;
            timeInQuantum++;

            if (processes[currentProcessIdx].remainingTime == 0) {
                processes[currentProcessIdx].finishTime = time + 1;
                processes[currentProcessIdx].turnAroundTime = processes[currentProcessIdx].finishTime - processes[currentProcessIdx].arrivalTime;
                processes[currentProcessIdx].NormTurnTime = processes[currentProcessIdx].turnAroundTime / processes[currentProcessIdx].serveTime;
                currentProcessIdx = -1;
                timeInQuantum = 0;
            }
        }

        for(int k=0; k<queues.size(); k++) {
             queue<int> temp = queues[k];
             while(!temp.empty()) {
                 int idx = temp.front(); temp.pop();
                 *(processesPrintingArray + idx * maxSeconds + time) = '.';
             }
        }
    }
}

void Scheduler::FB2i()
{
    vector<queue<int>> queues(numberOfProcesses + 5); 
    int currentProcessIdx = -1;
    int currentQLevel = 0;
    int timeInQuantum = 0;

    for (int time = 0; time < maxSeconds; time++)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                queues[0].push(i);
            }
        }

        if (currentProcessIdx != -1) {
            int currentQuantum = pow(2, currentQLevel);
            if (timeInQuantum == currentQuantum) {
                int nextLevel = currentQLevel + 1;
                if (nextLevel >= queues.size()) nextLevel = queues.size() - 1;
                queues[nextLevel].push(currentProcessIdx);
                currentProcessIdx = -1;
                timeInQuantum = 0;
            }
        }

        if (currentProcessIdx == -1) {
            for (int k = 0; k < queues.size(); k++) {
                if (!queues[k].empty()) {
                    currentProcessIdx = queues[k].front();
                    queues[k].pop();
                    currentQLevel = k;
                    timeInQuantum = 0;
                    break;
                }
            }
        }

        if (currentProcessIdx != -1) {
            *(processesPrintingArray + currentProcessIdx * maxSeconds + time) = '*';
            processes[currentProcessIdx].remainingTime--;
            timeInQuantum++;

            if (processes[currentProcessIdx].remainingTime == 0) {
                processes[currentProcessIdx].finishTime = time + 1;
                processes[currentProcessIdx].turnAroundTime = processes[currentProcessIdx].finishTime - processes[currentProcessIdx].arrivalTime;
                processes[currentProcessIdx].NormTurnTime = processes[currentProcessIdx].turnAroundTime / processes[currentProcessIdx].serveTime;
                currentProcessIdx = -1;
                timeInQuantum = 0;
            }
        }

        for(int k=0; k<queues.size(); k++) {
             queue<int> temp = queues[k];
             while(!temp.empty()) {
                 int idx = temp.front(); temp.pop();
                 *(processesPrintingArray + idx * maxSeconds + time) = '.';
             }
        }
    }
}

void Scheduler::AGE(int quantum)
{
    vector<int> readyList;
    int currentProcessIdx = -1;
    int timeInQuantum = 0;

    for (int time = 0; time < maxSeconds; time++)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == time) {
                readyList.push_back(i);
            }
        }

        bool reschedulingNeeded = false;
        
        if (currentProcessIdx != -1) {
             if (timeInQuantum == quantum) {
                 processes[currentProcessIdx].currentPriority = processes[currentProcessIdx].priority;
                 readyList.push_back(currentProcessIdx);
                 currentProcessIdx = -1;
                 timeInQuantum = 0;
                 reschedulingNeeded = true;
             }
        } else {
            reschedulingNeeded = true;
        }

        if (reschedulingNeeded && !readyList.empty()) {
            for(int &idx : readyList) {
                processes[idx].currentPriority++;
            }

            int bestIdx = 0;
            for(int i=1; i<readyList.size(); i++) {
                if (processes[readyList[i]].currentPriority > processes[readyList[bestIdx]].currentPriority) {
                    bestIdx = i;
                }
            }
            currentProcessIdx = readyList[bestIdx];
            readyList.erase(readyList.begin() + bestIdx);
        }

        if (currentProcessIdx != -1) {
            *(processesPrintingArray + currentProcessIdx * maxSeconds + time) = '*';
            processes[currentProcessIdx].remainingTime--;
            timeInQuantum++;

            if (processes[currentProcessIdx].remainingTime == 0) {
                processes[currentProcessIdx].finishTime = time + 1;
                processes[currentProcessIdx].turnAroundTime = processes[currentProcessIdx].finishTime - processes[currentProcessIdx].arrivalTime;
                processes[currentProcessIdx].NormTurnTime = processes[currentProcessIdx].turnAroundTime / processes[currentProcessIdx].serveTime;
                
                processes[currentProcessIdx].currentPriority = processes[currentProcessIdx].priority;
                
                currentProcessIdx = -1;
                timeInQuantum = 0;
            }
        }

        for(int idx : readyList) {
            *(processesPrintingArray + idx * maxSeconds + time) = '.';
        }
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