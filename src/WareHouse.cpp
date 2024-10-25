#include <string>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Action.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;
using std::vector;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.

WareHouse::WareHouse(const string &configFilePath)
:isOpen(false),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(0),volunteerCounter(0),orderCounter(0)
{
    CollectorVolunteer *volunteer = new CollectorVolunteer(-1,"-1",-1);
    volunteers.push_back(volunteer);
    SoldierCustomer *customer = new SoldierCustomer(-1,"-1",-1,-1);
    customers.push_back(customer);
    Order *order = new Order(-1,-1,-1);
    pendingOrders.push_back(order);
    parseConfig(configFilePath);
}

WareHouse::WareHouse(const WareHouse &other)
:isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter)
{
    deepCopy(other);
}
WareHouse& WareHouse:: operator=(const WareHouse &other){//assignment operator
    if(this != &other){
        isOpen = other.isOpen;
        deepDelete(*this);
        deepCopy(other);
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
    }
    return *this;
}
WareHouse::~WareHouse(){
    deepDelete(*this);
}
WareHouse:: WareHouse(WareHouse&& other):
isOpen(other.isOpen),actionsLog(other.actionsLog),volunteers(other.volunteers),pendingOrders(other.pendingOrders),inProcessOrders(other.inProcessOrders),completedOrders(other.completedOrders),customers(other.customers),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter)
{
    other.volunteerCounter = 0;
    other.customerCounter = 0;
    other.orderCounter = 0;
    other.isOpen = false;
}
WareHouse& WareHouse:: operator=(WareHouse&& other){
    if(this != &other){
        deepDelete(*this);
        
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        isOpen = other.isOpen;

        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
        actionsLog = std::move(other.actionsLog);

        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
        other.isOpen = false;
    }
    return *this;
}

void WareHouse::parseConfig(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath); 
    if (!configFile.is_open()) {
        std::cerr << "Error: Unable to open configuration file: " << configFilePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        parseLine(line);
    }

    configFile.close();
}

void WareHouse::parseLine(const std::string& line) {
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    if (command == "customer") {
        std::string name, type;
        int distance, maxOrders;
        iss >> name >> type >> distance >> maxOrders;
        Customer* customer = nullptr;

        if (type == "soldier") {
            customer = new SoldierCustomer(customerCounter, name, distance, maxOrders);
            customerCounter++;
        }else if(type == "civilian"){
            customer = new CivilianCustomer(customerCounter, name, distance, maxOrders);
            customerCounter++;
        }
        else {
            std::cerr << "Error: Unknown customer type - " << type << std::endl;
            return;
        }
        addCustomer(customer);
    }
    else if(command == "volunteer"){
        std::string name, role;
        int coolDown, maxDistance, distancePerStep, maxOrders;
        iss >> name >> role ;

        if (role == "collector") {
            iss >> coolDown;
            Volunteer* volunteer = new CollectorVolunteer(volunteerCounter, name, coolDown);
            volunteerCounter++;
            addVolunteer(volunteer);
        } else if (role == "limited_collector") {
            iss >> coolDown >> maxOrders;
            Volunteer* volunteer = new LimitedCollectorVolunteer(volunteerCounter, name, coolDown, maxOrders);
            volunteerCounter++;
            addVolunteer(volunteer);
        } else if (role == "driver") {
            iss >> maxDistance >> distancePerStep;
            Volunteer* volunteer = new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep);
            volunteerCounter++;
            addVolunteer(volunteer);
        } else if (role == "limited_driver") {
            iss >> maxDistance >> distancePerStep >> maxOrders;
            Volunteer* volunteer = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders);
            volunteerCounter++;
            addVolunteer(volunteer);
        } else {
            std::cerr << "Error: Unknown volunteer role - " << role << std::endl;
            return;
        }
    }
}

void WareHouse::deepDelete(WareHouse& other){
    for (auto *order : other.pendingOrders) {
        delete order;
        order = nullptr;
    }
    other.pendingOrders.clear();
    for (auto *order : other.inProcessOrders) {
        delete order;
        order = nullptr;
    }
    other.inProcessOrders.clear();
    for (auto *order : other.completedOrders) {
        delete order;
        order = nullptr;
    }
    other.completedOrders.clear();
    for (auto *customer : other.customers) {
        delete customer;
        customer = nullptr;
    }
    other.customers.clear();
    for (auto *volunteer : other.volunteers) {
        delete volunteer;
        volunteer = nullptr;
    }
    other.volunteers.clear();
    for (auto *action : other.actionsLog) {
        delete action;
        action = nullptr;
    }
    other.actionsLog.clear();
}
void WareHouse::deepCopy(const WareHouse& other){
    for(auto &volunteer:other.volunteers){
        Volunteer *newVolunteer = volunteer->clone();
        volunteers.push_back(newVolunteer);
    }
    for(auto &customer:other.customers){
        Customer* newCustomer = customer->clone();
        customers.push_back(newCustomer);
    }
    for(auto &order:other.pendingOrders){
        Order *newOrder = new Order(order->getId(),order->getCustomerId(),order->getDistance());
        newOrder->setStatus(order->getStatus());
        newOrder->setCollectorId(order->getCollectorId());
        newOrder->setDriverId(order->getDriverId());
        pendingOrders.push_back(newOrder);
    }
    for(auto &order:other.inProcessOrders){
        Order *newOrder = new Order(order->getId(),order->getCustomerId(),order->getDistance());
        newOrder->setStatus(order->getStatus());
        newOrder->setCollectorId(order->getCollectorId());
        newOrder->setDriverId(order->getDriverId());
        inProcessOrders.push_back(newOrder);
    }
    for(auto &order:other.completedOrders){
        Order *newOrder = new Order(order->getId(),order->getCustomerId(),order->getDistance());
        newOrder->setStatus(order->getStatus());
        newOrder->setCollectorId(order->getCollectorId());
        newOrder->setDriverId(order->getDriverId());
        completedOrders.push_back(newOrder);
    }
    for(auto &action:other.actionsLog){
        BaseAction *newAction = action->clone();
        actionsLog.push_back(newAction);
    }
}
void WareHouse::start() {
    isOpen = true;
    std::cout << "Warehouse is open! \n";

    std::string userInput;
    while (isOpen) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, userInput);

        if (userInput == "close") {
            close();
            break;
        } else {
            processCommand(userInput);
        }
    }
}
void WareHouse::processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "step") {
        int numSteps;
        if (iss >> numSteps) {
            SimulateStep *step = new SimulateStep(numSteps);
            step->act(*this);
        } else {
            std::cout << "Invalid syntax for 'step' command. Please use 'step <number of steps>'.\n";
        }
    } else if (cmd == "order") {
        int customerId;
        if (iss >> customerId) {
            AddOrder *newOrder = new AddOrder(customerId);
            newOrder->act(*this);
        } 
        
    } else if (cmd == "customer") {
        std::string name, type;
        int distance, maxOrders;
        iss >> name >> type >> distance >> maxOrders;
        AddCustomer *newCustomer = new AddCustomer(name,type,distance,maxOrders);
        newCustomer->act(*this);

    } else if (cmd == "orderStatus") {
        int orderId;
        if (iss >> orderId) {
            PrintOrderStatus *newStatus = new PrintOrderStatus(orderId);
            newStatus->act(*this);
        } 

    } else if (cmd == "customerStatus") {
        int customerId;
        if (iss >> customerId) {
            PrintCustomerStatus *newStatus = new PrintCustomerStatus(customerId);
            newStatus->act(*this);
        } 
    } else if (cmd == "volunteerStatus") {
        int volunteerId;
        if (iss >> volunteerId) {
            PrintVolunteerStatus *newStatus = new PrintVolunteerStatus(volunteerId);
            newStatus->act(*this);
        } 
    } else if (cmd == "log") {
        PrintActionsLog *newLog = new PrintActionsLog();
        newLog->act(*this);
    } else if (cmd == "backup") {
        BackupWareHouse *backup1 = new BackupWareHouse();
        backup1->act(*this);
    } else if (cmd == "restore") {
        RestoreWareHouse *restore = new RestoreWareHouse();
        restore->act(*this);
    } else {
        std::cout << "Invalid command. Please enter a valid command.\n";
    }
}

int WareHouse::getVolunteerCounter() const{
    return volunteerCounter;
}
int WareHouse::getCustomerCounter() const{
    return customerCounter;
}
int WareHouse::getOrderCounter() const{
    return orderCounter;
}
void WareHouse::setCustomerCounter(int customerCounter){
    this->customerCounter = customerCounter; 
}
void WareHouse::setOrderCounter(int orderCounter){
    this->orderCounter = orderCounter; 
}
void WareHouse::setVolunteerCounter(int volunteerCounter){
    this->volunteerCounter = volunteerCounter; 
}
void WareHouse::addOrder(Order* order){
    pendingOrders.push_back(order);
}
void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
}
void WareHouse::addCustomer(Customer* customer){
    customers.push_back(customer);
}
void WareHouse::addVolunteer(Volunteer* volunteer){
    volunteers.push_back(volunteer);
}
void WareHouse:: simulateStep(){
    bool hasMoved = false;
        for (auto it = pendingOrders.begin(); it != pendingOrders.end();){
            Order *order = *it;
            hasMoved = false;
            if ((order->enumConvert(order->getStatus()) == "COLLECTING")){
                for (auto *volunteer : volunteers){
                    if(volunteer->whoAmI() == "Driver" && volunteer->canTakeOrder(*order)){
                            volunteer->acceptOrder(*order);
                            order->setStatus(OrderStatus::DELIVERING);
                            order->setDriverId(volunteer->getId());
                            moveOrder(order->getId(),0);
                            hasMoved = true;
                            break;
                    }
                
                }
            }
            if(!hasMoved){
                it++;
            }
        }
        for (auto it = inProcessOrders.begin(); it != inProcessOrders.end();){
            Order *order = *it;
            hasMoved = false;
            if((order->enumConvert(order->getStatus()) == "DELIVERING")){
                if (order->getDriverId() != -1) {
                getVolunteer(order->getDriverId()).step();
                if(getVolunteer(order->getDriverId()).getActiveOrderId() == -1){
                    moveOrder(order->getId(),2);
                    hasMoved = true;
                    order->setStatus(OrderStatus::COMPLETED);
                }
            }
        }
        if(!hasMoved){
                it++;
            }
        }
        for (auto it = pendingOrders.begin(); it != pendingOrders.end();){
            Order *order = *it;
            hasMoved = false;
            if ((order->enumConvert(order->getStatus()) == "PENDING") && order->getId() != -1)
            {
                for (auto *volunteer : volunteers){
                    if(volunteer->whoAmI() == "Collector" && volunteer->canTakeOrder(*order) && volunteer->getId() != -1){
                            volunteer->acceptOrder(*order);
                            order->setStatus(OrderStatus::COLLECTING);
                            order->setCollectorId(volunteer->getId());
                            moveOrder(order->getId(),0);
                            hasMoved = true;
                            break;
                    }
                
                }
            }
            if(!hasMoved){
                it++;
            }
        }
        for (auto it = inProcessOrders.begin(); it != inProcessOrders.end();){
            Order *order = *it;
            hasMoved = false;
            if((order->enumConvert(order->getStatus()) == "COLLECTING")){
                if(order->getCollectorId() != -1){
                getVolunteer(order->getCollectorId()).step();
                if(getVolunteer(order->getCollectorId()).getActiveOrderId() == -1){
                    moveOrder(order->getId(),1);
                    hasMoved = true;
                    }
                }
            }
            if(!hasMoved){
                it++;
            }
        }
    }
    
void WareHouse:: PrintActionsLogWareHouse(){
    for(BaseAction *action: actionsLog){
        std::cout<<action->toString()<<std::endl;
    }
}
Customer &WareHouse::getCustomer(int customerId) const{
    if(customerCounter < customerId){
        return *customers.front();
    }
    for(const auto& customer : customers){
        if (customer->getId() == customerId){
            return *customer;
            }
    }
    return *customers.front();
}
Volunteer &WareHouse::getVolunteer(int volunteerId) const{
    for(const auto& volunteer : volunteers){
        if (volunteer->getId() == volunteerId){
            return *volunteer;}
    }
    return *volunteers.front();
}
Order &WareHouse::getOrder(int orderId) const{
    if(orderCounter < orderId){
        return *pendingOrders.front();
    }
    for(const auto& order : pendingOrders){
        if (order->getId() == orderId){
            return *order;}
    }
    for(const auto& order : inProcessOrders){
        if (order->getId() == orderId){
            return *order;}
    }
    for(const auto& order : completedOrders){
        if (order->getId() == orderId){
            return *order;}
    }
    return *pendingOrders.front();
}
const vector<BaseAction*>& WareHouse:: getActions() const{
    return actionsLog;
}
void WareHouse::close(){
    for(auto *order:pendingOrders){
        if(order->getId() != -1)
        std::cout<<"OrderID: " + std::to_string(order->getId()) + " ,CustomerID: " + std::to_string(order->getCustomerId()) + " OrderStatus: " + order->enumConvert(order->getStatus())<<std::endl;
    }
    for(auto *order:inProcessOrders){
        std::cout<<"OrderID: " + std::to_string(order->getId()) + " ,CustomerID: " + std::to_string(order->getCustomerId()) + " OrderStatus: " + order->enumConvert(order->getStatus())<<std::endl;   
         }
    for(auto *order:completedOrders){
        std::cout<<"OrderID: " + std::to_string(order->getId()) + " ,CustomerID: " + std::to_string(order->getCustomerId()) + " OrderStatus: " + order->enumConvert(order->getStatus())<<std::endl;   
         }
    isOpen = false;
}
void WareHouse::open(){
    isOpen = true;
}
void  WareHouse:: moveOrder(int orderId,int j){//j=0 move from pending-to inprocces, j=1 move from inprocces-to pending, j=2 move from inprocces-to completed
    if (j == 0)
    {
        auto counter = pendingOrders.begin();
        for(auto &order:pendingOrders){
            if(orderId==order->getId()){
                inProcessOrders.push_back(std::move(order));
                pendingOrders.erase(counter);
            }
            counter++;
        }
    }
    if ((j == 1))
    {
        auto counter = inProcessOrders.begin();
        for(auto &order:inProcessOrders){
            if(orderId==order->getId()){
                pendingOrders.push_back(std::move(order));
                inProcessOrders.erase(counter);
            }
            counter++;
        }
    }
    if ((j == 2))
    {
        auto counter = inProcessOrders.begin();
        for(auto &order:pendingOrders){
            if(orderId==order->getId()){
                completedOrders.push_back(std::move(order));
                inProcessOrders.erase(counter);
            }
            counter++;
        }
    }
    
}