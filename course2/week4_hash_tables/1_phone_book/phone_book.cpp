#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <functional>
#include <math.h>

using std::cin;
using std::string;
using std::vector;

struct Query
{
    string type, name;
    int number;
};

vector<Query> read_queries()
{
    // std::fstream cin("./test/04");

    int n;
    cin >> n;
    vector<Query> queries(n);
    for (int i = 0; i < n; ++i)
    {
        cin >> queries[i].type;
        if (queries[i].type == "add")
            cin >> queries[i].number >> queries[i].name;
        else
            cin >> queries[i].number;
    }
    return queries;
}

void write_responses(const vector<string> &result)
{
    for (size_t i = 0; i < result.size(); ++i)
        std::cout << result[i] << "\n";
}

vector<string> process_queries_naive(const vector<Query> &queries)
{
    vector<string> result;
    // Keep list of all existing (i.e. not deleted yet) contacts.
    vector<Query> contacts;
    for (size_t i = 0; i < queries.size(); ++i)
        if (queries[i].type == "add")
        {
            bool was_founded = false;
            // if we already have contact with such number,
            // we should rewrite contact's name
            for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number)
                {
                    contacts[j].name = queries[i].name;
                    was_founded = true;
                    break;
                }
            // otherwise, just add it
            if (!was_founded)
                contacts.push_back(queries[i]);
        }
        else if (queries[i].type == "del")
        {
            for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number)
                {
                    contacts.erase(contacts.begin() + j);
                    break;
                }
        }
        else
        {
            string response = "not found";
            for (size_t j = 0; j < contacts.size(); ++j)
                if (contacts[j].number == queries[i].number)
                {
                    response = contacts[j].name;
                    break;
                }
            result.push_back(response);
        }
    return result;
}

long long hash_number(int x)
{
    // hashing formula
    // ((ax + b) mod p) mod m
    long long result;
    long long L = pow(10, 7); // maximum length of data to hash, in this question it is 7
    long long p = L + 31;     // prime number bigger than 10 ^ L
    long long m = 10000000;      // cardinality/size of hash table
    long long a = 34;         // random number 1 < a < p - 1
    long long b = 31;          // random number 0 < b < p - 1

    result = (((a * x) + b) % p) % m;
    // std::cout << "hash of " << x << " = " << result << "\n";
    return result;
}

long long hash_string(string S)
{
    // hashing formula
    // sum from i = 0 to S.length() - 1
    // S[i] * (x ^ i) mod p
    long long result = 0;
    long long L = pow(10, 15); // maximum length of data to hash, in this question it is 15
    long long p = L + 19;      // prime number bigger than 10 ^ L
    long long x = 31;          // random number 1 < x < p - 1

    for (int i = S.length() - 1; i >= 0; i--)
    {
        result = (result * x + S[i]) % p;
    }

    return result;
}

long long hash_string_hash(long long x)
{
    // hashing formula
    // ((ax + b) mod p) mod m
    long long result;
    long long L = pow(10, 15); // maximum length of data to hash, in this question it is 15
    long long p = L + 19;      // prime number bigger than 10 ^ L
    long long m = 10000000;       // cardinality/size of hash table
    long long a = 34;          // random number 1 < a < p - 1
    long long b = 2;           // random number 0 < b < p - 1

    result = (((a * x) + b) % p) % m;

    return result;
}

vector<string> process_queries(const vector<Query> &queries)
{
    vector<string> result;

    int m = 10000000; // cardinality/size of hash table

    vector<string> name(m);
    vector<int> number(m);

    for (int i = 0; i < queries.size(); i++)
    {
        Query q = queries[i];
        int hashedNumber = hash_number(queries[i].number);

        long long hashedName = hash_string(queries[i].name);
        int shortenedHashName = hash_string_hash(hashedName);

        if (queries[i].type == "add")
        {
            name[hashedNumber] = queries[i].name;
            number[shortenedHashName] = queries[i].number;
        }
        else if (queries[i].type == "del")
        {
            string nameToErase = name[hashedNumber];

            name[hashedNumber] = "";

            long long hashedName = hash_string(nameToErase);
            int shortenedHashName = hash_string_hash(hashedName);
            number[shortenedHashName] = 0;
        }
        else
        {

            string response = "not found";
            if (name[hashedNumber].length() > 0)
            {
                response = name[hashedNumber];
            }
            result.push_back(response);
        }
    }

    return result;
}

string printstring(int n)
{
    char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                        'r', 's', 't', 'u', 'v', 'w', 'x',
                        'y', 'z'};
    string ran = "";
    for (int i = 0; i < n; i++)
        ran = ran + letters[rand() % 26];
    return ran;
}

void test_solution()
{
    while (true)
    {
        int randomQuerySize = rand() % 50 + 5;

        vector<string> types = {"add", "del", "find"};
        vector<string> randomNames;
        vector<int> randomPhoneNums;

        for (int i = 0; i <= randomQuerySize; i++)
        {
            string name = printstring(15);
            randomNames.push_back(name);

            int phoneNumber = rand() % 10000 + 10000000;
            randomPhoneNums.push_back(phoneNumber);
        }

        vector<Query> queries1;
        vector<Query> queries2;
        for (int i = 0; i < randomQuerySize; i++)
        {
            Query q = Query();
            q.type = types[rand() % 3];
            if (q.type == "add")
            {
                q.name = randomNames.back();
                randomNames.pop_back();
            }
            q.number = randomPhoneNums.back();
            randomPhoneNums.pop_back();

            queries1.push_back(q);
            queries2.push_back(q);
        }

        vector<string> res1 = process_queries_naive(queries1);
        vector<string> res2 = process_queries(queries2);

        if (res1 == res2)
        {
            std::cout << "\n====================================\n";
            std::cout << "OK!\n";

            // for(int i = 0; i < res1.size(); i++) {
            //     std::cout << res1[i] << ", ";
            // }
            // std::cout << "\nvs\n";

            // for(int i = 0; i < res2.size(); i++) {
            //     std::cout << res2[i] << ", ";
            // }
        }
        else
        {
            std::cout << "\n====================================\n";
            std::cout << "Fail! \n";

            std::cout << "\n!!!!!!!! Query !!!!!!!\n";
            std::cout << queries1.size() << "\n";
            for (int i = 0; i < queries1.size(); i++)
            {
                Query q = queries1[i];

                if (q.type == "add")
                {
                    std::cout << q.type << " " << q.number << " " << q.name << "\n";
                }
                else
                {
                    std::cout << q.type << " " << q.number << "\n";
                }
            }

            for (int i = 0; i < res1.size(); i++)
            {
                std::cout << res1[i] << ", ";
            }
            std::cout << "\nvs\n";

            for (int i = 0; i < res2.size(); i++)
            {
                std::cout << res2[i] << ", ";
            }

            break;
        }
    }
}

int main()
{
    // test_solution();
    write_responses(process_queries(read_queries()));
    return 0;
}
