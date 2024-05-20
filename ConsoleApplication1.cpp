#include <iostream>
#include <cctype>
#include <cstring>

using namespace std;

class СТРОКА {

public:
    char* str;
    int length;

    СТРОКА() : str(nullptr), length(0) {
        cout << "Конструктор без параметров вызван\n";
    }

    СТРОКА(const char* s) {
        length = strlen(s);
        str = new char[length + 1];
        strcpy(str, s);
        cout << "Конструктор с С-строкой вызван\n";
    }

    СТРОКА(char c) {
        str = new char[2];
        str[0] = c;
        str[1] = '\0';
        length = 1;
        cout << "Конструктор с символом вызван\n";
    }

    СТРОКА(const СТРОКА& other) {
        length = other.length;
        str = new char[length + 1];
        strcpy(str, other.str);
        cout << "Конструктор копирования вызван\n";
    }

    int getLength() const {
        return length;
    }

    void clear() {
        delete[] str;
        str = nullptr;
        length = 0;
    }

    ~СТРОКА() {
        delete[] str;
        cout << "Деструктор вызван\n";
    }
};


class СТРОКА_ИДЕНТИФИКАТОР : public СТРОКА {
public:
    СТРОКА_ИДЕНТИФИКАТОР() : СТРОКА() {
        cout << "Конструктор без параметров вызван\n";
    }

    СТРОКА_ИДЕНТИФИКАТОР(const char* s) : СТРОКА(s) {
        cout << "Конструктор с С-строкой вызван\n";
    }

    СТРОКА_ИДЕНТИФИКАТОР(char c) : СТРОКА(c) {
        cout << "Конструктор с символом вызван\n";
    }

    СТРОКА_ИДЕНТИФИКАТОР(const СТРОКА& base) : СТРОКА(base) {
        cout << "Конструктор копирования вызван\n";
    }

    virtual void toUpperCase() {
        if (getLength() == 0) return;
        for (int i = 0; i < getLength(); i++) {
            if (str[i] >= 'a' && str[i] <= 'z') {
                str[i] = str[i] - 'a' + 'A';
            }
        }
    }

    virtual void toLowerCase() {
        if (getLength() == 0) return;
        for (int i = 0; i < getLength(); i++) {
            if (str[i] >= 'A' && str[i] <= 'Z') {
                str[i] = str[i] - 'A' + 'a';
            }
        }
    }

    virtual int findFirstOccurrence(char c) {
        if (getLength() == 0) return -1;
        for (int i = 0; i < getLength(); i++) {
            if (str[i] == c) {
                return i;
            }
        }
        return -1;
    }

    ~СТРОКА_ИДЕНТИФИКАТОР() {
        cout << "Деструктор вызван\n";
    }

    // Переопределение оператора присваивания
    СТРОКА_ИДЕНТИФИКАТОР& operator=(const СТРОКА_ИДЕНТИФИКАТОР& other) {
        if (this != &other) {
            delete[] str;
            length = other.length;
            str = new char[length + 1];
            strcpy(str, other.str);
        }
        return *this;
    }

    // Переопределение оператора сложения для конкатенации строк
    СТРОКА_ИДЕНТИФИКАТОР operator+(const СТРОКА_ИДЕНТИФИКАТОР& other) const {
        СТРОКА_ИДЕНТИФИКАТОР temp;
        temp.length = length + other.length;
        temp.str = new char[temp.length + 1];
        strcpy(temp.str, str);
        strcat(temp.str, other.str);
        return temp;
    }

    // Переопределение оператора вычитания
    СТРОКА_ИДЕНТИФИКАТОР operator-(const СТРОКА_ИДЕНТИФИКАТОР& other) const {
        СТРОКА_ИДЕНТИФИКАТОР temp(str); // Создаем временный объект с текущей строкой
        for (int i = 0; i < other.length; ++i) {
            int pos;
            while ((pos = temp.findFirstOccurrence(other.str[i])) != -1) {
                // Сдвигаем все символы после найденного на один влево
                memmove(&temp.str[pos], &temp.str[pos + 1], temp.length - pos);
                temp.length--;
            }
        }
        temp.str[temp.length] = '\0'; // Обновляем нуль-терминатор
        return temp;
    }

    // Переопределение оператора сравнения на больше
    bool operator>(const СТРОКА_ИДЕНТИФИКАТОР& other) const {
        int min_length = min(length, other.length);
        for (int i = 0; i < min_length; ++i) {
            if (str[i] > other.str[i]) return true;
            if (str[i] < other.str[i]) return false;
        }
        return length > other.length;
    }

    // Переопределение оператора сравнения на меньше
    bool operator<(const СТРОКА_ИДЕНТИФИКАТОР& other) const {
        int min_length = min(length, other.length);
        for (int i = 0; i < min_length; ++i) {
            if (str[i] < other.str[i]) return true;
            if (str[i] > other.str[i]) return false;
        }
        return length < other.length;
    }
};


int main() {
    const int size = 4;
    СТРОКА** array = new СТРОКА * [size];

    array[0] = new СТРОКА_ИДЕНТИФИКАТОР("Hello");
    array[1] = new СТРОКА_ИДЕНТИФИКАТОР('A');
    array[2] = new СТРОКА_ИДЕНТИФИКАТОР();
    array[3] = new СТРОКА_ИДЕНТИФИКАТОР(*array[0]);

    // Проверка методов
    cout << "Проверка методов:\n";
    for (int i = 0; i < size; i++) {
        cout << "Длина строки: " << array[i]->getLength() << endl;
    }

    // Проверка оператора присваивания
    *array[2] = *array[0];
    cout << "После присваивания array[2] = array[0]: " << array[2]->str << endl;

    // Проверка оператора сложения
    СТРОКА_ИДЕНТИФИКАТОР concat = *static_cast<СТРОКА_ИДЕНТИФИКАТОР*>(array[0]) + *static_cast<СТРОКА_ИДЕНТИФИКАТОР*>(array[1]);
    cout << "Конкатенация array[0] и array[1]: " << concat.str << endl;

    // Проверка оператора вычитания
    СТРОКА_ИДЕНТИФИКАТОР subtract = *static_cast<СТРОКА_ИДЕНТИФИКАТОР*>(array[0]) - *static_cast<СТРОКА_ИДЕНТИФИКАТОР*>(array[1]);
    cout << "Вычитание array[1] из array[0]: " << subtract.str << endl;

    // Проверка операторов сравнения
    bool isGreater = *static_cast<СТРОКА_ИДЕНТИФИКАТОР*>(array[0]) > *static_cast<СТРОКА_ИДЕНТИФИКАТОР*>(array[1]);
    cout << "array[0] > array[1]: " << (isGreater ? "Да" : "Нет") << endl;

    bool isLess = *static_cast<СТРОКА_ИДЕНТИФИКАТОР*>(array[0]) < *static_cast<СТРОКА_ИДЕНТИФИКАТОР*>(array[1]);
    cout << "array[0] < array[1]: " << (isLess ? "Да" : "Нет") << endl;

    // Очистка памяти
    for (int i = 0; i < size; i++) {
        delete array[i];
    }
    delete[] array;

    return 0;
}
