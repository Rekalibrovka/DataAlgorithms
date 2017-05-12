// Definition of Datastructure class, hw1 of TIE-20100/TIE-20106

#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

#include <string>
using std::string;

#include <vector>
using std::vector;

struct Person
{
    string name;
    int salary;
};

class Datastructure
{
public:
    Datastructure();
    ~Datastructure();

    void add_person(string name, int salary);

    unsigned int size();
    void clear();

    //custom functions
    void sort_names(vector<Person*>& unsortedDB, int lo, int hi);
    void sort_salary(vector<Person*>& unsortedDB, int lo, int hi);
    //end of custom functions

    vector<Person*> personnel_alphabetically();
    vector<Person*> personnel_salary_order();

    Person* min_salary();
    Person* max_salary();

    Person* median_salary();
    Person* first_quartile_salary();
    Person* third_quartile_salary();

private:
    // Add your implementation here
    vector<Person*> personDB;
    bool sorted_ = false;
};

#endif // DATASTRUCTURE_HPP
