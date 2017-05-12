// Definition of Datastructure class, hw2 of TIE-20100/TIE-20106

#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <utility>
using std::pair;

#include <unordered_map>
using std::unordered_map;

using PersonID = string;
PersonID const NO_ID = "";

using Salary = int;
Salary const NO_SALARY = -1;

struct Person
{
    string name_;
    string title_;
    Salary salary_;

    PersonID bossid_ = "";
    bool visited_ = false;
};

class Datastructure
{
public:
    Datastructure();
    ~Datastructure();

    void add_person(string name, PersonID id, string title, Salary salary);
    void remove_person(PersonID id);

    string get_name(PersonID id);
    string get_title(PersonID id);
    Salary get_salary(PersonID id);
    vector<PersonID> find_persons(string name);
    vector<PersonID> personnel_with_title(string title);

    void change_name(PersonID id, string new_name);
    void change_salary(PersonID id, Salary new_salary);

    void add_boss(PersonID id, PersonID bossid);

    unsigned int size();
    void clear();

    vector<PersonID> underlings(PersonID id);
    vector<PersonID> personnel_alphabetically();
    vector<PersonID> personnel_salary_order();

    void name_sort(unordered_map<string, Person*>& companyDB, vector<PersonID>& alphabet_sorted);
    void salary_sort(unordered_map<string, Person*>& companyDB, vector<PersonID>& salary_sorted);

    PersonID find_ceo();
    PersonID nearest_common_boss(PersonID id1, PersonID id2);

    //void Nnodes(PersonID id_start, PersonID target, vector<int>& a, int& k, int& level);
    pair<unsigned int, unsigned int> higher_lower_ranks(PersonID id);

    PersonID min_salary();
    PersonID max_salary();

    PersonID median_salary();
    PersonID first_quartile_salary();
    PersonID third_quartile_salary();

private:
    // Add your implementation here
    unordered_map<PersonID, Person*> companyDB;

    vector<PersonID> alphabet_sorted;
    bool alphabet_sorted_ = false;
    vector<PersonID> salary_sorted;
    bool salary_sorted_ = false;
};

#endif // DATASTRUCTURE_HPP
