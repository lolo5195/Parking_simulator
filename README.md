# Symulator parkingu – Projekt z Programowania Obiektowego (C++)

## Opis projektu
Symulator parkingu to konsolowa aplikacja napisana w języku C++, której celem jest odwzorowanie podstawowej logiki działania parkingu dla różnych typów pojazdów. Program pozwala na:

- Rejestrację pojazdów (samochód, motocykl, furgonetka)
- Wyrejestrowanie pojazdu i obliczenie opłaty parkingowej
- Wyświetlenie listy pojazdów znajdujących się na parkingu

### Typy pojazdów:
- `StandardCar` – samochód osobowy (z paliwem: Electric, Petrol, Diesel)
- `Motorcycle` – motocykl
- `DeliveryVan` – furgonetka (lekka lub ciężka)

Opłata parkingowa zależy od:
- typu pojazdu,
- masy pojazdu,
- czasu parkowania,
- rodzaju paliwa.


##  Struktura projektu

### Klasy i relacje:

- `Vehicle` *(klasa abstrakcyjna)* – bazowa dla wszystkich pojazdów. Zawiera wspólne atrybuty, takie jak numer rejestracyjny, masa, czas wjazdu.
- `StandardCar`, `Motorcycle`, `DeliveryVan` – klasy dziedziczące z `Vehicle`, implementujące specyficzne cechy (np. waga, opłaty, typ paliwa).
- `Parking` – zarządza listą pojazdów, umożliwia dodawanie i usuwanie, obliczanie opłat.
- `Ticket` - przechowuje dane dotyczące pojedynczego biletu parkingowego.
---

## Przykładowe użycie
```bash
Uruchomiono symulator parkingu. Wpisz 'dodaj', 'wyrejestruj' lub 'lista' (exit, by zakonczyc):
> dodaj
Typ pojazdu (1 - samochod, 2 - motocykl, 3 - furgonetka): 1
Numer rejestracyjny: AB123CD
Waga (kg): 1200
Typ paliwa (1 - Electric, 2 - Petrol, 3 - Diesel): 2
Pojazd pomyślnie zarejestrowany.

> lista
Lista zaparkowanych pojazdów:
AB123CD

> wyrejestruj
Numer rejestracyjny: AB123CD
Vehicle: AB123CD
Issue Time: Mon Jun  2 18:10:00 2025
Exit Time: Mon Jun  2 20:30:00 2025
Fee: 15.0 zl
