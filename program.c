#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Space
{
    int id;
    int occupied;
    int totalOccupancy;
    int revenue;
    struct Space *next; // Linked list for parking spaces
} space;

struct time
{
    int year;
    int month;
    int day; // for the calculation of the duration between arrival and departuew time
    int hr;
    int min;
    int sec;
};

typedef struct vehicleDB
{
    char vehicleNumber[20];
    char OwnerName[100];
    struct time arrivalTime;
    struct time departureTime;
    int totalParkingHours;
    int spaceID;
    char membership[10];
    int totalAmountPaid;
    int parkingCount;
    struct vehicleDB *next; // Linked list for vehicleDBs
} vehicleDB;

space *parkingLot = NULL;      // Linked list for parking spaces
vehicleDB *vehicleDBDB = NULL; // Linked list for vehicleDB database   // Avoid the dangling pointers scenariao
space *goldPointer = NULL;
space *premiumPointer = NULL;
space *normalPointer = NULL;

int dim(int m, int y)
{
    int dim_list[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // for leap year
    if (m == 2 && (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)))
    { // if the year is divisible by 400 or year is not divisibale by 100 and year is divisible by 4
        return 29;
    }
    return dim_list[m - 1];
}

long long convert_to_sec(int y, int m, int d, int h, int min, int s)
{
    long long secs = 0;
    // Add seconds for each year
    // condidering the time calultion from the baase 2020
    for (int i = 2020; i < y; i++)
    {
        secs += (i % 4 == 0 && (i % 100 != 0 || i % 400 == 0)) ? 366 * 86400 : 365 * 86400;
    }
    // Add seconds for each month in the current year
    for (int i = 1; i < m; i++)
    {
        secs += dim(i, y) * 86400;
    }
    // Add seconds for the current day, hour, minute, and second
    secs += (d - 1) * 86400 + h * 3600 + min * 60 + s;
    return secs;
}

void take_arrival_time(struct time *cur)
{
    // int y,m,d,h,s;

    printf("enter the time in YYYY : MM : DD : HH : MM : SS \n");
    scanf("%d%d%d%d%d%d", &cur->year, &cur->month, &cur->day,
          &cur->hr, &cur->min, &cur->sec);
}

void departure_time(vehicleDB *cur)
{

    printf("enter the time in YYYY : MM : DD : HH : MM : SS \n");
    scanf("%d%d%d%d%d%d", &cur->departureTime.year, &cur->departureTime.month, &cur->departureTime.day,
          &cur->departureTime.hr, &cur->departureTime.min, &cur->departureTime.sec);
}
void initializedeparture_time(struct time *dt)
{
    dt->day = 00;
    dt->min = 00;
    dt->month = 00;
    dt->hr = 00;
    dt->sec = 00;
    dt->year = 00;
}
void initializeparkingSpaces(space **head)
{
    *head = NULL;
    space *prev = NULL;

    for (int i = 1; i <= 50; i++)
    {
        space *newSpace = (space *)malloc(sizeof(space));
        newSpace->id = i;
        newSpace->occupied = 0;
        newSpace->revenue = 0;
        newSpace->totalOccupancy = 0;
        newSpace->next = NULL;

        if (*head == NULL)
        {
            *head = newSpace;
        }
        else
        {
            prev->next = newSpace;
        }
        prev = newSpace;

        if (i == 1)
            goldPointer = newSpace;
        if (i == 11)
            premiumPointer = newSpace;
        if (i == 21)
            normalPointer = newSpace;
    }
}

void writeToFile()
{

    FILE *file1 = fopen("inputForVehicle.txt", "w");
    if (file1 == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    // Writing vehicle data
    vehicleDB *vehicle = vehicleDBDB;
    while (vehicle != NULL)
    {
        fprintf(file1, "%s %s %d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
                vehicle->vehicleNumber, vehicle->OwnerName,
                vehicle->totalParkingHours, vehicle->spaceID,
                vehicle->membership, vehicle->totalAmountPaid,
                vehicle->parkingCount,
                vehicle->arrivalTime.year, vehicle->arrivalTime.month, vehicle->arrivalTime.day,
                vehicle->arrivalTime.hr, vehicle->arrivalTime.min, vehicle->arrivalTime.sec,
                vehicle->departureTime.year, vehicle->departureTime.month, vehicle->departureTime.day,
                vehicle->departureTime.hr, vehicle->departureTime.min, vehicle->departureTime.sec);
        vehicle = vehicle->next;
    }

    fclose(file1);

    FILE *file2 = fopen("inputForSpaces.txt", "w");
    if (file2 == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    // Writing space data
    space *parkingSpace = parkingLot;
    while (parkingSpace != NULL)
    {
        fprintf(file2, "%d %d %d %d\n",
                parkingSpace->id, parkingSpace->occupied,
                parkingSpace->totalOccupancy, parkingSpace->revenue);
        parkingSpace = parkingSpace->next;
    }

    fclose(file2);
}

space *createSpaceNode(int id, int occupied, int totalOccupancy, float revenue)
{
    space *newNode = (space *)malloc(sizeof(space));
    newNode->id = id;
    newNode->occupied = occupied;
    newNode->totalOccupancy = totalOccupancy;
    newNode->revenue = revenue;
    newNode->next = NULL;
    return newNode;
}

// for file handling
void appendToSpace(space **SpaceHead, int id, int occupied, int totalOccupancy, int revenue)
{
    space *newNode = createSpaceNode(id, occupied, totalOccupancy, revenue);
    if (*SpaceHead == NULL)
    {
        *SpaceHead = newNode;
        return;
    }
    space *temp = *SpaceHead;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

void displaySpace_DB(space *SpaceHead)
{
    if (SpaceHead == NULL)
    {
        printf("No parking spaces to display.\n");
        return;
    }

    printf("\n\nParking Spaces:\n\n");
    printf("------------------------------------------------\n");
    printf("%-5s %-10s %-16s %-10s\n",
           "ID", "Occupied", "Total Occupancy", "Revenue");
    printf("------------------------------------------------\n");

    space *temp = SpaceHead;
    while (temp != NULL)
    {
        char occupiedStatus[4];
        strcpy(occupiedStatus, temp->occupied ? "Yes" : "No");

        printf("%-5d %-10s %-16d %-10d\n",
               temp->id,
               occupiedStatus,
               temp->totalOccupancy,
               temp->revenue);

        temp = temp->next;
    }
    printf("------------------------------------------------\n\n");
}

void appendTovehicleDB(vehicleDB **vehicleDBDB, char *number, char *owner, int hours, int id,
                       char *membership, int amount, int count, struct time *t, struct time *depa_time)
{
    vehicleDB *newvehicleDB = (vehicleDB *)malloc(sizeof(vehicleDB));
    strcpy(newvehicleDB->vehicleNumber, number);
    strcpy(newvehicleDB->OwnerName, owner);
    newvehicleDB->totalParkingHours = hours;
    newvehicleDB->spaceID = id;
    strcpy(newvehicleDB->membership, membership);
    newvehicleDB->totalAmountPaid = amount;
    newvehicleDB->parkingCount = count;
    newvehicleDB->arrivalTime.year = t->year;
    newvehicleDB->arrivalTime.month = t->month;
    newvehicleDB->arrivalTime.day = t->day;
    newvehicleDB->arrivalTime.hr = t->hr;
    newvehicleDB->arrivalTime.min = t->min;
    newvehicleDB->arrivalTime.sec = t->sec;

    newvehicleDB->departureTime.year = depa_time->year;
    newvehicleDB->departureTime.month = depa_time->month;
    newvehicleDB->departureTime.day = depa_time->day;
    newvehicleDB->departureTime.hr = depa_time->hr;
    newvehicleDB->departureTime.min = depa_time->min;
    newvehicleDB->departureTime.sec = depa_time->sec;
    newvehicleDB->next = NULL;

    if (*vehicleDBDB == NULL)
    {
        *vehicleDBDB = newvehicleDB;
    }
    else
    {
        vehicleDB *temp = *vehicleDBDB;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newvehicleDB;
    }
}

void gotoThatplaceAndUpdate(space *Spacehead, int id, int occupied, int totalOccupancy, int revenue)
{
    space *temp = Spacehead;
    while (temp && temp->id != id)
    {
        temp = temp->next;
    }

    if (temp)
    {
        temp->occupied = occupied;
        temp->totalOccupancy = totalOccupancy;
        temp->revenue = revenue;
    }
    else
    {
        printf("Error: Space ID %d not found.\n", id);
    }
}

void displayvehicleDB(vehicleDB *head)
{
    if (head == NULL)
    {
        printf("No vehicles to display.\n");
        return;
    }

    printf("Vehicle Info:\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%-15s %-20s %-20s %-10s %-10s %-15s %-12s %-20s %-20s\n", "Vehicle Number", "Owner Name", "Membership", "Park Hr", "Space ID", "Total Paid", "Park Count", "Arrival Time", "Depart Time");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    char arrivalTime[20];
    char departureTime[20];

    vehicleDB *temp = head;
    while (temp != NULL)
    {
        sprintf(arrivalTime, "%02d-%02d-%04d %02d:%02d:%02d",
                temp->arrivalTime.day, temp->arrivalTime.month, temp->arrivalTime.year,
                temp->arrivalTime.hr, temp->arrivalTime.min, temp->arrivalTime.sec);
        sprintf(departureTime, "%02d-%02d-%04d %02d:%02d:%02d",
                temp->departureTime.day, temp->departureTime.month, temp->departureTime.year,
                temp->departureTime.hr, temp->departureTime.min, temp->departureTime.sec);
        printf("%-15s %-20s %-20s %-10d %-10d %-15d %-10d %-20s %-20s\n",
               temp->vehicleNumber, temp->OwnerName, temp->membership,
               temp->totalParkingHours, temp->spaceID, temp->totalAmountPaid, temp->parkingCount, arrivalTime, departureTime);
        temp = temp->next;
    }

    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

vehicleDB *VehicleNewOrNot(char *vehicleNumber)
{
    vehicleDB *temp = vehicleDBDB;
    while (temp)
    {
        if (strcmp(temp->vehicleNumber, vehicleNumber) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL; // Not found
}

space *space_availableorNot(char *prev_membership)
{
    space *temp = NULL;
    space *ans = temp;
    if (strcmp(prev_membership, "Golden") == 0)
    {
        temp = goldPointer;
    }
    else if (strcmp(prev_membership, "Premium") == 0)
    {
        temp = premiumPointer;
    }
    else
    {
        temp = normalPointer;
    }

    int flag = 0;
    while (temp && !flag)
    {
        if (temp->occupied == 0)
        {
            ans = temp;
            flag = 1;
        }
        temp = temp->next;
    }
    return ans; // No space available
}

void updateSpaceDB(space *empty_space)
{
    empty_space->occupied = 1;
    empty_space->totalOccupancy++;
}

space *getSpaceNode(int spaceID)
{
    space *temp = parkingLot;
    while (temp->id != spaceID && temp != NULL)
    {
        temp = temp->next;
    }
    return temp;
}

void exitVehicle(char *vehicleNumber)
{
    // first check is vehicle is already therein the space thenonly departed

    vehicleDB *currentVehicle = VehicleNewOrNot(vehicleNumber);
    if (currentVehicle == NULL || currentVehicle->spaceID == 0)
    {
        printf("Vehicle not found.\n");
        return;
    }

    // Get departure time
    departure_time(currentVehicle);

    // Calculate duration in seconds
    struct time arr = currentVehicle->arrivalTime;
    struct time dep = currentVehicle->departureTime;

    long long arr_sec = convert_to_sec(arr.year, arr.month, arr.day, arr.hr, arr.min, arr.sec);
    long long dep_sec = convert_to_sec(dep.year, dep.month, dep.day, dep.hr, dep.min, dep.sec);
    long long duration_sec = dep_sec - arr_sec;

    int duration_hours = (duration_sec + 3599) / 3600; // Round up to nearest hour

    // Update vehicle's total parking hours and amount paid
    currentVehicle->totalParkingHours += duration_hours;

    // Check and update membership
    if (currentVehicle->totalParkingHours >= 200)
    {
        strcpy(currentVehicle->membership, "Golden");
    }
    else if (currentVehicle->totalParkingHours >= 100)
    {
        strcpy(currentVehicle->membership, "Premium");
    }

    // Calculate cost
    int cost;
    if (duration_hours <= 3)
    {
        cost = duration_hours * 100;
    }
    else
    {
        cost = 3 * 100 + (duration_hours - 3) * 50;
    }

    // Apply 10% discount if membership is not None
    if (strcmp(currentVehicle->membership, "None") != 0)
    {
        cost = cost * 0.9;
    }

    currentVehicle->totalAmountPaid += cost;
    currentVehicle->parkingCount += 1;

    // Update parking space
    space *parkingSpace = getSpaceNode(currentVehicle->spaceID);
    if (parkingSpace != NULL)
    {
        parkingSpace->occupied = 0;    // Mark as available
        parkingSpace->revenue += cost; // Add the cost to revenue
    }
    else
    {
        printf("Error: Parking space not found.\n");
    }
    currentVehicle->spaceID = 0;
    printf("Vehicle exited successfully. Amount paid: %d\n", cost);
    // writeToFile(); // Persist changes to file
}

// Merge two sorted lists
vehicleDB *merge(vehicleDB *a, vehicleDB *b)
{
    vehicleDB *result = NULL;

    // Base cases
    if (a == NULL)
        result = b;
    else if (b == NULL)
        result = a;

    // Sort in descending order of parkingCount
    else if (a->parkingCount >= b->parkingCount)
    {
        result = a;
        result->next = merge(a->next, b);
    }
    else
    {
        result = b;
        result->next = merge(a, b->next);
    }
    return result;
}

// split_for_vehicleDB list into two halves
void split_for_vehicleDB(vehicleDB *source, vehicleDB **front, vehicleDB **back)
{
    vehicleDB *fast;
    vehicleDB *slow = source;

    // Handle empty or single-element list
    if (source == NULL || source->next == NULL)
    {
        *front = source;
        *back = NULL;
        return;
    }

    // Slow moves 1 step, fast moves 2 steps
    fast = source->next;
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // split_for_vehicleDB at midpoint
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

void copy_the_data_from_spaceDBto_tempHead(space **tempHead)
{
    if (parkingLot == NULL)
    {
        printf("The spaces are empty no data.\n");
    }
    else
    {
        // space* temp1=tempHead;
        space *temp2 = parkingLot;
        while (temp2)
        {

            appendToSpace(tempHead, temp2->id, temp2->occupied, temp2->totalOccupancy, temp2->revenue);
            temp2 = temp2->next;
        }
    }
}

// Merge sort implementation
vehicleDB *mergeSort(vehicleDB *head)
{
    vehicleDB *a;
    vehicleDB *b;
    vehicleDB *ans;
    // Base caseempty list or single element
    if (head == NULL || head->next == NULL)
    {
        ans = head;
    }
    else
    {
        split_for_vehicleDB(head, &a, &b);
        // Recursively sort sublists
        a = mergeSort(a);
        b = mergeSort(b);
        ans = merge(a, b);
    }
    return ans;
}

// Main sorting function
vehicleDB *sortvehiclesBy_TotalParking(vehicleDB *head)
{
    return mergeSort(head);
}

// Merge two sorted lists by totalAmountPaid
vehicleDB *mergeAmount(vehicleDB *a, vehicleDB *b)
{
    vehicleDB *result = NULL;

    // Base cases
    if (a == NULL)
        result = b;
    else if (b == NULL)
        result = a;
    else if (a->totalAmountPaid >= b->totalAmountPaid)
    {
        result = a;
        result->next = mergeAmount(a->next, b);
    }
    else
    {
        result = b;
        result->next = mergeAmount(a, b->next);
    }
    return result;
}

// Merge sort implementation for amounts
vehicleDB *mergeSortAmount(vehicleDB *head)
{
    vehicleDB *a;
    vehicleDB *b;
    vehicleDB *res;

    // Base case - empty list or single element
    if (head == NULL || head->next == NULL)
    {
        res = head;
    }
    else
    {
        // split_for_vehicleDB into sublists
        split_for_vehicleDB(head, &a, &b);

        // Recursively sort sublists
        a = mergeSortAmount(a);
        b = mergeSortAmount(b);

        // Merge sorted sublists
        res = mergeAmount(a, b);
    }
    return res;
}

// Main sorting function for amounts
vehicleDB *sortvehiclesBy_TotalAmount(vehicleDB *head)
{
    return mergeSortAmount(head);
}

void split_for_SpaceDB(space *source, space **front, space **back)
{
    space *fast;
    space *slow = source;

    // Handle empty or single-element list
    if (source == NULL || source->next == NULL)
    {
        *front = source;
        *back = NULL;
        return;
    }

    // Slow moves 1 step, fast moves 2 steps
    fast = source->next;
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // split_for_vehicleDB at midpoint
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

space *mergeSpaces_Revenue(space *a, space *b)
{
    space *result = NULL;
    if (a == NULL)
        result = b;
    else if (b == NULL)
        result = a;

    else if (a->revenue >= b->revenue)
    {
        result = a;
        result->next = mergeSpaces_Revenue(a->next, b);
    }
    else
    {
        result = b;
        result->next = mergeSpaces_Revenue(a, b->next);
    }
    return result;
}

space *mergeSortForSpaceRevenue(space *tempHead)
{
    space *a;
    space *b;
    space *ans;
    if (tempHead == NULL || tempHead->next == NULL)
    {
        ans = tempHead;
    }
    else
    {
        split_for_SpaceDB(tempHead, &a, &b);
        a = mergeSortForSpaceRevenue(a);
        b = mergeSortForSpaceRevenue(b);
        ans = mergeSpaces_Revenue(a, b);
    }

    return ans;
}

space *sortSpacesByRevenue(space *parkingLot)
{
    space *tempHead = NULL;
    copy_the_data_from_spaceDBto_tempHead(&tempHead);
    // now sort the LL as  tempHead as the head
    // it is sorted by revenue
    return mergeSortForSpaceRevenue(tempHead);
}

space *mergeSpaces_ParkingCount(space *a, space *b)
{
    space *result = NULL;
    if (a == NULL)
        result = b;
    else if (b == NULL)
        result = a;

    else if (a->totalOccupancy >= b->totalOccupancy)
    {
        result = a;
        result->next = mergeSpaces_ParkingCount(a->next, b);
    }
    else
    {
        result = b;
        result->next = mergeSpaces_ParkingCount(a, b->next);
    }
    return result;
}

space *mergeSortFor_ParkingCount(space *tempHead)
{
    space *a;
    space *b;
    space *ans;
    if (tempHead == NULL || tempHead->next == NULL)
    {
        ans = tempHead;
    }
    else
    {
        split_for_SpaceDB(tempHead, &a, &b);
        a = mergeSortFor_ParkingCount(a);
        b = mergeSortFor_ParkingCount(b);
        ans = mergeSpaces_ParkingCount(a, b);
    }
    return ans;
}

space *sortSpacesBy_ParkingCount(space *parkingLot)
{
    space *tempHead = NULL;
    copy_the_data_from_spaceDBto_tempHead(&tempHead);
    // now sort the LL as  tempHead as the head
    // it is sorted by revenue
    return mergeSortFor_ParkingCount(tempHead);
}

int main()
{

    space *tempHead;

    // space* SpaceHead =NULL;
    initializeparkingSpaces(&parkingLot);

    FILE *file1 = fopen("inputForSpaces.txt", "r");
    if (file1 == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }
    else
    {
      
        int id, occupied, totalOccupancy;
        int revenue;
        // Read data from the file and add to the linked list
        while (fscanf(file1, "%d %d %d %d", &id, &occupied, &totalOccupancy, &revenue) != EOF)
        {
            // go to that id and update
            gotoThatplaceAndUpdate(parkingLot, id, occupied, totalOccupancy, revenue);
            // appendToSpace(&SpaceHead, id, occupied, totalOccupancy, revenue);
        }

        fclose(file1);
        displaySpace_DB(parkingLot);
    }

    FILE *file2 = fopen("inputForVehicle.txt", "r");
    if (file2 == NULL)
    {
        printf("Error opening file\n");
    }
    else
    {
        char vehicleNumber[20], ownerName[100], membership[10];
        int totalParkingHours, spaceID, totalAmountPaid, parkingCount;

        // Allocate memory for the time structure
        struct time *t = (struct time *)malloc(sizeof(struct time));
        struct time *depa_time = (struct time *)malloc(sizeof(struct time));

        if (depa_time == NULL)
        {
            printf("failed to allocated\n");
            return 1;
        }

        if (t == NULL)
        {
            printf("Memory allocation failed!\n");
            return 1; // Exit the program if memory allocation fails
        }

        while (fscanf(file2, "%s %s %d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
                      vehicleNumber, ownerName, &totalParkingHours,
                      &spaceID, membership, &totalAmountPaid, &parkingCount,
                      &t->year, &t->month, &t->day, &t->hr, &t->min, &t->sec,
                      &depa_time->year, &depa_time->month, &depa_time->day, &depa_time->hr, &depa_time->min, &depa_time->sec) != EOF)
        {

            // Append data to vehicleDB or process it

            appendTovehicleDB(&vehicleDBDB, vehicleNumber, ownerName,
                              totalParkingHours, spaceID, membership,
                              totalAmountPaid, parkingCount, t, depa_time);
        }
        free(t);
        t = NULL;
        free(depa_time);
        depa_time = NULL;
        fclose(file2);
        displayvehicleDB(vehicleDBDB);
    }

    // Display the linked list
    // for taking the input
    int choice = 1;
    while (choice >= 1 && choice < 9)
    {
        char vehicleNumber[20];
        char ownerName[100];
        char membership[10];

        printf("\n\n--- SMART CAR PARKING SYSTEM ---\n\n");
        printf("1. Register/Update Vehicle\n");
        printf("2. Vehicle Want to EXIT\n");
        printf("3. Sort Vehicles by Total Parkings\n");
        printf("4. Sort Vehicles by Total Amount Paid\n");
        printf("5. Sort Parking Spaces by Occupancy\n");
        printf("6. Sort Parking Spaces by Revenue\n");
        printf("7. Display Vehicle Information\n");
        printf("8. Display Parking Space Information\n");
        printf("9. CLOSE\n");
        printf("Plz Enter your choice (1-9): ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter vehicle number: ");
            scanf("%s", vehicleNumber);
            printf("Enter owner name: ");
            scanf("%s", ownerName); // assume one word is inputed

            vehicleDB *prev_membership = VehicleNewOrNot(vehicleNumber);
            space *empty_Space_for_vehicle;
            if (prev_membership != NULL)
            {
                if (prev_membership->spaceID != 0)
                {
                    printf("\nVehicle is already in the parking.\n\n");
                }
                else
                {
                    empty_Space_for_vehicle = space_availableorNot(prev_membership->membership);
                }
            }
            else
            {
                empty_Space_for_vehicle = space_availableorNot("None");
            }

            if (prev_membership != NULL && empty_Space_for_vehicle != NULL && prev_membership->spaceID == 0)
            {
                strcpy(membership, prev_membership->membership);

                prev_membership->spaceID = empty_Space_for_vehicle->id;
                struct time *t = (struct time *)malloc(sizeof(struct time));
                take_arrival_time(t);
                prev_membership->arrivalTime.year = t->year;
                prev_membership->arrivalTime.month = t->month;
                prev_membership->arrivalTime.day = t->day;
                prev_membership->arrivalTime.hr = t->hr;
                prev_membership->arrivalTime.min = t->min;
                prev_membership->arrivalTime.sec = t->sec;
                struct time *depa_time = (struct time *)malloc(sizeof(struct time));
                initializedeparture_time(depa_time);
                prev_membership->departureTime.year = depa_time->year;
                prev_membership->departureTime.month = depa_time->month;
                prev_membership->departureTime.day = depa_time->day;
                prev_membership->departureTime.hr = depa_time->hr;
                prev_membership->departureTime.min = depa_time->min;
                prev_membership->departureTime.sec = depa_time->sec;
                updateSpaceDB(empty_Space_for_vehicle);
            }
            else if (empty_Space_for_vehicle != NULL && prev_membership == NULL)
            {
                // can be enetr
                strcpy(membership, "None");
                struct time *t = (struct time *)malloc(sizeof(struct time));
                take_arrival_time(t);
                struct time *depa_time = (struct time *)malloc(sizeof(struct time));
                initializedeparture_time(depa_time);

                appendTovehicleDB(&vehicleDBDB, vehicleNumber, ownerName, 0, empty_Space_for_vehicle->id, membership, 0, 0, t, depa_time); // new vehicle

                updateSpaceDB(empty_Space_for_vehicle);

            }
            else if (empty_Space_for_vehicle == NULL)
            {
                printf("Cant be parked\n");
            }

            displaySpace_DB(parkingLot);
            displayvehicleDB(vehicleDBDB);

            break;

        case 2:
            printf("Enter vehicle number: ");
            scanf("%s", vehicleNumber);
            exitVehicle(vehicleNumber);
            displayvehicleDB(vehicleDBDB);
            displaySpace_DB(parkingLot);
            break;
        case 3:
            vehicleDBDB = sortvehiclesBy_TotalParking(vehicleDBDB);
            displayvehicleDB(vehicleDBDB);
            break;
        case 4:
            vehicleDBDB = sortvehiclesBy_TotalAmount(vehicleDBDB);
            printf("Vehicles sorted by total amount paid!\n");
            displayvehicleDB(vehicleDBDB);
            break;
        case 5:
            tempHead = sortSpacesBy_ParkingCount(parkingLot);
            printf("spaces sorted by the parking count.\n ");
            displaySpace_DB(tempHead);
            break;
        case 6:
            tempHead = sortSpacesByRevenue(parkingLot);
            printf("space Sorted by the Revenue.\n");
            displaySpace_DB(tempHead);
            break;
        case 7:
            displayvehicleDB(vehicleDBDB);
            break;

        case 8:
            displaySpace_DB(parkingLot);
            break;

        case 9:
            printf("\n\nExisting System ......\n\nThank You\n\n");
            writeToFile();
            break;
        default:
            printf("defult\n");
        }
    }

    writeToFile();
    return 0;
}
