#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
using namespace std;


int x_coordinate = 1000;
int y_coordinate = 1000;
double close_enough=0.05;
int sim_hours = 24;
int number_of_people = 25;
enum disease_status {VULNERABLE, INFECTED, DEAD};
double infection_time =(24*15);
double infection_probability = 0.5;
double fatality_rate = 0.01;
double initial_number_of_people_infected=5.0;
double social_distancing_probability=0.8;

//function used to randomize variables
bool try_event(double probability)
{
    const int resolution = 10000;
    double randomNumber = rand() % resolution ;
    randomNumber=(randomNumber/(double)resolution);
    return (randomNumber<=probability);
}



//declaring classes
class Employee; //forward declaration

class Movement
{
protected:
    Employee *employee;
public:
    Movement()
    {
        employee = NULL;
    }

    void setEmployee(Employee *p)
    {
        employee=p;
    }
    //Movement();
    //void setEmployee(Employee *p);
    virtual void moving() {};
};

class Position
{
//defining data members
private:
    double x;
    double y;
public:
//creating two constructors
//setting the default constructor
    Position()
    {
        //randomly selecting a value starting from X LIMIT
        x = rand() % x_coordinate;
        //randomly selecting a value starting from X LIMIT
        y = rand() % y_coordinate;
    }

//setting the parameterized constructor
    Position(double nx, double ny)
    {
        x=nx;
        y=ny;
    }

//creating the setter and getter functions for the Position class
    double getX()
    {
        return x;
    }
    double getY()
    {
        return y;
    }

    void setX(double nx)
    {
        x=nx;
    }

    void setY(double ny)
    {
        y=ny;
    }

//creating member functions to predict the Position of a Employee
    double getDistance(Position other_Position)
    {
        double dx=other_Position.x-x;
        double dy=other_Position.y-y;
        return sqrt((dx*dx)+(dy*dy));
    }

//creating a function to track the movement towards another Position
    bool move_toward(Position destination, double howfar)
    {
        double dx=destination.x-x;
        double dy=destination.y-y;
        double theta= atan2(dy,dx);

        double distance=sqrt((dx*dx)+(dy*dy));

        if(distance<howfar)          //function to make sure that the simulator stops at the desired Position
        {
            x=destination.x;
            y=destination.y;
            return true;
        }

        else
        {
            x=x+(howfar*cos(theta));
            y=y+(howfar*sin(theta));
            return false;
        }
    }



    bool at_Position(Position destination)   //checking if the two Positions are practically the same or not
    {
        return (getDistance(destination)<close_enough);
    }
};

class Employee
{
public:
    Employee ();
    bool try_infect(Employee other_Employee);
    void progress_disease();
    bool is_alive();

    Position Position;
    Movement *mobility_model;
    Movement *mobility_model1;
    Movement *mobility_model2;
    disease_status status;
    int disease_counter;
};

//the first position is that of the cafeteria, the next is that of the washroom and the next is that of the meeting room
static Position popularPlaces[3];

//this class inherits the Mobility Model class -- type of single inheritance
class Cafetaria : Movement
{
private:
    Position *waypoint;
    Position *work_station;
    int speed;
    int stay;
    double work_stay_probability;

public:
    Cafetaria()
    {
        speed = -1;
        work_station = new Position();
        if (try_event(social_distancing_probability))
        {
            work_stay_probability= 0.7;
        }
        else
        {
            work_stay_probability = 0.2;
        }
    }

    void pick_new_waypoint()
    {
        speed = 1 + (rand()%5);
        stay = 1;
        if (try_event(work_stay_probability))
        {
            waypoint = work_station;
        }
        else
        {
            waypoint = &(popularPlaces[0]);
        }
    }

    void moving()
    {
        if (speed<0)
        {
            employee->Position.setX(work_station->getX());
            employee->Position.setY(work_station->getY());
            pick_new_waypoint();
        }
        else if (employee->Position.at_Position(*waypoint))
        {
            stay--;
            if (stay <=0)
            {
                pick_new_waypoint();
            }
        }
        else
        {
            employee->Position.move_toward(*waypoint, speed);
        }
    }
};

class Washroom : public Movement
{
private:
    Position *waypoint;
    Position *work_station;
    int speed;
    double stay;
    double work_stay_probability;

public:
    Washroom()
    {
        speed = -1;
        work_station = new Position();
        if (try_event(social_distancing_probability))
        {
            work_stay_probability= 0.5;
        }
        else
        {
            work_stay_probability = 0.2;
        }
    }

    void pick_new_waypoint()
    {
        speed = 1 + (rand()%5);
        stay = 0.33;
        if (try_event(work_stay_probability))
        {
            waypoint = work_station;
        }
        else
        {
            waypoint = &(popularPlaces[1]);
        }
    }

    void moving()
    {
        if (speed<0)
        {
            employee->Position.setX(work_station->getX());
            employee->Position.setY(work_station->getY());
            pick_new_waypoint();
        }
        else if (employee->Position.at_Position(*waypoint))
        {
            stay--;
            if (stay <=0)
            {
                pick_new_waypoint();
            }
        }
        else
        {
            employee->Position.move_toward(*waypoint, speed);
        }
    }
};


class MeetingRoom: public Movement
{
private:
    Position *waypoint;
    Position *work_station;
    int speed;
    int stay;
    double work_stay_probability;

public:
    MeetingRoom()
    {
        speed = -1;
        work_station = new Position();
        if (try_event(social_distancing_probability))
        {
            work_stay_probability= 0.8;
        }
        else
        {
            work_stay_probability = 0.2;
        }
    }

    void pick_new_waypoint()
    {
        speed = 1 + (rand()%5);
        stay = 3;
        if (try_event(work_stay_probability))
        {
            waypoint = work_station;
        }
        else
        {
            waypoint = &(popularPlaces[2]);
        }
    }

    void moving()
    {
        if (speed<0)
        {
            employee->Position.setX(work_station->getX());
            employee->Position.setY(work_station->getY());
            pick_new_waypoint();
        }
        else if (employee->Position.at_Position(*waypoint))
        {
            stay--;
            if (stay <=0)
            {
                pick_new_waypoint();
            }
        }
        else
        {
            employee->Position.move_toward(*waypoint, speed);
        }
    }
};


Employee::Employee()
{
    Cafetaria* model = new Cafetaria();
    Washroom* washroom = new Washroom();
    MeetingRoom* meeting = new MeetingRoom();
    mobility_model= (Movement*) model;
    mobility_model->setEmployee(this);
    mobility_model1 = (Movement*) washroom;
    mobility_model1->setEmployee(this);
    mobility_model2 = (Movement*) meeting;
    mobility_model2->setEmployee(this);
    status=VULNERABLE;
    disease_counter = infection_time;
}

bool Employee::try_infect(Employee other_Employee)
{
    if (other_Employee.status != INFECTED)
    {
        return false;
    }
    if (Position.getDistance(other_Employee.Position)>0.75)
    {
        return false;
    }
    if (try_event(infection_probability))
    {
        if (status==VULNERABLE)
        {
            status=INFECTED;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

void Employee::progress_disease()
{
    if (status==INFECTED)
    {
        disease_counter--;
        if (disease_counter<=0)
        {
            if (try_event(fatality_rate))
            {
                status=DEAD;
            }
        }
    }
}

bool Employee::is_alive()
{
    return (status != DEAD);
}
//end of definition of the functions for Employee class



int main()
{
    cout<<endl;
    cout<<endl;
    for (int i=0; i<30; i++)
    {
        cout<<"--";
    }
    cout<<" Welcome ";
    for (int i=0; i<30; i++)
    {
        cout<<"--";
    }
    cout<<endl;
    cout<<endl;
    cout<<endl;
    string user_name;
    string admin_user_name = "Admin";
    string password;
    string admin_password = "AdminLogin";
    string employee_user_name = "Employee";
    string employee_password = "EmployeeLogin";
    cout<<"Enter user name: ";
    cin>>user_name;
    cout<<"Enter Password: ";
    cin>>password;

    const char separator    = ' ';
    const int nameWidth     = 40;
    const int numWidth      = 8;

    int number_of_days;
    int number_of_hours_per_day;
    double distancing_propability;
    char name_of_state;
    int gap_between_simulation_hour;
    char run_again;
    int simulation_number = 0;

    if (password == admin_password && user_name == admin_user_name)
    {
        cout<<"Valid Credentials!"<<endl;
        cout<<endl;
        cout<<endl;
        for (int i=0; i<30; i++)
        {
            cout<<"--";
        }
        cout<<" Office COVID 19 Simulator ";
        for (int i=0; i<30; i++)
        {
            cout<<"--";
        }
        cout<<endl;
        cout<<endl;
        cout<<endl;

        do
        {
            //collecting relevant information from the user
            cout<<"Enter the dimensions of the space (x y) : ";
            cin>>x_coordinate>>y_coordinate;
            cout<<"\nEnter the number of people you want this simulation to run on: ";
            cin>>number_of_people;
            cout<<"\nEnter the number of days you want to run this simulation (1-100): ";
            cin>>number_of_days;
            if (number_of_days > 100 || number_of_days <1)
            {
                cout<<"Invalid."<<endl;
                return 0;
            }
            cout<<"\nEnter the number of hours per day you want to run this simulation (1-24): ";
            cin>>number_of_hours_per_day;
            cout<<"\nEnter the after how many hours do want to display the record: ";
            cin>>gap_between_simulation_hour;
            if (number_of_hours_per_day > 24 || number_of_hours_per_day <1)
            {
                cout<<"Invalid."<<endl;
                return 0;
            }
            sim_hours = (number_of_days)*(number_of_hours_per_day);
            cout<<"\nEnter the probability that people social distance in said space (0-1): ";
            cin>>distancing_propability;
            if (distancing_propability > 1 || distancing_propability <0)
            {
                cout<<"Invalid."<<endl;
                return 0;
            }
            social_distancing_probability = distancing_propability;
            cout<<"\nEnter the number of people who initially contact the COVID virus: ";
            cin>>initial_number_of_people_infected;
            cout<<"\nEnter the your state (M for Maharashtra | D for Delhi | K for Kerela | O for Other): ";
            cin>>name_of_state;
            if (name_of_state == 'M')
            {
                fatality_rate = 0.02;
                infection_probability = 0.053;

            }
            else if (name_of_state == 'D')
            {
                fatality_rate = 0.017;
                infection_probability = 0.072;

            }
            else if (name_of_state == 'K')
            {
                fatality_rate = 0.06;
                infection_probability = 0.014;

            }
            else
            {
                fatality_rate = 0.0013;
                fatality_rate = 0.024;
            }

            cout<<endl;
            cout<<endl;
            cout<<endl;

            //setting the simulator
            Employee people[number_of_people];
            int infection_history[sim_hours];
            srand(time(0));

            //making sure that the history of people is zero
            for (int i=0;  i<sim_hours; i++)
            {
                infection_history[i]=0;
            }

            //infecting the first five people
            for (int i=0; i<initial_number_of_people_infected; i++)
            {
                people[i].status=INFECTED;
            }

            int max_infected_at_once = 0;

            cout<<"\nSimulation Number : "<<simulation_number+1<<endl;
            cout<<endl;
            cout<<endl;
            cout << left << setw(nameWidth) << setfill(separator) << "Simulation Hour";
            cout << left << setw(nameWidth) << setfill(separator) << "Number of Vulnerable people";
            cout << left << setw(nameWidth) << setfill(separator) << "Number of Infected people";
            cout << left << setw(nameWidth) << setfill(separator) << "Number of Dead people";
            cout<<endl;

//to move people
            for(int i=0; i<sim_hours; i++)
            {
                for (int p=0; p<number_of_people; p=p+3)
                {
                    if (people[p].is_alive())
                    {
                        people[p].mobility_model->moving();
                        people[p].progress_disease();
                    }

                }

                for (int p=1; p<number_of_people; p=p+3)
                {
                    if (people[p].is_alive())
                    {
                        people[p].mobility_model1->moving();
                        people[p].progress_disease();
                    }

                }

                for (int p=2; p<number_of_people; p=p+3)
                {
                    if (people[p].is_alive())
                    {
                        people[p].mobility_model2->moving();
                        people[p].progress_disease();
                    }

                }


//infection spread
                for (int p=0; p<number_of_people; p++)
                {
                    if (people[p].is_alive())
                    {
                        for (int p2=0; p2<number_of_people; p2++)
                        {
                            if (p != p2 && people[p2].is_alive())
                            {
                                people[p].try_infect(people[p2]);
                            }
                        }
                    }
                }

//report the status of the simulation
                double num_infected=0;
                double num_immune=0;
                double num_dead=0;
                double num_vulnerable=0;

                for (int p=0; p<number_of_people; p++)
                {
                    if (!people[p].is_alive())
                    {
                        num_dead++;
                    }

                    if (people[p].status==INFECTED)
                    {
                        num_infected++;
                    }
                    if(people[p].status ==VULNERABLE)
                    {
                        num_vulnerable++;

                    }
                }

                if (num_infected > max_infected_at_once)
                {
                    max_infected_at_once = num_infected;
                }

                if (i%gap_between_simulation_hour == 0 || num_infected == 0)
                {
                    cout << left << setw(nameWidth) << setfill(separator) << i;
                    cout << left << setw(nameWidth) << setfill(separator) << num_vulnerable;
                    cout << left << setw(nameWidth) << setfill(separator) << num_infected;
                    cout << left << setw(nameWidth) << setfill(separator) << num_dead;
                    cout<<endl;
                }

                infection_history[i]=num_infected;
                if (num_infected == 0)
                {
                    break;
                }
            }
            cout<<endl;
            cout<<endl;
            cout<<"Maximum number of people infected in the complete simulation = " <<max_infected_at_once<<endl;
            cout<<endl;
            cout<<endl;
            cout<<"Do you want to run this simulation again? (Y/N): ";
            cin>>run_again;
            simulation_number++;
            cout<<endl;
            cout<<endl;

        }
        while (run_again == 'Y' || run_again == 'y');


        for (int i=0; i<30; i++)
        {
            cout<<"--";
        }
        cout<<" End of simulation ";
        for (int i=0; i<30; i++)
        {
            cout<<"--";
        }
        cout<<endl;
        cout<<endl;
        cout<<endl;

        cout<<"Set office Capacity: "<<endl;
        cin>>number_of_people;
        cout<<"Set the days and hours for which you want to open the office : "<<endl;
        cin>>number_of_days>>number_of_hours_per_day;
        cout<<endl;
        cout<<endl;
        return 0;
    }

    if (password == employee_password && user_name == employee_user_name)
    {
        cout << left << setw(nameWidth) << setfill(separator) << "Maximum Capacity";
        cout << left << setw(nameWidth) << setfill(separator) << "Number of days";
        cout << left << setw(nameWidth) << setfill(separator) << "Number of hours per day";
        cout<<endl;
        cout << left << setw(numWidth) << setfill(separator) << number_of_people;
        cout << left << setw(numWidth) << setfill(separator) << number_of_days;
        cout << left << setw(numWidth) << setfill(separator) << number_of_hours_per_day;
        cout<<endl;
        for (int i=0; i<30; i++)
        {
            cout<<"--";
        }
        cout<<" Thank you for using the system ";
        for (int i=0; i<30; i++)
        {
            cout<<"--";
        }
        cout<<endl;
        cout<<endl;
        cout<<endl;
        return 0;
    }

    else {
        cout<<"Invalid credentials. Try again later."<<endl;
    }

    return 0;
}
