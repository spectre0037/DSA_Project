#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <list>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Struct to represent Concert_seats
struct Concert_Seats
{
    string seat_no; // Seat number
    bool isBooked;  // Seat booked status
};

// Struct to represent a hall
struct Concert_Hall
{
    string name;
    string location;
    Concert_Seats Concert_seats[50];
    bool isBooked;
    double price; // Price for booking this hall
};

// Struct to represent a Concert_reservation
// Struct to represent a Concert_reservation
struct Concert_Reservation
{
    string name;
    string gender;
    int age; // Added age field
    string ticketID;
    string time;
    string singer_name;
    string type;
    string date;
    double price;
    string location;
    string hallName;   // Concert_Hall name where the Concert_reservation is made
    string seatNumber; // Seat number reserved
};

// Struct to represent a concert
struct Concert
{
    bool isBooked;
    string name;
    string type;
    string date;
    string time;
    string venue;
    double price;
    list<Concert_Reservation> reservations; // List of reservations for this concert
    vector<Concert_Hall> halls;             // Halls associated with this concert

    // Constructor to initialize halls
    Concert()
    {
        halls.resize(5);
        halls[0] = {"Platinum Concert_Hall", venue, {}, false, 500}; // Platinum Concert_Hall
        halls[1] = {"Gold Concert_Hall", venue, {}, false, 400};     // Gold Concert_Hall
        halls[2] = {"Silver Concert_Hall", venue, {}, false, 300};   // Silver Concert_Hall
        halls[3] = {"Bronze Concert_Hall", venue, {}, false, 200};   // Bronze Concert_Hall
        halls[4] = {"VIP Concert_Hall", venue, {}, false, 600};      // VIP Concert_Hall
    }
};

// Class to manage the concert graph
class ConcertGraph
{
private:
    unordered_map<string, Concert> concerts; // Map of concert name to Concert object

public:
    // Constructor
    ConcertGraph() {}

    // Add a concert to the graph
    void addConcert(const string &name, const string &type, const string &date, const string &time, const string &venue, double price)
    {
        Concert newConcert;
        newConcert.isBooked = false;
        newConcert.name = name;
        newConcert.type = type;
        newConcert.date = date;
        newConcert.time = time;
        newConcert.venue = venue;
        newConcert.price = price;
        concerts[name] = newConcert; // Add to the graph
    }

    // Display all concerts
    void displayConcerts()
    {
        if (concerts.empty())
        {
            cout << "No Concerts available.\n";
            return;
        }
        cout << "========================================\n";
        cout << "          AVAILABLE CONCERTS:\n";
        cout << "========================================\n";
        for (const auto &pair : concerts)
        {
            const Concert &concert = pair.second;
            cout << "NAME  :" << concert.name << "\n";
            cout << "TYPE  :" << concert.type << "\n";
            cout << "DATE  :" << concert.date << "\n";
            cout << "TIME  :" << concert.time << "\n";
            cout << "VENUE :" << concert.venue << "\n";
            cout << "PRICE :" << concert.price << "\n";
        }
        cout << "========================================\n";
    }

    // Reserve a ticket for a concert
    void reserveTicket(const string &concertName, const string &name, const string &gender, int age)
    {
        if (concerts.find(concertName) == concerts.end())
        {
            cout << "Concert not found.\n";
            return;
        }

        Concert &concert = concerts[concertName];

        // Check if any seat or hall is already booked
        if (concert.isBooked || std::any_of(concert.halls.begin(), concert.halls.end(), [](const Concert_Hall &hall)
                                            { return hall.isBooked; }))
        {
            cout << "There is already a booking for this event.\n";
            return;
        }

        int choice;
        cout << "========================================\n";
        cout << "Choose booking type:\n";
        cout << "========================================\n";
        cout << "1. Book Entire Event\n";
        cout << "2. Book a Concert_Hall\n";
        cout << "3. Book a Seat\n";
        cout << "========================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: // Book entire event'
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 50; j++)
                {
                    if (concert.halls[i].Concert_seats[j].isBooked)
                    {
                        cout << " A Seat is already booked here\n";
                        return;
                    }
                }
            }
            concert.isBooked = true;
            {
                string ticketID = to_string(rand() % 10000); // Generate a random ticket ID
                cout << "Event booked successfully for " << concertName << ".\n";
                cout << "========================================\n";
                cout << "           RESERVATION DETAILS\n";
                cout << "========================================\n";
                cout << "| NAME:           | " << name << endl;
                cout << "| TICKET ID:      | " << ticketID << endl;
                cout << "| GENDER:         | " << gender << endl;
                cout << "| AGE:            | " << age << endl;
                cout << "| CONCERT NAME:   | " << concertName << endl;
                cout << "| CONCERT VENUE:  | " << concert.venue << endl;
                cout << "| CONCERT DATE:   | " << concert.date << endl;
                cout << "| CONCERT TIME:   | " << concert.time << endl;
                cout << "| TICKET PRICE:   | " << (concert.price) * 200 << endl;
                cout << "========================================\n";
            }
            break;

        case 2:
        { // Book a hall
            // Check if the event is already booked
            if (concert.isBooked)
            {
                cout << "There is already a booking for this event.\n";
                return;
            }

            cout << "Available Halls:\n";
            for (size_t i = 0; i < concert.halls.size(); ++i)
            {
                cout << i + 1 << ". " << concert.halls[i].name << " - Price: " << concert.halls[i].price << "\n";
            }
            int hallChoice;
            cout << "Select a hall to book: ";
            cin >> hallChoice;

            if (hallChoice < 1 || hallChoice > concert.halls.size() || concert.halls[hallChoice - 1].isBooked)
            {
                cout << "Invalid choice or hall already booked.\n";
                return;
            }

            Concert_Reservation Concert_reservation;
            Concert_reservation.name = name;
            Concert_reservation.gender = gender;
            Concert_reservation.age = age;
            Concert_reservation.ticketID = to_string(rand() % 10000);          // Generate a random ticket ID
            Concert_reservation.hallName = concert.halls[hallChoice - 1].name; // Set hall name
            concert.reservations.push_back(Concert_reservation);               // Add Concert_reservation to concert

            concert.halls[hallChoice - 1].isBooked = true;
            cout << "Concert_Hall " << concert.halls[hallChoice - 1].name << " booked successfully.\n";
            cout << "========================================\n";
            cout << "           RESERVATION DETAILS\n";
            cout << "========================================\n";
            cout << "| NAME:           | " << name << endl;
            cout << "| TICKET ID:      | " << Concert_reservation.ticketID << endl;
            cout << "| GENDER:         | " << gender << endl;
            cout << "| AGE:            | " << age << endl;
            cout << "| HALL NAME:      | " << Concert_reservation.hallName << endl;
            cout << "| CONCERT DATE:   | " << concert.date << endl;
            cout << "| CONCERT VENUE: | " << concert.venue << endl;
            cout << "| CONCERT TIME:   | " << concert.time << endl;
            cout << "========================================\n";
            break;
        }

        case 3:
        { // Book a seat
            // Check if the event is already booked
            if (concert.isBooked)
            {
                cout << "There is already a booking for this event.\n";
                return;
            }

            cout << "Available Halls:\n";
            for (size_t i = 0; i < concert.halls.size(); ++i)
            {
                cout << i + 1 << ". " << concert.halls[i].name << " - Price: " << concert.halls[i].price << "\n";
            }
            int hallChoice;
            cout << "Select a hall to book a seat: ";
            cin >> hallChoice;

            if (hallChoice < 1 || hallChoice > concert.halls.size() || concert.halls[hallChoice - 1].isBooked)
            {
                cout << "Invalid choice or hall already booked.\n";
                return;
            }

            Concert_Hall &selectedHall = concert.halls[hallChoice - 1];
            cout << "========================================\n";
            cout << "Available Concert_Seats in " << selectedHall.name << ":\n";
            cout << "========================================\n";

            // Display available Concert_seats
            for (int i = 0; i < 50; i++)
            {
                selectedHall.Concert_seats[i].seat_no = to_string(i + 1); // Assign seat numbers
            }
            for (int i = 0; i < 50; ++i)
            {
                if (!selectedHall.Concert_seats[i].isBooked)
                {
                    cout << selectedHall.Concert_seats[i].seat_no << " | "; // Print available seat numbers
                }
                if (i == 10 || i == 20 || i == 30 || i == 40)
                {
                    cout << "\n"; // New line for better readability
                }
            }
            cout << "\n"; // New line for better readability

            cout << "Enter seat number to book: ";
            string seatNumber;
            cin >> seatNumber;

            // Check if the seat number is valid
            bool seatFound = false;
            for (int i = 0; i < 50; ++i)
            {
                if (selectedHall.Concert_seats[i].seat_no == seatNumber)
                {
                    if (!selectedHall.Concert_seats[i].isBooked)
                    {
                        selectedHall.Concert_seats[i].isBooked = true;
                        cout << "Seat " << seatNumber << " booked successfully in " << selectedHall.name << ".\n";
                        seatFound = true;

                        // Create a Concert_reservation entry
                        Concert_Reservation Concert_reservation;
                        Concert_reservation.name = name;
                        Concert_reservation.gender = gender;
                        Concert_reservation.age = age;
                        Concert_reservation.ticketID = to_string(rand() % 10000); // Generate a random ticket ID
                        Concert_reservation.hallName = selectedHall.name;         // Set hall name
                        Concert_reservation.seatNumber = seatNumber;              // Set seat number
                        concert.reservations.push_back(Concert_reservation);      // Add Concert_reservation to concert

                        cout << "========================================\n";
                        cout << "           RESERVATION DETAILS\n";
                        cout << "========================================\n";
                        cout << "| NAME:           | " << name << endl;
                        cout << "| TICKET ID:      | " << Concert_reservation.ticketID << endl;
                        cout << "| GENDER:         | " << gender << endl;
                        cout << "| AGE:            | " << age << endl;
                        cout << "| HALL NAME:      | " << Concert_reservation.hallName << endl;
                        cout << "| SEAT NUMBER:    | " << Concert_reservation.seatNumber << endl;
                        cout << "| CONCERT DATE:   | " << concert.date << endl;
                        cout << "| CONCERT VENUE:  | " << concert.venue << endl;
                        cout << "| CONCERT TIME:   | " << concert.time << endl;
                        cout << "========================================\n";
                        break;
                    }
                    else
                    {
                        cout << "Seat " << seatNumber << " is already booked.\n";
                        return;
                    }
                }
            }
            if (!seatFound)
            {
                cout << "Invalid seat number.\n";
            }
            break;
        }

        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
    // Display reservations for a concert
    // Display reservations for a concert
    void displayReservations(const string &ticketID = "")
    {
        if (ticketID.empty())
        {
            cout << "No ticket ID provided.\n";
            return;
        }

        bool found = false; // Flag to check if the Concert_reservation is found

        // Iterate through all concerts to find the Concert_reservation by ticket ID
        for (const auto &concertPair : concerts)
        {
            const Concert &concert = concertPair.second;

            for (const auto &Concert_reservation : concert.reservations)
            {
                if (Concert_reservation.ticketID == ticketID)
                {
                    // Print ticket-like format
                    cout << "========================================\n";
                    cout << "               TICKET\n";
                    cout << "========================================\n";
                    cout << "| TICKET ID:      | " << Concert_reservation.ticketID << "\n";
                    cout << "| NAME:           | " << Concert_reservation.name << "\n";
                    cout << "| GENDER:         | " << Concert_reservation.gender << "\n";
                    cout << "| AGE:            | " << Concert_reservation.age << "\n";
                    cout << "| CONCERT:        | " << concertPair.first << "\n";
                    cout << "| CONCERT THEME:  | " << concert.type << "\n";
                    cout << "| VENUE:          | " << concert.venue << "\n"; // Assuming venue is a field in Concert
                    cout << "| HALL:           | " << Concert_reservation.hallName << "\n";
                    cout << "| SEAT:           | " << Concert_reservation.seatNumber << "\n";
                    cout << "| DATE:           | " << concert.date << "\n"; // Date and time of the Concert_reservation
                    cout << "| TIME:           | " << concert.time << "\n"; // Date and time of the Concert_reservation
                    cout << "========================================\n";
                    found = true; // Set the flag to true
                    break;        // Exit the inner loop
                }
            }
            if (found)
                break; // Exit the outer loop if found
        }

        if (!found)
        {
            cout << "No Concert_reservation found for ticket ID " << ticketID << ".\n";
        }
    }

    // Delete a Concert_reservation by ticket ID
    void deleteReservation(const string &concertName, const string &ticketID)
    {
        if (concerts.find(concertName) == concerts.end())
        {
            cout << "Concert not found.\n";
            return;
        }

        Concert &concert = concerts[concertName];
        auto it = concert.reservations.begin();
        while (it != concert.reservations.end())
        {
            if (it->ticketID == ticketID)
            {
                it = concert.reservations.erase(it); // Remove Concert_reservation
                cout << "Concert_Reservation with ticket ID " << ticketID << " deleted successfully.\n";
                return;
            }
            else
            {
                ++it;
            }
        }
        cout << "Ticket ID not found in reservations.\n";
    }
};

// Main function to interact with the user
int main()
{
    ConcertGraph graph;

    // Adding some concerts
    graph.addConcert("ATIF-ASLAM-CONCERT", "Music", "2025-01-11", "17:00", "Karachi", 250);
    graph.addConcert("ALI-ZAFAR-CONCERT", "Hip-Hop", "2025-01- 12", "19:00", "Karachi", 250);
    graph.addConcert("NUSRAT-FATEH-ALI-KHAN-CONCERT", "Qawwali", "2025-01-13", "20:00", "Karachi", 250);

    int roleChoice;
    do
    {
        cout << "\n1. Admin Panel\n2. User Panel\n3. Exit\nSelect your role: ";
        cin >> roleChoice;

        if (roleChoice == 1)
        {
            int adminChoice;
            do
            {
                cout << "========================================\n";
                cout << "|             ADMIN PANEL:             |\n";
                cout << "========================================\n";
                cout << "1. ADD CONCERT \n2. DISPLAY CONCERTS\n3. EXIT\nSELECT AN OPTION: ";
                cin >> adminChoice;

                switch (adminChoice)
                {
                case 1:
                {
                    string name, type, date, time, venue;
                    double price;
                    cout << "Enter concert name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter concert type: ";
                    getline(cin, type);
                    cout << "Enter concert date (YYYY-MM-DD): ";
                    getline(cin, date);
                    cout << "Enter concert time (HH:MM): ";
                    getline(cin, time);
                    cout << "Enter concert venue: ";
                    getline(cin, venue);
                    cout << "Enter concert price: ";
                    cin >> price;

                    graph.addConcert(name, type, date, time, venue, price);
                    cout << "Concert added successfully.\n";
                    break;
                }
                case 2:
                    graph.displayConcerts();
                    break;
                case 3:
                    cout << "Exiting admin panel.\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
                }
            } while (adminChoice != 3);
        }
        else if (roleChoice == 2)
        {
            int userChoice;
            do
            {
                cout << "========================================\n";
                cout << "         CONCERT TICKET RESERVATION        \n";
                cout << "========================================\n";
                cout << "1. Reserve Ticket\n2. View Reservations\n3. Delete Concert_Reservation\n4. Exit\n";
                cout << "Select an option: ";
                cin >> userChoice;

                switch (userChoice)
                {
                case 1:
                {
                    string concertName, name, gender;
                    int age;
                    cout << "Enter concert name: ";
                    cin.ignore();
                    getline(cin, concertName);
                    cout << "Enter your name: ";
                    getline(cin, name);
                    cout << "Enter your gender: ";
                    getline(cin, gender);
                    cout << "Enter your age: ";
                    cin >> age;

                    graph.reserveTicket(concertName, name, gender, age);
                    break;
                }
                case 2:
                {
                    string TicketID;
                    cout << "Enter ticket ID to view reservations: ";
                    cin.ignore();
                    getline(cin, TicketID);
                    graph.displayReservations(TicketID);
                    break;
                }
                case 3:
                {
                    string concertName, ticketID;
                    cout << "Enter concert name to delete Concert_reservation: ";
                    cin.ignore();
                    getline(cin, concertName);
                    cout << "Enter ticket ID to delete: ";
                    getline(cin, ticketID);
                    graph.deleteReservation(concertName, ticketID);
                    break;
                }
                case 4:
                    cout << "Exiting user panel.\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
                }
            } while (userChoice != 4);
        }
        else if (roleChoice != 3)
        {
            cout << "Invalid role chosen (admin/customer)\n";
        }
    } while (roleChoice != 3);
}
