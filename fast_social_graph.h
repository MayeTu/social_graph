#pragma once

#pragma once
#include "social_graph.h"
#include <unordered_map>
#include <unordered_set>

struct FastSocialGraphUser{
    std::string name;
    int age;
    Gender gender;
    std::string city;
    std::vector<int> friends;//std::unordered_set<int> friends;
};

struct FastSocialGraph : SocialGraph {  
  int addUser(const std::string &name, int age, Gender gender, const std::string &city) override {
    users.insert({users.size(),FastSocialGraphUser{name,age,gender,city,{}}});
    //relations.push_back({});
    return static_cast<int>(users.size()) - 1;
  }

  void addFriend(int userId, int friendId) override {
    if(userId == friendId){
        return;
    }
    /*if (relations[userId].find(friendId)!=relations[userId].end()){
      return;
    }*/
    if(std::find(users[userId].friends.begin(), users[userId].friends.end(), friendId) != users[userId].friends.end()) {
        return;
    }

    //relations[userId].insert(friendId);
    //relations[friendId].insert(userId);
    users[userId].friends.push_back(friendId);
    users[friendId].friends.push_back(userId);
  }

  GetUserResponse getUser(int id) override { 
    GetUserResponse response;
    FastSocialGraphUser& user=users[id];
    response.name=user.name;
    response.age = user.age;
    response.city = user.city;
    response.gender = user.gender;
    response.friends=user.friends;
    return response; 
  }

  FindUsersResponse findUsers(int userId, // current user id
                              const NameFilter &nameFilter,
                              const AgeFilter &ageFilter,
                              const GenderFilter &genderFilter,
                              const CityFilter &cityFilter, SortBy sortBy,
                              int limit) override {
    if (!nameFilter.active && !ageFilter.active && !genderFilter.active && !cityFilter.active){
      std::vector <int> filteredUsers;
      for (int i=0; i<users.size(); i++){
        if (i>=limit) break;
        filteredUsers.push_back(i);
      }
      FindUsersResponse response;
      response.userIds=filteredUsers;
      return response;
    }

    std::vector<int> filteredUsers;
    std::unordered_set<std::string> citiesSet;
    if(cityFilter.active) {
      for (uint i=0; i<cityFilter.cities.size(); i++){
        citiesSet.insert(cityFilter.cities[i]);
      }
    }
    for (uint i=0; i<users.size(); i++){
      FastSocialGraphUser& user=users[i];

      if (sortBy == SortBy_DontSort && i==limit) break;

      if(nameFilter.active && user.name != nameFilter.name) {
          continue;
      }
      if(ageFilter.active && (user.age < ageFilter.start || user.age > ageFilter.end)) {
          continue;
      }
      if(genderFilter.active && (user.gender != genderFilter.gender)) {
          continue;
      }
      if(cityFilter.active && citiesSet.find(user.city)==citiesSet.end()){
          continue;
      }

      filteredUsers.push_back(i);
    }

    if (sortBy != SortBy_DontSort){
      std::sort(filteredUsers.begin(),filteredUsers.end(), [this, userId, sortBy](auto u1, auto u2) {
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

  int countRelevance(int userId, const FastSocialGraphUser &user) {
    int relevance=0;
    /*std::vector <int> & vectorFr=users[userId].friends;
    std::unordered_set<int> userFriends;
    for (int i=0; i<users[userId].friends.size(); i++){
      userFriends.insert(vectorFr[i]);
    }
    for (int f: user.friends){ //можно найти oбьединение сетиков чтобы было еще быстрее но нужны сетики а не векторы)))
      if(userFriends.find(f)!=userFriends.end()) relevance++;
    }*/
    for(int f: user.friends) {
        for(int uf: users[userId].friends) {
            if(f == uf) {
                relevance++;
            }
        }
    }
    /*for(int f: relations[userId]){
      if (relations[user].count(f)>0) relevance++;
    }*/

    return relevance/2; 
  }
  
  std::unordered_map<int,FastSocialGraphUser> users;
  //std::vector<std::unordered_set<int>> relations;
};