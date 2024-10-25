#include <string>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Action.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
using std::string;
using std::vector;


Customer::Customer(int id, const string &name, int locationDistance, int maxOrders)
:id(id),name(name),locationDistance(locationDistance),maxOrders(maxOrders), ordersId()
{

}
const string &Customer::getName() const{
    return name;
}
int Customer::getId() const{
    return id;
}
int Customer::getCustomerDistance() const{
    return locationDistance;
}
int Customer::getMaxOrders() const{
    return maxOrders;
}
int Customer::getNumOrders() const{
    return ordersId.size(); 
}
bool Customer:: canMakeOrder() const{
    if(maxOrders > getNumOrders()){
        return true;
    }
    return false;
}
const vector<int> &Customer::getOrdersIds() const{
    return ordersId;
}
int Customer::addOrder(int orderId){
    if(canMakeOrder()){
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}


SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders)
:Customer(id,name,locationDistance,maxOrders)    
{}
SoldierCustomer* SoldierCustomer::clone() const{
    SoldierCustomer *newSoldier = new SoldierCustomer(getId(),getName(),getCustomerDistance(),getMaxOrders());
    for(int Order : this->getOrdersIds()){
        newSoldier->addOrder(Order);
    }
    return newSoldier;
} 



CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders)
:Customer(id,name,locationDistance,maxOrders)
{}
CivilianCustomer* CivilianCustomer::clone() const{
    CivilianCustomer *newCivilian = new CivilianCustomer(getId(),getName(),getCustomerDistance(),getMaxOrders());
    for(int Order : this->getOrdersIds()){
        newCivilian->addOrder(Order);
    }
    return newCivilian;
}