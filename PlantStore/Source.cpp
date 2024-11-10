#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // За използване на system()
#include <iomanip> // За формат на изхода
#include <windows.h> // За настройка на UTF-8 в Windows конзолата

using namespace std;

// Функция за промяна на цвета на конзолата
void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Функция за пауза, изискваща натискане на Enter
void pause() {
    cout << "Натиснете Enter, за да продължите...";
    cin.ignore();
    cin.get();
}

// Функция за валидация на положително количество
int validateQuantity(const string& prompt) {
    int quantity;
    do {
        cout << prompt;
        cin >> quantity;
        if (quantity < 0) {
            setConsoleColor(FOREGROUND_RED);
            cout << "Грешка: Количеството трябва да бъде положително число!\n";
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            pause();
        }
    } while (quantity < 0);
    return quantity;
}

// Функция за валидация на положителна цена
double validatePrice(const string& prompt) {
    double price;
    do {
        cout << prompt;
        cin >> price;
        if (price < 0) {
            setConsoleColor(FOREGROUND_RED);
            cout << "Грешка: Цената трябва да бъде положително число!\n";
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            pause();
        }
    } while (price < 0);
    return price;
}

// Клас Plant, който представлява информация за растение
class Plant {
private:
    string name;
    double price;
    int quantity;
    int sales;

public:
    // Конструктор
    Plant(string n, double p, int q) : name(n), price(p), quantity(q), sales(0) {}

    // Гетери за достъп до атрибутите
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getSales() const { return sales; }

    // Методи за задаване на атрибути и промяна на количеството
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    void sell(int qty) {
        if (qty <= quantity) {
            quantity -= qty;
            sales += qty;
        }
        else {
            setConsoleColor(FOREGROUND_RED);
            cout << "Грешка: Недостатъчна наличност за продажба!" << endl;
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            pause();
        }
    }

    double getRevenue() const {
        return sales * price;
    }

    // Метод за извеждане на информация за растението
    void display() const {
        cout << left << setw(25) << name
            << "$" << setw(14) << fixed << setprecision(2) << price
            << setw(15) << quantity << endl;
    }

    void displaySalesReport() const {
        if (sales > 0) {
            cout << left << setw(25) << name
                << setw(15) << fixed << setprecision(2) << price
                << setw(21) << sales 
                << "$" << setw(10) << fixed << setprecision(2) << getRevenue() << endl;
        }
    }
};

// Клас Inventory за управление на инвентара
class Inventory {
private:
    vector<Plant> plants;

public:
    void addPlant(const string& name, double price, int quantity) {
        for (const auto& plant : plants) {
            if (plant.getName() == name) {
                setConsoleColor(FOREGROUND_RED);
                cout << "Грешка: Растението вече съществува в инвентара." << endl;
                setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                pause();
                return;
            }
        }
        plants.emplace_back(name, price, quantity);
        setConsoleColor(FOREGROUND_GREEN);
        cout << "Растението беше успешно добавено!" << endl;
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        pause();
    }

    void updateStock(const string& name, int newQuantity) {
        for (auto& plant : plants) {
            if (plant.getName() == name) {
                plant.setQuantity(newQuantity);
                setConsoleColor(FOREGROUND_GREEN);
                cout << "Наличността беше успешно обновена!" << endl;
                setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                pause();
                return;
            }
        }
        setConsoleColor(FOREGROUND_RED);
        cout << "Растението не беше намерено." << endl;
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        pause();
    }

    void sellPlant(const string& name, int quantity) {
        for (auto& plant : plants) {
            // Проверка само дали името съвпада
            if (plant.getName() == name) {
                if (quantity <= plant.getQuantity()) {
                    plant.sell(quantity);
                    setConsoleColor(FOREGROUND_GREEN);
                    cout << "Продажбата беше успешна. Обща сума: $" << quantity * plant.getPrice() << endl;
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    pause();
                    return;
                }
                else {
                    setConsoleColor(FOREGROUND_RED);
                    cout << "Грешка: Недостатъчна наличност за продажба!" << endl;
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    pause();
                    return;
                }
            }
        }
        setConsoleColor(FOREGROUND_RED);
        cout << "Растението не беше намерено." << endl;
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        pause();
    }

    void viewInventory() const {
        system("cls");
        cout << "\nТекущ инвентар:\n";
        cout << left << setw(25) << "Име" << setw(15) << "Цена" << setw(15) << "Наличност" << endl;
        cout << "--------------------------------------------------------------------------\n";
        for (const auto& plant : plants) {
            plant.display();
        }
        pause();
    }

    void generateReport() const {
        system("cls");
        double totalRevenue = 0;
        cout << "\nОтчет за продажбите:\n";
        cout << left << setw(25) << "Име"
            << setw(15) << "Единична цена"
            << setw(21) << "Продадено количество"
            << setw(10) << "Приход" << endl;
        cout << "--------------------------------------------------------------------------\n";
        for (const auto& plant : plants) {
            plant.displaySalesReport();
            totalRevenue += plant.getRevenue();
        }
        cout << "--------------------------------------------------------------------------\n";
        cout << left << setw(60) << "Общ приход:"
            << "$" << fixed << setprecision(2) << totalRevenue << endl;
        pause();
    }


};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Inventory inventory;
    int choice;

    do {
        system("cls");
        cout << "Система за управление на магазин за растения\n";
        cout << "1. Добави ново растение\n";
        cout << "2. Обнови наличност\n";
        cout << "3. Продай растение\n";
        cout << "4. Виж инвентара\n";
        cout << "5. Генерирай отчет за продажби\n";
        cout << "6. Изход\n";
        cout << "Изберете опция: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            setConsoleColor(FOREGROUND_RED);
            cout << "Грешка: Невалиден избор, опитайте отново." << endl;
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            pause();
            continue;
        }

        string name;
        double price;
        int quantity;

        switch (choice) {
        case 1:
            cin.ignore();
            system("cls");
            cout << "Въведете име на растението: ";
            getline(cin, name);
            price = validatePrice("Въведете цена на растението: ");
            quantity = validateQuantity("Въведете начално количество на наличност: ");
            inventory.addPlant(name, price, quantity);
            break;
        case 2:
            cin.ignore();
            system("cls");
            cout << "Въведете име на растението за обновяване на наличност: ";
            getline(cin, name);
            quantity = validateQuantity("Въведете ново количество на наличност: ");
            inventory.updateStock(name, quantity);
            break;
        case 3:
            cin.ignore();
            system("cls");
            cout << "Въведете име на растението за продажба: ";
            getline(cin, name);
            quantity = validateQuantity("Въведете количество за продажба: ");
            inventory.sellPlant(name, quantity);
            break;
        case 4:
            inventory.viewInventory();
            break;
        case 5:
            inventory.generateReport();
            break;
        case 6:
            system("cls");
            cout << "Излизане...\n";
            break;
        default:
            system("cls");
            setConsoleColor(FOREGROUND_RED);
            cout << "Невалиден избор, опитайте отново." << endl;
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            pause();
        }
    } while (choice != 6);

    return 0;
}
