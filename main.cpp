#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <deque>
#include <stdlib.h>
#include <fstream>

using namespace std;


//represents base state
struct Intialstate{
    vector<int> state;
    int Ttime = 0;
    bool light = false;
    int StateNum = 0;
    int ParNum = 0;
};

//functions
//two test functions just test push, pull, and insert operations
void Test (vector<Intialstate> cur, int N);
void TestQueue (deque<Intialstate> queue, int N);
//Check the history or solution list to see if they are true
bool Check_History (vector<Intialstate> History, vector<Intialstate> Temp, int N);
bool Check_Soultion (vector<Intialstate> Solution,int N);
//Find the best answer uses uniform
void FindBest_Trace (vector<Intialstate> Soultion, vector<Intialstate> History, bool Uniform, int N, int state); //uniform/Best

ofstream output;



int main()
{


//idea use a vector to story both 0 or 1 for side value and to store the current values. Use nested loop
//to to switch between single and double changes for example when 2 people cross the tunnel that's a
//double change.

//Note: technially current is not doing anything and even isn't being used after the queue loops starts
//so just keep that in mind
vector<Intialstate> current;
//The queue
deque<Intialstate> queue;
//history list
vector<Intialstate> Hist;
//solution list
vector<Intialstate> Soultion_List;
//temporary list for queue
vector<Intialstate> Temp;
//stores queue state after pop
vector<Intialstate> Draw_back;

//objects for user choice
int State_num = 1;
bool BF = false;
bool DF = false;
bool Unit = false;
string choice = "";
string Confirm = "";
int Final_State_Num = 0;
output.open("Data.txt");



//max size of one vector
int N = 0;

//Get N value
cout << "how many people will cross?: ";
cin >> N;

//store time it takes for user to cross/is mapped to vector
int miles [N];

for(int x = 0; x < N; x++){
    int time;
    cout << "enter time " << x << ":";
    cin >> time;
    cout << "\n";
    miles[x] = time;
}

//confirm and check user choice
while (Confirm != "Y"){

//bool error = false;

    while (BF == false && DF == false){
    cout << "\n" << "Enter BF for breadth or DF for Depth: ";
    cin >> choice;
    if(choice == "BF"){
        BF = true;
    }else{BF = false;}

    if(choice == "DF"){
        DF = true;
    }else{DF = false;}

    }

    if(BF == true){
cout << "Would you like to use unitform search for best answer?";

    while (choice != "Y" && choice != "N"){
     cout << "\n" << "Enter Y for yes or N for no: ";
     cin  >> choice;
    }
    }

     if(choice == "Y"){
        Unit = true;
     }else{Unit = false;}


        cout << "You have entered" << "\n"
             <<"BF: " << BF << "\n"
             <<"DF: " << DF << "\n"
             <<"Unitform: " << choice <<"\n" << " is this correct?" << "\n" << "Enter Y or N: ";

             while (Confirm != "Y" && Confirm != "N"){
             cin  >> Confirm;
             }
}

output << "Current Setup: \n" <<"BF = "<< BF << "\n" <<"DF = " << DF <<"\n"<<"Unitform = "<< Unit << "\n\n";

current.push_back(Intialstate());
current[0].state.resize(N);

//Add intial state to queue and history
queue.push_back(current[0]);
Hist.push_back(current[0]);

//TestQueue(queue,N);


while (!queue.empty()){

    //copy over current queue value
    Temp.push_back(queue.front());
    //reset Temp to queue state if state repeat
    Draw_back.push_back(queue.front());
    //Pop the front of the queue BFS
    //TestQueue(queue,N);
    queue.pop_front();
    int change = 0;
    bool same = false;
    bool Sol =  false;


    //**code for going and creating new states.
    //if light equal to false
    if (Temp[0].light == false){
        for (int x = 0; x < N; x++){
            for (int y = 0; y < N; y++){
                    //find 0 number placement and change to 1 if change is not equal to 2
                    if(Temp[0].state[x] == 0 && change != 2){
                        Temp[0].state[x] = 1;
                        change++;
                    }
                    if(Temp[0].state[y] == 0 && change != 2){
                        Temp[0].state[y] = 1;
                        change++;
                    }
                    //once change equal 2 check to see if state is not a repeat
                    if (change == 2){
                            Temp[0].light = true;
                        same = Check_History(Hist,Temp,N);
                        if (same == true){
                            Temp.pop_back();
                            Temp.push_back(Draw_back[0]);
                            change = 0;
                        }
                        //if state is new make nessary changes to state then add to queue and history
                        else{
                            if(miles[x] >= miles[y]){
                                Temp[0].Ttime += miles[x];
                                }
                                else{
                                    Temp[0].Ttime += miles[y];
                                }


                            Temp[0].ParNum = Temp[0].StateNum;
                            Temp[0].StateNum = State_num++;
                           // cout << "State Number = " <<   Temp[0].StateNum;

                          Sol = Check_Soultion(Temp,N);

                          if(Sol == true && Unit == true){
                            Soultion_List.push_back(Temp[0]);
                            Hist.push_back(Temp[0]);
                         //   cout << "\n";
                          }
                          if(Sol == true && Unit == false){
                            Soultion_List.push_back(Temp[0]);
                            Hist.push_back(Temp[0]);
                            Final_State_Num = Temp[0].StateNum;
                            FindBest_Trace(Soultion_List,Hist,Unit,N,State_num-1);
                         //   cout << "\n";
                          }
                          if(Sol == false && BF == true){
                            queue.push_back(Temp[0]);
                            Hist.push_back(Temp[0]);
                        //    cout << "\n";
                          }
                          if(Sol == false && DF == true){
                            queue.push_front(Temp[0]);
                            Hist.push_back(Temp[0]);
                          //  cout << "\n";
                          }

                            Temp.pop_back();
                            Temp.push_back(Draw_back[0]);
                            change = 0;
                        }
                    }
            }
        }
    }

    if (Temp[0].light == true){
        for (int x = 0; x < N; x++){
                if(Temp[0].state[x] == 1){
                   Temp[0].state[x] = 0;
                   change++;
                  }
                  //check to see if state is new or repeated
                if(change == 1){
                   Temp[0].light = false;
                   same = Check_History(Hist,Temp,N);
                    if(same == true){
                          Temp.pop_back();
                          Temp.push_back(Draw_back[0]);
                          change = 0;
                    }
                    else{
                        Temp[0].Ttime = Temp[0].Ttime + miles[x];
                     //   Temp[0].light = false;
                        Temp[0].ParNum = Temp[0].StateNum;
                        Temp[0].StateNum = State_num++;
                        //cout << "State Number = " << State_num;

                        if(BF == true){
                            queue.push_back(Temp[0]);
                            Hist.push_back(Temp[0]);
                         //   cout << "\n";
                        }
                        if(DF == true){
                            queue.push_front(Temp[0]);
                            Hist.push_back(Temp[0]);
                         //   cout << "\n";
                        }

                        Temp.pop_back();
                        Temp.push_back(Draw_back[0]);
                        change = 0;
                    }
                }
            }
        }
 //delete current value for next queue value
    Draw_back.pop_back();
    Temp.pop_back();
}

// test struct vector for clarity
//Test (current,N);

FindBest_Trace(Soultion_List,Hist,Unit,N,State_num);

 return 1;
}






//functions defnitions
void Test (vector<Intialstate> cur, int N){
    //Check vector value
    for (int x = 0; x < N; x++ ){
        for (int y = 0; y < N; y++){
            cout << cur[0].state[x] << cur[0].state[y] << "\n";
            output << cur[0].state[x] << cur[0].state[y] << "\n";
            }
        }
    }

void TestQueue (deque<Intialstate> queue, int N){
    //Check queue value
    for (int x = 0; x < queue.size(); x++){
            cout << "\n\n";
            output << "\n\n";
            for (int y = 0; y < N; y++){
        cout << queue[x].state[y];
        output << queue[x].state[y];
            }
        }
}

bool Check_History (vector<Intialstate> History, vector<Intialstate> Temp, int N){


    int simular = 0;

    if (Check_Soultion(Temp,N) == true){

            return false;
    }

    for(int x = 0; x < History.size(); x++){
            simular = 0;
        for(int y = 0; y < N; y++){
               if (History[x].state[y] == Temp[0].state[y])
                simular++;
               if (simular == N && History[x].light == Temp[0].light)
                return true;
        }
    }
    return false;

}

bool Check_Soultion (vector<Intialstate> Solution,int N){

int soultion = 0;

for (int x = 0; x < N; x++){
    if (Solution[0].state[x] == 1){
        soultion++;
    }
    if(soultion == N){
        return true;
    }
}
return false;
}

void FindBest_Trace (vector<Intialstate> Soultion, vector<Intialstate> History, bool Uniform, int N, int state){

    int time = 0;
    int   postion = 0;
    int   statenum = 0;

    if (Uniform == true){
    //find best time
     cout <<"\n"<< " All Solutions Found: " << "\n";
     output <<"\n"<< " All Solutions Found: " << "\n";
    for (int x = 0; x < Soultion.size(); x++){
            cout << Soultion[x].Ttime << "\n\n";
            output << Soultion[x].Ttime << "\n\n";
        if (time == 0)
            time = Soultion[x].Ttime;

        if (time >= Soultion[x].Ttime){
            time = Soultion[x].Ttime;
           // postion = x;
            statenum = Soultion[x].StateNum;
         //   cout << "best time found: " << Soultion[x].Ttime;
        }
    }
 //   cout << "Soultion vector size: " << Soultion.size();
    }

    if (Uniform == false)
        statenum = state;

    //find state number in history
    for (int x = 0; x < History.size(); x++){
        if(History[x].StateNum == statenum){
            postion = x;
         //   cout << "int X at state number" << x;
        }
    }

    if (Uniform == true){
    cout << "Best Time = " << History[postion].Ttime << " Mins "<< "\n\n";
    output << "Best Time = " << History[postion].Ttime << " Mins "<< "\n\n";
    }
    else{
    cout << "Time Found = " << History[postion].Ttime <<" Mins "<< "\n\n";
    output << "Time Found = " << History[postion].Ttime <<" Mins "<< "\n\n";
    }

    cout << "Path to Solution:" << "\n";
    output << "Path to Solution:" << "\n";
    //track shortage path history
    while (History[postion].StateNum != 0){

            if (History[postion].light == true){
                    cout << "people crossed light on left hand side: ";
                    output << "people crossed light on left hand side: ";
                }
            if (History[postion].light == false){
                    cout << "Light brought back, now on right side:  ";
                    output << "Light brought back, now on right side:  ";
                }

           cout << "<";
           output << "<";
        for(int x = 0; x < N; x++){
            cout << History[postion].state[x];
            output << History[postion].state[x];
        }
           cout << ">" << "\n\n";
           output << ">" << "\n\n";

       postion = History[postion].ParNum;

       // find position of parent
       for(int x = 0; x < History.size(); x++){
           if(History[x].StateNum == postion)
           {
            postion = x;
            break;
           }
       }
    }

cout <<"initial state:                          "<< "<";
output <<"initial state:                          "<< "<";
for(int x = 0; x < N; x++){
    cout <<"0";
    output <<"0";
}
cout << ">" << "\n";
output << ">" << "\n";

    if (Uniform == false){

        exit(1);
    }

}



