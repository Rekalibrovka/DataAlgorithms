// Definition of Datastructure class, hw3 of TIE-20100/TIE-20106

#include "datastructure.hpp"

// For debug
#include <iostream>
using std::cerr;
using std::endl;

// Helper function to return a random (enough) number in range start-end
// I.e. random_in_range(1,4) produces a random number between 1, 2, 3, or 4.
template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = rand() % range;
    return static_cast<Type>(start+num);
}


Datastructure::Datastructure()
{

}

Datastructure::~Datastructure()
{

}

void Datastructure::add_person(std::string name, int salary)
{
    Person* new_person = new Person;
    new_person->name = name;
    new_person->salary = salary;

    personDB.push_back(new_person);
    sorted_ = false;
}

unsigned int Datastructure::size()
{
    return personDB.size();
}

void Datastructure::clear()
{
    vector<Person*>().swap(personDB);
    sorted_ = false;
}

void Datastructure::sort_names(vector<Person*>& unsortedDB, int lo, int hi){

    Person* x = unsortedDB[lo + (hi - lo) / 2];
    int i = lo;
    int j = hi;

    while(i <= j)
    {
        while((unsortedDB[i]->name) < (x->name)) i++;
        while((unsortedDB[j]->name) > (x->name)) j--;
        if(i <= j)
        {
            std::swap(unsortedDB[i], unsortedDB[j]);
            i++;
            j--;
        }
    }
    if (i<hi)
        sort_names(unsortedDB, i, hi);

    if (lo<j)
        sort_names(unsortedDB, lo, j);
}

void Datastructure::sort_salary(vector<Person*>& unsortedDB, int lo, int hi){

    Person* x = unsortedDB[lo + (hi - lo) / 2];
    int i = lo;
    int j = hi;

    while(i <= j)
    {
        while((unsortedDB[i]->salary) < (x->salary)) i++;
        while((unsortedDB[j]->salary) > (x->salary)) j--;
        if(i <= j)
        {
            std::swap(unsortedDB[i], unsortedDB[j]);
            i++;
            j--;
        }
    }
    if (i<hi)
        sort_salary(unsortedDB, i, hi);

    if (lo<j)
        sort_salary(unsortedDB, lo, j);
}

vector<Person *> Datastructure::personnel_alphabetically()
{
    sort_names(personDB, 0, personDB.size() - 1);
    sorted_ = false;
    return personDB;
}

vector<Person *> Datastructure::personnel_salary_order()
{
    if(!sorted_) {
        sort_salary(personDB, 0, personDB.size() - 1);
        sorted_ = true;
    }
    return personDB;
}

Person* Datastructure::min_salary()
{
    if(!sorted_)
        sort_salary(personDB, 0, personDB.size() - 1);
    return personDB.front();
}

Person* Datastructure::max_salary()
{
    if(!sorted_)
        sort_salary(personDB, 0, personDB.size() - 1);
    return personDB.back();
}

Person* Datastructure::median_salary()
{
    if(!sorted_)
        sort_salary(personDB, 0, personDB.size() - 1);
    return personDB[personDB.size()/2];
}

Person* Datastructure::first_quartile_salary()
{
    if(!sorted_)
        sort_salary(personDB, 0, personDB.size() - 1);
    return personDB[personDB.size()/4];
}

Person* Datastructure::third_quartile_salary()
{
    if(!sorted_)
        sort_salary(personDB, 0, personDB.size() - 1);
    return personDB[personDB.size()*3/4];
}
