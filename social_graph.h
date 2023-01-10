#pragma once

#include <string>
#include <vector>
#include <algorithm>

// Задача:
// Необходимо написать структуру данных для социального графа, чтобы эффективно выполнять следущие запросы:
// - addUser: добавление пользователя
// - addFriend: добавление друга пользователю
// - getUser: получение пользователя по id
// - findUsers: поиск среди всех пользователей 
// с заданными фильтрами (каждый фильтр может быть включен или выключен в зависимости от поля <active>)

// Поддерживаемые фильры
// - NameFilter: оставляет только пользователей с именем <name>
// - AgeFilter: оставляет только пользователей с возрастом в диапазоне от <start> до <end>
// - GenderFilter: оставляет только пользователей с заданным полом
// - CityFilter: оставляет только пользователей живущих в одном из городов, указанных в <cities>

// Так же поддерживается несколько видов сортировок пользователей
// - SortBy_DontSort - сортировка не используется
// - SortBy_Name: - сортировка по имени в лексикографическом порядке
// - SortBy_Age: - сортировка по возрасту
// - SortBy_Relevance: - сортировка по количеству общих друзей

// Поле limit ограничивает количество записей в результате заданным числом.
// Например при limit = 10, в результате должно оказаться 10 верхних записей в порядке сортировки, подходящих под заданные фильтры.
// Если записей меньше, чем limit, то необходимо вывести все подходящие записи.

enum Gender
{
    Male,
    Female,
};

struct GetUserResponse
{
    std::string name;
    int age;
    Gender gender;
    std::string city;
    std::vector<int> friends;
};

struct FindUsersResponse
{
    std::vector<int> userIds;
};

struct NameFilter
{
    bool active;
    std::string name;
};

struct AgeFilter
{
    bool active;
    int start;
    int end;
};

struct GenderFilter
{
    bool active;
    Gender gender;
};

struct CityFilter
{
    bool active;
    std::vector<std::string> cities;
};

enum SortBy {
    SortBy_DontSort,
    SortBy_Name,
    SortBy_Age,
    SortBy_Relevance,
};

struct SocialGraph
{
    virtual int addUser(const std::string &name, int age, Gender gender, const std::string & city) = 0;

    virtual void addFriend(int userId, int friendId) = 0;

    virtual GetUserResponse getUser(int id) = 0;

    virtual FindUsersResponse findUsers(
        int userId, // current user id
        const NameFilter &nameFilter,
        const AgeFilter &ageFilter,
        const GenderFilter &genderFilter,
        const CityFilter &cityFilter,
        SortBy sortBy,
        int limit) = 0;

    virtual int usersCount() const = 0;
};