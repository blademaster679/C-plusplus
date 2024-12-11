#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <iomanip>
struct Column
{
    std::string name;
    std::string type;
};
struct Record
{
    std::vector<std::string> localValues;
};
class Table
{
public:
    std::string name;
    std::vector<Column> columns;
    std::vector<Record> records;
    Table() = default;
    Table(const std::string &tableName) : name(tableName) {}
    void addColumns(const std::string &columnName, const std::string &columnType);
};
class Database
{
public:
    std::string name;
    std::unordered_map<std::string, Table> tables;
    Database() = default;
    Database(const std::string &dbName) : name(dbName) {}
    void addTable(const std::string &tableName);
};
class MiniDB
{
private:
    std::unordered_map<std::string, Database> databases;
    Database *currentDatabase;

public:
    Database *getCurrentDatabase() const
    {
        return currentDatabase;
    }
    void error(const std::string &message) const
    {
        std::cerr << "Error: " << message << std::endl;
    }
    MiniDB() {};
    void createDatabase(const std::string &DBname);
    void useDatabase(const std::string &DBname);
    void createTable(const std::string &command);
    void dropTable(const std::string &tableName);
    void loadDatabase(const std::string &DBname);
    void saveDatabase(const std::string &DBname);
    void insertIntoTable(const std::string &command, const std::string &tableName, const std::vector<std::string> &values);
    void select(const std::string &tableName, std::vector<std::string> &columns, const std::string &whereClause);
    void innerJoin(const std::string &tableName1, const std::string &tableName2, const std::string &base1, const std::string &base2, const std::string &column1, const std::string &column2, const std::string &whereClause);
    void update(const std::string &tableName, const std::string &setclause, const std::string &whereClause);
    void deleteRecord(const std::string &tableName, const std::string &whereClause);
    void parseWhereClause(const std::string &whereClause, std::vector<std::pair<std::string, std::pair<std::string, std::string>>> &conditions, std::string &logicalOperator);

    bool isInteger(const std::string &value);
    bool isFloat(const std::string &value);
    bool evaluateCondition(const std::string &tableName, const Record &record, const std::vector<std::pair<std::string, std::pair<std::string, std::string>>> &conditions, const std::string &logicalOperator);
};
#endif