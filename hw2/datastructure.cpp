// Definition of Datastructure class, hw2 of TIE-20100/TIE-20106

#include "datastructure.hpp"

// For debug
#include <iostream>
using std::cerr;
using std::endl;

#include <unordered_map>
using std::unordered_map;

#include <vector>
using std::vector;

#include <algorithm>
using std::sort;
using std::min_element;
using std::max_element;

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

void Datastructure::add_person(string name, PersonID id, string title, Salary salary)
{
    alphabet_sorted_ = false;
    salary_sorted_ = false;
    Person* new_person = new Person;
    new_person->name_ = name;
    new_person->title_ = title;
    new_person->salary_ = salary;

    companyDB.insert(make_pair(id, new_person));
}

void Datastructure::remove_person(PersonID id) // CHECK IT AGAIN!
{
    auto it = companyDB.find(id);
    if (it != companyDB.end()) {
        for (auto i : companyDB) {
            if (i.second->bossid_ == id) {
                i.second->bossid_ = it->second->bossid_;
            }
        }
        companyDB.erase (it);
    }

    for (int i = 0; i < alphabet_sorted.size(); ++i) {
        if (alphabet_sorted[i] == id) {
            alphabet_sorted.erase(alphabet_sorted.begin()+i);
        }
    }
    for (int i = 0; i < salary_sorted.size(); ++i) {
        if (salary_sorted[i] == id) {
            salary_sorted.erase(salary_sorted.begin()+i);
        }
    }
}

string Datastructure::get_name(PersonID id)
{
    return companyDB.find(id)->second->name_;
}

string Datastructure::get_title(PersonID id)
{
    return companyDB.find(id)->second->title_;
}

Salary Datastructure::get_salary(PersonID id)
{
    return companyDB.find(id)->second->salary_;
}

vector<PersonID> Datastructure::find_persons(string name)
{
    vector<PersonID> list_of_names;
    for (auto i : companyDB) {
        if(i.second->name_ == name) {
            list_of_names.push_back(i.first);
        }
    }
    sort(list_of_names.begin(),list_of_names.end());
    return list_of_names;
}

vector<PersonID> Datastructure::personnel_with_title(string title)
{
    vector<PersonID> list_of_titles;
    for (auto i : companyDB) {
        if(i.second->title_ == title) {
            list_of_titles.push_back(i.first);
        }
    }
    sort(list_of_titles.begin(), list_of_titles.end());
    return list_of_titles;
}

void Datastructure::change_name(PersonID id, string new_name)
{
    auto it = companyDB.find(id);
    if (it != companyDB.end())
        it->second->name_ = new_name;
    alphabet_sorted_ = false;
}

void Datastructure::change_salary(PersonID id, Salary new_salary)
{
    auto it = companyDB.find(id);
    if (it != companyDB.end())
        it->second->salary_ = new_salary;
    salary_sorted_ = false;
}

void Datastructure::add_boss(PersonID id, PersonID bossid)
{
    companyDB.find(id)->second->bossid_ = bossid;
}

unsigned int Datastructure::size()
{
    return companyDB.size();
}

void Datastructure::clear()
{
    if (!companyDB.empty()) companyDB.clear();
}

vector<PersonID> Datastructure::underlings(PersonID id)
{
    vector<PersonID> list;
    for (auto i : companyDB) {
        if (i.second->bossid_ == id) {
            list.push_back(i.first);
        }
    }
    sort(list.begin(), list.end());
    return list;
}

struct Compare_names {
    bool operator()(const pair<string, Person*> i_lhs, const pair<string, Person*> i_rhs) const {
        string left = i_lhs.second->name_;
        string right = i_rhs.second->name_;
        transform(left.begin(), left.end(), left.begin(), ::tolower);
        transform(right.begin(), right.end(), right.begin(), ::tolower);
        return ( left < right )?true:false;
    }
} comp_names;

void Datastructure::name_sort(unordered_map<string, Person*>& companyDB, vector<PersonID>& alphabet_sorted) {
    vector<PersonID>().swap(alphabet_sorted);
    vector<pair<string, Person*>> temp_vect(companyDB.begin(), companyDB.end());
    sort(temp_vect.begin(), temp_vect.end(), comp_names);
    auto it = temp_vect.begin();
    while (it != temp_vect.end()) {
        alphabet_sorted.push_back( it->first );
        ++it;
    }
    alphabet_sorted_ = true;
}

vector<PersonID> Datastructure::personnel_alphabetically()
{
    if (!alphabet_sorted_)
        name_sort(companyDB, alphabet_sorted);
    return alphabet_sorted;
}

struct Compare_salary {
    bool operator()(const pair<string, Person*> i_lhs, const pair<string, Person*> i_rhs) const {
        return (i_lhs.second->salary_ < i_rhs.second->salary_)?true:false;
    }
} comp_salary;

void Datastructure::salary_sort(unordered_map<string, Person*>& companyDB, vector<PersonID>& salary_sorted) {
    vector<PersonID>().swap(salary_sorted);
    vector<pair<string, Person*>> temp_vect(companyDB.begin(), companyDB.end());
    sort(temp_vect.begin(), temp_vect.end(), comp_salary);
    auto it = temp_vect.begin();
    while (it != temp_vect.end()) {
        salary_sorted.push_back( it->first );
        ++it;
    }
    salary_sorted_ = true;
}

vector<PersonID> Datastructure::personnel_salary_order()
{
    if (!salary_sorted_)
        salary_sort(companyDB, salary_sorted);
    return salary_sorted;
}

PersonID Datastructure::find_ceo()
{
    PersonID CEO;
    int counter = 0;
    for (auto i : companyDB) {
        if (i.second->bossid_ == "") {
            CEO = i.first;
            ++counter;
        }
        if (counter > 1) {
            return NO_ID;
        }
    }
    return CEO;
}


PersonID Datastructure::nearest_common_boss(PersonID id1, PersonID id2)
{
    for (auto i : companyDB)
        i.second->visited_ = false;

    auto it1 = companyDB[id1];
    auto it2 = companyDB[id2];

    if (it1->bossid_ == "" || it2->bossid_ == "")
        return NO_ID;

    //going through 1st branch and mark nodes
    while (it1->bossid_ != "") {
        it1 = companyDB[it1->bossid_];
        it1->visited_ = true;
    }

    //looking through 2nd branch for marked nodes
    while (it2->bossid_ != "") {
        auto it = companyDB.find(it2->bossid_);
        if (it != companyDB.end()){
            if ((it->second->visited_) == true) {
                return it->first;
            } else {
                //it2->visited_ = true;
                it2 = companyDB[it2->bossid_];
            }
        }
    }
    return NO_ID;
}
/*
void Datastructure::Nnodes(PersonID id_start, PersonID target, vector<int>& a, int& k, int& level) {
    std::cout << id_start << " - ID_change" << std::endl;
    if (id_start == target) level = k;
    if (companyDB.find(id_start)->second->bossid_ != "") {
        std::cout << k << " - k" << std::endl;
        k += 1;
        a[k] += 1;
        std::cout << a[k] << " - a[k]" << std::endl;
        for (auto it : companyDB) {
            if (it.second->bossid_ == id_start) {
                Nnodes(it.second->bossid_, target, a, k, level);
            }
        }
        k -= 1;
    } else {

    }
}

pair<unsigned int, unsigned int> Datastructure::higher_lower_ranks(PersonID id)
{
    vector<int> number_of_nodes;
    number_of_nodes.assign(companyDB.size(),0);
    int k = 0;
    int level = 0;
    PersonID root = find_ceo();
    Nnodes(root, id, number_of_nodes, k, level);
    std::cout << level << " - level" << std::endl;
    //CHECK AGAIN!
    return {0, 0};
}*/

pair<unsigned int, unsigned int> Datastructure::higher_lower_ranks(PersonID id)
{
    return {0, 0}; // If you do not implement this, leave this return value as it is.
}

bool mycmp (pair<string, Person*> left, pair<string,Person*> right) {
    return (left.second->salary_ < right.second->salary_)?true:false;
}

PersonID Datastructure::min_salary()
{
    /*if (!salary_sorted_)
        salary_sort(companyDB, salary_sorted);
    return salary_sorted.front();*/
    return min_element(companyDB.begin(), companyDB.end(), mycmp)->first;
}

PersonID Datastructure::max_salary()
{
    /*if (!salary_sorted_)
        salary_sort(companyDB, salary_sorted);
    return salary_sorted.back();*/
    return max_element(companyDB.begin(), companyDB.end(), mycmp)->first;
}

PersonID Datastructure::median_salary()
{
    if (!salary_sorted_)
        salary_sort(companyDB, salary_sorted);
    return salary_sorted.at(salary_sorted.size()/2);
}

PersonID Datastructure::first_quartile_salary()
{
    if (!salary_sorted_)
        salary_sort(companyDB, salary_sorted);
    return salary_sorted.at(salary_sorted.size()/4);
}

PersonID Datastructure::third_quartile_salary()
{
    if (!salary_sorted_)
        salary_sort(companyDB, salary_sorted);
    return salary_sorted.at(3*salary_sorted.size()/4);
}
