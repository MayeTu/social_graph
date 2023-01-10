#pragma once
#include "city_list.h"
#include "names_list.h"
#include "social_graph.h"

#include <random>
#include <iostream>

const char *random_city() {
    return g_city_list[rand() % (sizeof(g_city_list) / sizeof(char *))];
}

const char *random_name() {
    return g_names_list[rand() % (sizeof(g_names_list) / sizeof(char *))];
}

const int random_age() {
    return rand() % 60 + 16;
}

const Gender random_gender() {
    return (Gender)(rand() % 2);
}

void fill_graph(SocialGraph &graph, int users_count, int edges_count) {
    srand(81232);
    std::vector<int> ids;

    std::cout << "Generating User Accounts..." << std::endl;
    for(int i=0; i<users_count; i++) {
        auto name = random_name();
        auto city = random_city();
        auto age = random_age();
        auto gender = random_gender();

        int id = graph.addUser(name, age, gender, city);
        ids.push_back(id);
    }

    std::cout << "Generating User Relations..." << std::endl;

    for(int i=0; i<edges_count; i++) {
        if(i % (edges_count / 100) == 0) {
            std::cout << i * 100 / edges_count << "%" << std::endl;
        }
        graph.addFriend(ids[rand() % ids.size()], ids[rand() % ids.size()]);
    }

    std::cout << "Graph generation finished" << std::endl;
}