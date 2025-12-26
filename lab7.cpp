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
    int currentTime = 0;
    int completedProcesses = 0;
    queue<int> readyQ;

    while (completedProcesses < numberOfProcesses && currentTime < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i]. arrivalTime == currentTime) {
                readyQ.push(i);
            }
        }

        if (! processorBusy && ! readyQ.empty()) {
            currentProcess = processes[readyQ.front()];
            readyQ.pop();
            processorBusy = true;
        }

        if (processorBusy) {
            *(processesPrintingArray + currentProcess.id * maxSeconds + currentTime) = '*';
            currentProcess.remainingTime--;
            processes[currentProcess.id].remainingTime = currentProcess.remainingTime;

            if (currentProcess.remainingTime == 0) {
                processes[currentProcess.id].finishTime = currentTime + 1;
                processes[currentProcess. id].turnAroundTime = processes[currentProcess.id].finishTime - processes[currentProcess.id].arrivalTime;
                processes[currentProcess.id].NormTurnTime = (float)processes[currentProcess.id].turnAroundTime / processes[currentProcess. id].serveTime;
                processorBusy = false;
                completedProcesses++;
            }
        }

        queue<int> tempQ = readyQ;
        while (!tempQ.empty()) {
            int idx = tempQ.front(); tempQ.pop();
            *(processesPrintingArray + idx * maxSeconds + currentTime) = '.';
        }
        currentTime++;
    }
}

void Scheduler:: RR(int quantum)
{
    int currentTime = 0;
    int completedProcesses = 0;
    int timeSlice = 0;
    queue<int> readyQ;

    while (completedProcesses < numberOfProcesses && currentTime < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == currentTime) {
                readyQ.push(i);
            }
        }

        if (processorBusy) {
            if (timeSlice >= quantum) {
                readyQ.push(currentProcess.id);
                processorBusy = false;
                timeSlice = 0;
            }
        }

        if (!processorBusy && !readyQ.empty()) {
            currentProcess = processes[readyQ.front()];
            readyQ.pop();
            processorBusy = true;
            timeSlice = 0;
        }

        if (processorBusy) {
            *(processesPrintingArray + currentProcess.id * maxSeconds + currentTime) = '*';
            currentProcess.remainingTime--;
            processes[currentProcess.id].remainingTime = currentProcess.remainingTime;
            timeSlice++;

            if (currentProcess.remainingTime == 0) {
                processes[currentProcess.id].finishTime = currentTime + 1;
                processes[currentProcess.id]. turnAroundTime = processes[currentProcess.id].finishTime - processes[currentProcess.id].arrivalTime;
                processes[currentProcess.id].NormTurnTime = (float)processes[currentProcess.id].turnAroundTime / processes[currentProcess.id]. serveTime;
                processorBusy = false;
                completedProcesses++;
                timeSlice = 0;
            }
        }

        queue<int> tempQ = readyQ;
        while (!tempQ.empty()) {
            int idx = tempQ.front(); tempQ.pop();
            *(processesPrintingArray + idx * maxSeconds + currentTime) = '.';
        }
        currentTime++;
    }
}

void Scheduler::SPN()
{
    int currentTime = 0;
    int completedProcesses = 0;
    vector<int> readyList;

    while (completedProcesses < numberOfProcesses && currentTime < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i]. arrivalTime == currentTime) {
                readyList.push_back(i);
            }
        }

        if (!processorBusy && !readyList.empty()) {
            int bestIdx = 0;
            for (int i = 1; i < readyList.size(); i++) {
                if (processes[readyList[i]].serveTime < processes[readyList[bestIdx]].serveTime) {
                    bestIdx = i;
                }
            }
            currentProcess = processes[readyList[bestIdx]];
            readyList.erase(readyList.begin() + bestIdx);
            processorBusy = true;
        }

        if (processorBusy) {
            *(processesPrintingArray + currentProcess. id * maxSeconds + currentTime) = '*';
            currentProcess. remainingTime--;
            processes[currentProcess.id].remainingTime = currentProcess.remainingTime;

            if (currentProcess.remainingTime == 0) {
                processes[currentProcess.id].finishTime = currentTime + 1;
                processes[currentProcess. id].turnAroundTime = processes[currentProcess.id].finishTime - processes[currentProcess.id].arrivalTime;
                processes[currentProcess.id].NormTurnTime = (float)processes[currentProcess.id].turnAroundTime / processes[currentProcess. id].serveTime;
                processorBusy = false;
                completedProcesses++;
            }
        }

        for (int idx : readyList) {
            *(processesPrintingArray + idx * maxSeconds + currentTime) = '.';
        }
        currentTime++;
    }
}

void Scheduler::SRT()
{
    int currentTime = 0;
    int completedProcesses = 0;
    vector<int> readyList;

    while (completedProcesses < numberOfProcesses && currentTime < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == currentTime) {
                readyList. push_back(i);
            }
        }

        if (processorBusy) {
            int currentRem = currentProcess.remainingTime;
            int bestReadyIdx = -1;
            for(int i=0; i<readyList. size(); i++) {
                if(processes[readyList[i]].remainingTime < currentRem) {
                    currentRem = processes[readyList[i]].remainingTime;
                    bestReadyIdx = i;
                }
            }
            
            if (bestReadyIdx != -1) {
                readyList.push_back(currentProcess.id);
                currentProcess = processes[readyList[bestReadyIdx]];
                readyList.erase(readyList.begin() + bestReadyIdx);
            }
        }

        if (!processorBusy && !readyList. empty()) {
            int bestIdx = 0;
            for (int i = 1; i < readyList.size(); i++) {
                if (processes[readyList[i]].remainingTime < processes[readyList[bestIdx]].remainingTime) {
                    bestIdx = i;
                }
            }
            currentProcess = processes[readyList[bestIdx]];
            readyList.erase(readyList.begin() + bestIdx);
            processorBusy = true;
        }

        if (processorBusy) {
            *(processesPrintingArray + currentProcess. id * maxSeconds + currentTime) = '*';
            currentProcess. remainingTime--;
            processes[currentProcess.id].remainingTime = currentProcess.remainingTime;

            if (currentProcess.remainingTime == 0) {
                processes[currentProcess.id]. finishTime = currentTime + 1;
                processes[currentProcess.id].turnAroundTime = processes[currentProcess.id].finishTime - processes[currentProcess. id].arrivalTime;
                processes[currentProcess.id].NormTurnTime = (float)processes[currentProcess.id].turnAroundTime / processes[currentProcess.id].serveTime;
                processorBusy = false;
                completedProcesses++;
            }
        }

        for (int idx : readyList) {
            *(processesPrintingArray + idx * maxSeconds + currentTime) = '.';
        }
        currentTime++;
    }
}

void Scheduler::HRRN()
{
    int currentTime = 0;
    int completedProcesses = 0;
    vector<int> readyList;

    while (completedProcesses < numberOfProcesses && currentTime < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i]. arrivalTime == currentTime) {
                readyList.push_back(i);
            }
        }

        if (! processorBusy && !readyList.empty()) {
            int bestIdx = 0;
            double maxRatio = -1.0;
            for (int i = 0; i < readyList.size(); i++) {
                double W = (double)(currentTime - processes[readyList[i]].arrivalTime);
                double S = (double)processes[readyList[i]].serveTime;
                double ratio = (W + S) / S;
                if (ratio > maxRatio) {
                    maxRatio = ratio;
                    bestIdx = i;
                }
            }
            currentProcess = processes[readyList[bestIdx]];
            readyList. erase(readyList.begin() + bestIdx);
            processorBusy = true;
        }

        if (processorBusy) {
            *(processesPrintingArray + currentProcess.id * maxSeconds + currentTime) = '*';
            currentProcess.remainingTime--;
            processes[currentProcess.id]. remainingTime = currentProcess.remainingTime;

            if (currentProcess.remainingTime == 0) {
                processes[currentProcess.id].finishTime = currentTime + 1;
                processes[currentProcess.id].turnAroundTime = processes[currentProcess. id].finishTime - processes[currentProcess.id].arrivalTime;
                processes[currentProcess. id].NormTurnTime = (float)processes[currentProcess. id].turnAroundTime / processes[currentProcess.id].serveTime;
                processorBusy = false;
                completedProcesses++;
            }
        }

        for (int idx :  readyList) {
            *(processesPrintingArray + idx * maxSeconds + currentTime) = '.';
        }
        currentTime++;
    }
}

void Scheduler::FB1()
{
    int currentTime = 0;
    int completedProcesses = 0;
    int timeSlice = 0;
    vector<queue<int>> queues(numberOfProcesses + 10);
    
    for(int i=0; i<numberOfProcesses; ++i) processes[i].priority = 0;

    while (completedProcesses < numberOfProcesses && currentTime < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i]. arrivalTime == currentTime) {
                processes[i].priority = 0;
                queues[0].push(i);
            }
        }

        if (processorBusy) {
            bool preempt = false;
            int currentLevel = currentProcess.priority;
            for(int k=0; k < currentLevel; k++) {
                if(!queues[k].empty()) {
                    preempt = true;
                    break;
                }
            }
            
            if (preempt || timeSlice >= 1) {
                int nextLevel = currentLevel;
                if (! preempt) nextLevel = currentLevel + 1; 
                
                if (nextLevel >= numberOfProcesses) nextLevel = numberOfProcesses;
                
                processes[currentProcess.id].priority = nextLevel;
                queues[nextLevel].push(currentProcess. id);
                processorBusy = false;
                timeSlice = 0;
            }
        }

        if (!processorBusy) {
            for(int k=0; k < queues. size(); k++) {
                if(!queues[k].empty()) {
                    int idx = queues[k].front();
                    queues[k].pop();
                    currentProcess = processes[idx];
                    currentProcess.remainingTime = processes[idx].remainingTime; 
                    processorBusy = true;
                    timeSlice = 0;
                    break;
                }
            }
        }

        if (processorBusy) {
            *(processesPrintingArray + currentProcess.id * maxSeconds + currentTime) = '*';
            currentProcess.remainingTime--;
            processes[currentProcess.id].remainingTime = currentProcess. remainingTime;
            timeSlice++;

            if (currentProcess.remainingTime == 0) {
                processes[currentProcess.id].finishTime = currentTime + 1;
                processes[currentProcess.id].turnAroundTime = processes[currentProcess. id].finishTime - processes[currentProcess.id].arrivalTime;
                processes[currentProcess. id].NormTurnTime = (float)processes[currentProcess. id].turnAroundTime / processes[currentProcess.id].serveTime;
                processorBusy = false;
                completedProcesses++;
                timeSlice = 0;
            }
        }

        for(int k=0; k<queues.size(); k++) {
             queue<int> temp = queues[k];
             while(! temp.empty()) {
                 int idx = temp.front(); temp.pop();
                 *(processesPrintingArray + idx * maxSeconds + currentTime) = '.';
             }
        }
        currentTime++;
    }
}

void Scheduler::FB2i()
{
    int currentTime = 0;
    int completedProcesses = 0;
    int timeSlice = 0;
    vector<queue<int>> queues(numberOfProcesses + 10);
    
    for(int i=0; i<numberOfProcesses; ++i) processes[i].priority = 0;

    while (completedProcesses < numberOfProcesses && currentTime < maxSeconds)
    {
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == currentTime) {
                processes[i].priority = 0;
                queues[0].push(i);
            }
        }

        if (processorBusy) {
            bool preempt = false;
            int currentLevel = currentProcess.priority;
            for(int k=0; k < currentLevel; k++) {
                if(!queues[k].empty()) {
                    preempt = true;
                    break;
                }
            }
            
            int limit = pow(2, currentLevel);
            if (preempt || timeSlice >= limit) {
                int nextLevel = currentLevel;
                if (!preempt) nextLevel = currentLevel + 1;
                
                if (nextLevel >= numberOfProcesses) nextLevel = numberOfProcesses;
                
                processes[currentProcess.id].priority = nextLevel;
                queues[nextLevel].push(currentProcess.id);
                processorBusy = false;
                timeSlice = 0;
            }
        }

        if (! processorBusy) {
            for(int k=0; k < queues.size(); k++) {
                if(!queues[k].empty()) {
                    int idx = queues[k].front();
                    queues[k].pop();
                    currentProcess = processes[idx];
                    currentProcess.remainingTime = processes[idx].remainingTime;
                    processorBusy = true;
                    timeSlice = 0;
                    break;
                }
            }
        }

        if (processorBusy) {
            *(processesPrintingArray + currentProcess.id * maxSeconds + currentTime) = '*';
            currentProcess.remainingTime--;
            processes[currentProcess. id].remainingTime = currentProcess.remainingTime;
            timeSlice++;

            if (currentProcess.remainingTime == 0) {
                processes[currentProcess.id].finishTime = currentTime + 1;
                processes[currentProcess.id].turnAroundTime = processes[currentProcess. id].finishTime - processes[currentProcess.id].arrivalTime;
                processes[currentProcess. id].NormTurnTime = (float)processes[currentProcess. id].turnAroundTime / processes[currentProcess.id].serveTime;
                processorBusy = false;
                completedProcesses++;
                timeSlice = 0;
            }
        }

        for(int k=0; k<queues. size(); k++) {
             queue<int> temp = queues[k];
             while(!temp.empty()) {
                 int idx = temp.front(); temp.pop();
                 *(processesPrintingArray + idx * maxSeconds + currentTime) = '.';
             }
        }
        currentTime++;
    }
}

void Scheduler::AGE(int quantum)
{
    int currentTime = 0;
    int completedProcesses = 0;
    int timeSlice = 0;
    vector<int> readyList;
    
    while (completedProcesses < numberOfProcesses && currentTime < maxSeconds)
    {
        // Add newly arrived processes to ready list
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i]. arrivalTime == currentTime) {
                readyList.push_back(i);
            }
        }
        
        // Check if we need to reschedule (quantum expired or processor is idle)
        bool needsReschedule = false;
        
        if (processorBusy && timeSlice >= quantum) {
            // Quantum expired - add current process back to ready list
            readyList.push_back(currentProcess. id);
            processorBusy = false;
            needsReschedule = true;
        } else if (!processorBusy && ! readyList.empty()) {
            needsReschedule = true;
        }
        
        // Perform rescheduling (Xinu aging algorithm)
        if (needsReschedule) {
            // Reset current process priority to initial priority (if there was one)
            if (timeSlice >= quantum && timeSlice > 0) {
                // The process that just finished its quantum
                for (int idx : readyList) {
                    if (idx == readyList. back()) {
                        processes[idx].currentPriority = processes[idx]. priority;
                    }
                }
            }
            
            // Increase priority of all ready processes by 1
            for (int idx : readyList) {
                processes[idx].currentPriority++;
            }
            
            // Select the highest priority process
            int bestIdx = 0;
            for (int i = 1; i < readyList.size(); i++) {
                if (processes[readyList[i]].currentPriority > processes[readyList[bestIdx]].currentPriority) {
                    bestIdx = i;
                }
            }
            
            currentProcess = processes[readyList[bestIdx]];
            readyList.erase(readyList.begin() + bestIdx);
            processorBusy = true;
            timeSlice = 0;
        }

        // Execute current process
        if (processorBusy) {
            *(processesPrintingArray + currentProcess.id * maxSeconds + currentTime) = '*';
            currentProcess.remainingTime--;
            processes[currentProcess.id].remainingTime = currentProcess. remainingTime;
            timeSlice++;
            
            if (currentProcess. remainingTime == 0) {
                processes[currentProcess.id].finishTime = currentTime + 1;
                processes[currentProcess.id].turnAroundTime = processes[currentProcess.id]. finishTime - processes[currentProcess.id].arrivalTime;
                processes[currentProcess.id]. NormTurnTime = (float)processes[currentProcess.id]. turnAroundTime / processes[currentProcess.id].serveTime;
                processorBusy = false;
                completedProcesses++;
                timeSlice = 0;
            }
        }

        // Mark ready processes with '.'
        for (int idx : readyList) {
            *(processesPrintingArray + idx * maxSeconds + currentTime) = '.';
        }
        
        currentTime++;
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