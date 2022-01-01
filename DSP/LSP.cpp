// Liskov Substitution Principle

// Objects in a program should be replaceable with instances of their subtypes
// w/o altering the correctness of the program

#include <iostream>

class Rectangle
{
protected:
    int width, height;
public:
    Rectangle(const int width, const int height)
        : width{ width }, height{ height } { }

    int get_width() const { return width; }
    virtual void set_width(const int width) { this->width = width; }
    int get_height() const { return height; }
    virtual void set_height(const int height) { this->height = height; }

    int area() const { return width * height; }
};

class Square : public Rectangle
{
public:
    Square(int size) : Rectangle(size, size) {}
    void set_width(const int width) override { // ������ ����� ����ع���
        this->width = height = width;
    }
    void set_height(const int height) override { // ������ ����� ����ع���
        this->height = width = height;
    }
};

// �ذ��, rectangle�� ���� ���������� ��ӹ޴°��� ���� idea�� �ƴϴ�.
// can have rectangle boolean flag wheather it is Square or not
// or make factory

struct RectangleFactory
{
    static Rectangle create_rectangle(int w, int h);
    static Rectangle create_square(int size);
};

void process(Rectangle& r)
{
    int w = r.get_width();
    r.set_height(10); // process only make sense with rectangle, 

    std::cout << "expected area = " << (w * 10)
        << ", got " << r.area() << std::endl;
}

int main342342()
{
    Rectangle r{ 5,5 };
    process(r);

    Square s{ 5 };
    process(s);

    getchar();
    return 0;
}