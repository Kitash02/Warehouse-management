#include <string>
#include <vector>
#include <iostream>
using std::string;
using std::vector;
class BaseAction;
class Volunteer;
class Order;
class Customer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);//constructor
        WareHouse(const WareHouse &other);//copy constructor
        WareHouse& operator=(const WareHouse &other);//assignment operator
        WareHouse& operator=(WareHouse &&other);//move assignment operator
        WareHouse(WareHouse &&other);//move constructor
        ~WareHouse();//destructor
        void parseConfig(const std::string& configFilePath);//2 methods for parsing the file
        void parseLine(const std::string& line);
        void processCommand(const std::string& command) ;//mmethod for proccesing the input of the user
        void deepDelete(WareHouse &other);//method to delete an object (other)
        void deepCopy(const WareHouse &other);//method for deep copy an object into this object
        int getVolunteerCounter() const;
        int getCustomerCounter() const;
        int getOrderCounter() const;
        void setOrderCounter(int orderCounter);
        void setCustomerCounter(int customerCounter);
        void setVolunteerCounter(int volunteerCounter);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer);
        void addVolunteer(Volunteer* volunteer);
        void simulateStep();
        void PrintActionsLogWareHouse();   
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        void moveOrder(int orderId, int j);//moves the order between the different order vectors


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter;//For assigning unique counteer IDs
};

