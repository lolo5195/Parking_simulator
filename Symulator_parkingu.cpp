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

    int getVehicleWeight() const {
        if(vehicleWeight <= 0) {
            throw invalid_argument("Vehicle weight must be greater than zero.");
        }
        return vehicleWeight;
    }
public:
    Vehicle(string registrationNumber, time_t entryTime, int vehicleWeight)
        : registrationNumber(registrationNumber), entryTime(entryTime), vehicleWeight(vehicleWeight) {}
    string getRegistrationNumber() const { 
        if (registrationNumber.empty()) {
            throw invalid_argument("Registration number cannot be empty.");
        }
        if (registrationNumber.length() > 10) {
            throw invalid_argument("Registration number cannot exceed 10 characters.");
        }
        if (registrationNumber.length() < 5) {
            throw invalid_argument("Registration number must be at least 5 characters long.");
        }
        if (registrationNumber.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") != string::npos) {
            throw invalid_argument("Registration number can only contain alphanumeric characters.");
        }
        if (registrationNumber.find(" ") != string::npos) {
            throw invalid_argument("Registration number cannot contain spaces.");
        }
        if (registrationNumber.find_first_of("0123456789") == string::npos) {
            throw invalid_argument("Registration number must contain at least one digit.");
        }
        if (registrationNumber.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos) {
            throw invalid_argument("Registration number must contain at least one letter.");
        }
        return registrationNumber; 
    }
    time_t getEntryTime() const { return entryTime; }
    void SetEntryTime(time_t now) { entryTime = now; }
    virtual double CalculateParkingFee(double parkingDuration) const = 0; // Zmiana: parametr parkingDuration
    virtual ~Vehicle() = default;
};

class StandardCar : public Vehicle {
private:
    string fuelType;
public:
    double CalculateParkingFee(double parkingDuration) const override { // Zmiana: parametr parkingDuration
        if (fuelType == "Electric") return ELECTRIC_FEE * parkingDuration; // 1 zł/godz. dla elektrycznych
        return getVehicleWeight() * 0.005 * parkingDuration; // 0.005 zł/kg/godz.
    }
    StandardCar(string registrationNumber, time_t entryTime, int vehicleWeight, string fuelType)
        : Vehicle(registrationNumber, entryTime, vehicleWeight), fuelType(fuelType) {
        if(fuelType != "Electric" && fuelType != "Diesel" && fuelType != "Petrol") {
            throw invalid_argument("Fuel type must be either Electric, Diesel, or Petrol.");
        }
    }
};

class Motorcycle : public Vehicle {
public:
    double CalculateParkingFee(double parkingDuration) const override { // Zmiana: parametr parkingDuration
        return getVehicleWeight() * 0.02 * parkingDuration; // 0.02 zł/kg/godz.
    }
    Motorcycle(string registrationNumber, time_t entryTime, int vehicleWeight)
        : Vehicle(registrationNumber, entryTime, vehicleWeight) {}
};

enum class VanType { light, heavy };
class DeliveryVan : public Vehicle {
private:
    VanType vehicleType;
    string fuelType;
public:
    double CalculateParkingFee(double parkingDuration) const override { // Zmiana: parametr parkingDuration
        if (fuelType == "Electric") return ELECTRIC_FEE * parkingDuration; // 1 zł/godz. dla elektrycznych
        double feePerKG = (vehicleType == VanType::light) ? 0.01 : 0.02; // 0.01 zł/kg/godz. dla lekkich, 0.02 zł/kg/godz. dla ciężkich
        return getVehicleWeight() * feePerKG * parkingDuration;
    }
    DeliveryVan(string registrationNumber, time_t entryTime, int vehicleWeight, VanType vehicleType, string fuelType)
        : Vehicle(registrationNumber, entryTime, vehicleWeight), vehicleType(vehicleType), fuelType(fuelType) {
        if(fuelType != "Electric" && fuelType != "Diesel" && fuelType != "Petrol") {
            throw invalid_argument("Fuel type must be either Electric, Diesel, or Petrol.");
        }
    }
};

class Ticket {
private:
    shared_ptr<Vehicle> vehicle;
    time_t issueTime;
    time_t exitTime;
    double fee;
public:
    Ticket(shared_ptr<Vehicle> v, time_t issue, time_t exit)
        : vehicle(v), issueTime(issue), exitTime(exit), fee(0.0) {}

    void setExitTime(time_t exit) { exitTime = exit; }
    void setIssueTime(time_t issue) { issueTime = issue; }

    double CalculateFee() {
        if (!vehicle) return 0.0;
        double parkingDuration = difftime(exitTime, issueTime) / 3600.0;
        if (parkingDuration < 0) {
            throw invalid_argument("Exit time cannot be earlier than issue time.");
        }
        fee = vehicle->CalculateParkingFee(parkingDuration); // Zmiana: przekazanie parkingDuration
        return fee;
    }
    void showTicket() {
        if (!vehicle) {
            cout << "No vehicle registered for this ticket." << endl;
            return;
        }
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

    void registerVehicle(shared_ptr<Vehicle> vehicle) {
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
            Ticket ticket(vehicle, vehicle->getEntryTime(), exitTime);
            ticket.CalculateFee();
            vehicles.erase(it);
            return ticket;
        } else {
            cout << "Vehicle not found!" << endl;
            return Ticket(nullptr, 0, 0); // Zwraca pusty bilet
        }
    }
    bool isFull() {
        return vehicles.size() >= capacity;
    }
    const vector<shared_ptr<Vehicle>>& ReturnVehicles() const {
        return vehicles;
    }
};

int main() {
    Parking parking(3);
    cout << "Uruchomiono symulator parkingu. Wpisz 'dodaj', 'wyrejestruj' lub 'lista' (exit, by zakonczyc):" << endl;
    string command;

    while (cin >> command && command != "exit") {
        transform(command.begin(), command.end(), command.begin(), ::tolower);

        switch (command[0]) {
            case 'd': { // 'dodaj'
                string reg;
                int weight, vehicleType, fuelType, vanType;
                cout << "Typ pojazdu (1 - samochod, 2 - motocykl, 3 - furgonetka): ";
                cin >> vehicleType;
                
                if (vehicleType < 1 || vehicleType > 3) {
                    cout << "Błąd: Nieprawidłowy typ pojazdu. Użyj 1, 2 lub 3." << endl;
                    break;
                }

                cout << "Numer rejestracyjny: ";
                cin >> reg;
                cout << "Waga (kg): ";
                cin >> weight;

                try {
                    if (vehicleType == 1) { // Samochód
                        cout << "Typ paliwa (1 - Electric, 2 - Petrol, 3 - Diesel): ";
                        cin >> fuelType;
                        if (fuelType < 1 || fuelType > 3) {
                            throw invalid_argument("Nieprawidłowy typ paliwa. Użyj 1, 2 lub 3.");
                        }
                        string fuel = (fuelType == 1) ? "Electric" : (fuelType == 2) ? "Petrol" : "Diesel";
                        parking.registerVehicle(make_shared<StandardCar>(reg, time(nullptr), weight, fuel));
                    } else if (vehicleType == 2) { // Motocykl
                        parking.registerVehicle(make_shared<Motorcycle>(reg, time(nullptr), weight));
                    } else { // Furgonetka
                        cout << "Typ furgonetki (1 - lekka, 2 - ciezka): ";
                        cin >> vanType;
                        if (vanType < 1 || vanType > 2) {
                            throw invalid_argument("Nieprawidłowy typ furgonetki. Użyj 1 lub 2.");
                        }
                        cout << "Typ paliwa (1 - Electric, 2 - Petrol, 3 - Diesel): ";
                        cin >> fuelType;
                        if (fuelType < 1 || fuelType > 3) {
                            throw invalid_argument("Nieprawidłowy typ paliwa. Użyj 1, 2 lub 3.");
                        }
                        string fuel = (fuelType == 1) ? "Electric" : (fuelType == 2) ? "Petrol" : "Diesel";
                        VanType type = (vanType == 1) ? VanType::light : VanType::heavy;
                        parking.registerVehicle(make_shared<DeliveryVan>(reg, time(nullptr), weight, type, fuel));
                    }
                    cout << "Pojazd pomyślnie zarejestrowany." << endl;
                } catch (const invalid_argument& e) {
                    cout << "Błąd: " << e.what() << endl;
                }
                break;
            }
            case 'w': { // 'wyrejestruj'
                string reg;
                cout << "Numer rejestracyjny: ";
                cin >> reg;
                bool found = false;
                for (auto& v : parking.ReturnVehicles()) {
                    try {
                        if (v->getRegistrationNumber() == reg) {
                            Ticket ticket = parking.unregisterVehicle(v);
                            ticket.showTicket();
                            found = true;
                            break;
                        }
                    } catch (const invalid_argument& e) {
                        cout << "Błąd: " << e.what() << endl;
                    }
                }
                if (!found) {
                    cout << "Pojazd o numerze rejestracyjnym " << reg << " nie został znaleziony." << endl;
                }
                break;
            }
            case 'l': { // 'lista'
                if (parking.ReturnVehicles().empty()) {
                    cout << "Parking jest pusty." << endl;
                } else {
                    cout << "Lista zaparkowanych pojazdów:" << endl;
                    for (auto& v : parking.ReturnVehicles()) {
                        try {
                            cout << v->getRegistrationNumber() << endl;
                        } catch (const invalid_argument& e) {
                            cout << "Błąd: " << e.what() << endl;
                        }
                    }
                }
                break;
            }
            default:
                cout << "Nieznane polecenie. Użyj 'dodaj', 'wyrejestruj' lub 'lista'." << endl;
                break;
        }
        cout << "Wpisz 'dodaj', 'wyrejestruj' lub 'lista' (exit, by zakonczyc):" << endl;
    }

    return 0;
}