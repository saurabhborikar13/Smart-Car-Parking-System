# Smart Car Parking System

## Overview

The **Smart Car Parking System** is a C-based application designed to manage and automate the operations of a car parking facility. It provides functionalities such as vehicle registration, parking space management, vehicle owner membership, vehicle exit handling, and various sorting and reporting features. The system uses linked lists to efficiently manage parking spaces and vehicle records.

---

## Features

1. **Vehicle Registration/Update**:
   - Register new vehicles entering the parking lot.
   - Update existing vehicle records.

2. **Vehicle Exit Handling**:
   - Handle vehicle exits by calculating parking duration and fees.
   - Apply discounts based on membership status.
   - Also update the membership if it is possible as per conditions.

3. **Sorting and Reporting**:
   - Sort vehicles by total parking hours.
   - Sort vehicles by total amount paid.
   - Sort parking spaces by occupancy.
   - Sort parking spaces by revenue generated.

4. **Data Persistence**:
   - Save vehicle and parking space data to files (`inputForVehicle.txt` and `inputForSpaces.txt`).
   - Load data from files upon system startup.

5. **Membership Management**:
   - Automatically upgrade vehicle membership based on total parking hours.
   - Apply membership discounts on parking fees.

---

## Data Structures

- **Space**: Represents a parking space with attributes like ID, occupancy status, total occupancy count, and revenue generated.
- **VehicleDB**: Represents a vehicle record with attributes like vehicle number, owner name, arrival and departure times, parking duration, space ID, membership status, total amount paid, and parking count.

---

## How It Works

1. **Initialization**:
   - The system initializes parking spaces and loads existing data from files (`inputForVehicle.txt` and `inputForSpaces.txt`).

2. **Vehicle Entry**:
   - When a vehicle enters, the system checks if it is already registered.
   - If not, it registers the vehicle and allocates an available parking space based on membership status.
   - The arrival time is recorded.

3. **Vehicle Exit**:
   - When a vehicle exits, the system calculates the parking duration and fees.
   - Discounts are applied based on membership status.
   - The parking space is marked as available, and the revenue is updated.

4. **Sorting and Display**:
   - The system allows sorting of vehicles and parking spaces based on various criteria (e.g., total parking hours, revenue, etc.).
   - Users can view detailed information about vehicles and parking spaces.

---

## **Compilation and Execution**

#### **On Windows, macOS, and Linux:**
1. **Open a terminal or command prompt.**
2. **Navigate to the directory containing `smart_parking.c`.**
3. **Compile and run the code:**
   - **On Windows:**
     ```bash
     gcc program.c -o program.exe
     program.exe
     ```
   - **On macOS/Linux:**
     ```bash
     gcc program.c -o program
     ./program
     ```

---

## **Installation**
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/Smart-Car-Parking-System.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Smart-Car-Parking-System
   ```
3. Compile and run as per the instructions above.
