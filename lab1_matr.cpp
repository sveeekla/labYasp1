//Михайловская Светлан 9 группа
//Задача 11
//В матрице удалить столбцы с максимальным и минимальным элементами матрицы,
//а затем на место первого из них добавить столбец из произведений элементов
//соответствующих строк.
#include <iostream>
#include <fstream>
#include <iomanip>
#include <Windows.h>

void deleting_columns_with_extremes(int**& matrix, int& columns, int rows);//функция удаляет столбцы содержащие экстремумы кроме первого его эл-ты меняются на произведение
void find_min_max(int** matrix, int rows, int columns, int& min_value, int& max_value, int& number_first_extrems);//поиск минимума и максимума и номер первого столбца в котором есть минимум или максимум
void replacing_colum(int**& matrix, int column, int rows, int columns);//замена элементов столбца
int product_row(int** matrix, int row, int columns);// функция подсчета произведения эллементов передаваемой строки
int check_correct_input(const char* message);// функция считывания и проверки правильности ввода столбцов и строк матрицы с клавиатуры

int** memory_allocation(int rows, int columns);// функция создания матрицы создается массив указателей на массивы столбцов
void fill_matrix(int**& matrix, int rows, int columns, std::istream& stream = std::cin);//заполнение матрицы из потока(с клавиатуры, либо из файла)
void print_matrix(int** matrix, int rows, int columns, std::ostream& stream = std::cout);//печать матрицы на экран или в файл
void free_memory(int**& matrix, int columns);
void print_answer_menu(int** matrix, int rows, int columns);// меню печати результирующей матрицы

bool conditional(int choice, int max_choice);//функция задоющая условие проверки ввода
void read_and_check(int& choice, int max_choice, bool(*conditional)(int, int), const char* message);//функция чтения и проверки вводимого значения
void menu(const char* menu_message, const char* read_and_check_message, int& choice, int max_choice);//меню
void end_menu(int& choice);//функция завершения работы

int main()
{
    SetConsoleOutputCP(1251);
    int choice_task{};  //переменная хранящая в себе выбор менюшки
    int rows{}, columns{};
    int** matrix = {};
    do
    {
        menu("Меню:\n1.Считывать матрицу из файла\n2.Считывать матрицу с клавиатуры\n", "Введите выбранный вами пункт: ", choice_task, 2);
        int rows{}, columns{};
        if (choice_task == 1)
        {
            char inp_file_name[255];
            bool flag{};
            do
            {
                std::cout << "Введите имя файла: ";
                std::cin >> inp_file_name;
                std::ifstream inp_file(inp_file_name);
                if (inp_file.is_open())
                {
                    flag = true;
                    inp_file >> rows >> columns;
                    matrix = memory_allocation(rows, columns);
                    fill_matrix(matrix, rows, columns, inp_file);
                }
                else
                    std::cout << "Файл не найден!\n";
            } while (!flag);
        }
        else
        {
            rows = check_correct_input("\nВведите количество строк в матрице: ");
            columns = check_correct_input("\nВведите количество столбцов в матрице: ");
            matrix = memory_allocation(rows, columns);
            fill_matrix(matrix, rows, columns);
            
        }
        print_matrix(matrix, rows, columns);
        deleting_columns_with_extremes(matrix, columns, rows);
        print_answer_menu(matrix, rows, columns);
        end_menu(choice_task);
    } while (choice_task != 3);
    free_memory(matrix, columns);
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    return 0;
}

void print_answer_menu(int** matrix, int rows, int columns)
{
    std::ofstream out_file("outFile.txt");
    int choice_input{};
    menu("Куда вывести ответ:\n1.На экран\n2.В файл\n", "Введите выбранный вами пункт: ", choice_input, 2);
    if (choice_input == 1)
        print_matrix(matrix, rows, columns);
    else
        print_matrix(matrix, rows, columns, out_file);
}

void deleting_columns_with_extremes(int**& matrix, int& columns, int rows)
{
    int min_value{}, max_value{}, number_first_extrems{};
    find_min_max(matrix, rows, columns, min_value, max_value, number_first_extrems);
    for (int j{ number_first_extrems + 1 }; j < columns; ++j)
    {
        bool is_extrems = false;
        int i{};
        while (!is_extrems && i < rows)
        {
            if (matrix[j][i] == min_value || matrix[j][i] == max_value)
                is_extrems = true;
            i++;
        }
        if (is_extrems)
        {
            for (int n = j; n < columns; n++)
                matrix[n] = matrix[n + 1];
            --columns;
        }
    }
    replacing_colum(matrix, number_first_extrems, rows, columns);
}

void find_min_max(int** matrix, int rows, int columns, int& min_value, int& max_value, int& number_first_extrems)
{
    min_value = INT_MAX;
    max_value = INT_MIN;
    int min_column{ -1 };
    int max_column{ -1 };
    number_first_extrems = 0;
    for (int j{}; j < columns; ++j)
    {
        int min_in_column = INT_MAX;
        int max_in_column = INT_MIN;
        for (int i{}; i < rows; ++i)
        {
            if (matrix[j][i] < min_in_column)
                min_in_column = matrix[j][i];
            if (matrix[j][i] > max_in_column)
                max_in_column = matrix[j][i];
        }
        if (min_value > min_in_column)
        {
            min_value = min_in_column;
            min_column = j;
        }
        if (max_value < max_in_column)
        {
            max_value = max_in_column;
            max_column = j;
        }
    }
    number_first_extrems = min_column < max_column ? min_column : max_column;
}

void replacing_colum(int**& matrix, int column, int rows, int columns)
{
    for (int i{}; i < rows; ++i)
        matrix[column][i] = product_row(matrix, i, columns);
}

int product_row(int** matrix, int row, int columns)
{
    int product{ 1 };
    for (int j{}; j < columns; ++j)
        product *= matrix[j][row];
    return product;
}

int check_correct_input(const char* message)
{
    int number{};
    std::cout << message;
    while (!(std::cin >> number) && number >= 0) {
        std::cout << "Неверный ввод. Пожалуйста, введите целое число: ";
        std::cin.clear();
        std::cin.ignore(std::cin.rdbuf()->in_avail());
    }
    return number;
}


int** memory_allocation(int rows, int columns)
{
    int** matrix = new int* [columns];
    for (int i = 0; i < columns; ++i)
        matrix[i] = new int[rows];
    return matrix;
}

void fill_matrix(int**& matrix, int rows, int columns, std::istream& stream)
{
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            stream >> matrix[j][i];
}

void print_matrix(int** matrix, int rows, int columns, std::ostream& stream)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
            stream << std::setw(7) << matrix[j][i];
        stream << '\n';
    }
}
void free_memory(int**& matrix, int columns)
{
    for (int i = 0; i < columns; ++i)
        delete[] matrix[i];
    delete[] matrix;
}

bool conditional(int choice, int max_choice)
{
    return choice >= 1 && choice <= max_choice;
}

void read_and_check(int& choice, int max_choice, bool(*conditional)(int, int), const char* message)
{
    std::cout << message;
    while (!(std::cin >> choice && conditional(choice, max_choice)))
    {
        std::cout << "Ошибка ввода!\n";
        std::cin.clear();
        std::cin.ignore(std::cin.rdbuf()->in_avail());
        std::cout << message;
    }
}

void menu(const char* menu_message, const char* read_and_check_message, int& choice, int max_choice)
{
    std::cout << menu_message;
    read_and_check(choice, max_choice, conditional, read_and_check_message);
}
void end_menu(int& choice)
{
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cout << "\nЗавершить работу? (Y - да / любая другая клавиша - нет): ";
    char isExit;
    std::cin >> isExit;
    if (isExit == 'Y' || isExit == 'y')
        choice = 3;
}