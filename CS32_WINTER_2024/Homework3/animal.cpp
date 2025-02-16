#include <iostream>
#include <string>
using namespace std;

class Animal
{
	public:
		Animal(string name) : m_name(name) {} 
		virtual void speak() const = 0;
		string name() const;
		virtual string moveAction() const;
		virtual ~Animal() {}
	private:
		string m_name;
};

class Cat : public Animal
{
	public:
		Cat(string name) : Animal(name) {}
		virtual void speak() const;
		virtual ~Cat() {cout << "Destroying " << name() << " the cat\n"; }
};

class Pig : public Animal
{
	public:
		Pig(string name, int age) : Animal(name), m_age(age) {}
		virtual void speak() const;
		virtual ~Pig() {cout << "Destroying " << name() << " the pig\n"; }
	private:
		int m_age;
};

class Duck : public Animal
{
	public:
		Duck(string name) : Animal(name) {}
		virtual void speak() const;
		virtual string moveAction() const;
		virtual ~Duck() {cout << "Destroying " << name() << " the duck\n"; }
};

void Cat::speak() const{ cout <<"Meow"; }

void Pig::speak() const{ if(m_age>=180) cout << "Grunt"; else cout << "Oink"; }

void Duck::speak() const{ cout << "Quack"; }

string Animal::name() const{ return m_name; }

string Animal::moveAction() const{ return "walk"; }

string Duck::moveAction() const{ return "swim"; }

void animate(const Animal* a)
{
    a->speak();
    cout << "!  My name is " << a->name()
         << ".  Watch me " << a->moveAction() << "!\n";
}

int main()
{
    Animal* animals[4];
    animals[0] = new Cat("Puss in Boots");
      // Pigs have a name and a weight in pounds.  Pigs under 180
      // pounds oink; pigs weighing at least 180 pounds grunt.
    animals[1] = new Pig("Napoleon", 190);
    animals[2] = new Pig("Wilbur", 50);
    animals[3] = new Duck("Daffy");

    cout << "Here are the animals." << endl;
    for (int k = 0; k < 4; k++)
        animate(animals[k]);

      // Clean up the animals before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete animals[k];
}
