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
    std::vector<int> friends;
};

struct FastSocialGraph : SocialGraph {  
  int addUser(const std::string &name, int age, Gender gender, const std::string &city) override {
    users.insert({users.size(),FastSocialGraphUser{name,age,gender,city,{}}});
    relations.push_back({});
    return static_cast<int>(users.size()) - 1;
  }

  void addFriend(int userId, int friendId) override {
    if(userId == friendId){
        return;
    }
    if (relations[userId].count(friendId)>0){
      return;
    }

    relations[userId].insert(friendId);
    relations[friendId].insert(userId);
    users[userId].friends.push_back(friendId);
    users[friendId].friends.push_back(userId);
  }

  GetUserResponse getUser(int id) override { 
    FastSocialGraphUser& user=users[id];
    /*GetUserResponse response;
    response.name=user.name;
    response.age = user.age;
    response.city = user.city;
    response.gender = user.gender;
    response.friends=user.friends;
    return response;*/
    return GetUserResponse{user.name,user.age,user.gender,user.city,user.friends}; 
  }

  FindUsersResponse findUsers(int userId, // current user id
                              const NameFilter &nameFilter,
                              const AgeFilter &ageFilter,
                              const GenderFilter &genderFilter,
                              const CityFilter &cityFilter, SortBy sortBy,
                              int limit) override {

    std::vector<int> filteredUsers;
    std::unordered_set<std::string> citiesSet;
    if(cityFilter.active) {
      for (size_t i=0; i<cityFilter.cities.size(); i++){
        citiesSet.insert(cityFilter.cities[i]);
      }
    }
    for (size_t i=0; i<users.size(); i++){
      FastSocialGraphUser& user=users[i];

      if(sortBy == SortBy_DontSort && i==limit) {
        break;
      }

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
          auto& user1 = users[u1];
          auto& user2 = users[u2];

          if(sortBy == SortBy_Age) {
              return user1.age < user2.age;
          }
          if(sortBy == SortBy_Name) {
              return user1.name < user2.name;
          }
          if(sortBy == SortBy_Relevance) {
              return countRelevance(userId, u1 )> countRelevance(userId, u2 );
          }
          return false;
      }); 
    }

    if (filteredUsers.size()>limit){ 
        filteredUsers.resize(limit);
    }

    return {filteredUsers};
  }

  int usersCount() const override { 
    return static_cast<int>(users.size()); 
  }

  int countRelevance(int userId, int user) {
    if (userId > user){
      std::swap(userId,user);
    }
    int64_t key = (static_cast<int64_t>(userId) << 32) + user;
    
    if (relevanceCount.count(key)>0){
      return relevanceCount[key];
    }
    int64_t relevance=0;
    
    for(int f: users[userId].friends){
      if (relations[user].count(f)>0) relevance++;
    }

    relevanceCount[key]=relevance;
    return relevance; 
  }
  
  std::unordered_map<int,FastSocialGraphUser> users;
  std::vector<std::unordered_set<int>> relations;
  std::unordered_map<int64_t,int> relevanceCount;
};