//yusuf demirci
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <ctime>

using namespace std;

struct flight{
  string from;
  string to;
  int hour;
  int min;
  int price;
  int ID;
  
  flight *next;
  flight *prev;
  
  //default constructor for ensuring the initial values of elements are proper
  flight() {
     from = "";
     to = "";
     hour = 0;
     min = 0;
     price = 0;
     ID = 0;

     next = nullptr;
     prev = nullptr;

  }

  //constructor for reading the needed elements from the file
  flight(string vec_from, string vec_to, int vec_hour, int vec_min, int vec_price) {
      from = vec_from;
      to = vec_to;
      hour = vec_hour;
      min = vec_min;
      price = vec_price;

      ID = 0;
      next = nullptr;
      prev = nullptr;
  }

  //constructor for creating a node system for flights
  flight(string assign_from, string assign_to, int assign_hour, int assign_min, int assign_price,int assign_ID, flight *prev_link, flight *next_link)
      : from(assign_from), to(assign_to), hour(assign_hour), min(assign_min), price(assign_price), prev(prev_link), next(next_link),ID(assign_ID)
  {}
};

struct airline{
  string name;
  int ID;
  airline *next;
  airline *head;
  flight *flights;

  //default constructor for ensuring the initial values of elements are proper
  airline(){
      name = "";
      ID = 0;
      next = nullptr;
      head = nullptr;
      flights = nullptr;
  }
  //constructor for creating a airline node system
  airline(string &vec_name,airline* link, int assign_ID)
      : name(vec_name), next(link), ID(assign_ID), flights(nullptr), head(nullptr)
  {}
};

//function for reading files
pair<vector<string>, vector<vector<flight>>> read_files(bool input_done){
  
  ///READ FLIGHTS///
  vector<string> airlines;
  vector<vector<flight>> flights(100); //start with a previously sized vector or you get seg fault (for sure)
  
  while(!input_done){
    string airline_name;
    string filename;
    cout << "Enter the name of airline: ";
    cin >> airline_name;
    cout << "Enter the name of airline file: ";
    cin >> filename;
    ifstream input(filename);

    if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
    }
    
    while( input.fail() ){
      cout << "Could not open file: " << filename << endl;
      cout << "Enter the name of airline: ";
      cin >> airline_name;
      cout << "Enter the name of airline file: ";
      cin >> filename;
      input.open(filename);

      if(filename == "done"){
      input_done = true;
      return make_pair(airlines, flights);
      }
    }
    
    airlines.push_back(airline_name);
    
   
    cout << "Processing input file: " << filename <<endl;
    
    int line_no = 0;
    vector<string> lines;
    string line;
    int vector_location = airlines.size() - 1;
    while(getline(input, line)){
      lines.push_back(line);
      line_no += 1;
      if(line_no % 5 == 0){
	    flight a_flight(lines[line_no-5], lines[line_no-4], stoi(lines[line_no-3]), stoi(lines[line_no-2]), stoi(lines[line_no-1]));
	    flights[vector_location].push_back(a_flight);
      }
    }
  }
  if(input_done){
    cout << "Input files are already read, however you can do manual changes.." << endl;
  }

  return make_pair(airlines, flights);
}


//function for making a linkedlist structure
airline* make_linked_list_structure(vector<string> airlines, vector<vector<flight>> flights){
    
    //if no files are read will return a null pointer for head
    if (airlines.empty()) {
        return nullptr;
    }
    
    //flights vector's size is predefined so it may still have empty spaces thus it is not appropirate to use flights.size() instead we will use the continueing variable
    int flights_size = airlines.size();
    //ID variable for appending ID's of node elements
    int ID_counter = 0;

    //creating the first airline and appending the first airline data in list
    airline* head_airlines = new airline(airlines[0], nullptr,ID_counter);
    //ID is now incremented
    ID_counter++;

    //creating a pointer so that the head node (head_airlines) wont be lost
    airline* ptra = head_airlines;

    //loop ensures all airlines are read
    for (int idx = 1; idx < airlines.size(); idx++) {

        //creating a new airline node and incrementing ID so that every airline gets its ID accordingly
        ptra->next = new airline(airlines[idx], nullptr, ID_counter);
        ID_counter++;
        ptra = ptra->next;
    }

    //ptra is reseted to head node for future uses
    ptra = head_airlines;
    //ID is reseted to 0 for flight IDs
    ID_counter = 0;

    //loops ensure all airline and their according flight datas are read
    for (int idk = 0; idk < flights_size; idk++) {

        //creating a head for doubly linkedlist
        flight* head_flights = new flight(flights[idk][0].from, flights[idk][0].to, flights[idk][0].hour, flights[idk][0].min, flights[idk][0].price, ID_counter, nullptr, nullptr);
        //creating a head pointer so that the original head wont be lost
        flight* ptrf = head_flights;

        //linkig current airline's flight node with the head of the flight nodes
        ptra->flights = ptrf;

        for (int idt = 1; idt < flights[idk].size(); idt++) {
            //incrementing ID so it generates continious ID's to append
            ID_counter++;
            //creating a flight linkedlist node
            ptrf->next = new flight(flights[idk][idt].from, flights[idk][idt].to, flights[idk][idt].hour, flights[idk][idt].min, flights[idk][idt].price, ID_counter, ptrf, nullptr);

            ptrf = ptrf->next;
        }
        ID_counter++;
        ptra = ptra->next; 
    }

    //time sorting algorithm for nodes of flight linkedlists
    //reseting the ptra to head of airline node
    ptra = head_airlines;

    //loops ensure every node is visited in flights
    for (int ide = 0; ide < flights_size; ide++) {
        //incrementing count and using it in loop ensures sorted nodes will be skipped
        int count = 0;
        

        for (int idw = 0; idw < flights[ide].size(); idw++) {

            //making temp the head of the current airline's flight's head
            //temp2 will be used to store the latest hour's pointer
            flight* temp = ptra->flights;
            flight* temp2 = nullptr;
            int late_time = 0;
            
            //using count to skip sorted nodes
            for (int idq = 0; idq < count; idq++) {
                temp = temp->next;
            }

            //loop to find the latest time
            while (temp) {
                
                if (((temp->hour * 100) + (temp->min)) > late_time) {
                    late_time = ((temp->hour * 100) + (temp->min));
                    temp2 = temp;
                }
                temp = temp->next;
            }
            //all cases to replace selected node in its sorted place
            //if node is at the start
            if (!temp2->prev) {

            }
            //if node is at the end
            else if (!temp2->next) {
                temp2->prev->next = nullptr;
                temp2->prev = nullptr;
                ptra->flights->prev = temp2;
                temp2->next = ptra->flights;
                ptra->flights = temp2;
            }
            //remaining cases
            else {
                temp2->prev->next = temp2->next;
                temp2->next->prev = temp2->prev;
                temp2->prev = nullptr;
                ptra->flights->prev = temp2;
                temp2->next = ptra->flights;
                ptra->flights = temp2;
            }
            count++;
        }
        ptra = ptra->next;
    }
    
    return head_airlines;
}

//function for printing all linked list profiles
void print_all(airline* &head) {
    //ptr is used because we dont want to change our original head
    airline* ptr = head;
    //will loop until airline linkedlist's end has reached
    //ptr = ptr->next ensures the movement between nodes
    while (ptr) {
        cout << "###################################" << endl;
        cout << "### AIRLINE ID: " << ptr->ID << " ###" << endl;
        cout << "NAME:" << ptr->name << endl;
        cout << "FLIGHTS: ";
        //creating a temporary head for current airline's first flight so that the original head wont be corrupted
        flight* ptri = ptr->flights;
        //will loop until flight linkedlist's end has reached
        while (ptri) {
            cout << "#[" << ptri->ID << "|" << ptri->from << "->" << ptri->to << "|" << ptri->hour << ":" << ptri->min << "|" << ptri->price << "TRY]#";
            ptri = ptri->next;
        }
        cout << endl;
        ptr = ptr->next;
    }
    //for the case that list has no elements
    if (!head) {
        cout << "List is empty.." << endl;
    }

}

//function for adding a flight node manually
void add_flight_with_input(airline*& head) {
    cout << "Adding manual entry:" << endl;
    string add_airline, add_from, add_to;
    int add_hour = 0,add_min = 0,add_price = 0, add_ID = 0;
    //bool to check if the input airline is a new or an already existing one
    bool does_exist = false;

    //inputs
    cout << "AIRLINE: ";
    cin >> add_airline;
   
    cout << "FROM: ";
    cin >> add_from;
   
    cout << "TO: ";
    cin >> add_to;
    
    cout << "HOUR: ";
    cin >> add_hour;
    
    cout << "MIN: ";
    cin >> add_min;
    
    cout << "PRICE: ";
    cin >> add_price;
    
    //if airline list is empty
    if (!head) {
        head = new airline(add_airline, nullptr, 0);
        head->flights = new flight(add_from, add_to, add_hour, add_min, add_price, 0, nullptr, nullptr);
    }
    else {

        //making a temporary head
        airline* ptrc = head;

        //using ptrc and temp5 to find the highest ID from the current linkedlists and incrementing it once to ensure new ID is ready
        //creating a ptr for later use
        flight* ptr = ptrc->flights;

        while (ptrc) {
            flight*temp5 = ptrc->flights;
            while (temp5) {
                if (temp5->ID > add_ID) {
                    add_ID = temp5->ID;
                }
                temp5 = temp5->next;
            }

            ptrc = ptrc->next;
        }
        
        add_ID++;

        //reseting ptrc to head and creating temp to hold the pointer of the airline if it is already in the list
        ptrc = head;
        airline* temp = nullptr;

        while (ptrc) {
            if (ptrc->name == add_airline) {
                does_exist = true;
                temp = ptrc;
            }
            ptrc = ptrc->next;
        }

        flight* ptrl = nullptr;
        //if its a already existing airline
        if (does_exist) {
            //making ptr the head of airline which new flight will be implemented
            ptr = temp->flights;
            
            //looping through flights to find time sorted place for new node
            while (ptr) {
                if ((ptr->hour * 100 + ptr->min) > (add_hour * 100 + add_min)) {

                    //if the will be added node is at the beginning
                    if (!ptr->prev) {
                        ptr->prev = new  flight(add_from, add_to, add_hour, add_min, add_price, add_ID, nullptr, ptr);
                        temp->flights = ptr->prev;
                        break;
                    }
                    //other cases except adding to the last node
                    else {
                        ptr->prev = new  flight(add_from, add_to, add_hour, add_min, add_price, add_ID, ptr->prev, ptr);
                        ptr->prev->prev->next = ptr->prev;
                        break;
                    } 
                }
                //taking the last node incase if the new node to be placed as last node
                if (!ptr->next) {
                    ptrl = ptr;
                }
                ptr = ptr->next;
            }
            //if the new added node is the latest flight
            if (!ptr) {
                ptrl->next = new  flight(add_from, add_to, add_hour, add_min, add_price, add_ID, ptrl, nullptr);
            }
        }
        //if its a new airline
        else {
            //creating a new airline ID for new airline
            int airline_ID = 0;
            ptrc = head;
            airline* temp3 = head;
            while (temp3) {
                if (temp3->ID > airline_ID) {
                    airline_ID = temp3->ID;
                }
                if (!temp3->next) {
                    ptrc = temp3;
                }
                temp3 = temp3->next;
            }
            airline_ID++;
            //placing the new airline and its flight node
            ptrc->next = new airline(add_airline, nullptr, airline_ID);
            ptrc->next->flights = new flight(add_from, add_to, add_hour, add_min, add_price, add_ID, nullptr, nullptr);
        }
    }
    
    cout << "Flight ID " << add_ID << " is added to the list.." << endl;
}

//function for recursive pathfinding
void pathfinderRec(string& location, string& aim, int stepsize, int& optimalcost, vector<flight*>& currentPath, vector<flight*>& bestPath, airline* head) {

    //for preventing infinite loop and ensuring path found includes given number of transitions at max
    if (stepsize < 0) {
        return;
    }

    //creating a temporary head for airline nodes
    airline* currentAirline = head;
    
    //loops ensure the movement through airlines and their flight nodes
    while (currentAirline) {

        //creating a temporary head for flight nodes
        flight* currentFlight = currentAirline->flights;

        while (currentFlight) {

            //if the current location matches the current inspected flight node's from 
            if (location == currentFlight->from) {
                currentPath.push_back(currentFlight);

                //if the current inspected flight node's to is same as the aimed destination
                if (aim == currentFlight->to) {

                    //variable for holding current flight node's price (will be used to compare path prices)
                    int currentprice = 0; 

                    //loop for calculating the price of the stored path
                    for (int idj = 0; idj < currentPath.size(); idj++) {
                        currentprice += currentPath[idj]->price;
                    }

                    //if the current path's cost is lower appends the route as the most efficient price route
                    //if the first time path's cost will be the most efficient price because the optimalcost is initialized as INT_MAX
                    if (currentprice < optimalcost) {
                        bestPath = currentPath;
                        optimalcost = currentprice;
                    }
                }
                //recursion for entering the airline-flight nodes again
                //location and stepsize are updated accordingly
                else {
                    pathfinderRec(currentFlight->to, aim, stepsize - 1, optimalcost, currentPath, bestPath, head);
                }
                //popping the elements of current path because the currentPath vector will be used for next path and needs to be empty
                currentPath.pop_back();
            }
            currentFlight = currentFlight->next;
        }
        currentAirline = currentAirline->next;
    }
    
}

//function for calling recursive path finding function
int pathfinder(airline* head) {

    //creating elements needed for recursive function
    string location;
    string aim;
    int stepsize;
    //optimalcost is initialized as maximum integer because when firs comparison is made we want it to change for sure
    int optimalcost = INT_MAX;
    vector<flight*> currentPath;
    vector<flight*> bestPath;
    
    //taking user inputs
    cout << "Where are you now? " << endl;
    cin >> location;

    cout << "Where do you want to go " << endl;
    cin >> aim;

    cout << "Maximum number of transfers: " << endl;
    cin >> stepsize;

    //calling the recursive function to find the route
    pathfinderRec(location, aim, stepsize, optimalcost, currentPath, bestPath,  head);

    //outputs for all cases
    //case for no appropirate path found
    if (optimalcost == INT_MAX) {
        cout << "No path found between " << location << " and " << aim << endl;
    }
    //case for a path found
    //using bestPath to print out most efficient route
    else {
        int finalcost = 0;
        cout << "##Best price path##" << endl;
        for (int idm = 0; idm < bestPath.size(); ++idm) {
            if (idm > 0) {
                cout << "->";
            }
            cout << "[" << bestPath[idm]->ID << "|" << bestPath[idm]->from << "->" << bestPath[idm]->to << "|" << bestPath[idm]->hour << ":" << bestPath[idm]->min << "|" << bestPath[idm]->price << "]";

            finalcost += bestPath[idm]->price;
        }
        cout << " $TOTAL PRICE: " << finalcost << endl;
    }

    return 0;
}

//function for deleting the whole linkedlist system
void delete_linked_list(airline* &head){
  //temporary head is defined as ptr
  airline* ptr = head;

  //ensures looping until the end of airline linkedlist
  while(ptr){

      //defining a new pointer so that while deleting a node pointer wont be lost
      airline* ptr2 = ptr;
      //temporary head for flight linkedlist
      flight* temp =  ptr->flights;

      //ensures looping until the end of flight linkedlist
      while (temp) {

          //edge case for removing the last element of flight linkedlist
          if (!temp->next) {
              delete temp;
              temp = nullptr;
          }

          //deleting the nodes of flight linkedlist
          else {
              temp = temp->next;
              delete temp->prev;
          }     
      }
      ptr = ptr->next;
      delete ptr2;
      
  }
  //head now points to nothing
  head = nullptr;
}

//function for removing flight manually with ID
void remove_flight_with_input(airline*& head) {

    //getting id to remove from user
    int removed_ID = 0;
    cout << "Flight id to remove: ";
    cin >> removed_ID;

    //creating airline pointers to iterate through airlines
    airline* ptr = head;
    //temp is for holding the previous of ptr (for airline deletion)
    airline* temp = nullptr;

    //ensuring the movement through airline nodes
    while (ptr) {

        //pointer for moving to the flights of current airline
        flight* ptrf = ptr->flights;

        //ensuring the movement through flight nodes
        while (ptrf) {

            //if ID's of node matches with removal ID
            if (ptrf->ID == removed_ID) {
                //all cases of removal
                //if its the last remaining node
                if (!ptrf->next && !ptrf->prev) {
                    ptr->flights = nullptr;
                    delete ptrf;
                    cout << "Flight ID " << removed_ID << " is removed from the list.." << endl;
                    break;
                }
                //if its not the last remaining node but the first node
                else if (!ptrf->prev) {
                    ptr->flights = ptrf->next;
                    ptrf->next->prev = nullptr;
                    delete ptrf;
                    cout << "Flight ID " << removed_ID << " is removed from the list.." << endl;
                    break;
                }
                //if its not the last remaining node but the last node
                else if (!ptrf->next) {
                    ptrf->prev->next = nullptr;
                    delete ptrf;
                    cout << "Flight ID " << removed_ID << " is removed from the list.." << endl;
                    break;
                }
                //remaining cases
                else {
                    ptrf->prev->next = ptrf->next;
                    ptrf->next->prev = ptrf->prev;
                    delete ptrf;
                    cout << "Flight ID " << removed_ID << " is removed from the list.." << endl;
                    break;
                }
            }
            ptrf = ptrf->next;
        }

        //case for if an airline has no flight node after the delete
        if (!ptr->flights) {
            //all edge cases
            //if it is a last remaining node
            if (!ptr->next && ptr == head) {
                head = nullptr;
                delete ptr;
                break;
            }
            //if its not last remaining but the last node
            else if(!ptr->next) {
                temp->next = nullptr;
                delete ptr;
                break;
            }
            //if its not last remaining but the first node
            else if (ptr == head) {
                head = ptr->next;
                delete ptr;
                break;

            }
            //remaining case
            else {
                temp->next = ptr->next;
                delete ptr;
                break;
            }
            
        }
        temp = ptr;
        ptr = ptr->next;
    }
}

//for printing the menu visual after exit statement is given
void printMainMenu() {
  cout << endl;
  cout <<"I***********************************************I"<<endl
       <<"I               0 - DELETE DATA                 I"<<endl
       <<"I               1 - READ FILES                  I"<<endl
       <<"I               2 - PRINT ALL FLIGHTS           I"<<endl
       <<"I               3 - ADD FLIGHT                  I"<<endl
       <<"I               4 - REMOVE FLIGHT               I"<<endl
       <<"I               5 - PATH FINDER                 I"<<endl
       <<"I               6 - EXIT                        I"<<endl
       <<"I***********************************************I"<<endl
       <<">>";
  cout << endl;
}  

void processMainMenu() {
  //input_done is for storing if a file is read or not << currently not so false
  bool input_done = false;
  pair<vector<string>, vector<vector<flight>>> lines_flights;
  airline* head = nullptr;
  
  char input;
  do{
    printMainMenu();
    cout << "Please enter your option " << endl;
    cin >> input;
    switch (input) {
    case '0':
      delete_linked_list(head);
      cout << "Data is deleted!" << endl;
      input_done = false;
      break;
    case '1':
      if(!input_done){
	    lines_flights = read_files(input_done);
	    head = make_linked_list_structure(lines_flights.first, lines_flights.second);
	    cout << "Files have read.." << endl;
      }
      else{
	    cout << "Files are already read.." << endl;
      }
      input_done = true;
      break;
    case '2':
      print_all(head);
      break;
    case '3':
      add_flight_with_input(head);
      break;
    case '4':
      remove_flight_with_input(head);
      break;
    case '5':
      pathfinder(head);
      break;
    case '6':
      cout << "Exiting.." << endl;
      exit(0);
    default:
      cout << "Invalid option: please enter again" << endl;
    }
  } while(true);
}


int main(){
  
  //calling the mainMenu function
  processMainMenu();
  return 0;
  
}



  
