#include <string>
#include <ctime>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
using namespace std;



class Vehicle {
private:
    string registrationNumber;
    time_t entryTime; 
    int vehicleWeight;
protected:
    static constexpr double ELECTRIC_FEE = 1.0;

    double CalculateParkingDuration() const {
        time_t now = time(nullptr); // Bieżący parkingDuration
        return difftime(now, entryTime) / 3600.0; // Różnica w godzinach
    }
    int getVehicleWeight() const { return vehicleWeight; }
public:
    Vehicle(string registrationNumber, time_t entryTime, int vehicleWeight)
        : registrationNumber(registrationNumber), entryTime(entryTime), vehicleWeight(vehicleWeight) {}
    string getRegistrationNumber() const { return registrationNumber; }
    time_t getEntryTime() const { return entryTime; }
    void SetEntryTime(time_t now) { entryTime = now; }
    virtual double CalculateParkingFee() const = 0;
    virtual ~Vehicle() = default;
};

class StandardCar : public Vehicle {
private:
    string fuelType;
public:
    double CalculateParkingFee() const override {
        double parkingDuration = CalculateParkingDuration();
        if (fuelType == "Electric") return ELECTRIC_FEE * parkingDuration; // 1 zł/godz. dla elektrycznych
        return getVehicleWeight() * 0.005 * parkingDuration; // 0.01 zł/kg/godz.
    }
    StandardCar(string registrationNumber, time_t entryTime, int vehicleWeight, string fuelType)
        : Vehicle(registrationNumber, entryTime, vehicleWeight), fuelType(fuelType) {}
};

class Motorcycle : public Vehicle {
public:
    double CalculateParkingFee() const override {
        return getVehicleWeight() * 0.02 * CalculateParkingDuration(); // 0.02 zł/kg/godz.
    }
    Motorcycle(string registrationNumber, time_t entryTime, int vehicleWeight)
        : Vehicle(registrationNumber, entryTime, vehicleWeight) {}
};

class DeliveryVan : public Vehicle {
public:
    enum class Type { light, heavy };
private:
    Type vehicleType;
    string fuelType;
public:
    double CalculateParkingFee() const override {
        double parkingDuration = CalculateParkingDuration();
        if (fuelType == "Electric") return ELECTRIC_FEE * parkingDuration; // 1 zł/godz. dla elektrycznych
        if (vehicleType == Type::light) return getVehicleWeight() * 0.01 * parkingDuration; 
        return getVehicleWeight() * 0.005 * parkingDuration; 
    }
    DeliveryVan(string registrationNumber, time_t entryTime, int vehicleWeight, Type vehicleType, string fuelType)
        : Vehicle(registrationNumber, entryTime, vehicleWeight), vehicleType(vehicleType), fuelType(fuelType) {}
};

    class Ticket {
private:
    Vehicle* vehicle;
    time_t issueTime;
    time_t exitTime;
    double fee;
public:
    Ticket(Vehicle* v, time_t issue, time_t exit)
        : vehicle(v), issueTime(issue), exitTime(exit), fee(0.0) {}

    void setExitTime(time_t exit) { exitTime = exit; }
    void setIssueTime(time_t issue) { issueTime = issue; }

    double CalculateFee() {
        if (vehicle == nullptr) return 0.0;
        double parkingDuration = difftime(exitTime, issueTime) / 3600.0;
        fee = vehicle->CalculateParkingFee() * parkingDuration;
        return fee;
    }
    void showTicket() {
        cout << "Vehicle: " << vehicle->getRegistrationNumber() << endl;
        cout << "Issue Time: " << ctime(&issueTime);
        cout << "Exit Time: " << ctime(&exitTime);
        cout << "Fee: " << fee << " zl" << endl;
    }
};


class Parking {
private:
    int capacity;
    vector<shared_ptr<Vehicle>> vehicles;
public:
    Parking(int capacity_) : capacity(capacity_) {} 

    void registerVehicle(shared_ptr<Vehicle> vehicle)
    {
        if (vehicles.size() < capacity) {
            vehicles.push_back(vehicle);
        } else {
            cout << "Parking is full!" << endl;
        }
    }
    Ticket unregisterVehicle(shared_ptr<Vehicle> vehicle) {
        auto it = find(vehicles.begin(), vehicles.end(), vehicle);
        if (it != vehicles.end()) {
            time_t exitTime = time(nullptr);
            Ticket ticket(vehicle.get(), vehicle->getEntryTime(), exitTime);
            ticket.CalculateFee();
            vehicles.erase(it);
            return ticket;
        } else {
            cout << "Vehicle not found!" << endl;
            return Ticket(nullptr, 0, 0); // Zwraca pusty bilet
        }
    }
    bool isFull(){
        return vehicles.size() >= capacity;
    }
    const vector<shared_ptr<Vehicle>>& ReturnVehicles() const {
        return vehicles;
    }
};
    







int main() {
    
    time_t now = time(nullptr) - 7200; // 2 godziny temu

    StandardCar car("KR12345", now, 1200, "Petrol");
    Motorcycle moto("KR54321", now, 200);
    DeliveryVan van("KR11111", now, 2500, DeliveryVan::Type::light, "Electric");
    DeliveryVan heavyVan("KR22222", now, 3500, DeliveryVan::Type::heavy, "Diesel");

    cout << "Car fee: " << car.CalculateParkingFee() << " zl" << endl;
    cout << "Moto fee: " << moto.CalculateParkingFee() << " zl" << endl;
    cout << "Van fee: " << van.CalculateParkingFee() << " zl" << endl;
    cout << "Heavy Van fee: " << heavyVan.CalculateParkingFee() << " zl" << endl;


    // Parking o pojemności 3 pojazdów
    Parking parking(3);
    parking.registerVehicle(make_shared<StandardCar>("KR12345", time(nullptr) - 3600, 1200, "Petrol"));
    parking.registerVehicle(make_shared<Motorcycle>("KR54321", time(nullptr) - 1800, 200));
    parking.registerVehicle(make_shared<DeliveryVan>("KR11111", time(nullptr) - 5400, 2500, DeliveryVan::Type::light, "Electric"));

    // Wyświetl zaparkowane pojazdy
    cout << "\nLista zaparkowanych pojazdów:" << endl;
    for (const auto& pojazd : parking.ReturnVehicles()) {
        cout << pojazd->getRegistrationNumber() << endl;
    }

    // Dodanie pojazdu, który przekroczy pojemność parkingu
    parking.registerVehicle(make_shared<StandardCar>("KR22222", time(nullptr), 1300, "Diesel"));

    // Wyrejestruj pojazd i pokaż bilet
    auto pojazdDoWyjazdu = parking.ReturnVehicles().front();
    Ticket ticket = parking.unregisterVehicle(pojazdDoWyjazdu);
    cout << "\nBilet po wyjeździe pojazdu:" << endl;
    ticket.showTicket();

    // Wyświetl listę po wyjeździe
    cout << "\nLista po wyjeździe:" << endl;
    for (const auto& pojazd : parking.ReturnVehicles()) {
        cout << pojazd->getRegistrationNumber() << endl;
    }

    return 0;
}
