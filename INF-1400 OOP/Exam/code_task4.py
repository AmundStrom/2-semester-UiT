# -*- coding: utf-8 -*-

import random
import string


NUMBER_OF_SPACES = 100
NUMBER_OF_ADD_AND_REMOVE_ITERATIONS = 50

class make_vehicle():
    '''
    Generates a vehicle
    '''
    def __init__(self):
        self.registrationNumber = self.make_registrationNumber()
        self.year = random.randint(1950,2022)
    
    def make_registrationNumber():
        '''
        Generates a random registration number in format XXYYYYY, 
        where X is a letter and Y is a digit
        '''
        registrationNumber=""
        registrationNumber+="".join(random.choice(string.ascii_uppercase) for _ in range(2))
        registrationNumber+="".join(random.choice(string.digits) for _ in range(5))
            
        return registrationNumber

# New class
class Passenger_fosil_car(make_vehicle):
    """ Sub-class of make_vehicle """
    def __init__(self):
        super().__init__()
        self.type = "Passenger fosil car"
        self.capacity = "liters" + str(random.randint(45,65)) # new attribute
        self.weight = random.randint(1100,2400)
        self.seats = random.randint(2,7)

# New class
class Passenger_electric_car(make_vehicle):
    """ Sub-class of make_vehicle """
    def __init__(self):
        super().__init__()
        self.type = "Passenger electric car"
        self.capacity = "kWh" + str(random.randint(40,70)) # new attribute
        self.weight = random.randint(1100,2400)
        self.seats = random.randint(2,7)

class Van(make_vehicle):
    """ Sub-class of make_vehicle """
    def __init__(self):
        super().__init__()
        self.type = "Van"
        self.weight = random.randint(1500,3500)
        self.load = random.randint(700,1800)

class Garage_operations():
    """ Holds every operations that can be done on the garage """
    def __init__(self):
        pass

    def locate_vehicle(self, reg_number):
        '''
        Find the vehicle in a garage, based on registration number.    
        returns position where the vehicle is located, if not found return value -1
        '''
        for position,vehicle in enumerate(self.garage):
            print(position,vehicle)
            if vehicle != None and vehicle.registrationNumber == reg_number:
                return position
        return -1

    def is_space_avaible(self, position):
        """ Return True if position is empty, otherwise False """
        if self.garage[position] == None:
            return True
        else:
            return False

    def avaible_spaces(self):
        '''
        Find which spaces that are avaible and return the list with the numbers
        '''
        free_spaces = []
        for position,vehicle in enumerate(self.garage):
            if vehicle == None:
                free_spaces.append(position)
        return free_spaces

    def remove_vehicle(self, reg_number):
        '''
        When a vehicle is been sold, 
        take it out of the garage and set the space avaible
        '''
        for vehicle in self.garage:
            if vehicle != None and vehicle.registrationNumber == reg_number:
                self.garage.remove(vehicle)
                return True
        return False
        

    def add_vehicle(self, vehicle):
        ''' 
        Adds a vehicle to the garage, if there are free spaces
        '''
        free_spaces = self.avaible_spaces()    
        if len(free_spaces)>0:
            self.garage.insert(random.choice(free_spaces),vehicle)
            return True
        return False

    def print_garage(self):
        ''' 
        Prints what is placed on each place in the garage
        '''
        for position, vehicle in enumerate(self.garage):
            if vehicle == None:
                print(f"At Position: {position} vehicle: None")
            else:
                print(f"At Position: {position} vehicle: {vehicle.registrationNumber},{vehicle.type}")


class Simulate_Garage(Garage_operations):
    """ Will populate the garage with random amount of cars, then add and remove randomly selected cars """
    def __init__(self):
        super().__init__()
        self.garage = []

        self.populate_garage()
        self.print_garage()

        self.add_and_remove()
        self.print_garage()

    def populate_garage(self):
        #Populate the garage with vehicle or None if the place have no vehicle
        for _ in range(NUMBER_OF_SPACES):
            if random.random() > 0.2:
                # Passenger cars are 4 times more common than vans
                self.garage.append(random.choices([Passenger_fosil_car, Passenger_electric_car, Van], [2,2,1])[0])
            else:
                self.garage.append(None)

    def add_and_remove(self):
        #adds new and remove vehicles randomly
        for _ in range(NUMBER_OF_ADD_AND_REMOVE_ITERATIONS): 
            if random.random()>0.5:
                for position in range(NUMBER_OF_SPACES):
                    if self.is_space_avaible(position) == False:
                        vehicle = self.garage[position]
                        if self.remove_vehicle(vehicle.registrationNumber):
                            print(f"Vehicle: {vehicle.registrationNumber} have been removed from the garage")
                        else:
                            print("This shouldnt happen, but vehicle are not removed...")
                        break
            else:
                # Passenger cars are 4 times more common than vans
                if self.add_vehicle(random.choices([Passenger_fosil_car,Passenger_electric_car, Van], [2,2,1])[0]):
                    print("Vehicle been placed in the garage")
                else:
                    print("No spaces left...")
           

if __name__ == "__main__":
    # Simulates the garage being populated, and vehicles being sold and new vehicles added.
    Simulate_Garage()