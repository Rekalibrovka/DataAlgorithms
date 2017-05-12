// Definition of Datastructure class, hw3 of TIE-20100/TIE-20106

#include "datastructure.hpp"

// For debug
#include <iostream>
using std::cerr;
using std::endl;

#include <unordered_map>
using std::unordered_map;

#include <vector>
using std::vector;

#include <set>
using std::set;

#include <utility>
using std::pair;

#include <algorithm>
using std::sort;
using std::min_element;
using std::max_element;
using std::nth_element;

#include <queue>
using std::queue;
using std::priority_queue;
using std::greater;

#include <climits>
int INF = INT_MAX;

#include <random>
using std::minstd_rand;
using std::uniform_int_distribution;

minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

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

void Datastructure::remove_person(PersonID id)
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
    if (!edges.empty()) edges.clear();
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
        i.second->color_ = 0;

    auto it1 = companyDB[id1];
    auto it2 = companyDB[id2];

    if (it1->bossid_ == "" || it2->bossid_ == "")
        return NO_ID;

    //going through 1st branch and mark nodes
    while (it1->bossid_ != "") {
        it1 = companyDB[it1->bossid_];
        it1->color_ = 1;
    }

    //looking through 2nd branch for marked nodes
    while (it2->bossid_ != "") {
        auto it = companyDB.find(it2->bossid_);
        if (it != companyDB.end()){
            if ((it->second->color_) == 1) {
                return it->first;
            } else {
                it2 = companyDB[it2->bossid_];
            }
        }
    }
    return NO_ID;
}

pair<unsigned int, unsigned int> Datastructure::higher_lower_ranks(PersonID id)
{
    return {0,0}; // Replace this with the actual implementation, if any
}

bool mycmp (pair<string, Person*> left, pair<string,Person*> right) {
    return (left.second->salary_ < right.second->salary_)?true:false;
}

PersonID Datastructure::min_salary()
{
    return min_element(companyDB.begin(), companyDB.end(), mycmp)->first;
}

PersonID Datastructure::max_salary()
{
    return max_element(companyDB.begin(), companyDB.end(), mycmp)->first;
}

PersonID Datastructure::median_salary()
{
    vector<pair<string, Person*>> tmp_array(begin(companyDB),end(companyDB));
    nth_element(tmp_array.begin(), tmp_array.begin() + tmp_array.size()/2, tmp_array.end(), mycmp);
    PersonID target = (tmp_array[tmp_array.size()/2]).first;
    return target;
}

PersonID Datastructure::first_quartile_salary()
{
    vector<pair<string, Person*>> tmp_array(begin(companyDB),end(companyDB));
    nth_element(tmp_array.begin(), tmp_array.begin() + tmp_array.size()/4, tmp_array.end(), mycmp);
    PersonID target = (tmp_array[tmp_array.size()/2]).first;
    return target;
}

PersonID Datastructure::third_quartile_salary()
{
    vector<pair<string, Person*>> tmp_array(begin(companyDB),end(companyDB));
    nth_element(tmp_array.begin(), tmp_array.begin() + 3*tmp_array.size()/4, tmp_array.end(), mycmp);
    PersonID target = (tmp_array[tmp_array.size()/2]).first;
    return target;
}

// After this are the new operations of hw3
// NOTE!!! Add all your new hw3 code here below these comments (to keep it
// separated from old hw2 code).
void Datastructure::add_friendship(PersonID id, PersonID friendid, Cost cost)
{
    auto it1 = companyDB.find(id);
    if (it1 != companyDB.end()) {
        it1->second->friends.insert(make_pair(friendid, cost));
        edges.insert(make_pair(cost, make_pair(id, friendid)));
    }
    auto it2 = companyDB.find(friendid);
    if (it2 != companyDB.end()) {
        it2->second->friends.insert(make_pair(id, cost));
    }
}

void Datastructure::remove_friendship(PersonID id, PersonID friendid)
{
    auto it1 = companyDB.find(id);
    if (it1 != companyDB.end()) {
        auto it2 = (it1->second->friends).find(friendid);
        if (it2 != (it1->second->friends).end()) {
            (it1->second->friends).erase(it2);
            for (auto i : edges) {
                if (i.second.first == id && i.second.second == friendid) {
                    edges.erase(i);
                }
            }
        }
    }
    auto it11 = companyDB.find(friendid);
    if (it11 != companyDB.end()) {
        auto it22 = (it11->second->friends).find(id);
        if (it22 != (it11->second->friends).end()) {
            (it11->second->friends).erase(it22);
        }
    }
}

vector<pair<PersonID, Cost>> Datastructure::get_friends(PersonID id)
{
    vector <pair<PersonID, Cost>> tmp;
    auto it = companyDB.find(id);
    if (it != companyDB.end()) {
        for (auto k : it->second->friends)
            tmp.push_back(k);
    }
    return tmp;
}

vector<pair<PersonID, PersonID>> Datastructure::all_friendships()
{
    vector<pair<PersonID, PersonID>> result;
    /*set<pair<PersonID, PersonID>> tmp_set;
    for (auto ii : companyDB) {
        for (auto jj : ii.second->friends) {
            if(tmp_set.find(make_pair(ii.first, jj.first)) == tmp_set.end() && tmp_set.find(make_pair(jj.first, ii.first)) == tmp_set.end()) {
                if ((ii.first).compare(jj.first) < 0) {
                    tmp_set.insert(make_pair(ii.first, jj.first));
                } else {
                    tmp_set.insert(make_pair(jj.first, ii.first));
                }
            }
        }
    }


    for (auto it : tmp_set) {
        tmp_vec.push_back(it);
    }*/
    for (auto it : edges) {
        if ((it.second.first).compare(it.second.second) < 0) {
            result.push_back(make_pair(it.second.first, it.second.second));
        } else {
            result.push_back(make_pair(it.second.second, it.second.first));
        }
    }
    sort(result.begin(),result.end());
    return result;
}

//BFS O(|V|+|E|)
//Dijkstra Algorithm O(E+ V log V) with weight 1 edges
vector<pair<PersonID, Cost> > Datastructure::shortest_friendpath(PersonID fromid, PersonID toid)
{
    auto check1 = companyDB.find(fromid);
    auto check2 = companyDB.find(toid);
    if (check1 == companyDB.end() || check2 == companyDB.end()) {
        return {};
    }
    vector<pair<PersonID, Cost>> result;
    priority_queue< pair<Cost, PersonID>, vector <pair<Cost, PersonID>> , greater<pair<Cost, PersonID>> > pq;
    unordered_map<PersonID, Cost> dist;
    unordered_map<PersonID, PersonID> path;
    Cost weight = 1;

    for (auto iter : companyDB) {
        dist.insert(make_pair(iter.first, INF));
    }

    pq.push(make_pair(0, fromid));
    dist[fromid] = 0;
    path.insert(make_pair(fromid,"-1"));

    while (!pq.empty()) {
        PersonID u = pq.top().second;
        pq.pop();

        auto it = companyDB.find(u);
        for (auto i : it->second->friends) {
            PersonID id = (i).first;

            //  If there is shorted path to id through u.
            if (dist[id] > dist[u] + weight) {
                // Updating distance of id
                dist[id] = dist[u] + weight;
                path[id] = u;
                pq.push(make_pair(dist[id], id));
            }
        }
    }
    if (dist[toid] != INF) {
        PersonID curr = toid;
        while (curr != fromid) {
            auto it1 = companyDB.find(path[curr]);
            auto it2 = it1->second->friends.find(curr);
            if (it2 != it1->second->friends.end()) {
                result.push_back(*it2);
                //std::cout<< it2->first << ":" << it2->second << "->";
            }
            curr = path[curr];
        }
        //std::cout<<std::endl;
        reverse(result.begin(), result.end());
        return result;
    } else {
        return {};
    }
    return {};
}

bool Datastructure::isCyclic (PersonID node) {
    auto curr = companyDB.find(node);
    if (curr != companyDB.end()) {
        curr->second->color_ = 1;
        PersonID to = curr->second->bossid_;
        auto to_it = companyDB.find(to);
        if (to_it != companyDB.end()) {
            if (to_it->second->color_ == 0) {
                if (isCyclic(to_it->first))
                    return true;
            } else if (to_it->second->color_ == 1) {
                return true;
            }
        }
        curr->second->color_ = 2;
        return false;
    }
}

bool Datastructure::isHierarchical () {
    int number_of_big_bosses = 0;
    for (auto it1 : companyDB) {
        if (it1.second->bossid_ == "") {
            number_of_big_bosses++;
            if (number_of_big_bosses > 1) {
                return false;
                break;
            }
            for (auto it2 : companyDB) {
                if (it1.first == it2.second->bossid_) {
                    return true;
                    break;
                }
            }
        }
    }
    return false;
}


bool Datastructure::check_boss_hierarchy()
{
    for (auto i : companyDB)
        i.second->color_ = 0;
    isCyclic_ = false;

    for (auto it : companyDB) {
        if (it.second->color_ == 0) {
            if (isCyclic(it.first)) {
                isCyclic_ = true;
                break;
            }
        }
    }
    if (!isCyclic_ && isHierarchical())
        return true;
    else
        return false;

}

//Dijkstra Algorithm O(E+ V log V) with priority queue
vector<pair<PersonID, Cost>> Datastructure::cheapest_friendpath(PersonID fromid, PersonID toid)
{
    auto check1 = companyDB.find(fromid);
    auto check2 = companyDB.find(toid);
    if (check1 == companyDB.end() || check2 == companyDB.end()) {
        return {};
    }
    vector<pair<PersonID, Cost>> result;
    priority_queue< pair<Cost, PersonID>, vector <pair<Cost, PersonID>> , greater<pair<Cost, PersonID>> > pq;
    unordered_map<PersonID, Cost> dist;
    unordered_map<PersonID, PersonID> path;

    for (auto iter : companyDB) {
        dist.insert(make_pair(iter.first, INF));
    }

    pq.push(make_pair(0, fromid));
    dist[fromid] = 0;
    path.insert(make_pair(fromid,"-1"));

    while (!pq.empty()) {
        PersonID u = pq.top().second;
        pq.pop();

        auto it = companyDB.find(u);
        for (auto i : it->second->friends) {
            PersonID id = (i).first;
            Cost weight = (i).second;

            //  If there is shorted path to id through u.
            if (dist[id] > dist[u] + weight) {
                // Updating distance of id
                dist[id] = dist[u] + weight;
                path[id] = u;
                pq.push(make_pair(dist[id], id));
            }
        }
    }
    if (dist[toid] != INF) {
        PersonID curr = toid;
        while (curr != fromid) {
            auto it1 = companyDB.find(path[curr]);
            auto it2 = it1->second->friends.find(curr);
            if (it2 != it1->second->friends.end()) {
                result.push_back(*it2);
                //std::cout<< it2->first << ":" << it2->second << "->";
            }
            curr = path[curr];
        }
        //std::cout<<std::endl;
        reverse(result.begin(), result.end());
        return result;
    } else {
        return {};
    }
    return {};
}

void Datastructure::DisjointSets(unordered_map<PersonID, PersonID>& parent, unordered_map<PersonID, int>& rnk )
{
    // Initially, all vertices are in
    // different sets and have rank 0.
    for (auto i : companyDB)
    {
        rnk[i.first] = 0;

        //every element is parent of itself
        parent[i.first] = i.first;
    }
}

// Find the parent of a node 'u'
// Path Compression
PersonID Datastructure::find(PersonID u, unordered_map<PersonID, PersonID>& parent)
{
    /* Make the parent of the nodes in the path
       from u--> parent[u] point to parent[u] */
    if (u != parent[u])
        parent[u] = find(parent[u], parent);
    return parent[u];
}

// Union by rank
void Datastructure::merge(PersonID x, PersonID y, unordered_map<PersonID, PersonID>& parent, unordered_map<PersonID, int>& rnk)
{
    x = find(x, parent), y = find(y, parent);

    /* Make tree with smaller height
       a subtree of the other tree  */
    if (rnk[x] > rnk[y])
        parent[y] = x;
    else // If rnk[x] <= rnk[y]
        parent[x] = y;

    if (rnk[x] == rnk[y])
        rnk[y]++;
}

//Krushkal's Algorithm O(E log V) + removal = O(N^2)
pair<unsigned int, Cost> Datastructure::leave_cheapest_friendforest()
{
    vector<pair<PersonID, PersonID>> forest;
    int count = companyDB.size();
    unordered_map<PersonID, PersonID> parent;
    unordered_map<PersonID, int> rnk;
    Cost weight = 0;

    DisjointSets(parent,rnk);

    for (auto it : edges) {
        PersonID u = it.second.first;
        PersonID v = it.second.second;

        PersonID set_u = find(u,parent);
        PersonID set_v = find(v,parent);

        if(set_u != set_v) {
            // save edge (u,v)
            forest.push_back(make_pair(u,v));
            count--;
            weight += it.first;
            merge(set_u,set_v,parent,rnk);
        }
    }
    for (auto rem : edges) {
        bool found = false;
        for (auto save : forest) {
            if (rem.second.first == save.first && rem.second.second == save.second) {
                found = true;
                break;
            }
        }
        if (!found) {
            remove_friendship(rem.second.first, rem.second.second);
        }
    }

    return {count, weight};
}
