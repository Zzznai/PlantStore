#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // �� ���������� �� system()
#include <iomanip> // �� ������ �� ������
#include <windows.h> // �� ��������� �� UTF-8 � Windows ���������

using namespace std;

// ������� �� ������� �� ����� �� ���������
void setConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// ������� �� �����, ��������� ��������� �� Enter
void pause() {
    cout << "��������� Enter, �� �� ����������...";
    cin.ignore();
    cin.get();
}

// ������� �� ��������� �� ����������� ����������
int validateQuantity(const string& prompt) {
    int quantity;
    do {
        cout << prompt;
        cin >> quantity;
        if (quantity < 0) {
            setConsoleColor(FOREGROUND_RED);
            cout << "������: ������������ ������ �� ���� ����������� �����!\n";
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            pause();
        }
    } while (quantity < 0);
    return quantity;
}

// ������� �� ��������� �� ����������� ����
double validatePrice(const string& prompt) {
    double price;
    do {
        cout << prompt;
        cin >> price;
        if (price < 0) {
            setConsoleColor(FOREGROUND_RED);
            cout << "������: ������ ������ �� ���� ����������� �����!\n";
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            pause();
        }
    } while (price < 0);
    return price;
}

// ���� Plant, ����� ������������ ���������� �� ��������
class Plant {
private:
    string name;
    double price;
    int quantity;
    int sales;

public:
    // �����������
    Plant(string n, double p, int q) : name(n), price(p), quantity(q), sales(0) {}

    // ������ �� ������ �� ����������
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getSales() const { return sales; }

    // ������ �� �������� �� �������� � ������� �� ������������
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    void sell(int qty) {
        if (qty <= quantity) {
            quantity -= qty;
            sales += qty;
        }
        else {
            setConsoleColor(FOREGROUND_RED);
            cout << "������: ������������ ��������� �� ��������!" << endl;
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            pause();
        }
    }

    double getRevenue() const {
        return sales * price;
    }

    // ����� �� ��������� �� ���������� �� ����������
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

// ���� Inventory �� ���������� �� ���������
class Inventory {
private:
    vector<Plant> plants;

public:
    void addPlant(const string& name, double price, int quantity) {
        for (const auto& plant : plants) {
            if (plant.getName() == name) {
                setConsoleColor(FOREGROUND_RED);
                cout << "������: ���������� ���� ���������� � ���������." << endl;
                setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                pause();
                return;
            }
        }
        plants.emplace_back(name, price, quantity);
        setConsoleColor(FOREGROUND_GREEN);
        cout << "���������� ���� ������� ��������!" << endl;
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        pause();
    }

    void updateStock(const string& name, int newQuantity) {
        for (auto& plant : plants) {
            if (plant.getName() == name) {
                plant.setQuantity(newQuantity);
                setConsoleColor(FOREGROUND_GREEN);
                cout << "����������� ���� ������� ��������!" << endl;
                setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                pause();
                return;
            }
        }
        setConsoleColor(FOREGROUND_RED);
        cout << "���������� �� ���� ��������." << endl;
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        pause();
    }

    void sellPlant(const string& name, int quantity) {
        for (auto& plant : plants) {
            // �������� ���� ���� ����� �������
            if (plant.getName() == name) {
                if (quantity <= plant.getQuantity()) {
                    plant.sell(quantity);
                    setConsoleColor(FOREGROUND_GREEN);
                    cout << "���������� ���� �������. ���� ����: $" << quantity * plant.getPrice() << endl;
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    pause();
                    return;
                }
                else {
                    setConsoleColor(FOREGROUND_RED);
                    cout << "������: ������������ ��������� �� ��������!" << endl;
                    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    pause();
                    return;
                }
            }
        }
        setConsoleColor(FOREGROUND_RED);
        cout << "���������� �� ���� ��������." << endl;
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        pause();
    }

    void viewInventory() const {
        system("cls");
        cout << "\n����� ��������:\n";
        cout << left << setw(25) << "���" << setw(15) << "����" << setw(15) << "���������" << endl;
        cout << "--------------------------------------------------------------------------\n";
        for (const auto& plant : plants) {
            plant.display();
        }
        pause();
    }

    void generateReport() const {
        system("cls");
        double totalRevenue = 0;
        cout << "\n����� �� ����������:\n";
        cout << left << setw(25) << "���"
            << setw(15) << "�������� ����"
            << setw(21) << "��������� ����������"
            << setw(10) << "������" << endl;
        cout << "--------------------------------------------------------------------------\n";
        for (const auto& plant : plants) {
            plant.displaySalesReport();
            totalRevenue += plant.getRevenue();
        }
        cout << "--------------------------------------------------------------------------\n";
        cout << left << setw(60) << "��� ������:"
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
        cout << "������� �� ���������� �� ������� �� ��������\n";
        cout << "1. ������ ���� ��������\n";
        cout << "2. ������ ���������\n";
        cout << "3. ������ ��������\n";
        cout << "4. ��� ���������\n";
        cout << "5. ��������� ����� �� ��������\n";
        cout << "6. �����\n";
        cout << "�������� �����: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            setConsoleColor(FOREGROUND_RED);
            cout << "������: ��������� �����, �������� ������." << endl;
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
            cout << "�������� ��� �� ����������: ";
            getline(cin, name);
            price = validatePrice("�������� ���� �� ����������: ");
            quantity = validateQuantity("�������� ������� ���������� �� ���������: ");
            inventory.addPlant(name, price, quantity);
            break;
        case 2:
            cin.ignore();
            system("cls");
            cout << "�������� ��� �� ���������� �� ���������� �� ���������: ";
            getline(cin, name);
            quantity = validateQuantity("�������� ���� ���������� �� ���������: ");
            inventory.updateStock(name, quantity);
            break;
        case 3:
            cin.ignore();
            system("cls");
            cout << "�������� ��� �� ���������� �� ��������: ";
            getline(cin, name);
            quantity = validateQuantity("�������� ���������� �� ��������: ");
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
            cout << "��������...\n";
            break;
        default:
            system("cls");
            setConsoleColor(FOREGROUND_RED);
            cout << "��������� �����, �������� ������." << endl;
            setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            pause();
        }
    } while (choice != 6);

    return 0;
}
