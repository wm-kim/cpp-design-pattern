// A component which is instantiated once#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <boost/lexical_cast.hpp>
#include <vector>

// for making dummyDatabase
class Database
{
public:
    virtual int get_population(const std::string& name) = 0;
};

class SingletonDatabase : public Database
{
    SingletonDatabase()
    {
        std::cout << "Initializing database" << std::endl;

        std::ifstream ifs("capitals.txt");

        std::string s, s2;
        while (getline(ifs, s))
        {
            getline(ifs, s2);
            int pop = boost::lexical_cast<int>(s2);
            capitals[s] = pop;
        }
        //instance_count++;
    }

    std::map<std::string, int> capitals;

public:
    //static int instance_count;

    SingletonDatabase(SingletonDatabase const&) = delete; // copy constructor
    void operator=(SingletonDatabase const&) = delete; // copy assignment operator

    static SingletonDatabase& get()
    {
        static SingletonDatabase db; // construct, this is how to make a singleton object
        return db;
    }

    int get_population(const std::string& name) override
    {
        return capitals[name];
    }

    // another way to make singleton
    /*
    static SingletonDatabase* get_instance()
    {
      if (!instance)
        instance = new SingletonDatabase;
      return instance; // atexit
    }
    */
};

//int SingletonDatabase::instance_count = 0;

// 테스트를 위한 가짜 
class DummyDatabase : public Database
{
    std::map<std::string, int> capitals;
public:
    DummyDatabase()
    {
        capitals["alpha"] = 1;
        capitals["beta"] = 2;
        capitals["gamma"] = 3;
    }

    int get_population(const std::string& name) override {
        return capitals[name];
    }
};

// ============ problems with singleton ==================

// want to test this
struct SingletonRecordFinder
{
    int total_population(std::vector<std::string> names)
    {
        int result = 0;
        for (auto& name : names)
            result += SingletonDatabase::get().get_population(name); // dependency, strongly tied to real datatase
        // it is itegration test, did not want to test the operation of the real database, 
        // but due to the strong coupling, it is hard to test
        // can't get proper unit test -> get dummy data of our own
        // use little bit of dependency injection -> don't imply have to use spcecial heavy weight library
        // just means that you have a point which you can insert the dependency instead of actual object
        return result;
    }
};

TEST(RecordFinderTests, SingletonTotalPopulationTest) // Google C++ Testing Framework
{
    SingletonRecordFinder rf;
    vector<string> names{"Seoul", "Mexico City"};
    int tp = rf.total_population(names);
    EXPECT_EQ(175000000 + 174000000, tp);

}

TEST(RecordFinderTests, DependantTotalPopulationTest) // Google C++ Testing Framework
{
    DummyDatabase db;
    // ...
}

struct ConfigurableRecordFinder
{
    Database& db;

    explicit ConfigurableRecordFinder(Database& db)
        : db{ db }
    {
    ｝

    int total_population(std::vector<std::string> names) const
    {
        int result = 0;
        for (auto& name : names)
            result += db.get_population(name);
        return result;
    }
};

int main(int ac, char* av[])
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TEST();
}

// what is dependency injection 
// https://medium.com/@jang.wangsu/di-dependency-injection-%EC%9D%B4%EB%9E%80-1b12fdefec4f