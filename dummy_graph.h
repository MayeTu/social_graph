#pragma once
#include "social_graph.h"

struct DummySocialGraphUser {
    std::string name;
    int age;
    Gender gender;
    std::string city;
    std::vector<int> friends;
};

struct DummySocialGraph : SocialGraph {
    int addUser(const std::string &name, int age, Gender gender, const std::string &city) override {
        DummySocialGraphUser user;
        user.name = name;
        user.age = age;
        user.gender = gender;
        user.city = city;
        user.friends = {};
        users.push_back(user);
        return static_cast<int>(users.size()) - 1;
    }

    void addFriend(int userId, int friendId) override {
        if(std::find(users[userId].friends.begin(), users[userId].friends.end(), friendId) != users[userId].friends.end()) {
            return;
        }

        if(userId == friendId) {
            return;
        }

        users[userId].friends.push_back(friendId);
        users[friendId].friends.push_back(userId);
    }

    GetUserResponse getUser(int id) override {
        DummySocialGraphUser user = users[id];
        GetUserResponse response;
        response.name = user.name;
        response.age = user.age;
        response.city = user.city;
        response.gender = user.gender;
        response.friends = user.friends;
        return response;
    }

    FindUsersResponse findUsers(
        int userId, // current user id
        const NameFilter &nameFilter,
        const AgeFilter &ageFilter,
        const GenderFilter &genderFilter,
        const CityFilter &cityFilter,
        SortBy sortBy,
        int limit) override {

        std::vector<int> filteredUsers;

        for(int i=0; i<users.size(); i++) {
            DummySocialGraphUser user = users[i];

            if(nameFilter.active && user.name != nameFilter.name) {
                continue;
            }

            if(ageFilter.active && (user.age < ageFilter.start || user.age > ageFilter.end)) {
                continue;
            }

            if(genderFilter.active && (user.gender != genderFilter.gender)) {
                continue;
            }

            if(cityFilter.active) {
                if(std::find(cityFilter.cities.begin(), cityFilter.cities.end(), user.city) == cityFilter.cities.end()) {
                    continue;
                }
            }

            filteredUsers.push_back(i);
        }

        if(sortBy != SortBy_DontSort) {
            std::sort(filteredUsers.begin(), filteredUsers.end(), [this, userId, sortBy](auto u1, auto u2) {
                auto user1 = users[u1];
                auto user2 = users[u2];

                if(sortBy == SortBy_Age) {
                    return user1.age < user2.age;
                }

                if(sortBy == SortBy_Name) {
                    return user1.name < user2.name;
                }

                if(sortBy == SortBy_Relevance) {
                    return countRelevance(userId, user1) > countRelevance(userId, user2);
                }

                return false;
            });
        }

        filteredUsers.resize(limit);

        FindUsersResponse response;
        response.userIds = filteredUsers;

        return response; 
    }

    int usersCount() const override {
        return static_cast<int>(users.size());
    }

    int countRelevance(int userId, const DummySocialGraphUser &user) {
        int relevance = 0;
        for(int f: user.friends) {
            for(int uf: users[userId].friends) {
                if(f == uf) {
                    relevance++;
                }
            }
        }
        return relevance / 2;
    }

    std::vector<DummySocialGraphUser> users;
};