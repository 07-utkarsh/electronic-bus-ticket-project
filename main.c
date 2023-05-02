#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUSES 10
#define MAX_SEATS 40

// Structure to hold seat information
typedef struct
{
    int number;
    int is_booked;
} Seat;

// Structure to hold bus information
typedef struct
{
    int bus_id;
    char origin[20];
    char destination[20];
    int fare;
    int num_seats;
    Seat seats[MAX_SEATS];
} Bus;

// Global array to hold buses
Bus buses[MAX_BUSES];
int num_buses = 0;

// Function to display available buses for a route
void displayBuses(char *origin, char *destination)            //Working perfectly:)
{
    printf("Available buses for %s to %s route:\n", origin, destination);
    int count = 0;
    FILE *fp = fopen("KheraTest.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    Bus bus;
    while (fread(&bus, sizeof(Bus), 1, fp) == 1)
    {
        if (strcmp(bus.origin, origin) == 0 && strcmp(bus.destination, destination) == 0)
        {
            printf("%d. Bus ID: %d, Fare: %d\n", count + 1, bus.bus_id, bus.fare);
            count++;
        }
    }
    fclose(fp);

    if (count == 0)
    {
        printf("No buses found for %s to %s route.\n", origin, destination);
    }
}

// Function to add a new bus
void addBus()                   //Working perfectly:)
{
    if (num_buses == MAX_BUSES)
    {
        printf("Maximum number of buses reached.\n");
        return;
    }
    Bus new_bus;
    printf("Enter bus ID: ");
    scanf("%d", &new_bus.bus_id);
    printf("Enter origin: ");
    scanf("%s", new_bus.origin);
    printf("Enter destination: ");
    scanf("%s", new_bus.destination);
    printf("Enter fare: ");
    scanf("%d", &new_bus.fare);
    printf("Enter number of seats: ");
    scanf("%d", &new_bus.num_seats);
    for (int i = 0; i < new_bus.num_seats; i++)
    {
        new_bus.seats[i].number = i + 1;
        new_bus.seats[i].is_booked = 0;
    }
    buses[num_buses] = new_bus;
    num_buses++;
    // printf("Bus added successfully.\n");

    FILE *fp = fopen("KheraTest.txt", "ab");
    if (fp == NULL)
    {
        printf("Error: could not open file\n");
        return;
    }
    fwrite(buses, sizeof(Bus), MAX_BUSES, fp);
    printf("Bus Record inserted successfully\n");
    fclose(fp);
}

// Function to book a seat
void bookSeat(int bus_id, int seat_number)       //Working perfectly:)
{
    FILE *fp = fopen("KheraTest.txt", "rb+");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    Bus bus;
    while (fread(&bus, sizeof(Bus), 1, fp) == 1)
    {
        if (bus.bus_id == bus_id)
        {
            if (seat_number < 1 || seat_number > bus.num_seats)
            {
                printf("Invalid seat number.\n");
                fclose(fp);
                return;
            }
            if (bus.seats[seat_number - 1].is_booked)
            {
                printf("Seat already booked.\n");
                fclose(fp);
                return;
            }
            bus.seats[seat_number - 1].is_booked = 1;
            fseek(fp, -sizeof(Bus), SEEK_CUR);
            fwrite(&bus, sizeof(Bus), 1, fp);
            printf("Seat %d booked successfully.\n", seat_number);
            fclose(fp);
            return;
        }
    }
    printf("Bus not found.\n");
    fclose(fp);
}

// Function to cancel a seat
void cancelSeat(int bus_id, int seat_number) {  //Working perfectly:)
    FILE* fp = fopen("KheraTest.txt", "rb+");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Bus bus;
    while (fread(&bus, sizeof(Bus), 1, fp) == 1) {
        if (bus.bus_id == bus_id) {
            if (seat_number < 1 || seat_number > bus.num_seats) {
                printf("Invalid seat number.\n");
                fclose(fp);
                return;
            }
            if (!bus.seats[seat_number - 1].is_booked) {
                printf("Seat not booked.\n");
                fclose(fp);
                return;
            }
            bus.seats[seat_number - 1].is_booked = 0;
            fseek(fp, -sizeof(Bus), SEEK_CUR);
            fwrite(&bus, sizeof(Bus), 1, fp);
            printf("Seat %d cancelled successfully.\n", seat_number);
            fclose(fp);
            return;
        }
    }
    printf("Bus not found.\n");
    fclose(fp);
}


// Function to delete a bus
void deleteBus(int bus_id) {
    FILE *fp = fopen("KheraTest.txt", "rb");
    if (fp == NULL) {
        printf("Error: could not open file\n");
        return;
    }
    
    fread(buses, sizeof(Bus), MAX_BUSES, fp);
    
    fclose(fp);
Bus bus;
    for (int i = 0; i < num_buses; i++) {
        if (buses[i].bus_id == bus_id) {
            // for (int j = i; j < num_buses - 1; j++) {
            //     buses[j] = buses[j + 1];
            // }
            // num_buses--;

            buses[i].bus_id = 0;
            strcpy(buses[i].origin, "");
            strcpy(buses[i].destination, "");
            buses[i].fare = 0;
            buses[i].num_seats = 0;
            printf("Bus with ID %d deleted successfully.\n", bus_id);
            

            fp = fopen("KheraTest.txt", "wb");
            if (fp == NULL) {
                printf("Error: could not open file\n");
                return;
            }
            fwrite(buses, sizeof(Bus), MAX_BUSES, fp);
            fclose(fp);
            
            return;
        }
    }
    printf("Bus not found.\n");
}



// Function to update a bus
// void updateBus(int bus_id) {


// for (int i = 0; i < num_buses; i++) {
// if (buses[i].bus_id == bus_id) {
// printf("Enter new origin: ");
// scanf("%s", buses[i].origin);
// printf("Enter new destination: ");
// scanf("%s", buses[i].destination);
// printf("Enter new fare: ");
// scanf("%d", &buses[i].fare);
// printf("Bus with ID %d updated successfully.\n", bus_id);
// return;
// }
// }
// printf("Bus not found.\n");
// }


// Main function
int main()
{
    int choice, bus_id, seat_number;
    char origin[20], destination[20];
    while (1)
    {
        printf("\n!!!!! Electronic Bus Ticket Generator !!!!!\n");
        printf("1. Display available buses for a route\n");
        printf("2. Add new bus\n");
        printf("3. Book seat\n");
        printf("4. Cancel seat\n");
        printf("5. Delete a particular bus\n");
        // printf("6. Update a particular bus\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter origin: ");
            scanf("%s", origin);
            printf("Enter destination: ");
            scanf("%s", destination);
            displayBuses(origin, destination);
            break;
        case 2:
            addBus();
            break;
        case 3:
            printf("Enter bus ID: ");
            scanf("%d", &bus_id);
            printf("Enter seat number: ");
            scanf("%d", &seat_number);
            bookSeat(bus_id, seat_number);
            break;
        case 4:
            printf("Enter bus ID: ");
            scanf("%d", &bus_id);
            printf("Enter seat number: ");
            scanf("%d", &seat_number);
            cancelSeat(bus_id, seat_number);
            break;
        case 5:
            printf("Enter bus ID: ");
            scanf("%d", &bus_id);
            deleteBus(bus_id);
            break;
        // case 6:
        //     printf("Enter bus ID: ");
        //     scanf("%d", &bus_id);
        //     updateBus(bus_id);
        //     break;
        case 6:
            exit(0);
        default:
            printf("Invalid choice.\n");
        }
    }

    return 0;
}
