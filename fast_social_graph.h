#pragma once

#pragma once
#include "social_graph.h"

struct FastSocialGraph : SocialGraph {
  int addUser(const std::string &name, int age, Gender gender,
              const std::string &city) override {
    return 0;
  }

  void addFriend(int userId, int friendId) override {}

  GetUserResponse getUser(int id) override { return {}; }

  FindUsersResponse findUsers(int userId, // current user id
                              const NameFilter &nameFilter,
                              const AgeFilter &ageFilter,
                              const GenderFilter &genderFilter,
                              const CityFilter &cityFilter, SortBy sortBy,
                              int limit) override {
    return {};
  }

  int usersCount() const override { return 0; }
};