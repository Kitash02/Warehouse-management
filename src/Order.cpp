#include <string>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Action.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
using std::string;
using std::vector;


Order::Order(int id, int customerId, int distance)
:id(id),customerId(customerId),distance(distance),status(OrderStatus::PENDING),collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER)
{
}

int Order::getId() const {
    return id;
}
int Order::getCustomerId() const {
    return customerId;
}
int Order::getDistance() const{
    return distance;
}
int Order::getCollectorId() const {
    return collectorId;
}
int Order::getDriverId() const {
    return driverId;
}
OrderStatus Order::getStatus() const {
    return status;
}
void Order::setStatus(OrderStatus status){
    this->status = status; 
}
void Order::setCollectorId(int collectorId){
    this->collectorId = collectorId; 
}
void Order::setDriverId(int driverId){
    this->driverId = driverId;
}
const string Order::toString() const{
    string printStatus;
    if(status == OrderStatus::PENDING){
        printStatus = "Pending"; 
    }
    else if(status == OrderStatus::COLLECTING){
        printStatus = "Collecting";
    }
    else if(status == OrderStatus::DELIVERING){
        printStatus = "Delivering";
    }
    else if(status == OrderStatus::COMPLETED){
        printStatus = "Completed";
    }
    string result = "OrderId: " + std::to_string(id) + "\n";
    result += "OrderStatus: " + printStatus + "\n";
    if(customerId == NO_VOLUNTEER){
         result += "customerID:None \n";
    }
    else{
        result += "CustomerID: " + std::to_string(customerId) + "\n";
    }
    if(collectorId == NO_VOLUNTEER){
        result += "collector:None \n";
    }
    else{
        result += "Collector: " + std::to_string(collectorId) + "\n";
    }
    if(driverId == NO_VOLUNTEER){
        result += "driverID:None \n";
    }
    else{
        result += "Driver: " + std::to_string(driverId) + "\n";//fix the option when there is no driver and i should return None
    }
    return result;
}
string Order::enumConvert(OrderStatus type) const{
    if(type == OrderStatus::PENDING){
        return "PENDING";
    }
    else if(type == OrderStatus::COLLECTING){
        return "COLLECTING";
    }
    else if(type == OrderStatus::DELIVERING){
        return "DELIVERING";
    }
    else{
        return "COMPLETED";
    }
}
