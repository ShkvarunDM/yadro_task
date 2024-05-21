#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>

struct ComputerClub
{
    void start()
    {
        fin.open("input.txt");

        if (!fin.is_open()){
            std::cout << "Can't open file";
            return;
        }

        if(!acceptTables()) {std::cout << "Error in line 1"; return;}
        if(!acceptTime()) {std::cout << "Error in line 2"; return;}
        if(!acceptMoney()) {std::cout << "Error in line 3"; return;}

        std::cout << timeStart << "\n";

        processEvents();

        std::cout << timeEnd << "\n";

        std::vector <workTable>::iterator pr2;
         for (pr2 = workTables.begin(); pr2!= workTables.end(); pr2++){
            std::cout << pr2->number <<  " " << pr2->revenue << " " << intToStrTime(pr2->totalMinutes) << "\n";
        }
    }

private:

    bool acceptTables()
    {
        std::string line;
        const std::regex checkInt{"[1-9][0-9]*"};

        getline(fin, line);
        countTables = stoi(line);
        return regex_match(line, checkInt);
    }

    bool acceptMoney()
    {
        std::string line;
        const std::regex checkInt{"[1-9][0-9]*"};

        getline(fin, line);
        money = stoi(line);

        return regex_match(line, checkInt);
    }

    bool acceptTime()
    {

        const std::regex timeCheck1{"[01][0-9]:[0-5][0-9]"};
        const std::regex timeCheck2{"[2][0-3]:[0-5][0-9]"};

        getline(fin,timeStart,' ');
        getline(fin,timeEnd);

        return ((regex_match(timeStart, timeCheck1) || regex_match(timeStart, timeCheck2)) && (regex_match(timeEnd, timeCheck1) || regex_match(timeEnd, timeCheck2)) && (timeStart < timeEnd) );
    }

    bool checkFormat1(const std::string line)
    {
        const std::regex Check1{"[01][0-9]:[0-5][0-9] [134] ([a-z0-9_-])*"};
        const std::regex Check2{"[2][0-3]:[0-5][0-9] [134] ([a-z0-9_-])*"};

        return (regex_match(line, Check1) || regex_match(line, Check2));
    }

    bool checkFormat2(const std::string line)
    {
        const std::regex Check1{"[01][0-9]:[0-5][0-9] [2] ([a-z0-9_-])* [1-9][0-9]*"};
        const std::regex Check2{"[2][0-3]:[0-5][0-9] [2] ([a-z0-9_-])* [1-9][0-9]*"};

        return (regex_match(line, Check1) || regex_match(line, Check2));
    }

    bool checkCameEarly(const std::string timeCurrent){
        return (timeCurrent > timeStart);
    }

     bool checkClientAtTable(const std::string name){
         std::vector<Table>::iterator pr;
         for (pr = tables.begin(); pr!= tables.end(); pr++){
             if (pr->client == name)
                return true;
         }
         return false;
     }

      int checkClientAtTable2(const std::string name){
         std::vector<Table>::iterator pr;
         for (pr = tables.begin(); pr!= tables.end(); pr++){
             if (pr->client == name)
                return pr->number;
         }
         return false;
     }

     bool checkClientInQueue(const std::string name){
         std::vector <std::string>::iterator pr;
         for (pr = clients.begin(); pr!= clients.end(); pr++){
             if (*pr == name)
                return true;
         }
         return false;
     }

     bool checkBusyTable (const unsigned int num){
         vector<Table>::iterator pr;
         for (pr = tables.begin(); pr!= tables.end(); pr++){
             if (pr->number == num)
                return false;
         }
         return true;
     }

     bool checkFreeSeat(){
         return (tables.size()< countTables);
     }

     bool checkFreeSeatQueue(){
         return (clients.size() < countTables);
     }


  unsigned int countTime(std::string timeCom,std::string timeDepar){
            unsigned int hoursStart,hoursEnd, minuteStart, minuteEnd;
            std::string hoursStart1,hoursEnd1, minuteStart1, minuteEnd1;

            std::istringstream iss(timeCom);
            getline(iss,hoursStart1, ':');
            getline(iss,minuteStart1);


            hoursStart = stoi(hoursStart1);
            minuteStart = stoi(minuteStart1);

            std::istringstream iss2(timeDepar);
            getline(iss2,hoursEnd1, ':');
            getline(iss2,minuteEnd1);
            hoursEnd = stoi(hoursEnd1);
            minuteEnd = stoi(minuteEnd1);

            int difference = ((hoursEnd*60+minuteEnd) - (hoursStart*60+minuteStart));
            return difference;

     }

     std::string intToStrTime(unsigned int diff){
                std::string hoursStr;
                std::string minuteStr;

                unsigned int hours  = diff/60;
                unsigned int minute = diff%60;

                if (hours < 10){
                    hoursStr = '0' + std::to_string (hours);
                }
                else{
                    hoursStr = std::to_string (hours);
                }
                if (minute < 10){
                    minuteStr = '0' + std::to_string (minute);
                }
                else {
                    minuteStr = std::to_string (minute);
                }

                return (hoursStr + ':'+ minuteStr);
     }


     clientLeft(std::string name,std::string time, int id_Event){

         workTable wt;
         int diff = 0;
         int totalMinutes = 0;
         int revenue = 0;
         std::vector <Table>::iterator pr;
         for (pr = tables.begin(); pr!= tables.end(); pr++){
                if (pr->client == name){
                         diff = countTime(pr->time,time);
                          if (diff%60 != 0)
                                revenue += (diff/60+1) * money;
                          else
                                revenue += (diff/60) * money;

                        if (!checkWorkTable(pr->number)){
                            wt.number = pr->number;
                            wt.totalMinutes = diff;
                            wt.revenue = revenue;
                            workTables.push_back(wt);

                            if(!clients.empty()){
                                Table tb;
                                tb.number = pr->number;
                                tb.client = clients[0];
                                tb.time = time;

                                tables.erase(pr);
                                tables.push_back(tb);
                                showEvent2(time,12,clients[0],tb.number);
                            }


                        }
                        else{
                            std::vector <workTable>::iterator pr4;
                             for (pr4 = workTables.begin(); pr4!= workTables.end(); pr4++){
                                 if (pr4->number  == pr->number){
                                    wt.number = pr->number;
                                    wt.totalMinutes = pr4->totalMinutes + diff;
                                    wt.revenue = pr4->revenue + revenue;
                                    tables.erase(pr);
                                    workTables.erase(pr4);
                                    workTables.push_back(wt);
                                 }
                            }
                        }

                }

    }
}



     bool checkWorkTable(unsigned int id){
         std::vector <workTable>::iterator pr4;
         for (pr4 = workTables.begin(); pr4!= workTables.end(); pr4++){
             if (id == pr4->number)
                return true;
         }
         return false;
     }
    void showEvent1(std::string time, unsigned int id, std::string str){
        std::cout << time << " " << id << " " << str << "\n";
        return;
    }

    void showEvent2(std::string time, unsigned int id, std::string str, unsigned int number ){
        std::cout << time << " " << id << " " << str << " " << number <<  "\n";
        return;
    }

    void processEvents()
    {
        std::string line;
        while(getline(fin, line))
        {
            countStrok++;

            if (!(checkFormat1(line) || checkFormat2(line))) {std::cout << "Error in line " << countStrok << "\n"; return;}

            std::istringstream iss(line);
            iss >> timeEvent;
            iss >> idEvent;
            iss >> nameClient;

            if (idEvent == 2 ){
                iss >> numberTable;
            }
            if (countStrok == 4){
                timeEventN = timeEvent;
            }
            if (timeEventN > timeEvent) {
                std::cout << "Error in line " << countStrok << "\n";
                std::cout << "Time event(N+1) is less time event(N)";
                return;
            }

            switch(idEvent){
                case 1:{
                    showEvent1(timeEvent,idEvent,nameClient);

                    if (!checkCameEarly(timeEvent)) showEvent1(timeEvent,13,"NotOpenYet");
                    else if ((checkClientAtTable(nameClient)) || (checkClientInQueue(nameClient))){
                        showEvent1(timeEvent,13,"YouShallNotPass");
                    }
                    else
                        clients.push_back(nameClient);
                    break;
                }

               case 2:{
                    showEvent2(timeEvent,idEvent,nameClient,numberTable);
                    if (!checkBusyTable(numberTable)) showEvent1(timeEvent,13,"PlaceIsBusy");
                    else if ((!checkClientAtTable(nameClient)) && (!checkClientInQueue(nameClient))) showEvent1 (timeEvent,13,"ClientUnknown");
                    else{

                        Table table;
                        table.client = nameClient;
                        table.number = numberTable;
                        table.time = timeEvent;
                        tables.push_back(table);

                        clients.erase(clients.begin());

                    }

                    break;
                }
               case 3:{
                   showEvent1(timeEvent,idEvent,nameClient);
                   if (checkFreeSeat()) showEvent1 (timeEvent,13,"ICanWaitNoLonger!");
                   else if (!checkFreeSeatQueue()) showEvent1(timeEvent,11, nameClient);
                   break;
               }
               case 4:{
                   showEvent1(timeEvent,idEvent,nameClient);
                   if ((!checkClientAtTable(nameClient)) && (!checkClientInQueue(nameClient))) showEvent1 (timeEvent,13,"ClientUnknown");
                   else if (checkClientAtTable(nameClient)) {
                        clientLeft(nameClient,timeEvent,4);
                   }
                   break;
               }
        }

        timeEventN = timeEvent;
    }





    }

    std::ifstream fin;
    int countStrok = 3;
    std::string timeStart;
    std::string timeEnd;
    std::string timeEvent;
    std::string timeEventN;
    unsigned int idEvent = 0;
    std::string nameClient;
    unsigned int numberTable = 0;
    unsigned int countTables = 0;
    unsigned int money = 0;
    std::vector <string> clients;

    struct Table
    {
        unsigned int number = 0;
        std::string client;
        std::string time;
    };

    struct workTable
    {
        unsigned int number = 0;
        unsigned int totalMinutes = 0;
        unsigned int revenue = 0;
    };
    vector <workTable> workTables;
    vector <Table> tables;

};

int main()
{
    ComputerClub cb;
    cb.start();
    return 0;
}
