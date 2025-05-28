#include <string>
#include <ctime>
#include <vector>
#include <memory>
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

    return 0;
}