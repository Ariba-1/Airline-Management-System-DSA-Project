# Airline Reservation System (C++ & DSA)

## Project Overview
This is a console-based **Airline Reservation System** developed in C++ as a Software Engineering and Data Structures project. The system manages flight schedules, passenger registrations, and seat bookings. It features a dual-portal system for **Admins** and **Passengers**, focusing on efficient data management and persistence.



## Data Structures & Technical Implementation
The project leverages specific Data Structures (DSA) to optimize performance:

* **Binary Search Tree (BST):** Passenger records are organized in a BST, allowing for efficient $O(\log n)$ searching and insertion based on Passenger ID.
* **Singly Linked Lists:** Used to manage the dynamic collection of available Flights and active Bookings.
* **File Handling (`fstream`):** Implements data persistence by saving and loading records from `flights.txt`, `passengers.txt`, and `bookings.txt`.
* **Regular Expressions (`<regex>`):** Ensures data integrity through strict validation of emails, 11-digit phone numbers, and passport formats.
* **Exception Handling:** Uses a custom `BackToMenuException` to allow users to exit any sub-process by entering 'x', improving the overall UX flow.

## Key Features

### Admin Portal
* **Flight Management:** Add new flight routes with unique IDs and schedules.
* **System Oversight:** View a complete list of all registered passengers.
* **Reservation Tracking:** Monitor all current bookings across the system.

### Passenger Portal
* **Account Creation:** Secure registration and login using BST-based lookups.
* **Booking Engine:** Real-time seat availability checks and total price calculation.
* **Flexible Cancellation:** Options for both **Full Cancellation** (removing the booking) and **Partial Cancellation** (restoring specific seats to the flight).
* **Payment Flow:** A simulated credit/debit card payment interface.
