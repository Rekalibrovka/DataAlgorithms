// Definition of Datastructure class, hw3 of TIE-20100/TIE-20106

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

#include <map>
using std::map;

#include <set>
using std::multiset;

// #include <queue>
// using std::priority_queue;
// using std::greater;

using PersonID = string;

PersonID const NO_ID = "";

using Salary = int;
Salary const NO_SALARY = -1;

using Cost = int;
Cost const NO_COST = -1;

struct Person
{
    string name_;
    string title_;
    Salary salary_;

    PersonID bossid_ = ""; //should be pointer to Person instead. Did not have time to fix
    int color_ = 0;
    map <PersonID, int> friends;
};

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        //auto h1 = std::hash<T1>{}(p.first);
        //auto h2 = std::hash<T2>{}(p.second);

        // Better to use other hash function
        //return h1 ^ h2;
        return ((std::hash<string>()(p.first)
                     ^ (std::hash<string>()(p.second) << 1)) >> 1)
                     ^ (std::hash<int>()(25) << 1);
    }
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
    pair<unsigned int, unsigned int> higher_lower_ranks(PersonID id);

    PersonID min_salary();
    PersonID max_salary();

    PersonID median_salary();
    PersonID first_quartile_salary();
    PersonID third_quartile_salary();

    // After this are the new operations of hw3
    void add_friendship(PersonID id, PersonID friendid, Cost cost);
    void remove_friendship(PersonID id, PersonID friendid);
    vector<pair<PersonID, Cost>> get_friends(PersonID id);
    vector<pair<PersonID, PersonID>> all_friendships();

    bool isCyclic (PersonID node);
    bool isHierarchical ();
    bool check_boss_hierarchy();

    vector<pair<PersonID, Cost>> shortest_friendpath(PersonID fromid, PersonID toid);
    vector<pair<PersonID, Cost>> cheapest_friendpath(PersonID fromid, PersonID toid);

    void DisjointSets(unordered_map<PersonID, PersonID>& parent, unordered_map<PersonID, int>& rnk );
    void merge(PersonID x, PersonID y, unordered_map<PersonID, PersonID>& parent, unordered_map<PersonID, int>& rnk);
    PersonID find(PersonID u, unordered_map<PersonID, PersonID>& parent);
    pair<unsigned int, Cost> leave_cheapest_friendforest();

private:
    // Add your implementation here
    unordered_map<PersonID, Person*> companyDB;
    vector<PersonID> alphabet_sorted;
    bool alphabet_sorted_ = false;
    vector<PersonID> salary_sorted;
    bool salary_sorted_ = false;

    //HW3 part
    bool isCyclic_ = false;
    struct Compare_priority {
        bool operator()(const pair<Cost, pair<PersonID, PersonID>> i_lhs, const pair<Cost, pair<PersonID, PersonID>> i_rhs) const {
            return (i_lhs.first < i_rhs.first)?true:false;
        }
    };
    multiset<pair<Cost, pair<PersonID, PersonID>>, Compare_priority> edges;
};

#endif // DATASTRUCTURE_HPP
