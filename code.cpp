#include <iostream>
#include <regex>
#include <fstream>
#include <string>
using namespace std;

// --- ADDED FOR GLOBAL EXIT ---
class BackToMenuException
{
};

string getInput(string prompt)
{
    string value;
    cout << prompt;
    cin >> value;
    if (value == "x" || value == "X")
    {
        throw BackToMenuException();
    }
    return value;
}

struct passengerNode
{
    string passengerID;
    string Firstname;
    string Lastname;
    string email;
    string phone_number;
    string passportId;
    string password;
    passengerNode *left;
    passengerNode *right;
};

struct FlightNode
{
    string flightID;
    string departure;
    string arrival;
    string departureTime;
    int availableSeats;
    int priceperseat;
    FlightNode *next;
};

struct bookingNode
{
    string passengerID;
    string flightID;
    int seatsBooked;
    int totalPrice;
    bookingNode *next;
};

void saveFlightsToFile(FlightNode *head);
void savePassengersToFile(passengerNode *root, ofstream &fout);
void saveBookingsToFile(bookingNode *head);

void displayFlights(FlightNode *head)
{
    FlightNode *temp = head;
    if (head == 0)
    {
        cout << endl;
        cout << "No Flight available" << endl;
    }
    while (temp != 0)
    {
        cout << "------------------------------------" << endl;
        cout << "Flight ID: " << temp->flightID << "                     " << endl;
        cout << "Departure: " << temp->departure << "                    " << endl;
        cout << "Arrival: " << temp->arrival << "                        " << endl;
        cout << "Departure Time: " << temp->departureTime << "  " << endl;
        cout << "Available Seats: " << temp->availableSeats << "" << endl;
        cout << "Price per Seat: " << temp->priceperseat << "" << endl;
        cout << "------------------------------------" << endl;

        temp = temp->next;
    }
}
passengerNode *addPassenger(passengerNode *root, string passengerID, string Firstname,
                            string Lastname, string email, string phone_number, string passportId, string password)

{
    if (root == nullptr)
    {
        passengerNode *passengernode = new passengerNode;
        passengernode->passengerID = passengerID;
        passengernode->Firstname = Firstname;
        passengernode->Lastname = Lastname;
        passengernode->email = email;
        passengernode->phone_number = phone_number;
        passengernode->passportId = passportId;
        passengernode->password = password;
        passengernode->left = nullptr;
        passengernode->right = nullptr;
        return passengernode;
    }

    if (passengerID < root->passengerID)
    {

        root->left = addPassenger(root->left, passengerID, Firstname, Lastname,
                                  email, phone_number, passportId, password);
    }
    else if (passengerID > root->passengerID)
    {
        root->right = addPassenger(root->right, passengerID, Firstname, Lastname,
                                   email, phone_number, passportId, password);
    }
    else
    {
        cout << "Error: A passenger with ID (" << passengerID << ") already exists." << endl;
    }

    return root;
}
void displayPassengers(passengerNode *root)
{
    if (root == nullptr)
    {
        return;
    }

    displayPassengers(root->left);

    cout << "------------------------------------" << endl;
    cout << " Passenger ID: " << root->passengerID << endl;
    cout << " First name: " << root->Firstname << endl;
    cout << " Last name: " << root->Lastname << endl;
    cout << " Email: " << root->email << "  " << endl;
    cout << " Phone number: " << root->phone_number << endl;
    cout << " Passport Id: " << root->passportId << endl;
    cout << "------------------------------------" << endl;

    displayPassengers(root->right);
}

void displayPassengerById(passengerNode *root, string passengerID)
{
   
    if (root == nullptr)
    {
        cout << "Passenger with ID " << passengerID << " not found." << endl;
        return;
    }

    if (root->passengerID == passengerID)
    {
        cout << "------------------------------------" << endl;
        cout << " Passenger ID: " << root->passengerID << endl;
        cout << " First name: " << root->Firstname << endl;
        cout << " Last name: " << root->Lastname << endl;
        cout << " Email: " << root->email << endl;
        cout << " Phone number: " << root->phone_number << endl;
        cout << " Passport Id: " << root->passportId << endl;
        cout << "------------------------------------" << endl;
        return; 
    }
    
    if (passengerID < root->passengerID)
    {
        return displayPassengerById(root->left, passengerID);
    }
    else
    {
        return displayPassengerById(root->right, passengerID);
    }
}

void cancelBooking(bookingNode *&bookingHead, FlightNode *flightHead, string passengerID)
{
    if (bookingHead == 0)
    {
        cout << "No bookings available to cancel." << endl;
        return;
    }
    bookingNode *temp = bookingHead;
    bookingNode *prev = nullptr;

    while (temp != 0 && temp->passengerID != passengerID)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == 0)
    {
        cout << "No booking found for this passenger." << endl;
        return;
    }
    if (prev == 0)
    {
        bookingHead = temp->next;
    }
    else
    {
        prev->next = temp->next;
    }

    FlightNode *flight = flightHead;
    while (flight != 0 && flight->flightID != temp->flightID)
    {
        flight = flight->next;
    }

    if (flight != 0)
    {
        flight->availableSeats += temp->seatsBooked;
        cout << "Booking canceled successfully! " << temp->seatsBooked << " seats have been restored." << endl;

        saveFlightsToFile(flightHead);
        saveBookingsToFile(bookingHead);
    }
    delete temp;
}

void partialCancelBooking(bookingNode *&bookingHead, FlightNode *flightHead, const string &passengerID)
{
    if (!bookingHead)
    {
        cout << "No bookings available to cancel." << endl;
        return;
    }
    bookingNode *temp = bookingHead;
    bookingNode *prev = nullptr;

    while (temp && temp->passengerID != passengerID)
    {
        prev = temp;
        temp = temp->next;
    }
    if (!temp)
    {
        cout << "No booking found for this passenger." << endl;
        return;
    }
    cout << "You have booked " << temp->seatsBooked << " seats on flight " << temp->flightID << "." << endl;
    cout << "How many seats do you want to cancel? ";
    string cancelInput = getInput("");
    int cancelSeats = stoi(cancelInput);

    if (cancelSeats <= 0 || cancelSeats > temp->seatsBooked)
    {
        cout << "Invalid number of seats to cancel." << endl;
        return;
    }
    FlightNode *flight = flightHead;
    while (flight && flight->flightID != temp->flightID)
    {
        flight = flight->next;
    }
    if (flight)
    {
        flight->availableSeats += cancelSeats;
    }
    temp->seatsBooked -= cancelSeats;
    temp->totalPrice = temp->seatsBooked * flight->priceperseat;

    if (temp->seatsBooked == 0)
    {
        if (prev)
            prev->next = temp->next;
        else
            bookingHead = temp->next;

        delete temp;
        cout << "All seats canceled and booking deleted." << endl;
    }
    else
    {
        cout << cancelSeats << " seats canceled successfully. Remaining seats: " << temp->seatsBooked << endl;
    }

    saveFlightsToFile(flightHead);
    saveBookingsToFile(bookingHead);
}

void popLatestBooking(bookingNode *&bookingHead, FlightNode *flightHead)
{
    if (bookingHead == nullptr)
    {
        cout << "No bookings available to cancel." << endl;
        return;
    }
    bookingNode *latestBooking = bookingHead;
    FlightNode *flight = flightHead;
    while (flight != nullptr && flight->flightID != latestBooking->flightID)
    {
        flight = flight->next;
    }
    if (flight != nullptr)
    {

        flight->availableSeats += latestBooking->seatsBooked;
        cout << "Seats restored to the flight: " << flight->flightID << endl;
    }
    else
    {
        cout << "Flight not found. Could not restore seats." << endl;
    }
    bookingHead = bookingHead->next;
    cout << "Latest booking removed successfully!" << endl;
    cout << "Passenger ID: " << latestBooking->passengerID << endl;
    cout << "Flight ID: " << latestBooking->flightID << endl;
    cout << "Seats Booked: " << latestBooking->seatsBooked << endl;
    cout << "Total Price: " << latestBooking->totalPrice << endl;
   
    saveFlightsToFile(flightHead);
    saveBookingsToFile(bookingHead);

    delete latestBooking;
}
void paymentMethod(bookingNode *&bookingHead, FlightNode *flightHead)
{
    string choice;
    cout << "----Select Payment Method----" << endl;
    cout << "1. Credit Card" << endl;
    cout << "2. Debit Card" << endl;
    cout << "3. Cancel Reservation" << endl;
    choice = getInput("Enter your choice: ");
    if (choice == "1")
    {
        cout << "Payment completed using Credit Card." << endl;
    }
    else if (choice == "2")
    {
        cout << "Payment completed using Debit Card." << endl;
    }
    else if (choice == "3")
    {
        popLatestBooking(bookingHead, flightHead);
    }
    else
    {
        cout << "Invalid choice!" << endl;
    }
}

void bookSeat(passengerNode *passengerRoot, FlightNode *flightHead, bookingNode *&bookingHead, string passengerID)
{
    string flightID;
    int seatsToBook;

    flightID = getInput("Enter the Flight ID to book: ");

    FlightNode *flight = flightHead;
    while (flight != nullptr && flight->flightID != flightID)
    {
        flight = flight->next;
    }
    if (flight == nullptr)
    {
        cout << "Flight ID not found. Booking failed." << endl;
        return;
    }
    cout << "Available Seats: " << flight->availableSeats << endl;
    string seatsInput = getInput("Enter the number of seats to book: ");
    seatsToBook = stoi(seatsInput);

    if (seatsToBook > flight->availableSeats || seatsToBook <= 0)
    {
        cout << "Invalid number of seats. Booking failed." << endl;
        return;
    }

    flight->availableSeats -= seatsToBook;

    bookingNode *newBooking = new bookingNode;

    newBooking->passengerID = passengerID;
    newBooking->flightID = flightID;
    newBooking->seatsBooked = seatsToBook;
    newBooking->totalPrice = seatsToBook * flight->priceperseat;
    newBooking->next = nullptr;

    if (bookingHead == nullptr)
    {

        bookingHead = newBooking;
        bookingHead->next = nullptr;
    }
    else
    {

        newBooking->next = bookingHead;
        bookingHead = newBooking;
    }
    cout << "Booking successful! Total Price: " << newBooking->totalPrice << endl;

   
    saveFlightsToFile(flightHead);
    saveBookingsToFile(bookingHead);

    paymentMethod(bookingHead, flightHead);
}

void passengerOptions(passengerNode *&root, FlightNode *&flighthead, string passengerId, bookingNode *&Bookinghead)
{
    while (true)
    {
        cout << endl
             << endl;
        cout << "------------------------------------------" << endl;
        cout << "          Passenger Options                " << endl;
        cout << "------------------------------------------" << endl;
        cout << " 1. View Available Flights               " << endl;
        cout << " 2. Book a Seat                         " << endl;
        cout << " 3. View My Details                     " << endl;
        cout << " 4. Cancel My Booking (Partial/Full)    " << endl;
        cout << " 5. Logout                              " << endl;
        cout << "-----------------------------------------" << endl;
        cout << " Enter your choice:  ";

        string choice = getInput("");

        if (choice == "1")
        {
            displayFlights(flighthead);
        }
        else if (choice == "2")
        {
            bookSeat(root, flighthead, Bookinghead, passengerId);
        }
        else if (choice == "3")
        {
            displayPassengerById(root, passengerId);
        }
        else if (choice == "4")
        {
            string subChoice;
            cout << "1. Cancel Entire Booking\n";
            cout << "2. Cancel Part of Booking\n";
            subChoice = getInput("Enter your choice: ");

            if (subChoice == "1")
                cancelBooking(Bookinghead, flighthead, passengerId);
            else if (subChoice == "2")
                partialCancelBooking(Bookinghead, flighthead, passengerId);
            else
                cout << "Invalid choice.\n";
        }
        else if (choice == "5")
        {
            cout << "Logging out..." << endl;
            return;
        }
        else
        {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

void searchPassenger(passengerNode *root, string passengerId, string password, FlightNode *&flighthead, bookingNode *&Bookinghead)
{
    if (root == nullptr)
    {
        cout << "Account not found or incorrect password.\n";
        return;
    }

    if (root->passengerID == passengerId)
    {
        if (root->password == password)
        {
            cout << " -----------------------------" << endl;
            cout << "|      Login successful!      |" << endl;
            cout << " -----------------------------" << endl;

            passengerOptions(root, flighthead, passengerId, Bookinghead);
            return;
        }
        else
        {
            cout << "Incorrect password.\n";
            return;
        }
    }

    if (passengerId < root->passengerID)
    {
        return searchPassenger(root->left, passengerId, password, flighthead, Bookinghead);
    }
    else
    {
        return searchPassenger(root->right, passengerId, password, flighthead, Bookinghead);
    }
}

void login(passengerNode *&passengerRoot, FlightNode *&flighthead, bookingNode *&Bookinghead)
{
    string passengerId;
    string password;

    cout << "Enter Passenger Id: ";
    passengerId = getInput(" ");

    cout << "Enter Password: ";
    password = getInput(" ");
    searchPassenger(passengerRoot, passengerId, password, flighthead, Bookinghead);
}

void addFlight(FlightNode *&head, string flightID, string departure, string arrival, string departureTime, int availableSeats, int priceperseat)
{
    FlightNode *newFlight = new FlightNode;
    FlightNode *temp = head;
    while (temp != nullptr)
    {
        if (temp->flightID == flightID)
        {
            cout << "Error: A flight with the same ID (" << flightID << ") already exists. Cannot add duplicate flight." << endl;
            return;
        }
        temp = temp->next;
    }
    newFlight->flightID = flightID;
    newFlight->departure = departure;
    newFlight->arrival = arrival;
    newFlight->departureTime = departureTime;
    newFlight->availableSeats = availableSeats;
    newFlight->priceperseat = priceperseat;
    newFlight->next = head;
    head = newFlight;
}

void displayAllBookings(bookingNode *bookingHead, FlightNode *flightHead)
{
    if (bookingHead == 0)
    {
        cout << "No bookings available." << endl;
        return;
    }

    bookingNode *temp = bookingHead;
    while (temp != nullptr)
    {
        cout << "Passenger ID: " << temp->passengerID << endl;
        cout << "Flight ID: " << temp->flightID << endl;
        cout << "Seats Booked: " << temp->seatsBooked << endl;
        cout << "Total Price: " << temp->totalPrice << endl;
        cout << "-------------------------------" << endl;

        temp = temp->next;
    }

    return;
}

class Admin
{

private:
    string username;
    string password;

public:
    Admin(FlightNode *&head, passengerNode *&passengerRoot, bookingNode *&Bookinghead)
    {
    x:
        cout << "-------------------------------------" << endl;
        cout << " Enter Username: ";
        username = getInput("");
        cout << " Enter Password: ";
        password = getInput("");
        if (username != "user" || password != "123")
        {
            cout << "Invalid Username and Password" << endl;
            goto x;
        }
        else
        {

            string choice;

            while (true)
            {

                cout << "\n\n ----------------------------------" << endl;
                cout << "|            Welcome Admin         |" << endl;
                cout << " ----------------------------------" << endl;

                cout << " 1. Add Flight           " << endl;
                cout << " 2. View Reservations    " << endl;
                cout << " 3. View Flights         " << endl;
                cout << " 4. View Users           " << endl;
                cout << " 5. Exit                 " << endl;
                cout << "-------------------------------" << endl;
                cout << " Enter your choice:     ";
                choice = getInput(" ");

                if (choice == "1")
                {
                    string flightID, departure, arrival, departureTime;
                    int availableSeats, priceperseat;

                    cout << "----------------------------------" << endl;
                    cout << "|        Add Flight Details       |" << endl;
                    cout << "----------------------------------" << endl;

                    do
                    {
                        flightID = getInput("Enter Flight ID (Alphanumeric, Max 10 chars):\n");
                        if (!regex_match(flightID, regex("^[a-zA-Z0-9]{1,10}$")))
                        {
                            cout << "Invalid Flight ID! Please try again." << endl;
                        }
                    } while (!regex_match(flightID, regex("^[a-zA-Z0-9]{1,10}$")));
                    do
                    {
                        departure = getInput("Enter Departure Location (Alphabetic only):\n");
                        if (!regex_match(departure, regex("^[a-zA-Z]+$")))
                        {
                            cout << "Invalid Departure Location! Please try again.\n"
                                 << endl;
                        }
                    } while (!regex_match(departure, regex("^[a-zA-Z]+$")));

                    do
                    {
                        arrival = getInput("Enter Arrival Location (Alphabetic only):\n");
                        if (!regex_match(arrival, regex("^[a-zA-Z]+$")))
                        {
                            cout << "Invalid Arrival Location! Please try again.\n"
                                 << endl;
                        }
                    } while (!regex_match(arrival, regex("^[a-zA-Z]+$")));

                    do
                    {
                        cout << "Enter Departure Time (YYYY-MM-DD HH:MM (AM|PM)):\n";
                        if (!(getline(cin >> ws, departureTime)))
                        {
                            cin.clear();
                            continue;
                        }
                        if (departureTime == "x" || departureTime == "X")
                        {
                            throw BackToMenuException();
                        }
                        regex timePattern(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2} (AM|PM)$)");

                        if (regex_match(departureTime, timePattern))
                        {
                            break; 
                        }
                        else
                        {
                            cout << " >> Error: Invalid Format! Use YYYY-MM-DD HH:MM AM/PM (e.g., 2026-01-22 08:09 AM)\n";
                        }
                    } while (true);

                    do
                    {
                        string seatsStr = getInput("Enter Available Seats (Positive Integer):\n");
                        availableSeats = stoi(seatsStr);
                        if (availableSeats <= 0)
                        {
                            cout << "Invalid Number of Seats! Please try again.\n"
                                 << endl;
                        }
                    } while (availableSeats <= 0);

                    do
                    {
                        string priceStr = getInput("Enter Seats Price:\n");
                        priceperseat = stoi(priceStr);
                        if (priceperseat <= 0)
                        {
                            cout << "Invalid Price! Please try again.\n"
                                 << endl;
                        }
                    } while (priceperseat <= 0);

                    addFlight(head, flightID, departure, arrival, departureTime, availableSeats, priceperseat);
                    cout << "Flight added successfully!" << endl;
                    
                    saveFlightsToFile(head);
                }
                else if (choice == "2")
                {
                    displayAllBookings(Bookinghead, head);
                }

                else if (choice == "3")
                {
                    displayFlights(head);
                }
                else if (choice == "4")
                {
                    displayPassengers(passengerRoot);
                }
                else if (choice == "5")
                {
                    cout << "Exiting portal..." << endl;
                    return;
                }
                else
                {
                    cout << "Invalid choice, please try again." << endl;
                }
            }
        }
    }
};

class passenger
{
public:
    passenger(passengerNode *&passengerRoot, FlightNode *&flighthead, bookingNode *&Bookinghead)
    {
        while (true)
        {

            string choice;
            cout << "------------------------------------------" << endl;
            cout << "          Welcome to Our System            " << endl;
            cout << "------------------------------------------" << endl;
            cout << " 1. Create an account           " << endl;
            cout << " 2. Login to an account         " << endl;
            cout << " 3. Exit                        " << endl;
            cout << "------------------------------------------" << endl;
            cout << " Enter your choice: ";
            choice = getInput(" ");

            if (choice == "1")
            {

                string passengerID, Firstname, Lastname, email, phone_number, password;
                string passportId;

                cout << "----------------------------------" << endl;
                cout << "|       Add Passenger Details     |" << endl;
                cout << "----------------------------------" << endl;

                do
                {
                    passengerID = getInput("Enter Passenger ID (Alphanumeric, Max 10 characters:\n");
                    if (!regex_match(passengerID, regex("^[a-zA-Z0-9]{1,10}$")))
                    {
                        cout << "Invalid ID! Please try again.               \n" << endl;
                    }
                } while (!regex_match(passengerID, regex("^[a-zA-Z0-9]{1,10}$")));
                do
                {
                    Firstname = getInput("Enter First name:\n");
                    if (!regex_match(Firstname, regex("^[a-zA-Z]+$")))
                    {
                        cout << "Invalid name! Please try again.      \n" << endl;
                    }
                } while (!regex_match(Firstname, regex("^[a-zA-Z]+$")));

                do
                {
                    Lastname = getInput("Enter Last name:\n");
                    if (!regex_match(Lastname, regex("^[a-zA-Z]+$")))
                    {
                        cout << "Invalid name! Please try again.      \n" << endl;
                    }
                } while (!regex_match(Lastname, regex("^[a-zA-Z]+$")));

                do
                {
                    email = getInput("Enter Email ID:\n");
                    if (!regex_match(email, regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$")))
                    {
                        cout << "Invalid Email! Please try again.                     \n" << endl;
                    }
                } while (!regex_match(email, regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$")));

                do
                {
                    phone_number = getInput("Enter Phone Number:\n");
                    if (!regex_match(phone_number, regex("^[0-9]{11}$")))
                    {
                        cout << "Invalid Phone Number! Must be exactly 11 digits.     \n" << endl;
                    }
                } while (!regex_match(phone_number, regex("^[0-9]{11}$")));
                do
                {
                    passportId = getInput("Enter Passport Number:\n");
                    if (!regex_match(passportId, regex("^[a-zA-Z0-9]{6,9}$")))
                    {
                        cout << "Invalid Passport Number! Must be 6-9 alphanumeric characters.  \n" << endl;
                    }
                } while (!regex_match(passportId, regex("^[a-zA-Z0-9]{6,9}$")));

                do
                {
                    password = getInput("Enter Password (8 digits):\n");
                    if (!regex_match(password, regex("^[0-9]{8}$")))
                    {
                        cout << "Invalid Password! Must contain exactly 8 digits.     \n" << endl;
                    }
                } while (!regex_match(password, regex("^[0-9]{8}$")));

                passengerRoot = addPassenger(passengerRoot, passengerID, Firstname, Lastname, email, phone_number, passportId, password);
                cout << "Passenger added successfully!" << endl;

                ofstream fout("passengers.txt");
                if (fout.is_open())
                {
                    savePassengersToFile(passengerRoot, fout);
                    fout.close();
                }
            }
            else if (choice == "2")
            {

                login(passengerRoot, flighthead, Bookinghead);
            }
            else if (choice == "3")
            {
                cout << "Exiting portal..." << endl;
                return;
            }
            else
            {
                cout << "Invalid choice, please try again." << endl;
            }
        }
    }
};

void saveFlightsToFile(FlightNode *head)
{
    ofstream fout("flights.txt");
    FlightNode *temp = head;
    while (temp)
    {
        fout << temp->flightID << "," << temp->departure << "," << temp->arrival << ","
             << temp->departureTime << "," << temp->availableSeats << "," << temp->priceperseat << "\n";
        temp = temp->next;
    }
    fout.close();
}

void loadFlightsFromFile(FlightNode *&head)
{
    ifstream fin("flights.txt");
    string flightID, departure, arrival, departureTime;
    int availableSeats, priceperseat;

    while (getline(fin, flightID, ','))
    {
        getline(fin, departure, ',');
        getline(fin, arrival, ',');
        getline(fin, departureTime, ',');
        fin >> availableSeats;
        fin.ignore();
        fin >> priceperseat;
        fin.ignore();

        addFlight(head, flightID, departure, arrival, departureTime, availableSeats, priceperseat);
    }
    fin.close();
}
void savePassengersToFile(passengerNode *root, ofstream &fout)
{

    if (root == nullptr)
    {
        return;
    }
    fout << root->passengerID << "," << root->Firstname << "," << root->Lastname << ","
         << root->email << "," << root->phone_number << "," << root->passportId << "," << root->password << "\n";
    savePassengersToFile(root->left, fout);
    savePassengersToFile(root->right, fout);
}

void loadPassengersFromFile(passengerNode *&root)
{
    ifstream fin("passengers.txt");
    if (!fin.is_open())
        return;

    string passengerID, Firstname, Lastname, email, phone, passportId, password;

    while (getline(fin, passengerID, ','))
    {
        getline(fin, Firstname, ',');
        getline(fin, Lastname, ',');
        getline(fin, email, ',');
        getline(fin, phone, ',');
        getline(fin, passportId, ',');
        getline(fin, password);
        root = addPassenger(root, passengerID, Firstname, Lastname, email, phone, passportId, password);
    }
    fin.close();
}

void saveBookingsToFile(bookingNode *head)
{
    ofstream fout("bookings.txt");
    while (head)
    {
        fout << head->passengerID << "," << head->flightID << "," << head->seatsBooked << "," << head->totalPrice << "\n";
        head = head->next;
    }
    fout.close();
}

void loadBookingsFromFile(bookingNode *&head)
{
    ifstream fin("bookings.txt");
    string passengerID, flightID;
    int seats, total;
    while (getline(fin, passengerID, ','))
    {
        getline(fin, flightID, ',');
        fin >> seats;
        fin.ignore();
        fin >> total;
        fin.ignore();

        bookingNode *newBooking = new bookingNode{passengerID, flightID, seats, total, head};
        head = newBooking;
    }
    fin.close();
}

int main()
{
    FlightNode *head = 0;
    passengerNode *passengerRoot = nullptr;
    bookingNode *Bookinghead = 0;

    loadFlightsFromFile(head);
    loadPassengersFromFile(passengerRoot);
    loadBookingsFromFile(Bookinghead);

    cout << "\n-----------------Flight Reservation System-----------------" << endl
         << endl
         << endl;

    while (true)
    {
        try
        {
            string option;
        t:
            cout << "------------------------------------" << endl;
            cout << " Enter 1: If you're Admin    " << endl;
            cout << " Enter 2: If you're Passenger" << endl;
            cout << " Enter 3: Exit               " << endl;
            cout << "-----------------------------------" << endl;
            cout << " Enter Your Choice: ";
            option = getInput(" ");

            cout << endl
                 << endl;

            if (option == "1")
            {
                Admin admin(head, passengerRoot, Bookinghead);
                goto t;
            }
            else if (option == "2")
            {
                passenger p(passengerRoot, head, Bookinghead);
                goto t;
            }
            else if (option == "3")
            {
                saveFlightsToFile(head);
                ofstream fout("passengers.txt");
                if (fout.is_open())
                {
                    savePassengersToFile(passengerRoot, fout);
                    fout.close();
                }
                saveBookingsToFile(Bookinghead);
                return 0;
            }
            else
            {
                cout << "Invalid Entry. Please enter 1 or 2 or 3.\n";
                goto t;
            }
        }
        catch (const BackToMenuException &e)
        {
            cout << "\n--- Action Cancelled. Returning to Main Menu ---\n"
                 << endl;
        }
    }
    return 0;
}
