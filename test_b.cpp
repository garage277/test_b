#include <iostream>

using namespace std;

class Overturner
{
public:
    const static uint16_t max_mod{10000};

    Overturner(int16_t value)
    {
        source_value = value;
        current_value = std::abs(value);
        negative_flag = (source_value != current_value);

        if( current_value > max_mod )
            throw std::invalid_argument("передано недопустимое значение");
    }

    friend std::ostream& operator << ( std::ostream &, const Overturner &);

private:
    int16_t source_value{0}; //введенное пользователем значение
    mutable int16_t current_value{0}; //текущее рабочее значение
    bool negative_flag{false}; //признак отрицательного значения
    mutable bool flag_number{false}; //нам надо знать, была ли уже распечатана цифра, чтобы знать, печатать ли 0
    mutable uint8_t depth{0}; //глубина рекурсии

    //приводит класс к состоянию, пригодному для повторного вывода
    void clear() const
    {
        current_value = std::abs(source_value);
        depth = 0;
        flag_number = false;
    }
};

std::ostream& operator << ( std::ostream &os, const Overturner &x)
{
    //0 - признак завершения рекурсии
    if( x.current_value == 0 )
    {
        //FIXME если пользователь ввел 0... выводим 0 ?
        if( x.depth == 0 )
            os << x.current_value;

        x.clear(); //готовим XPrinter к возможному повторному выводу
        return os;
    }

    //на первой итерации выводим минус для отрицательных чисел
    if( x.depth == 0 && x.negative_flag )
        os << "-";

    //получаем последнюю цифру
    auto ostatok = x.current_value % 10;

    //отсекать нули в начале числа будем по flag_number
    //flag_number поднимается, если уже было что-то отличное от 0, а значит можно печатать всё
    if( ostatok != 0 )
        x.flag_number = true;

    if( x.flag_number )
        os << ostatok;

    ++x.depth;
    x.current_value /= 10;

    return (os << x);
}

int main()
{
    //cout << "введите число, по модулю не превосходящее " << Overturner::max_mod << endl;;

    int16_t x{0};
    cin.exceptions(std::ios::failbit);
    cin >> x;

    Overturner ot(x);

    cout << ot << endl;

    return 0;
}
