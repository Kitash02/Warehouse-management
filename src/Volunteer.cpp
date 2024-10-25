#include <string>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Action.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
using std::string;
using std::vector;


Volunteer::Volunteer(int id, const string &name)
:completedOrderId(NO_ORDER),activeOrderId(NO_ORDER),id(id),name(name)
{
}
 
int Volunteer::getId() const{
    return id;
}
const string &Volunteer::getName() const{
    return name;
}
int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}
int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}
bool Volunteer::isBusy() const{
    if(activeOrderId != NO_ORDER){
        return true;
    }
    return false;
}

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown)
:Volunteer(id,name),coolDown(coolDown),timeLeft(0)
{
}
CollectorVolunteer* CollectorVolunteer::clone() const{
    CollectorVolunteer *v = new CollectorVolunteer(getId(),getName(),coolDown);
    v->setTimeLeft(timeLeft);
    v->activeOrderId = activeOrderId;
    v->completedOrderId = completedOrderId;
    return v;
}
void CollectorVolunteer::step(){
    if(decreaseCoolDown()){
    Volunteer::completedOrderId = Volunteer::activeOrderId;
    Volunteer::activeOrderId = NO_ORDER;
    }
}
int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}
int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}
void CollectorVolunteer::setTimeLeft(int TimeLeft){
    timeLeft = TimeLeft;
}
bool CollectorVolunteer::decreaseCoolDown(){
    timeLeft--;
    if(timeLeft == 0){
        return true;
    }
    return false;    
}
bool CollectorVolunteer::hasOrdersLeft() const{   
    if(coolDown == 0){  
    return false;
    }
    else{
        return true;
    }
}
bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    if(Volunteer::isBusy() && order.getId() >= 0){
      return false;  
    }
    return true;
} 
void CollectorVolunteer::acceptOrder(const Order &order){
    activeOrderId = order.getId();
    timeLeft = coolDown;
}
string CollectorVolunteer::toString() const{
    string busy;
    if(isBusy()){
        busy = "True";
    }
    else{
        busy = "False";
    }
    string result = "VolunteerID: " + std::to_string(getId()) + "\n";
    result += "isBusy: " + busy + "\n";
    if(isBusy()){
    result += "OrderID: " + std::to_string(getActiveOrderId()) + "\n";
    result += "timeLeft: " + std::to_string(getTimeLeft()) + "\n";
    }
    else{
        result += "OrderID: None \n";
        result += "timeLeft: None \n";
    }
    result += "ordersLeft: No Limit\n";
    return result;
}
    string CollectorVolunteer:: whoAmI() const {
        return "Collector";
    }

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int maxOrders)
:CollectorVolunteer(id,name,coolDown),maxOrders(maxOrders),ordersLeft(maxOrders)
{}
LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone()const{
    LimitedCollectorVolunteer *v = new LimitedCollectorVolunteer(getId(),getName(),getCoolDown(),maxOrders);
    v->setTimeLeft(getTimeLeft());
    v->activeOrderId = this->activeOrderId;
    v->completedOrderId = this->completedOrderId;
    v->setNumOrdersLeft(ordersLeft);
    return v;
}
bool LimitedCollectorVolunteer::hasOrdersLeft() const{
    if(ordersLeft != 0){
        return true;
    }
    return false;
}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    if(hasOrdersLeft() && CollectorVolunteer::canTakeOrder(order)){
        return true;
    }
    return false;
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    
        CollectorVolunteer::acceptOrder(order);
        ordersLeft--;
         
}
int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}
int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}
void LimitedCollectorVolunteer::setNumOrdersLeft(int OrdersLeft){
    ordersLeft = OrdersLeft;
}
string LimitedCollectorVolunteer::toString() const{
    if(!hasOrdersLeft()){
        return "Volunteer doesn't exist.\n";
    }
    string busy;
    if(isBusy()){
        busy = "True";
    }
    else{
        busy = "False";
    }
    string result = "VolunteerID: " + std::to_string(getId()) + "\n";
    result += "isBusy: " + busy + "\n";
    if(isBusy()){
    result += "OrderID: " + std::to_string(getActiveOrderId()) + "\n";
    result += "timeLeft: " + std::to_string(getTimeLeft()) + "\n";
    }
    else{
        result += "OrderID: None \n";
        result += "timeLeft: None \n";
    }
    result += "ordersLeft: " + std::to_string(ordersLeft) + "\n";
    return result;
}
string LimitedCollectorVolunteer::whoAmI() const {
    return "Collector";
}

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
:Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep),distanceLeft(maxDistance)
{}
DriverVolunteer* DriverVolunteer::clone() const{
    DriverVolunteer *d = new DriverVolunteer(getId(),getName(),maxDistance, distancePerStep);
    d->setDistanceLeft(distanceLeft);
    d->activeOrderId = this->activeOrderId;
    d->completedOrderId = this->completedOrderId;
    return d;
}
int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}
int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}
int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}
void DriverVolunteer::setDistanceLeft(int DistanceLeft){
    distanceLeft = DistanceLeft;
}
bool DriverVolunteer::decreaseDistanceLeft() {
    distanceLeft = distanceLeft - distancePerStep;
    if(distanceLeft <= 0){
        distanceLeft = 0;
        return true;
    }
    return false;
}
bool DriverVolunteer::hasOrdersLeft() const{
    if(distancePerStep == 0){
        return false;
    }
    else return true;
}
bool DriverVolunteer::canTakeOrder(const Order &order) const{
    if(!Volunteer::isBusy() && maxDistance >= order.getDistance() && order.getDistance() >= 0){//checking that we do not take 
        return true;
    }
    return false;
}
void DriverVolunteer::acceptOrder(const Order &order){
    
        activeOrderId = order.getId();
        distanceLeft = order.getDistance();
    
}
void DriverVolunteer::step(){
    if(decreaseDistanceLeft()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}
string DriverVolunteer::toString() const{
    string busy;
    if(isBusy()){
        busy = "True";
    }
    else{
        busy = "False";
    }
    string result = "VolunteerID: " + std::to_string(getId()) + "\n";
    result += "isBusy: " + busy + "\n";
    if(isBusy()){
    result += "OrderID: " + std::to_string(getActiveOrderId()) + "\n";
    result += "TimeLeft: " + std::to_string(getDistanceLeft()) + "\n";
    }
    else{
        result += "OrderID: None \n";
        result += "TimeLeft: None \n";
    }
    result += "ordersLeft: No Limit\n";
    return result;
}
    string DriverVolunteer:: whoAmI() const {
        return "Driver";
    }

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders)
:DriverVolunteer(id,name,maxDistance,distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders)
{}
LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const{
    LimitedDriverVolunteer *d = new LimitedDriverVolunteer(getId(),getName(),getMaxDistance(),getDistancePerStep(),maxOrders);
    d->setDistanceLeft(getDistanceLeft());
    d->activeOrderId = this->activeOrderId;
    d->completedOrderId = this->completedOrderId;
    d->setNumOrdersLeft(ordersLeft);
    return d;
}
int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}
int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}
void LimitedDriverVolunteer::setNumOrdersLeft(int OrdersLeft){
    ordersLeft = OrdersLeft;
}
bool LimitedDriverVolunteer::hasOrdersLeft() const{
    if(ordersLeft != 0){
        return true;
    }
    return false;
}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{
    if(DriverVolunteer::canTakeOrder(order) && hasOrdersLeft()){
        return true;
    }
    return false;
}
void LimitedDriverVolunteer::acceptOrder(const Order &order){
    DriverVolunteer::acceptOrder(order);
    ordersLeft--;
}
string LimitedDriverVolunteer::toString() const{
    if(!hasOrdersLeft()){
        return "Volunteer doesn't exist.\n";
    }
    string busy;
    if(isBusy()){
        busy = "True";
    }
    else{
        busy = "False";
    }
    string result = "VolunteerID: " + std::to_string(getId()) + "\n";
    result += "isBusy: " + busy + "\n";
    if(isBusy()){
    result += "OrderID: " + std::to_string(getActiveOrderId()) + "\n";
    result += "timeLeft: " + std::to_string(getDistanceLeft()) + "\n";
    }
    else{
        result += "OrderID: None \n";
        result += "timeLeft: None \n";
    }
    result += "ordersLeft: " + std::to_string(ordersLeft) + "\n";
    return result;
}
    string LimitedDriverVolunteer:: whoAmI() const {
        return "Driver";
    }