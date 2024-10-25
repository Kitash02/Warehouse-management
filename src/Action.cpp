#include "../include/WareHouse.h"
#include "../include/Action.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
#include <string>
#include <iostream>
using std::string;
using std::vector;

extern WareHouse* backup;

BaseAction::BaseAction()
: errorMsg("-1"), status(ActionStatus::COMPLETED)
{}
ActionStatus BaseAction::getStatus() const{
    return status;
}
void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std:: cout << errorMsg + "\n";
}
string BaseAction::getErrorMsg() const{
    return errorMsg;
}

SimulateStep:: SimulateStep(int numOfSteps):
BaseAction(), numOfSteps(numOfSteps)
{}

void SimulateStep:: act(WareHouse &wareHouse){
    for (int i=0;i<numOfSteps;i++)
    {
        wareHouse.simulateStep();
    }
    wareHouse.addAction(this);
}
string SimulateStep::toString() const{
    ActionStatus g = getStatus();
    string s;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    return "SimulateStep " + std::to_string(numOfSteps) + s;
}
SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);
}


AddOrder::AddOrder(int id)
:BaseAction(),customerId(id)
{}
void AddOrder::act(WareHouse &wareHouse) {
    int tmpCounter = wareHouse.getOrderCounter();
    if(wareHouse.getCustomer(customerId).getId() != -1 && wareHouse.getCustomer(customerId).addOrder(tmpCounter) != -1){
        int tmpDistance = wareHouse.getCustomer(customerId).getCustomerDistance();
        Order* order = new Order(tmpCounter,customerId,tmpDistance);
        wareHouse.setOrderCounter(tmpCounter+1);
        wareHouse.addOrder(order);
    }
    else{
        error("Cannot place this order");
    }
    wareHouse.addAction(this);
}
AddOrder* AddOrder::clone() const{
    return new AddOrder(*this);
} 

string AddOrder:: toString() const{
    ActionStatus g = getStatus();
    string s;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    return "Order " + std::to_string(customerId) + s;
}

AddCustomer::AddCustomer(const string  &customerName, const string &customerType, int distance, int maxOrders)
:BaseAction(), customerName(customerName),customerType(stringConvertCustomer(customerType)), distance(distance), maxOrders(maxOrders)
{}
void AddCustomer::act(WareHouse &wareHouse){
    int tmpCounter = wareHouse.getCustomerCounter();
    if(customerType == CustomerType::Soldier){
        SoldierCustomer* customer = new SoldierCustomer(tmpCounter, customerName, distance, maxOrders);
        wareHouse.addAction(this);
        wareHouse.addCustomer(customer);
    }
    else{
        CivilianCustomer* customer = new CivilianCustomer(tmpCounter,customerName,distance,maxOrders);
        wareHouse.addAction(this);
        wareHouse.addCustomer(customer);
    }
    wareHouse.setCustomerCounter(tmpCounter+1);
}
CustomerType AddCustomer::stringConvertCustomer(const std::string& str){
    if(str == "Soldier"){
        return CustomerType::Soldier;
    }
    else{
        return CustomerType::Civilian;
    }
}
AddCustomer* AddCustomer::clone() const{
    return new AddCustomer(*this);
} 
string AddCustomer:: toString() const{
    ActionStatus g = getStatus();
    string s, d;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    if (customerType == CustomerType::Civilian){
        d = " Civilian ";
    }
    else{
        d = " Soldier ";
    }
    return ("Customer " + customerName + d  + std::to_string(distance)+ " " + std::to_string(maxOrders) + s);
}


PrintOrderStatus:: PrintOrderStatus(int id)
:BaseAction::BaseAction(), orderId(id)
{};

void PrintOrderStatus:: act(WareHouse &wareHouse) {
    if(wareHouse.getOrder(orderId).getId() != -1){
        std:: cout <<wareHouse.getOrder(orderId).toString();
    }
    else {
        error("Order doesn't exist");
    }
    wareHouse.addAction(this);
}
PrintOrderStatus* PrintOrderStatus::clone() const{
    return new PrintOrderStatus(*this);
}
string PrintOrderStatus:: toString() const{
    ActionStatus g = getStatus();
    string s;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    return "OrderStatus " + std::to_string(orderId) + s;
}


PrintCustomerStatus::PrintCustomerStatus(int customerId):
BaseAction(),customerId(customerId)
{}

void PrintCustomerStatus::act(WareHouse &wareHouse){
    if(wareHouse.getCustomer(customerId).getId() != -1){
        std:: cout<<("CustomerID = " + std::to_string(wareHouse.getCustomer(customerId).getId()) + "\n");
        for(int order : wareHouse.getCustomer(customerId).getOrdersIds()){
            std::cout<<("OrderID: " + std::to_string(order) + "\n");
            Order realOrder = wareHouse.getOrder(order);
            std::cout<<("OrderStatus: " + realOrder.enumConvert(realOrder.getStatus()) + "\n");
        }
        std::cout<<("numOrdersLeft: " + std::to_string(wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders()) + "\n");
    }
    else {
        error("Customer doesn't exist");
    }
    wareHouse.addAction(this);
}

PrintCustomerStatus* PrintCustomerStatus:: clone() const{
    return new PrintCustomerStatus(*this);
}
string PrintCustomerStatus:: toString() const{
    ActionStatus g = getStatus();
    string s;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    return "CustomerStatus " + std::to_string(customerId) + s;
}

PrintVolunteerStatus::PrintVolunteerStatus(int id):
BaseAction(),volunteerId(id){

}
void PrintVolunteerStatus:: act(WareHouse &wareHouse) {
    if(wareHouse.getVolunteer(volunteerId).getId()!= -1){
        std::cout<<wareHouse.getVolunteer(volunteerId).toString();
    }
    else{
    error("Volunteer doesn't exist");
    }
    wareHouse.addAction(this);
}
PrintVolunteerStatus* PrintVolunteerStatus::clone() const{
    return new PrintVolunteerStatus(*this);
}
string PrintVolunteerStatus:: toString() const{
        ActionStatus g = getStatus();
    string s;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    return "VolunteerStatus " + std::to_string(volunteerId) + s;
}


PrintActionsLog:: PrintActionsLog():BaseAction()
{};

void PrintActionsLog::act(WareHouse &wareHouse) {
    wareHouse.PrintActionsLogWareHouse();
    wareHouse.addAction(this);
}
PrintActionsLog* PrintActionsLog::clone()const{
    return new PrintActionsLog(*this);
}

string PrintActionsLog:: toString()const{
    ActionStatus g = getStatus();
    string s;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    return "log " + s;
}

Close::Close():BaseAction(){
}

void Close::act(WareHouse &wareHouse){
    wareHouse.close();
    wareHouse.addAction(this);
    }

Close* Close::clone() const{
    return new Close(*this);
}

string Close:: toString()const{
    ActionStatus g = getStatus();
    string s;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    return "close " + s;
}


BackupWareHouse::BackupWareHouse():BaseAction(){

}

void BackupWareHouse::act(WareHouse &wareHouse){
    if(backup != nullptr){
        delete backup;
    }
    backup = new WareHouse(wareHouse);
    wareHouse.addAction(this);
}

BackupWareHouse* BackupWareHouse::clone() const{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const{
    ActionStatus g = getStatus();
    string s;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    return "BackupWareHouse " + s;
}



RestoreWareHouse:: RestoreWareHouse():BaseAction(){

}

void RestoreWareHouse:: act(WareHouse &wareHouse){
    if(backup == nullptr){
        error("No backup available");
    }
    else{
        wareHouse = *backup;
    }
    wareHouse.addAction(this);
}

RestoreWareHouse* RestoreWareHouse:: clone() const{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse:: toString() const{
    ActionStatus g = getStatus();
    string s;
    if(g == ActionStatus::COMPLETED){
        s = " COMPLETED";
    }
    else{
        s = " ERROR";
    }
    return "RestoreWareHouse " + s;
}