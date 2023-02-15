#include "dummy_graph.h"
#include "fast_social_graph.h"
#include "utils/data_gen.h"
#include "utils/measure.h"

#include <cassert>
#include <iostream>
#include <time.h>

void run_simple_queries(SocialGraph &graph) {
  std::cout << "Benchmark getUser: "
            << measure<std::chrono::nanoseconds>::execution([&]() {
                 for (int i = 0; i < 10000; i++) {
                   graph.getUser(i);
                 }
               })
            << " ns" << std::endl;

  std::cout << "Benchmark addFriend: "
            << measure<std::chrono::nanoseconds>::execution([&]() {
                 for (int i = 0; i < 10000; i++) {
                   int u = rand() % graph.usersCount();
                   int f = rand() % graph.usersCount();

                   graph.addFriend(u, f);
                 }
               })
            << " ns" << std::endl;
}

void run_find_user(SocialGraph &graph) {

  for (int i = 0; i < 5; i++) {
    std::cout << "Benchmark findUsers no filters, no sort, limit="
              << (2 << (4 * i)) << ": "
              << measure<std::chrono::milliseconds>::execution([&]() {
                   for (int i = 0; i < 10; i++) {

                     NameFilter name;
                     name.active = false;

                     AgeFilter age;
                     age.active = false;

                     GenderFilter gender;
                     gender.active = false;

                     CityFilter city;
                     city.active = false;

                     auto response =
                         graph.findUsers(100, name, age, gender, city,
                                         SortBy_DontSort, 2 << (4 * i));

                     assert(response.userIds.size() > 0);
                   }
                 })
              << " ms" << std::endl;
  }

  for (int i = 0; i < 5; i++) {
    std::cout
        << "Benchmark findUsers with age and city filters, no sort, limit="
        << (2 << (4 * i)) << ": "
        << measure<std::chrono::milliseconds>::execution([&]() {
             for (int i = 0; i < 10; i++) {

               NameFilter name;
               name.active = false;

               AgeFilter age;
               age.active = true;
               age.start = 15;
               age.end = 45;

               GenderFilter gender;
               gender.active = false;

               CityFilter city;
               city.active = true;
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());

               auto response = graph.findUsers(100, name, age, gender, city,
                                               SortBy_DontSort, 2 << (4 * i));

               assert(response.userIds.size() > 0);
             }
           })
        << " ms" << std::endl;
  }

  for (int i = 0; i < 5; i++) {
    std::cout
        << "Benchmark findUsers with age and city filters, no sort, limit="
        << (2 << (4 * i)) << ": "
        << measure<std::chrono::milliseconds>::execution([&]() {
             for (int i = 0; i < 10; i++) {

               NameFilter name;
               name.active = false;

               AgeFilter age;
               age.active = true;
               age.start = 15;
               age.end = 45;

               GenderFilter gender;
               gender.active = false;

               CityFilter city;
               city.active = true;
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());

               auto response = graph.findUsers(100, name, age, gender, city,
                                               SortBy_DontSort, 2 << (4 * i));

               assert(response.userIds.size() > 0);
             }
           })
        << " ms" << std::endl;
  }

  for (int i = 0; i < 5; i++) {
    std::cout
        << "Benchmark findUsers with age and city filters, name sort, limit="
        << (2 << (4 * i)) << ": "
        << measure<std::chrono::milliseconds>::execution([&]() {
             for (int i = 0; i < 10; i++) {

               NameFilter name;
               name.active = false;

               AgeFilter age;
               age.active = true;
               age.start = 15;
               age.end = 45;

               GenderFilter gender;
               gender.active = false;

               CityFilter city;
               city.active = true;
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());
               city.cities.push_back(random_city());

               auto response = graph.findUsers(100, name, age, gender, city,
                                               SortBy_Name, 2 << (4 * i));

               assert(response.userIds.size() > 0);
             }
           })
        << " ms" << std::endl;
  }

  for (int i = 0; i < 5; i++) {
    std::cout << "Benchmark findUsers with age and city filters, relevance "
                 "sort, limit="
              << (2 << (4 * i)) << ": "
              << measure<std::chrono::milliseconds>::execution([&]() {
                   for (int i = 0; i < 10; i++) {

                     NameFilter name;
                     name.active = false;

                     AgeFilter age;
                     age.active = true;
                     age.start = 15;
                     age.end = 45;

                     GenderFilter gender;
                     gender.active = false;

                     CityFilter city;
                     city.active = true;
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());

                     auto response =
                         graph.findUsers(100, name, age, gender, city,
                                         SortBy_Relevance, 2 << (4 * i));

                     assert(response.userIds.size() > 0);
                   }
                 })
              << " ms" << std::endl;
  }
}

void run_high_selective_find_user(SocialGraph &graph) {

  for (int i = 0; i < 5; i++) {
    std::cout << "Benchmark findUsers high selective name, gender filter, no "
                 "sort, limit="
              << (2 << (4 * i)) << ": "
              << measure<std::chrono::milliseconds>::execution([&]() {
                   for (int i = 0; i < 10; i++) {

                     NameFilter name;
                     name.active = true;
                     name.name = random_name();

                     AgeFilter age;
                     age.active = false;

                     GenderFilter gender;
                     gender.active = true;
                     gender.gender = Male;

                     CityFilter city;
                     city.active = false;

                     auto response =
                         graph.findUsers(100, name, age, gender, city,
                                         SortBy_DontSort, 2 << (4 * i));

                     assert(response.userIds.size() > 0);
                   }
                 })
              << " ms" << std::endl;
  }

  for (int i = 0; i < 5; i++) {
    std::cout << "Benchmark findUsers high selective with age, city and gender "
                 "filters, no sort, limit="
              << (2 << (4 * i)) << ": "
              << measure<std::chrono::milliseconds>::execution([&]() {
                   for (int i = 0; i < 10; i++) {

                     NameFilter name;
                     name.active = false;

                     AgeFilter age;
                     age.active = true;
                     age.start = 15;
                     age.end = 20;

                     GenderFilter gender;
                     gender.active = true;
                     gender.gender = Female;

                     CityFilter city;
                     city.active = true;
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());

                     auto response =
                         graph.findUsers(100, name, age, gender, city,
                                         SortBy_DontSort, 2 << (4 * i));

                     assert(response.userIds.size() > 0);
                   }
                 })
              << " ms" << std::endl;
  }

  for (int i = 0; i < 5; i++) {
    std::cout << "Benchmark findUsers high selective with age and city "
                 "filters, name sort, limit="
              << (2 << (4 * i)) << ": "
              << measure<std::chrono::milliseconds>::execution([&]() {
                   for (int i = 0; i < 10; i++) {

                     NameFilter name;
                     name.active = false;

                     AgeFilter age;
                     age.active = true;
                     age.start = 15;
                     age.end = 20;

                     GenderFilter gender;
                     gender.active = false;

                     CityFilter city;
                     city.active = true;
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());

                     auto response =
                         graph.findUsers(100, name, age, gender, city,
                                         SortBy_Name, 2 << (4 * i));

                     assert(response.userIds.size() > 0);
                   }
                 })
              << " ms" << std::endl;
  }

  for (int i = 0; i < 5; i++) {
    std::cout << "Benchmark findUsers high selective with age and city "
                 "filters, relevance sort, limit="
              << (2 << (4 * i)) << ": "
              << measure<std::chrono::milliseconds>::execution([&]() {
                   for (int i = 0; i < 10; i++) {

                     NameFilter name;
                     name.active = false;

                     AgeFilter age;
                     age.active = true;
                     age.start = 15;
                     age.end = 20;

                     GenderFilter gender;
                     gender.active = false;

                     CityFilter city;
                     city.active = true;
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());
                     city.cities.push_back(random_city());

                     auto response =
                         graph.findUsers(100, name, age, gender, city,
                                         SortBy_Relevance, 2 << (4 * i));

                     assert(response.userIds.size() > 0);
                   }
                 })
              << " ms" << std::endl;
  }
}

void benchmark_graph(SocialGraph &graph) {
  fill_graph(graph, 100000, 10000000);

  std::cout << "Begin Benchmarks...\n\n";

  run_simple_queries(graph);
  run_find_user(graph);
  run_high_selective_find_user(graph);
}

int main() {
  double firstTime=clock()/(double)CLOCKS_PER_SEC;
  {
    std::cout << "Benchmarking DummySocialGraph ..." << std::endl;
    DummySocialGraph graph;
    benchmark_graph(graph);
  }
  double secondTime=clock()/(double)CLOCKS_PER_SEC;
  {
     std::cout << "Benchmarking FastSocialGraph ..." << std::endl;
     FastSocialGraph graph;
     benchmark_graph(graph);
  }
  double thirdTime=clock()/(double)CLOCKS_PER_SEC;

  std::cout << "1st alg spent "<< (secondTime-firstTime)<< "sec"<< std::endl;
  std::cout << "2nd alg spent "<< (thirdTime-secondTime)<< "sec"<< std::endl;

}