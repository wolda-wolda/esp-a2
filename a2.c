//---------------------------------------------------------------------------------------------------------------------
// a2.c
//
// This program simulates the operation of an arbitrary number
// of elevators in a hotel with an arbitrary number of floors
//
//---------------------------------------------------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const int MAX_ARRAY_LENGTH = 50;
const int EMPTY_ELEVATOR = -1;
const int UPWARDS = 1;
const int DOWNWARDS = -1;
const int MEMORY_ALLOC_ERROR = -1;
const int MAX_FLOOR = 10;
const int MIN_FLOOR = 3;
const int MIN_ELEVATOR_AMOUNT = 1;
const int MAX_ELEVATOR_AMOUNT = 5;
const int MIN_ELEVATOR_CAPACITY = 1;
const int MAX_ELEVATOR_CAPACITY = 9;
const int MIN_AMOUNT_OF_PEOPLE = 2;
const int MAX_AMOUNT_OF_PEOPLE = 20;
const int MAX_INT_DIGITS = 11;

typedef struct _Elevator_
{
    int current_floor_;
    int index_;
    int pax_;
    int *arr_;
    int direction_;
    int capacity_;
} Elevator;

Elevator *initializeStruct(int number_of_lifts, int number_of_floors, int elevator_capacity);

char *inputHotelName(void);

void removeTrailingNewLine(char *string);

int inputFloors(char *hotel_name);

int inputLifts(char *hotel_name);

int inputLiftCapacity(char *hotel_name);

int inputWaitingPeople(void);

int **inputDestinationFloors(int number_of_floors, int waiting_people);

int printOutOfMemory(void);

void freeMemory(int index, int **p_int);

int inputInitialState(void);

int checkIfStringsEqual(char *string1, char *string2);

int **create2DArray(int number_of_floors, int waiting_people);

int compareIntegers(const void *a, const void *b);

void printHotelIllustration(int **destination_floor, int waiting_people, int number_of_floors, int number_of_lifts,
                            char *hotel_name, Elevator *lift);

int countString(const char *hotelname);

void printElevatorList(Elevator *lift, int number_of_lifts);

void printHotelRoof(int width, char *hotel_name, int number_lifts);

void printHotelFloors(int number_of_floors, int number_of_lifts, Elevator *lift, int waiting_people,
                      int **destination_floor);

void printHotelGround(int number_of_lifts);

int inputStartSimulation(void);

int displayOptions(void);

void onOffBoardingPassengers(int *counter, int **destination_floors, Elevator *lift, int number_of_floors,
                             int number_of_lifts,
                             int waiting_people);

int findClosestPerson(int *destination_floors, int waiting_people, int current_floor, int number_of_floors,
                      int direction);

int checkIfSimulationFinished(int **destination_floors, int number_of_floors, int waiting_people, Elevator *lift,
                              int number_of_lifts);

void changeLiftDirection(Elevator *lift, int number_of_floors, int number_of_lifts, int simulation_counter);

void changeLiftPosition(Elevator *lift, int number_of_floors);

int checkOutOfBound(int *destination_floors, int waiting_people, int number_of_floors, int index);

int startElevator(void);

int checkForMemoryAllocError(void *pointer);

void freeAlLMemory(char *hotel_name, Elevator *lifts, int **destination_floors, int number_of_floors,
                   int number_of_lifts);

void printFinalState(int simulation_counter, int **destination_floors, int waiting_people, int number_of_floors,
                     int number_of_lifts, char *hotel_name, Elevator *lifts);


//---------------------------------------------------------------------------------------------------------------------
///
/// Main function start and end point of my program.
///
/// @param void
///
/// @return steps of simulation or -1(Memory Alloc Error)
//
int main(void)
{
  printf("____________________\n"
         " ____   ____   ____ \n"
         "|____  |____  |____|\n"
         "|____   ____| |     \n"
         "____________________\n"
         "\n"
         "Welcome to the\n"
         "Elevator Simulation Program!\n"
         "\n");
  return startElevator();
}


//---------------------------------------------------------------------------------------------------------------------
///
/// Calls all Input, print and simulation functions.
///
/// @param void
///
/// @return steps of simulation or -1(Memory Alloc Error)
//
int startElevator(void)
{
  char *hotel_name;
  int **destination_floors;
  int number_of_floors;
  int number_of_lifts;
  int capacity;
  int waiting_people;
  int confirmation;
  int display_option;
  int simulation_counter = 1;
  Elevator *lifts = NULL;

  hotel_name = inputHotelName();
  if (hotel_name == NULL) return MEMORY_ALLOC_ERROR;
  number_of_floors = inputFloors(hotel_name);
  number_of_lifts = inputLifts(hotel_name);
  capacity = inputLiftCapacity(hotel_name);
  waiting_people = inputWaitingPeople();
  destination_floors = inputDestinationFloors(number_of_floors, waiting_people);
  if (destination_floors == NULL) return MEMORY_ALLOC_ERROR;

  confirmation = inputInitialState();
  if (confirmation == -1) return MEMORY_ALLOC_ERROR;

  lifts = initializeStruct(number_of_lifts, number_of_floors, capacity);
  if (confirmation == 0 && lifts != NULL) {
    printHotelIllustration(destination_floors, waiting_people, number_of_floors,
                           number_of_lifts,hotel_name, lifts);
  }

  confirmation = inputStartSimulation();
  if (confirmation == 1) {
    freeAlLMemory(hotel_name, lifts, destination_floors, number_of_floors, number_of_lifts);
    return 0;
  }

  display_option = displayOptions();
  while (confirmation != -1)
  {
    changeLiftDirection(lifts, number_of_floors, number_of_lifts, simulation_counter);
    if (display_option == 0) printf("=== Simulation Step %d ===\n", simulation_counter);
    onOffBoardingPassengers(&simulation_counter, destination_floors, lifts, number_of_floors,
                            number_of_lifts, waiting_people);
    confirmation = checkIfSimulationFinished(destination_floors, number_of_floors, waiting_people, lifts,
                                             number_of_lifts);
    if (display_option == 0)
    {
      printHotelIllustration(destination_floors, waiting_people, number_of_floors,
                             number_of_lifts,hotel_name, lifts);
    }

    if (confirmation == -1) break;

    changeLiftPosition(lifts, number_of_lifts);
  }
  printFinalState(simulation_counter, destination_floors, waiting_people, number_of_floors, number_of_lifts,
                  hotel_name, lifts);

  freeAlLMemory(hotel_name, lifts, destination_floors, number_of_floors, number_of_lifts);
  return simulation_counter - 1;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Prints the final state of my hotel.
///
/// @param simulation_counter
/// @param destination_floors 2D-Array which contains floors and waiting people on each floor
/// @param waiting_people number of waiting people on each floor
/// @param number_of_floors
/// @param number_of_lifts
/// @param hotel_name char pointer points to first char in string
/// @param lifts Array that contains my Elevator structs which contains all important infos about my Elveator like
/// capacity, position, number of passengers, direction etc.
///
/// @return void
//
void printFinalState(int simulation_counter, int **destination_floors, int waiting_people, int number_of_floors,
                     int number_of_lifts, char *hotel_name, Elevator *lifts)
{
  printf("\n"
         "=================\n"
         "   FINAL STATE\n"
         "=================\n"
         "\n");
  printHotelIllustration(destination_floors, waiting_people, number_of_floors, number_of_lifts,
                         hotel_name, lifts);
  printf("Simulation done in %d steps!\n", simulation_counter - 1);
}


//---------------------------------------------------------------------------------------------------------------------
///
/// Frees all dynamically allocated memory.
///
/// @param destination_floors 2D-Array which contains floors and waiting people on each floor
/// @param number_of_floors
/// @param number_of_lifts
/// @param hotel_name char pointer points to first char in string
/// @param lifts Array that contains my Elevator structs which contains all important infos about my Elevator like
/// capacity, position, number of passengers, direction etc.
///
/// @return void
//
void freeAlLMemory(char *hotel_name, Elevator *lifts, int **destination_floors, int number_of_floors,
                   int number_of_lifts)
{
  free(hotel_name);
  for (int lift_counter = 0; lift_counter < number_of_lifts; ++lift_counter)
  {
    free(lifts[lift_counter].arr_);
  }
  for (int floor_counter = 0; floor_counter < number_of_floors; ++floor_counter)
  {
    free(destination_floors[floor_counter]);
  }
  free(destination_floors);
  free(lifts);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Changes the floor of the elevator.
///
/// @param number_of_floors
/// @param lifts Array that contains my Elevator structs which contains all important infos about my Elevator like
/// capacity, position, number of passengers, direction etc.
///
/// @return void
//
void changeLiftPosition(Elevator *lift, int number_of_floors)
{
  for (int floor_counter = 0; floor_counter < number_of_floors; floor_counter++)
  {
    lift[floor_counter].current_floor_ = lift[floor_counter].current_floor_ + lift[floor_counter].direction_;
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Changes the direction of the elevator when they reach the top or bottom.
///
/// @param number_of_floors
/// @param number_of_lifts
/// @param simulation_counter
/// @param lifts Array that contains my Elevator structs which contains all important infos about my Elevator like
/// capacity, position, number of passengers, direction etc.
///
/// @return void
//
void changeLiftDirection(Elevator *lift, int number_of_floors, int number_of_lifts, int simulation_counter)
{
  if (simulation_counter > 1)
  {
    for (int lift_counter = 0; lift_counter < number_of_lifts; lift_counter++)
    {
      if (lift[lift_counter].current_floor_ == 0 || lift[lift_counter].current_floor_ == number_of_floors - 1)
      {
        lift[lift_counter].direction_ = -lift[lift_counter].direction_;
      }
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Checks if my elevators are empty and if all waiting people reached their final destination.
///
/// @param destination_floors 2D-Array which contains number_of_floors and waiting waiting_people on each floor
/// @param number_of_floors
/// @param number_of_lifts
/// @param waiting_people number of people waiting on each floor
/// @param lifts Array that contains my Elevator structs which contains all important infos about my Elevator like
/// capacity, position, number of passengers, direction etc.
///
/// @return 0 or -1 for finished or not finished
//
int checkIfSimulationFinished(int **destination_floors, int number_of_floors, int waiting_people, Elevator *lift,
                              int number_of_lifts)
{
  for (int floor_counter = 0; floor_counter < number_of_floors; floor_counter++)
  {
    for (int people_counter = 0; people_counter < waiting_people; people_counter++)
    {
      for (int lift_counter = 0; lift_counter < number_of_lifts; ++lift_counter)
      {
        if (destination_floors[floor_counter][people_counter] != EMPTY_ELEVATOR || lift[lift_counter].pax_ != 0)
        {
          return 0;
        }
      }
    }
  }
  return EMPTY_ELEVATOR;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Calculate distance between waiting_people and destination floor. To determine who can bord the elevator first.
///
/// @param destination_floors 2D-Array which contains number_of_floors and waiting waiting_people on each floor
/// @param number_of_floors
/// @param current_floor floor on which the elevator is currently on
/// @param waiting_people number of waiting_people waiting on each floor
/// @param direction in which the elevator is heading
///
/// @return index of person with shortest distance
//
int findClosestPerson(int *destination_floors, int waiting_people, int current_floor,
                      int number_of_floors, int direction)
{
  int destination = EMPTY_ELEVATOR;
  int distance = number_of_floors + 1;
  int absoluteValue;

  for (int j = 0; j < waiting_people; j++)
  {
    if (destination_floors[j] != EMPTY_ELEVATOR)
    {
      if ((direction > 0 && destination_floors[j] >= current_floor) ||
          (direction < 0 && destination_floors[j] <= current_floor))
      {
        absoluteValue = abs(destination_floors[j] - current_floor);
        if (absoluteValue < distance)
        {
          distance = absoluteValue;
          destination = j;
        }
      }
    }
  }
  return destination;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// This function lets waiting_people off the elevator and boards new ones.
///
/// @param *counter pointer to simulation_counter to increase the counter with having to return it
/// @param destination_floors 2D-Array which contains number_of_floors and waiting waiting_people on each floor
/// @param number_of_floors
/// @param number_of_lifts
/// @param waiting_people number of waiting_people waiting on each floor
///
/// @return void
//
void onOffBoardingPassengers(int *counter, int **destination_floors, Elevator *lift, int number_of_floors,
                             int number_of_lifts, int waiting_people)
{
  *counter = *counter + 1;
  int distance;
  for (int lift_counter = 0; lift_counter < number_of_lifts; lift_counter++)
  {
    if (lift[lift_counter].pax_ > 0)
    {
      for (int capacity_counter = 0; capacity_counter < lift[lift_counter].capacity_; capacity_counter++)
      {
        if (lift[lift_counter].arr_[capacity_counter] == lift[lift_counter].current_floor_)
        {
          printf("A person reached their destination floor %d with elevator %d.\n",
                 lift[lift_counter].current_floor_, lift[lift_counter].index_);
          lift[lift_counter].arr_[capacity_counter] = EMPTY_ELEVATOR;
          lift[lift_counter].pax_--;
        }
      }
    }
    if (lift[lift_counter].capacity_ != lift[lift_counter].pax_)
    {
      for (int capacity_counter = 0; capacity_counter < lift[lift_counter].capacity_; capacity_counter++)
      {
        distance = findClosestPerson(destination_floors[lift[lift_counter].current_floor_],
                                     waiting_people, lift[lift_counter].current_floor_, number_of_floors,
                                     lift[lift_counter].direction_);
        if (distance != EMPTY_ELEVATOR)
        {
          for (int capacity_counter = 0; capacity_counter < lift[lift_counter].capacity_; capacity_counter++)
          {
            if (lift[lift_counter].arr_[capacity_counter] == EMPTY_ELEVATOR)
            {
              lift[lift_counter].arr_[capacity_counter] =
                      destination_floors[lift[lift_counter].current_floor_][distance];
              break;
            }
          }
          destination_floors[lift[lift_counter].current_floor_][distance] = EMPTY_ELEVATOR;
          lift[lift_counter].pax_++;
        }
        if (lift[lift_counter].capacity_ == lift[lift_counter].pax_)
        {
          break;
        }
      }
      qsort(destination_floors[lift[lift_counter].current_floor_], waiting_people, sizeof(int),
            compareIntegers);
      qsort(lift[lift_counter].arr_, lift[lift_counter].capacity_, sizeof(int),
            compareIntegers);
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// User input for display all steps or end step of simulation
///
/// @param void
///
/// @return 0(all steps) or 1(end result) depending on user input
//
int displayOptions(void)
{
  char user_input[MAX_ARRAY_LENGTH];
  while (1)
  {
    printf("Show all steps of the simulation? (\"all steps\"/\"end result\"):\n > ");
    if (fgets(user_input, MAX_ARRAY_LENGTH, stdin) == NULL)
    {
      return MEMORY_ALLOC_ERROR;
    }
    removeTrailingNewLine(user_input);
    if (checkIfStringsEqual(user_input, "all steps") == 0)
    {
      return 0;
    }
    else if (checkIfStringsEqual(user_input, "end result") == 0)
    {
      return 1;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// User input for start or skip simulation
///
/// @param void
///
/// @return 0(start) or 1(skip) depending on user input
//
int inputStartSimulation(void)
{
  char user_input[MAX_ARRAY_LENGTH];
  while (1)
  {
    printf("Start the simulation? (\"start\"/\"skip\"):\n > ");
    if (fgets(user_input, MAX_ARRAY_LENGTH, stdin) == NULL)
    {
      return MEMORY_ALLOC_ERROR;
    }
    removeTrailingNewLine(user_input);
    if (checkIfStringsEqual(user_input, "start") == 0)
    {
      return 0;
    }
    else if (checkIfStringsEqual(user_input, "skip") == 0)
    {
      return 1;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Prints out of memory error
///
/// @param void
///
/// @return -1
//
int printOutOfMemory(void)
{
  printf("Out of memory! Program terminated!\n");
  return MEMORY_ALLOC_ERROR;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// User input for hotelname and returns string
///
/// @param void
///
/// @return hotel_name
//
char *inputHotelName(void)
{
  char *hotel_name = (char *) malloc(MAX_ARRAY_LENGTH * sizeof(char));
  if (hotel_name == NULL)
  {
    printOutOfMemory();
    return NULL;
  }
  printf("Enter the name of the hotel:\n"
         " > ");
  if (fgets(hotel_name, MAX_ARRAY_LENGTH, stdin) == NULL)
  {
    printf("Error reading Input\n");
  }

  removeTrailingNewLine(hotel_name);
  char *p_hotel_name = hotel_name;
  while (*p_hotel_name)
  {
    *p_hotel_name = toupper((unsigned char) *p_hotel_name);
    p_hotel_name++;
  }
  return hotel_name;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Removes the trailing \n when using fgets for user input
///
/// @param *string
///
/// @return void
//
void removeTrailingNewLine(char *string)
{
  int string_index = 0;
  while (string[string_index] != '\0')
  {
    if (string[string_index] == '\n')
    {
      string[string_index] = '\0';
      break;
    }
    string_index++;
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// User input how many floors the hotel has
///
/// @param *hotel_name
///
/// @return user_input which is number of floors
//
int inputFloors(char *hotel_name)
{
  int ch;
  int user_input = 0;
  do
  {
    printf("Enter the number of floors in hotel %s:\n"
           " > ", hotel_name);
    scanf("%d", &user_input);
    while ((ch = getchar()) != '\n' && ch != EOF); //clear stdin buffer

    if (user_input < MIN_FLOOR || user_input > MAX_FLOOR)
    {
      printf("Wrong input, the number of floors must be between %d and %d!\n", MIN_FLOOR, MAX_FLOOR);
    }
  } while (user_input < MIN_FLOOR || user_input > MAX_FLOOR);
  return user_input;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// User input number of elevators.
///
/// @param *hotel_name
///
/// @return user_input which is the number of elevators
//
int inputLifts(char *hotel_name)
{
  int user_input = 0;
  int ch;

  do
  {
    printf("Enter the number of elevators in hotel %s:\n"
           " > ", hotel_name);
    scanf("%d", &user_input);
    while ((ch = getchar()) != '\n' && ch != EOF);

    if (user_input < MIN_ELEVATOR_AMOUNT || user_input > MAX_ELEVATOR_AMOUNT)
    {
      printf("Wrong input, the number of elevators must be between %d and %d!\n",
             MIN_ELEVATOR_AMOUNT, MAX_ELEVATOR_AMOUNT);
    }
  } while (user_input < MIN_ELEVATOR_AMOUNT || user_input > MAX_ELEVATOR_AMOUNT);
  return user_input;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// User input elevator capacity.
///
/// @param *hotel_name
///
/// @return user_input which is capacity of elevator
//
int inputLiftCapacity(char *hotel_name)
{
  int user_input = 0;
  int ch;

  do
  {
    printf("Enter the capacity of elevators in hotel %s:\n"
           " > ", hotel_name);
    scanf("%d", &user_input);
    while ((ch = getchar()) != '\n' && ch != EOF);
    if (user_input < MIN_ELEVATOR_CAPACITY || user_input > MAX_ELEVATOR_CAPACITY)
    {
      printf("Wrong input, the capacity of elevators must be between %d and %d person(s)!\n",
      MIN_ELEVATOR_CAPACITY, MAX_ELEVATOR_CAPACITY);
    }
  } while (user_input < MIN_ELEVATOR_CAPACITY || user_input > MAX_ELEVATOR_CAPACITY);
  return user_input;
}


//---------------------------------------------------------------------------------------------------------------------
///
/// User input waiting people on each floor.
///
/// @param void
///
/// @return user_input which is waiting people
//
int inputWaitingPeople(void)
{
  int ch;
  int user_input = 0;
  do
  {
    printf("Enter the number of people waiting on each floor:\n"
           " > ");
    scanf("%d", &user_input);
    while ((ch = getchar()) != '\n' && ch != EOF);

    if (user_input < MIN_AMOUNT_OF_PEOPLE || user_input > MAX_AMOUNT_OF_PEOPLE)
    {
      printf("Wrong input, the number of people waiting on each floor must be between %d and %d!\n",
             MIN_AMOUNT_OF_PEOPLE, MAX_AMOUNT_OF_PEOPLE);
    }
  } while (user_input < MIN_AMOUNT_OF_PEOPLE || user_input > MAX_AMOUNT_OF_PEOPLE);
  return user_input;
}


//---------------------------------------------------------------------------------------------------------------------
///
/// User input destination of people on each floor. Converts string to intengers and calls a function to
/// create a 2D-Array
///
/// @param number_of_floors
/// @param waiting_people
///
/// @return destination_floors which is a 2D-Array [number_of_floors][destination_of_waiting_people]
//
int **inputDestinationFloors(int number_of_floors, int waiting_people)
{
  int floor_index;
  int **destination_floors;

  destination_floors = create2DArray(number_of_floors, waiting_people);
  char *input_buffer = (char *) malloc(waiting_people * (sizeof(char) + MAX_INT_DIGITS));
  if (input_buffer == NULL || destination_floors == NULL)
  {
    return NULL;
  }

  for (int floor_counter = 0; floor_counter < number_of_floors; floor_counter++)
  {
    floor_index = 0;
    printf("Enter the destination floors of the people [floor: %d]:\n"
           " > ", floor_counter);
    if (fgets(input_buffer, waiting_people * (sizeof(char) + MAX_INT_DIGITS), stdin) == NULL)
    {
      printOutOfMemory();
      return NULL;
    }
    else
    {
      for (int string_counter = 0; input_buffer[string_counter] != '\0'; string_counter++)
      {
        if ((input_buffer[string_counter] >= '0' && input_buffer[string_counter] <= '9') ||
            (input_buffer[string_counter] == '-' && input_buffer[string_counter + 1] >= '0' &&
            input_buffer[string_counter + 1] <= '9'))
        {
          int number = 0;
          int sign = 1;

          if (input_buffer[string_counter] == '-')
          {
            sign = -1;
            string_counter++;
          }

          while (input_buffer[string_counter] >= '0' && input_buffer[string_counter] <= '9')
          {
            number = number * 10 + (input_buffer[string_counter] - '0');
            string_counter++;
          }

          number *= sign;

          destination_floors[floor_counter][floor_index] = number;
          floor_index++;
        }
        else if (input_buffer[string_counter] == '\n')
        {
          break;
        }
      }
      if (checkOutOfBound(destination_floors[floor_counter], waiting_people, number_of_floors, floor_counter) == 1)
      {
        floor_counter--;
      }
    }
  }

  free(input_buffer);

  for (int floor_counter = 0; floor_counter < number_of_floors; floor_counter++)
  {
    qsort(destination_floors[floor_counter], waiting_people, sizeof(int), compareIntegers);
  }

  return destination_floors;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Checks if user input is out of bound or on the same floor
///
/// @param destination_floors only waiting people on each floor not whole 2D-Array
/// @param waiting_people
/// @param number_of_floors
/// @param index
///
/// @return 0(not out of bound) 1(out of bound)
//
int checkOutOfBound(int *destination_floors, int waiting_people, int number_of_floors, int index)
{
  int error = 0;
  for (int j = 0; j < waiting_people; ++j)
  {
    if (destination_floors[j] >= 0 && destination_floors[j] < number_of_floors)
    {
      if (destination_floors[j] == index)
      {
        printf("Wrong input, the destination floor cannot be the current floor!\n");
        error = 1;
      }
    }
    else
    {
      printf("Wrong input, the destination floor %d is out of range (0 to %d)!\n", destination_floors[j],
             number_of_floors - 1);
      error = 1;
    }
  }
  if (error)
  {
    return 1;
  }
  return 0;
}


//---------------------------------------------------------------------------------------------------------------------
///
/// compares two integers and returns the difference
///
/// @param a
/// @param b
///
/// @return difference of the two numbers
//
//From: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
// begin
int compareIntegers(const void *a, const void *b)
{
  return (*(int *) a - *(int *) b);
}
//end

//---------------------------------------------------------------------------------------------------------------------
///
/// Creates a 2D-Array
///
/// @param number_of_floors
/// @param waiting_people
///
/// @return destination_floors
//
int **create2DArray(int number_of_floors, int waiting_people)
{
  int **destination_floors = (int **) malloc(number_of_floors * sizeof(int *));

  if (destination_floors == NULL)
  {
    printOutOfMemory();
    return NULL;
  }

  for (int floor_counter = 0; floor_counter < number_of_floors; floor_counter++)
  {
    destination_floors[floor_counter] = (int *) malloc(waiting_people * sizeof(int));
    if (destination_floors[floor_counter] == NULL)
    {
      freeMemory(floor_counter, destination_floors);
      return NULL;
    }
  }
  return destination_floors;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// frees memory in case a 2D-Array alloc fails
///
/// @param index
/// @param p_int 2D-Array
///
/// @return void
//
void freeMemory(int index, int **p_int)
{
  for (int j = 0; j < index; j++)
  {
    free(p_int[j]);
  }
  free(p_int);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// User input print initial state yes or no
///
/// @param void
///
/// @return 0(yes) 1(no)
//
int inputInitialState(void)
{
  int equal_yes;
  int equal_no;
  char *userinput = (char *) malloc(MAX_ARRAY_LENGTH * sizeof(char));
  if (userinput == NULL)
  {
    return printOutOfMemory();
  }

  do
  {
    printf("Show the initial state? (\"yes\"/\"no\"):\n"
           " > ");
    fgets(userinput, MAX_ARRAY_LENGTH, stdin);
    removeTrailingNewLine(userinput);
    equal_yes = checkIfStringsEqual(userinput, "yes");
    equal_no = checkIfStringsEqual(userinput, "no");
  } while (equal_yes == 1 && equal_no == 1);

  if (equal_yes == 0)
  {
    free(userinput);
    printf("\n"
           "=================\n"
           "  INITIAL STATE\n"
           "=================\n"
           "\n");
    return 0;
  }
  else
  {
    free(userinput);
    return 1;
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Compares strings if they are the same or not
///
/// @param string1
/// @param string2
///
/// @return 0(yes) 1(no)
//
int checkIfStringsEqual(char *string1, char *string2)
{
  while (*string1 != '\0' && *string2 != '\0')
  {
    if (tolower(*string1) != tolower(*string2))
    {
      return 1;
    }

    string1++;
    string2++;
  }

  if (*string1 == '\0' && *string2 == '\0')
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// main print function that calls all other print functions. calculates width of hotel
///
/// @param destination_floor 2D-Array
/// @param waiting_people
/// @param number_of_floors
/// @param number_of_lifts
/// @param hotel_name
/// @param lift Elevator struct
///
/// @return void
//
void printHotelIllustration(int **destination_floor, int waiting_people, int number_of_floors, int number_of_lifts,
                            char *hotel_name, Elevator *lift)
{
  int width = number_of_lifts * 5 + number_of_lifts - 2;

  printHotelRoof(width, hotel_name, number_of_lifts);
  printHotelFloors(number_of_floors, number_of_lifts, lift, waiting_people, destination_floor);
  printHotelGround(number_of_lifts);
  printElevatorList(lift, number_of_lifts);
}

//---------------------------------------------------------------------------------------------------------------------
///
/// Initializes my Elevator struct with relevant user input values
///
/// @param destination_floor 2D-Array
/// @param number_of_lifts
/// @param number_of_floors
///
/// @return lift elevator struct
//
Elevator *initializeStruct(int number_of_lifts, int number_of_floors, int elevator_capacity)
{
  Elevator *lift = NULL;
  lift = (Elevator *) malloc(number_of_lifts * sizeof(Elevator));
  if (lift == NULL)
  {
    return NULL;
  }
  else
  {
    for (int lift_counter = 0; lift_counter < number_of_lifts; lift_counter++)
    {
      if (lift_counter % 2 == 0)
      {
        lift[lift_counter].current_floor_ = 0;
        lift[lift_counter].index_ = lift_counter;
        lift[lift_counter].direction_ = UPWARDS;
      }
      else
      {
        lift[lift_counter].current_floor_ = number_of_floors - 1;
        lift[lift_counter].index_ = lift_counter;
        lift[lift_counter].direction_ = DOWNWARDS;
      }
      lift[lift_counter].pax_ = 0;
      lift[lift_counter].arr_ = (int *) malloc(elevator_capacity * sizeof(int));
      for (int j = 0; j < elevator_capacity; j++)
      {
        lift[lift_counter].arr_[j] = EMPTY_ELEVATOR;
      }
      lift[lift_counter].capacity_ = elevator_capacity;
    }
    return lift;
  }
}

int countString(const char *hotelname)
{
  int string_index;
  for (string_index = 0; string_index < MAX_ARRAY_LENGTH; string_index++)
  {
    if (hotelname[string_index] == '\0')
    {
      break;
    }
  }
  return string_index;
}

//---------------------------------------------------------------------------------------------------------------------
///
/// prints the roof of my hotel
///
/// @param number_of_lifts
/// @param hotel_name
/// @param width
///
/// @return void
//
void printHotelRoof(int width, char *hotel_name, int number_lifts)
{
  int len;
  int center;
  len = countString(hotel_name);
  center = (width / 2) - (len / 2);

  printf("++");
  for (int width_counter = 0; width_counter <= width; width_counter++)
  {
    printf("-");
  }

  printf("++\n++");
  for (int width_counter = 0; width_counter <= width; width_counter++)
  {
    if (width_counter == center)
    {
      printf("%s", hotel_name);
      width_counter += len - 1;
    }
    else
    {
      printf(" ");
    }
  }
  printf("++\n++");
  for (int lift_counter = 0; lift_counter < number_lifts; lift_counter++)
  {
    if (number_lifts - 1 == lift_counter)
    {
      printf("-----++");
    }
    else
    {
      printf("-----+");
    }
  }
  printf("\n");
}

//---------------------------------------------------------------------------------------------------------------------
///
/// prints the number_of_floors of my hotel.
///
/// @param destination_floor my 2D-Array
/// @param waiting_people
/// @param number_of_floors
/// @param number_of_lifts
/// @param hotel_name
/// @param lift Elevator struct
///
/// @return void
//
void printHotelFloors(int number_of_floors, int number_of_lifts, Elevator *lift, int waiting_people,
                      int **destination_floor)
{
  for (int floor_counter = number_of_floors - 1; floor_counter >= 0; floor_counter--)
  {
    printf("|");
    for (int lift_counter = 0; lift_counter < number_of_lifts; lift_counter++)
    {
      printf("|");
      if (lift[lift_counter].current_floor_ == floor_counter && lift[lift_counter].index_ == lift_counter)
      {
        printf(" [%d] ", lift[lift_counter].pax_);
      }
      else
      {
        printf("     ");
      }
    }
    printf("||");
    printf("  (");
    for (int people_counter = 0; people_counter < waiting_people; people_counter++)
    {
      if (destination_floor[floor_counter][people_counter] != EMPTY_ELEVATOR)
      {
        printf("%d", destination_floor[floor_counter][people_counter]);
      }
      else
      {
        printf("-");
      }
      if (people_counter != waiting_people - 1)
      {
        printf(",");
      }
    }
    printf(")\n");
    if (floor_counter > 0)
    {
      printf("||");
      for (int lift_counter = 0; lift_counter < number_of_lifts; lift_counter++)
      {
        if (lift_counter > 0 && lift_counter <= number_of_lifts - 1)
        {
          printf("+-   -");
        }
        else
        {
          printf("-   -");
        }
      }
      printf("||\n");
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------
///
/// print the bottom part of my hotel
///
/// @param number_of_lifts
///
/// @return void
//
void printHotelGround(int number_of_lifts)
{
  printf("++");
  for (int lift_counter = 0; lift_counter < number_of_lifts; lift_counter++)
  {
    if (number_of_lifts - 1 == lift_counter)
    {
      printf("-----++");
    }
    else
    {
      printf("-----+");
    }
  }
  printf("\n");
}

//---------------------------------------------------------------------------------------------------------------------
///
/// print the elevators and the people in it
///
/// @param number_of_lifts
/// @param lift Elevator struct
///
/// @return void
//
void printElevatorList(Elevator *lift, int number_of_lifts)
{
  int totalPax = 0;

  for (int lift_counter = 0; lift_counter < number_of_lifts; lift_counter++)
  {
    totalPax += lift[lift_counter].pax_;
  }

  if (totalPax > 0)
  {
    printf("Elevators:\n");

    for (int lift_counter = 0; lift_counter < number_of_lifts; lift_counter++)
    {
      printf("%d: (", lift_counter);
      for (int capacity_counter = 0; capacity_counter < lift->capacity_; capacity_counter++)
      {
        if (lift[lift_counter].arr_[capacity_counter] != EMPTY_ELEVATOR)
        {
          printf("%d", lift[lift_counter].arr_[capacity_counter]);
        }
        else
        {
          printf("-");
        }
        if (capacity_counter < lift->capacity_ - 1)
        {
          printf(",");
        }
      }
      printf(")\n");
    }
  }
  printf("\n");
}

