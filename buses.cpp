#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// Structure for linked list node (Stations)
struct Station_BUS
{
    string name;
    unordered_map<string, double> distances; // Distances to connected stations
    Station_BUS *next;
};
struct Seats_BUS
{
    string seat_no; // Seat number
    bool isBooked;  // Seat booked status
};
struct Bus
{
    string busNumber; // Bus number
    Seats_BUS Seats_BUS[44];  // 44 Seats_BUS per bus
    bool isBooked;    // Bus booked status
    // You can add more attributes as needed
};

struct Reservation_BUS
{
    string ticketID;
    double price;
    string departureStation;
    string arrivalStation;
    string name;
    string gender;
    Seats_BUS Seats_BUS;
    Reservation_BUS *left;
    Reservation_BUS *right;
    Bus bus[5];
};
struct AVLNode_BUS
{
    Reservation_BUS *reservation_BUS; // Pointer to the reservation_BUS
    AVLNode_BUS *left;                    // Left child
    AVLNode_BUS *right;                   // Right child
    int height;                       // Height of the node
};

// Function to get the height of the tree
int height(AVLNode_BUS *node)
{
    if (node == nullptr)
        return 0;
    return node->height;
}

// Function to get the balance factor of the node
int getBalance(AVLNode_BUS *node)
{
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

// Right rotate
AVLNode_BUS *rightRotate(AVLNode_BUS *y)
{
    AVLNode_BUS *x = y->left;
    AVLNode_BUS *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

// Left rotate
AVLNode_BUS *leftRotate(AVLNode_BUS *x)
{
    AVLNode_BUS *y = x->right;
    AVLNode_BUS *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}
// Structure for tree node (Seats_BUS and Cabins)

// Class for Train Ticket Reservation_BUSSystem
class BusSystem
{
private:
    Station_BUS *StationHead_BUS;                                    // Linked list for stations
    AVLNode_BUS *reservationRoot_BUS;                                // Binary Search Tree for reservations
    unordered_map<string, Reservation_BUS *> reservationMap_BUS; // Map for quick ticket lookup

    // Generate unique ticket ID
    string generateUniqueID()
    {
        srand(time(0));
        return "TICKET-" + to_string(rand() % 10000 + 1);
    }
    AVLNode_BUS *insertReservation(AVLNode_BUS *node, Reservation_BUS *reservation_BUS)
    {
        // Normal BST insert
        if (node == nullptr)
        {
            AVLNode_BUS *newNode = new AVLNode_BUS();
            newNode->reservation_BUS = reservation_BUS;
            newNode->left = nullptr;
            newNode->right = nullptr;
            newNode->height = 1; // New node is initially added at leaf
            return newNode;
        }

        if (reservation_BUS->ticketID < node->reservation_BUS->ticketID)
            node->left = insertReservation(node->left, reservation_BUS);
        else if (reservation_BUS->ticketID > node->reservation_BUS->ticketID)
            node->right = insertReservation(node->right, reservation_BUS);
        else // Duplicate ticket IDs are not allowed
            return node;

        // Update the height of this ancestor node
        node->height = 1 + max(height(node->left), height(node->right));

        // Get the balance factor of this ancestor node to check whether
        // this node became unbalanced
        int balance = getBalance(node);

        // If this node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && reservation_BUS->ticketID < node->left->reservation_BUS->ticketID)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && reservation_BUS->ticketID > node->right->reservation_BUS->ticketID)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && reservation_BUS->ticketID > node->left->reservation_BUS->ticketID)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && reservation_BUS->ticketID < node->right->reservation_BUS->ticketID)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Return the (unchanged) node pointer
        return node;
    }
    // Insert Reservation_BUSinto BST
    AVLNode_BUS *deleteReservation(AVLNode_BUS *root, string ticketID)
    {
        if (root == nullptr)
            return root;

        // Perform standard BST delete
        if (ticketID < root->reservation_BUS->ticketID)
            root->left = deleteReservation(root->left, ticketID);
        else if (ticketID > root->reservation_BUS->ticketID)
            root->right = deleteReservation(root->right, ticketID);
        else
        {
            // Node with only one child or no child
            if ((root->left == nullptr) || (root->right == nullptr))
            {
                AVLNode_BUS *temp = root->left ? root->left : root->right;

                // No child case
                if (temp == nullptr)
                {
                    temp = root;
                    root = nullptr;
                }
                else               // One child case
                    *root = *temp; // Copy the contents of the non-empty child

                delete temp;
            }
            else
            {
                // Node with two children: Get the inorder successor (smallest in the right subtree)
                AVLNode_BUS *temp = root->right;
                while (temp->left != nullptr)
                    temp = temp->left;

                // Copy the inorder successor's data to this node
                root->reservation_BUS = temp->reservation_BUS;

                // Delete the inorder successor
                root->right = deleteReservation(root->right, temp->reservation_BUS->ticketID);
            }
        }

        // If the tree had only one node then return
        if (root == nullptr)
            return root;

        // Update the height of the current node
        root->height = 1 + max(height(root->left), height(root->right));

        // Get the balance factor of this node to check whether
        // this node became unbalanced
        int balance = getBalance(root);

        // If this node becomes unbalanced, then there are 4 cases

        // Left Left Case
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        // Left Right Case
        if (balance > 1 && getBalance(root->left) < 0)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        // Right Left Case
        if (balance < -1 && getBalance(root->right) > 0)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // In-order traversal to display reservations
    void displayReservationsInOrder(AVLNode_BUS *root)
    {
        if (!root)
            return;

        displayReservationsInOrder(root->left);

        for (int i = 0; i < 4; i++)
        {
            if (root->reservation_BUS->bus[i].isBooked == true)
            {
                cout << "========================================\n";
                cout << "             BUS Reservation_BUS          \n";
                cout << "========================================\n";
                cout << "| TICKET ID:            | " << root->reservation_BUS->ticketID << endl;
                cout << "| NAME:                 | " << root->reservation_BUS->name << endl;
                cout << "| GENDER:               | " << root->reservation_BUS->gender << endl;
                cout << "| Station_BUS OF DEPARTURE: | " << root->reservation_BUS->departureStation << endl;
                cout << "| Station_BUS OF ARRIVAL:   | " << root->reservation_BUS->arrivalStation << endl;
                cout << "| BUS NUMBER:           | " << root->reservation_BUS->bus[i].busNumber << endl;
                cout << "| PRICE:                | " << (root->reservation_BUS->price) * (44) << " PKR\n";
                cout << "========================================\n";
            }
        }

        for (int i = 0; i < 4; i++)
        {
            if (root->reservation_BUS->bus[i].isBooked == false)
            {
                for (int j = 0; j < 12; j++)
                {
                    if (root->reservation_BUS->bus[i].Seats_BUS[j].isBooked == true)
                    {
                        cout << "========================================\n";
                        cout << "            CABIN Reservation_BUS          \n";
                        cout << "========================================\n";
                        cout << "| TICKET ID:            | " << root->reservation_BUS->ticketID << endl;
                        cout << "| NAME:                 | " << root->reservation_BUS->name << endl;
                        cout << "| GENDER:               | " << root->reservation_BUS->gender << endl;
                        cout << "| Station_BUS OF DEPARTURE: | " << root->reservation_BUS->departureStation << endl;
                        cout << "| Station_BUS OF ARRIVAL:   | " << root->reservation_BUS->arrivalStation << endl;
                        cout << "| SEAT NUMBER:          | " << j + 1 << endl;
                        cout << "| PRICE:                | " << (root->reservation_BUS->price) << " PKR\n";
                        cout << "========================================\n";
                    }
                }
            }
        }
        displayReservationsInOrder(root->right);
    }

public:
    BusSystem() : StationHead_BUS(nullptr), reservationRoot_BUS(nullptr) {}

    //===========================================================================Admin functions================================================================================================
    void addStation(const string &name, const unordered_map<string, double> &distances)
    {
        Station_BUS *newStation = new Station_BUS{name, distances, nullptr};
        if (!StationHead_BUS)
        {
            StationHead_BUS = newStation;
            return;
        }
        Station_BUS *temp = StationHead_BUS;
        while (temp->next)
            temp = temp->next;
        temp->next = newStation;

        // Now ensure the connection is bidirectional by adding reverse connections
        for (const auto &pair : distances)
        {
            string connectedStation = pair.first;
            double distance = pair.second;

            // Find the connected Station_BUS in the list
            Station_BUS *connectedStationNode = StationHead_BUS;
            while (connectedStationNode && connectedStationNode->name != connectedStation)
                connectedStationNode = connectedStationNode->next;

            if (connectedStationNode)
            {
                // Add reverse connection (connectedStation -> Station_BUS)
                connectedStationNode->distances[name] = distance;
            }
        }
    }

    // Admin: Display all stations
    void displayStations()
    {
        Station_BUS *temp = StationHead_BUS;
        if (!temp)
        {
            cout << "No stations available.\n";
            return;
        }
        cout << "========================================\n";
        cout << "          AVAILABLE STATIONS:\n";
        cout << "========================================\n";
        while (temp)
        {
            cout << "- " << temp->name << "\n";
            temp = temp->next;
        }
        cout << "========================================\n";
    }

    // Admin: Edit a Station_BUS
    void editStation()
    {
        displayStations();
        string stationName;
        cout << "Enter the name of the Station_BUS to edit: ";
        cin >> stationName;

        Station_BUS *temp = StationHead_BUS;
        while (temp && temp->name != stationName)
            temp = temp->next;

        if (!temp)
        {
            cout << "Station_BUS not found.\n";
            return;
        }

        cout << "Editing distances for Station_BUS: " << stationName << endl;
        for (auto &pair : temp->distances)
        {
            cout << "Enter distance to " << pair.first << " (current: " << pair.second << " km): ";
            cin >> pair.second;
        }
        cout << "Station_BUS distances updated successfully.\n";
    }
    Station_BUS *merge(Station_BUS *left, Station_BUS *right)
    {
        if (!left)
            return right;
        if (!right)
            return left;

        if (left->name < right->name)
        {
            left->next = merge(left->next, right);
            return left;
        }
        else
        {
            right->next = merge(left, right->next);
            return right;
        }
    }
    void Sort()
    {
        mergeSort(&StationHead_BUS);
    }

    // Function to split the linked list into two halves
    void split(Station_BUS *source, Station_BUS **front, Station_BUS **back)
    {
        Station_BUS *fast;
        Station_BUS *slow;
        slow = source;
        fast = source->next;

        // Move fast two nodes and slow one node
        while (fast != nullptr)
        {
            fast = fast->next;
            if (fast != nullptr)
            {
                slow = slow->next;
                fast = fast->next;
            }
        }

        // Slow is before the midpoint, so split it in two
        *front = source;
        *back = slow->next;
        slow->next = nullptr;
    }

    // Function to perform merge sort on the linked list
    void mergeSort(Station_BUS **headRef)
    {
        Station_BUS *head = *headRef;
        Station_BUS *left;
        Station_BUS *right;

        // Base case: if the list is empty or has one node
        if (!head || !head->next)
        {
            return;
        }

        // Split the list into two halves
        split(head, &left, &right);

        // Recursively sort the sublists
        mergeSort(&left);
        mergeSort(&right);

        // Merge the sorted lists
        *headRef = merge(left, right);
    }
    // Function to delete a specific Station_BUS from the linked list
    void deleteStation(const string &stationName)
    {
        Station_BUS *temp = StationHead_BUS;
        Station_BUS *prev = nullptr;

        // Traverse the list to find the Station_BUS
        while (temp != nullptr && temp->name != stationName)
        {
            prev = temp;
            temp = temp->next;
        }

        // If the Station_BUS was not found
        if (temp == nullptr)
        {
            cout << "Station_BUS not found.\n";
            return;
        }

        // If the Station_BUS is the head of the list
        if (prev == nullptr)
        {
            StationHead_BUS = temp->next; // Change head
        }
        else
        {
            prev->next = temp->next; // Unlink the Station_BUS
        }

        delete temp; // Free memory
    }

    //================================================================================================================================================================================================================================
    void reserveTicket()
    {
        displayStations();
        string departureStation, arrivalStation, name, gender;
        int coach_number, cabin_number, seat_number;
        Bus Coach_no[4];
        cout << "========================================\n";
        cout << "    ENTER YOUR CURRENT Station_BUS NAME: \n";
        cout << "========================================\n";
        cin >> departureStation;
        cout << "========================================\n";

        // Find the departure Station_BUS in the linked list
        Station_BUS *departureNode = StationHead_BUS;
        while (departureNode && departureNode->name != departureStation)
        {
            departureNode = departureNode->next;
        }

        // Check if the departure Station_BUS exists
        if (!departureNode)
        {
            cout << "Station_BUS NOT FOUND [please try again]\n";
            return;
        }

        // Display the list of connected cities for the departure Station_BUS
        cout << "CITIES CONNECTED TO " << departureStation << ":\n";
        cout << "========================================\n";
        for (const auto &pair : departureNode->distances)
        {
            cout << "- " << pair.first << "\n"; // Display connected cities
        }

        cout << "ENTER YOUR DESTINATION FROM THE ABOVE LIST : ";
        cin >> arrivalStation;

        // Check if the arrival Station_BUS exists in the connected cities
        if (departureNode->distances.find(arrivalStation) == departureNode->distances.end())
        {
            cout << "INVALID DESTINATION [please try again].\n";
            return;
        }
        double distance = departureNode->distances[arrivalStation];
        double prices = distance * 11.67; // Price per km

        // Generate a unique ticket ID
        string ticketID = generateUniqueID();

        cout << "ENTER YOUR NAME: ";
        cin >> name;
        cout << "EnNTER YOUR GENDER: ";
        cin >> gender;

        int reservation_option;
        cout << "========================================\n";
        cout << "1. RESERVE A SEAT" << endl;
        cout << "2. RESERVE A COMPLETE BUS " << endl;
        cout << "========================================\n";
        cout << "  PLEASE ENTER YOUR Reservation_BUSOPTION  \n";
        cout << "========================================\n";
        cin >> reservation_option;

        if (reservation_option == 1)
        {
            cout << "AVAILABLE COACHES" << endl;
            for (int i = 0; i < 4; i++)
            {
                if (!Coach_no[i].isBooked)
                {
                    cout << " COACH NUMBER : " << i + 1 << " " << endl;
                }
            }

            cout << "========================================\n";
            cout << "      PLEASE ENTER THE BUS NUMBER : \n";
            cout << "========================================\n";
            cin >> coach_number;

            if (coach_number < 1 || coach_number > 4)
            {
                cout << "INVALID COACH NUMBER [please enter a number from the above given list]\n";
                return;
            }

            cout << "Available Cabins" << endl;
            for (int i = 0; i < 44; i++)
            {
                if (!Coach_no[coach_number - 1].Seats_BUS[i].isBooked)
                {
                    cout << "Seat Number: " << i + 1 << endl;
                }
            }

            cout << "========================================\n";
            cout << "     PLEASE ENTER THE SEAT NUMBER: \n";
            cout << "========================================\n";
            cin >> seat_number;

            if (seat_number < 1 || seat_number > 44)
            {
                cout << "INVALID SEAT NUMBER\n";
                return;
            }

            // Mark seat as booked
            Coach_no[coach_number - 1].Seats_BUS[seat_number - 1].isBooked = true;

            // Create a new reservation_BUS
            Reservation_BUS *newReservation_BUS = new Reservation_BUS{
                ticketID,
                prices,
                departureStation,
                arrivalStation,
                name,
                gender,
                Coach_no[coach_number - 1].Seats_BUS[seat_number - 1]};

                // Insert the Reservation_BUSinto the AVL
                reservationRoot_BUS = insertReservation(reservationRoot_BUS, newReservation_BUS);
            reservationMap_BUS[ticketID] = newReservation_BUS;
            cout << "========================================\n";
            cout << "       TICKET RESERVED SUCCESSFULY!\n";
            cout << "========================================\n";
            cout << "| TICKET ID:         | " << ticketID << endl;
            cout << "| NAME:              | " << name << endl;
            cout << "| GENDER:            | " << gender << endl;
            cout << "| DEPARTURE Station_BUS: | " << departureStation << endl;
            cout << "| ARRIVAL Station_BUS:   | " << arrivalStation << endl;
            cout << "| DISTANCE:          | " << distance << " km\n";
            cout << "| PRICE:             | " << prices << " PKR\n";
            cout << "| BUS NUMBER:        | " << coach_number << endl;
            cout << "| SEAT NUMBER:       | " << seat_number << endl;
            cout << "| DISTANCE:          | " << distance << " PKR\n";
            cout << "========================================\n";
        }
        else if (reservation_option == 2)
        {
            cout << "========================================\n";
            cout << "            AVAILABLE BUSES" << endl;
            cout << "========================================\n";
            for (int i = 0; i < 4; i++)
            {
                if (!Coach_no[i].isBooked)
                {
                    cout << " BUS NUMBER: " << i + 1 << " " << endl;
                }
            }

            cout << "========================================\n";
            cout << "    PLEASE ENTER THE BUS NUMBER: ";
            cout << "========================================\n";
            cin >> coach_number;

            if (coach_number < 1 || coach_number > 4)
            {
                cout << "INVALID COACH NUMBER\n";
                return;
            }

            Coach_no[coach_number - 1].isBooked = true;
            Seats_BUS seat;
            Reservation_BUS *newReservation_BUS = new Reservation_BUS{
                ticketID,
                prices,
                departureStation,
                arrivalStation,
                name,
                gender,
                seat,
                nullptr,
                nullptr,
                Coach_no[coach_number - 1]};

            // Insert the Reservation_BUSinto the BST
            reservationRoot_BUS = insertReservation(reservationRoot_BUS, newReservation_BUS);
            reservationMap_BUS[ticketID] = newReservation_BUS;
        }
        else
        {
            cout << "INVALID Reservation_BUSOPTION" << endl;
            return;
        }

        // Calculate the distance and price

        // Display the Reservation_BUSdetails
        cout << "========================================\n";
        cout << "       BUS RESERVED SUCCESSFULY!\n";
        cout << "========================================\n";
        cout << "| TICKET ID:         | " << ticketID << endl;
        cout << "| NAME:              | " << name << endl;
        cout << "| GENDER:            | " << gender << endl;
        cout << "| DEPARTURE Station_BUS: | " << departureStation << endl;
        cout << "| ARRIVAL Station_BUS:   | " << arrivalStation << endl;
        cout << "| DISTANCE:          | " << distance << " km\n";
        cout << "| PRICE:             | " << prices << " PKR\n";
        cout << "| BUS NUMBER:        | " << coach_number << endl;
        cout << "| SEAT NUMBER:       | " << "1-44" << endl;
        cout << "| DISTANCE:          | " << distance << " PKR\n";
        cout << "========================================\n";
    }

    // User: Delete a reservation_BUS
    void
    deleteTicket()
    {
        string ticketID;
        cout << "ENTER THE TICKET ID TO DELETE IT: ";
        cin >> ticketID;

        if (reservationMap_BUS.find(ticketID) == reservationMap_BUS.end())
        {
            cout << "NO TICKET FOUND WITH ID : " << ticketID << ".\n";
            return;
        }

        reservationRoot_BUS = deleteReservation(reservationRoot_BUS, ticketID);
        reservationMap_BUS.erase(ticketID);
        cout << "Reservation_BUSWITH TICKET ID " << ticketID << " DELETED SUCCESSFULLY.\n";
    }

    // User: Display all reservations
    void displayReservations()
    {
        if (!reservationRoot_BUS)
        {
            cout << "NO RESERVATIONS FOUND.\n";
            return;
        }
        displayReservationsInOrder(reservationRoot_BUS);
    }
};

// Main function
int main()
{
    BusSystem system;

    // Initial stations setup
    system.addStation("Karachi", {{"Lahore", 1211}, {"Islamabad", 1411}, {"Quetta", 683.7}, {"Sukkur", 474.5}, {"Multan", 1053.7}, {"Faisalabad", 1281.7}, {"Peshawar", 1483.7}, {"Hyderabad", 163.7}, {"Gwadar", 653.7}, {"Sialkot", 1411}, {"Sargodha", 1411}});
    system.addStation("Lahore", {{"Karachi", 1211}, {"Islamabad", 375}, {"Faisalabad", 128}, {"Multan", 331}, {"Sialkot", 1411}, {"Sargodha", 1411}, {"Peshawar", 189}, {"Sukkur", 1411}, {"Hyderabad", 1411}, {"Gwadar", 1411}, {"Quetta", 1411}, {"Sargodha", 1411}});
    system.addStation("Islamabad", {{"Lahore", 375}, {"Peshawar", 189}, {"Karachi", 1411}, {"Sukkur", 1411}, {"Multan", 1411}, {"Faisalabad", 1411}, {"Hyderabad", 1411}, {"Gwadar", 1411}, {"Quetta", 1411}, {"Sialkot", 1411}, {"Sargodha", 1411}});
    system.addStation("Peshawar", {{"Islamabad", 189}, {"Lahore", 189}, {"Karachi", 1411}, {"Sukkur", 1411}, {"Multan", 1411}, {"Faisalabad", 1411}, {"Hyderabad", 1411}, {"Gwadar", 1411}, {"Quetta", 1411}, {"Sialkot", 1411}, {"Sargodha", 1411}});
    system.addStation("Quetta", {{"Karachi", 683.7}, {"Lahore", 1411}, {"Islamabad", 1411}, {"Sukkur", 1411}, {"Multan", 1411}, {"Faisalabad", 1411}, {"Hyderabad", 1411}, {"Gwadar", 1411}, {"Peshawar", 1411}, {"Sialkot", 1411}, {"Sargodha", 1411}});
    system.addStation("Multan", {{"Karachi", 1053.7}, {"Lahore", 331}, {"Islamabad", 1411}, {"Sukkur", 1411}, {"Faisalabad", 1411}, {"Hyderabad", 1411}, {"Gwadar", 1411}, {"Quetta", 1411}, {"Peshawar", 1411}, {"Sialkot", 1411}, {"Sargodha", 1411}});
    system.addStation("Faisalabad", {{"Karachi", 1281.7}, {"Lahore", 128}, {"Islamabad", 1411}, {"Sukkur", 1411}, {"Multan", 1411}, {"Hyderabad", 1411}, {"Gwadar", 1411}, {"Quetta", 1411}, {"Peshawar", 1411}, {"Sialkot", 1411}, {"Sargodha", 1411}});
    system.addStation("Sukkur", {{"Karachi", 474.5}, {"Lahore", 1411}, {"Islamabad", 1411}, {"Quetta", 1411}, {"Multan", 1411}, {"Faisalabad", 1411}, {"Hyderabad", 1411}, {"Gwadar", 1411}, {"Peshawar", 1411}, {"Sialkot", 1411}, {"Sargodha", 1411}});
    system.addStation("Hyderabad", {{"Karachi", 163.7}, {"Lahore", 1411}, {"Islamabad", 1411}, {"Quetta", 1411}, {"Multan", 1411}, {"Faisalabad", 1411}, {"Sukkur", 1411}, {"Gwadar", 1411}, {"Peshawar", 1411}, {"Sialkot", 1411}, {"Sargodha", 1411}});
    system.addStation("Gwadar", {{"Karachi", 653.7}, {"Lahore", 1411}, {"Islamabad", 1411}, {"Quetta", 1411}, {"Multan", 1411}, {"Faisalabad", 1411}, {"Hyderabad", 1411}, {"Peshawar", 1411}, {"Sialkot", 1411}, {"Sargodha", 1411}, {"Sukkur", 1411}});
    int roleChoice;
    do
    {
        cout << "\n1. Admin Panel\n2. User Panel\n3. Exit\nSelect your role: ";
        cin >> roleChoice;
        string delete_station;

        if (roleChoice == 1)
        {
            int adminChoice;
            do
            {
                cout << "========================================\n";
                cout << "|             ADMIN PANEL:             |\n";
                cout << "========================================\n";
                cout << "1. ADD STATIONS \n2. EDIT STATIONS\n3. DISPLAY STATIONS\n4.SORT THE STATIONS\n5.DELETE A Station_BUS\n6. EXIT\nSELECT AN OPTION: ";
                cin >> adminChoice;

                switch (adminChoice)
                {
                case 1:
                {
                    unordered_map<string, double> distances;
                    string station_name;
                    int connections;

                    cout << "ENTER THE Station_BUS NAME : ";
                    cin.ignore(); // Clear the input buffer to handle the next getline properly
                    getline(cin, station_name);

                    cout << "ENTER THE NUMBER OF CONNECTED STATIONS : ";
                    cin >> connections;

                    for (int i = 0; i < connections; ++i)
                    {
                        string connectedStation;
                        double distance;
                        cout << "ENTER THE NAME OF THE CONNECTED STATIONS AND THE DISTANCE : ";
                        cin >> connectedStation >> distance;
                        distances[connectedStation] = distance;
                    }

                    system.addStation(station_name, distances);
                    cout << "Station_BUS " << station_name << " ADDED SUCCESSFULLY.\n";
                }
                break;
                case 2:
                    system.editStation();
                    cout << "===============================" << endl;
                    cout << "  Station_BUS EDITED SUCCESSFULLY\n";
                    cout << "===============================" << endl;
                    break;
                case 3:
                    system.displayStations();
                    break;
                case 4:
                    system.Sort();
                    cout << "===============================" << endl;
                    cout << "  Station_BUS SORTED SUCCESSFULLY\n";
                    cout << "===============================" << endl;
                    break;
                case 5:

                    cout << "Enter the name of the Station_BUS you want to delete";
                    cin >> delete_station;
                    system.deleteStation(delete_station);
                    cout << "===============================" << endl;
                    cout << " Station_BUS DELETED SUCCESSFULLY\n";
                    cout << "===============================" << endl;
                    break;
                case 6:
                    cout << "===============================" << endl;
                    cout << "    EXITING ADMIN PANEL.\n";
                    cout << "===============================" << endl;
                    break;
                default:
                    cout << "===============================" << endl;
                    cout << "   INVALID CHOICE TRY AGAIN.\n";
                    cout << "===============================" << endl;
                }
            } while (adminChoice != 6);
        }
        else if (roleChoice == 2)
        {
            int userChoice;
            do
            {
                cout << "========================================\n";
                cout << "         BUS TICKET Reservation_BUS       \n";
                cout << "========================================\n";
                cout << "1. Reserve Ticket\n2. Delete Ticket\n3. View Reservations\n4. Exit\n";
                cout << "========================================\n";
                cout << "Select an option: ";
                cin >> userChoice;

                switch (userChoice)
                {
                case 1:
                    system.reserveTicket();
                    break;
                case 2:
                    system.deleteTicket();
                    break;
                case 3:
                    system.displayReservations();
                    break;
                case 4:
                    cout << "===========================================\n";
                    cout << "EXITING THE BUS TICKET Reservation_BUSPANEL.\n";
                    cout << "===========================================\n";
                    break;
                default:
                    cout << "========================================\n";
                    cout << "       INVALID CHOICE .TRY AGAIN\n";
                    cout << "========================================\n";
                }
            } while (userChoice != 4);
        }
        else if (roleChoice != 3)
        {
            cout << "========================================\n";
            cout << "  INVALID ROLE CHOSEN (admin/customer)\n";
            cout << "========================================\n";
        }
    } while (roleChoice != 3);
    cout << "========================================\n";
    cout << "      EXITING THE SYSTEM [GOODBYE] !\n";
    cout << "========================================\n";
    return 0;
}
