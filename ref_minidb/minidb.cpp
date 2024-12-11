#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Column
{
    string name;
    string type;
};
struct Record
{
    vector<string> localValues;
};

ostream &operator<<(ostream &os, const Column &col)
{
    os << col.name << " " << col.type;
    return os;
}
class Table
{
public:
    string name;
    vector<Column> columns;
    vector<Record> records;
    Table() = default;
    Table(const string &tableName) : name(tableName) {}
    void addColumn(const string &columnName, const string &columnType)
    {
        columns.push_back({columnName, columnType});
    }
};
class Database
{
public:
    string name;
    unordered_map<string, Table> tables;
    Database() = default;
    Database(const string &dbName) : name(dbName) {}
    void addTable(const string &tableName)
    {
        tables[tableName] = Table(tableName);
    }
};

string join(const vector<string> &elements, const string &delimiter)
{
    ostringstream os;
    for (auto it = elements.begin(); it != elements.end(); ++it)
    {
        if (it != elements.begin())
        {
            os << delimiter;
        }
        os << *it;
    }
    return os.str();
}

class MiniDB
{
private:
    unordered_map<string, Database> databases;
    Database *currentDatabase;

public:
    Database *getCurrentDatabase() const
    {
        return currentDatabase;
    }
    void error(const string &message) const
    {
        cerr << "Error: " << message << endl;
    }

public:
    MiniDB() {}
    void createDatabase(const string &DBname)
    {
        if (databases.find(DBname) == databases.end())
        {
            databases[DBname] = Database{DBname};
            saveDatabase(DBname);
        }
        else
        {
            return;
        }
    }
    void useDatabase(const string &DBname)
    {
        string dbName = DBname;

        if (databases.find(dbName) != databases.end())
        {
            currentDatabase = &databases[dbName];

            loadDatabase(dbName);
        }
    }
    void createTable(const string &command)
    {

        const string keyword = "CREATE TABLE";
        if (command.find(keyword) != 0)
        {
            cout << "Invalid command. Expected 'CREATE TABLE'." << endl;
            return;
        }

        size_t start = keyword.length();
        size_t end = command.find('(', start); // find the opening parenthesis
        if (end == string::npos)
        {
            cout << "Invalid command. Missing '(' after table name." << endl;
            return;
        }

        // extract table name
        string tableName = command.substr(start, end - start);
        tableName.erase(0, tableName.find_first_not_of(" \t\n"));
        tableName.erase(tableName.find_last_not_of(" \t\n") + 1);

        if (tableName.empty())
        {
            cout << "Invalid command. Table name is empty." << endl;
            return;
        }

        // check if table already exists
        if (currentDatabase->tables.find(tableName) != currentDatabase->tables.end())
        {
            cout << "Table " << tableName << " already exists." << endl;
            return;
        }

        Table newTable(tableName);
        size_t columnStart = end + 1;

        // find the end of column definitions
        size_t columnEnd = command.find(')', columnStart);
        if (columnEnd == string::npos)
        {
            cout << "Invalid command. Missing ')' at the end of column definitions." << endl;
            return;
        }

        // extract column definitions
        string columns = command.substr(columnStart, columnEnd - columnStart);

        // erase leading and trailing whitespaces
        columns.erase(0, columns.find_first_not_of(" \t\n"));
        columns.erase(columns.find_last_not_of(" \t\n") + 1);

        // deal with multi-line column definitions
        istringstream iss(columns);
        string columnDef;

        while (getline(iss, columnDef, ','))
        {
            // erase leading and trailing whitespaces
            columnDef.erase(0, columnDef.find_first_not_of(" \t\n"));
            columnDef.erase(columnDef.find_last_not_of(" \t\n") + 1);

            if (columnDef.empty())
                continue; // skip empty column definitions

            // extract column name and type
            istringstream colStream(columnDef);
            string columnName, columnType;
            colStream >> columnName >> columnType;

            // check if column type is valid
            if (columnType != "INTEGER" && columnType != "TEXT" && columnType != "FLOAT")
            {
                cout << "Invalid column type: " << columnType << endl;
                return;
            }

            // add column to table
            newTable.addColumn(columnName, columnType);
        }

        // add table to current database
        currentDatabase->tables[tableName] = newTable;
        saveDatabase(currentDatabase->name);
    }
    void insertIntoTable(const string &command, const string &tableName, const vector<string> &values)
    { // check if the database is empty
        if (currentDatabase == nullptr)
        {
            cout << "No database selected." << endl;
            return;
        }

        // check if the table exists in the current database

        if (currentDatabase->tables.find(tableName) == currentDatabase->tables.end())
        {
            cout << "Table " << tableName << " does not exist." << endl;
            return;
        }
        // extract values from the command
        size_t valuesStart = command.find("VALUES") + 6;
        size_t valuesEnd = command.find(';', valuesStart);
        if (valuesStart == string::npos || valuesEnd == string::npos)
        {
            cout << "Invalid command." << endl;
            return;
        }
        string valuepart = command.substr(valuesStart, valuesEnd - valuesStart);
        valuepart.erase(0, valuepart.find_first_not_of(" \t\n"));
        valuepart.erase(valuepart.find_last_not_of(" \t\n") + 1);
        if (valuepart.front() != '(' || valuepart.back() != ')')
        {
            cout << "Invalid command." << endl;
            return;
        }
        valuepart = valuepart.substr(1, valuepart.size() - 2);

        istringstream iss(valuepart);
        string value;
        vector<string> localValues;

        while (getline(iss, value, ','))
        {
            // remove leading and trailing whitespace
            value.erase(0, value.find_first_not_of(" \t\n"));
            value.erase(value.find_last_not_of(" \t\n") + 1);

            localValues.push_back(value);
        }
        // check if the number of values matches the number of columns
        auto &table = currentDatabase->tables[tableName];

        if (localValues.size() != table.columns.size())
        {
            cout << "Error: Number of values does not match number of columns." << endl;
            return;
        }
        // insert the record
        Record newRecord;
        bool valid = true;
        for (size_t i = 0; i < table.columns.size(); i++)
        {
            const Column &column = table.columns[i];
            const string &value = localValues[i];
            // type checking
            if (column.type == "INTEGER")
            {
                try
                {
                    stoi(value);
                }
                catch (const invalid_argument &)
                {
                    cout << "Error: Invalid value for INTEGER column." << endl;
                    valid = false;
                    break;
                }
            }
            else if (column.type == "FLOAT")
            {
                try
                {
                    stof(value);
                }
                catch (const invalid_argument &)
                {
                    cout << "Error: Invalid value for FLOAT column." << endl;
                    valid = false;
                    break;
                }
            }
            else if (column.type == "TEXT")
            {
                // do nothing
            }
            else
            {
                cout << "Error: Invalid column type." << column.type << endl;
                valid = false;
                break;
            }
            newRecord.localValues.push_back(value);
        }
        if (valid)
        {
            table.records.push_back(newRecord);
        }
    }
    void select(const string &tableName, vector<string> &columns, const string &whereClause)
    {
        if (currentDatabase == nullptr)
        {
            cout << "No database selected." << endl;
            return;
        }

        // Check if the table exists in the current database
        auto &db = *currentDatabase;
        if (db.tables.find(tableName) != db.tables.end())
        {
            Table &table = db.tables[tableName];

            // If columns = *, print all columns
            if (columns[0] == "*")
            {
                // Print all column names
                for (size_t i = 0; i < table.columns.size(); ++i)
                {
                    cout << table.columns[i].name;
                    columns.push_back(table.columns[i].name);
                    if (i < table.columns.size() - 1)
                    {
                        cout << ",";
                    }
                }
                cout << endl; // New line after column names
            }

            // Parse the WHERE clause into conditions and logical operators
            vector<pair<string, pair<string, string>>> conditions;
            string logicalOperator = "AND"; // Default to AND if no logical operator is found
            // Handle both simple and complex WHERE clauses
            if (!whereClause.empty())
            {
                string whereClause1, whereClause2;
                vector<string> whereclauses;
                if (whereClause.find("AND") != string::npos)
                {
                    whereClause1 = whereClause.substr(0, whereClause.find("AND") - 1);
                    whereClause2 = whereClause.substr(whereClause.find("AND") + 4);
                    logicalOperator = "AND";
                    whereclauses.push_back(whereClause1);
                    whereclauses.push_back(whereClause2);
                }
                else if (whereClause.find("OR") != string::npos)
                {
                    whereClause1 = whereClause.substr(0, whereClause.find("OR") - 1);
                    whereClause2 = whereClause.substr(whereClause.find("OR") + 3);
                    logicalOperator = "OR";
                    whereclauses.push_back(whereClause1);
                    whereclauses.push_back(whereClause2);
                }
                else
                {
                    whereClause1 = whereClause;
                    whereclauses.push_back(whereClause1);
                }
                for (auto &where : whereclauses)
                {
                    istringstream iss(where);
                    string token, columnName, op, value;
                    while (iss >> token)
                    {
                        // Column name
                        columnName = token;
                        iss >> op;
                        getline(iss, value);
                        if (value.front() == ' ')
                            value = value.substr(1);
                        if (value.front() == '\'' && value.back() == '\'')
                        {
                            value = value.substr(1, value.length() - 2);
                        }
                        conditions.emplace_back(columnName, make_pair(op, value));
                    }
                }
            }

            for (auto &record : table.records)
            {

                // Check if the record satisfies the WHERE clause
                if (evaluateCondition(tableName, record, conditions, logicalOperator))
                {
                    // Print the selected columns for the record
                    for (size_t i = 0; i < columns.size(); ++i)
                    {

                        auto it = find_if(table.columns.begin(), table.columns.end(), [&columns, i](const Column &c)
                                          { return c.name == columns[i]; });
                        if (it != table.columns.end())
                        {
                            // Find the index of the column in the record and print its value
                            size_t columnIndex = distance(table.columns.begin(), it);
                            if (table.columns[columnIndex].type == "FLOAT")
                            {
                                cout << fixed << setprecision(2) << stof(record.localValues[columnIndex]);
                            }
                            else
                            {
                                cout << record.localValues[columnIndex];
                            }
                            if (i < columns.size() - 1)
                            {
                                cout << ",";
                            }
                        }
                    }
                    cout << endl; // New line after each record
                }
            }
        }
        cout << "---" << endl;
    }
    bool evaluateCondition(const string &tableName, const Record &record, const vector<pair<string, pair<string, string>>> &conditions, const string &logicalOperator)
    {
        auto &table = currentDatabase->tables[tableName];
        bool result = true; // Start with true for AND conditions

        for (size_t i = 0; i < conditions.size(); ++i)
        {
            const auto &cond = conditions[i];
            string columnName = cond.first;
            string op = cond.second.first;
            string value = cond.second.second;
            if (value.front() == '\'' && value.back() == '\'')
            {
                value = value.substr(1, value.length() - 2);
            }

            // Find the column index
            auto colIt = find_if(table.columns.begin(), table.columns.end(),
                                 [&columnName](const Column &col)
                                 { return col.name == columnName; });

            if (colIt == table.columns.end())
            {
                cerr << "Column not found: " << columnName << endl;
                return false;
            }

            size_t columnIndex = distance(table.columns.begin(), colIt);

            // Evaluate condition based on column type
            const Column &column = table.columns[columnIndex];
            bool conditionResult = false;

            // Check for string type (TEXT)
            if (column.type == "TEXT")
            {
                string tempValue = record.localValues[columnIndex];
                // Remove leading and trailing spaces (if any)
                if (tempValue.front() == '\'' && tempValue.back() == '\'')
                {
                    tempValue = tempValue.substr(1, tempValue.length() - 2); // Remove the quotes
                }

                // String comparison
                if (op == "=")
                {
                    conditionResult = (tempValue == value);
                }
            }
            // Check for integer type (INTEGER)
            else if (column.type == "INTEGER")
            {
                int recordValue = stoi(record.localValues[columnIndex]);
                int targetValue = stoi(value);
                if (op == "=")
                {
                    conditionResult = (recordValue == targetValue);
                }
                else if (op == ">")
                {
                    conditionResult = (recordValue > targetValue);
                }
                else if (op == "<")
                {
                    conditionResult = (recordValue < targetValue);
                }
            }
            // Check for float type (FLOAT)
            else if (column.type == "FLOAT")
            {
                float recordValue = stof(record.localValues[columnIndex]);
                float targetValue = stof(value);
                if (op == "=")
                {
                    conditionResult = (recordValue == targetValue);
                }
                else if (op == ">")
                {
                    conditionResult = (recordValue > targetValue);
                }
                else if (op == "<")
                {
                    conditionResult = (recordValue < targetValue);
                }
            }

            // Combine conditions using logical operators
            if (i == 0)
            {
                result = conditionResult; // For the first condition, just set the result
            }
            else if (logicalOperator == "AND")
            {
                result = result && conditionResult;
            }
            else if (logicalOperator == "OR")
            {
                result = result || conditionResult;
            }
        }

        return result;
    }
    void innerJoin(const string &tableName1, const string &tableName2, const string &base1, const string &base2, const string &column1, const string &column2, const string &whereClause)
    {
        // Check if the database is empty
        if (currentDatabase == nullptr)
        {
            cout << "No database selected." << endl;
            return;
        }
        auto &db = *currentDatabase;
        // check if the tables exist in the current database

        Table &table1 = db.tables[tableName1];
        Table &table2 = db.tables[tableName2];
        auto it1 = find_if(table1.columns.begin(), table1.columns.end(), [&base1](const Column &c)
                           { return c.name == base1; });
        auto it2 = find_if(table2.columns.begin(), table2.columns.end(), [&base2](const Column &c)
                           { return c.name == base2; });
        auto it3 = find_if(table1.columns.begin(), table1.columns.end(), [&column1](const Column &c)
                           { return c.name == column1; });
        auto it4 = find_if(table2.columns.begin(), table2.columns.end(), [&column2](const Column &c)
                           { return c.name == column2; });
        // Check if the base columns exist in the tables
        if (it1 == table1.columns.end() || it2 == table2.columns.end())
        {
            cout << "Column does not exist." << endl;
            return;
        }
        // Find the index of the base columns
        size_t index1 = distance(table1.columns.begin(), it1);
        size_t index2 = distance(table2.columns.begin(), it2);
        size_t index3 = distance(table1.columns.begin(), it3);
        size_t index4 = distance(table2.columns.begin(), it4);
        string tableName;
        // Print the header
        if (whereClause.empty())
        {
            cout << tableName1 << "." << column1 << "," << tableName2 << "." << column2 << endl;
            for (const auto &record1 : table1.records)
            {
                for (const auto &record2 : table2.records)
                {
                    if (record1.localValues[index1] == record2.localValues[index2])
                    {
                        cout << record1.localValues[index3] << "," << record2.localValues[index4] << endl;
                    }
                }
            }
        }

        else
        {
            // Parse the WHERE clause
            vector<pair<string, pair<string, string>>> conditions;
            string logicalOperator = "AND";
            if (!whereClause.empty())
            {

                string columnName, op, value;
                tableName = whereClause.substr(0, whereClause.find("."));
                columnName = whereClause.substr(whereClause.find(".") + 1, whereClause.find("=") - whereClause.find(".") - 1);
                columnName = columnName.erase(0, columnName.find_first_not_of(" \t\n;"));
                columnName = columnName.erase(columnName.find_last_not_of(" \t\n;") + 1);

                if (whereClause.find("=") != string::npos)
                {
                    op = "=";
                }
                else if (whereClause.find(">") != string::npos)
                {
                    op = ">";
                }
                else if (whereClause.find("<") != string::npos)
                {
                    op = "<";
                }
                value = whereClause.substr(whereClause.find(op) + 1);
                value = value.erase(0, value.find_first_not_of(" \t\n;"));
                value = value.erase(value.find_last_not_of(" \t\n;") + 1);

                conditions.emplace_back(columnName, make_pair(op, value));
            }
            cout << tableName1 << "." << column1 << "," << tableName2 << "." << column2 << endl;
            for (const auto &record1 : table1.records)
            {
                for (const auto &record2 : table2.records)
                {
                    // Evaluate the WHERE clause conditions for both tables
                    if (record1.localValues[index1] == record2.localValues[index2] && evaluateCondition(tableName, record1, conditions, logicalOperator))
                    {
                        cout << record1.localValues[index3] << "," << record2.localValues[index4] << endl;
                    }
                }
            }
        }
        cout << "---" << endl;
    }
    void dropTable(const string &tableName)
    {
        if (currentDatabase == nullptr)
        {
            cout << "No database selected." << endl;
            return;
        }
        auto &db = *currentDatabase;

        if (db.tables.find(tableName) == db.tables.end())
        {
            cout << "Table [" << tableName << "] does not exist." << endl;
            return;
        }
        db.tables.erase(tableName);

        saveDatabase(currentDatabase->name);
    }
    void update(const string &tableName, const string &setclause, const string &whereClause)
    {
        if (currentDatabase == nullptr)
        {
            cout << "No database selected." << endl;
            return;
        }
        auto &db = *currentDatabase;
        if (db.tables.find(tableName) == db.tables.end())
        {
            cout << "Table does not exist." << endl;
            return;
        }
        Table &table = db.tables[tableName];
        // Parse the SET clause
        istringstream iss(setclause);
        string ope, change_columnName, content;
        iss >> change_columnName >> ope;
        content = setclause.substr(setclause.find(ope) + 1);
        // Parse the WHERE clause
        vector<pair<string, pair<string, string>>> conditions;
        string logicalOperator = "AND";
        if (!whereClause.empty())
        {
            istringstream iss(whereClause);
            string token, columnName, op, value;
            while (iss >> token)
            {
                if (token == "AND" || token == "OR")
                {
                    logicalOperator = token;
                }
                else
                {
                    columnName = token;
                    iss >> op;
                    getline(iss, value);
                    if (value.front() == ' ')
                        value = value.substr(1);
                    if (value.front() == '\'' && value.back() == '\'')
                    {
                        value = value.substr(1, value.length() - 2);
                    }
                    conditions.emplace_back(columnName, make_pair(op, value));
                }
            }
        }

        for (auto &record : table.records)
        {
            // Check if the record satisfies the WHERE clause
            if (evaluateCondition(tableName, record, conditions, logicalOperator))
            {
                auto it = find_if(table.columns.begin(), table.columns.end(), [&change_columnName](const Column &c)
                                  { return c.name == change_columnName; });
                if (it != table.columns.end())
                {
                    // Find the index of the column in the record and print its value
                    size_t columnIndex = distance(table.columns.begin(), it);
                    if (content.find("+") != string::npos || content.find("-") != string::npos || content.find("*") != string::npos || content.find("/") != string::npos)
                    {
                        if (table.columns[columnIndex].type == "INTEGER")
                        {
                            int currentVal = stoi(record.localValues[columnIndex]);
                            int newVal = stoi(content.substr(content.length() - 1));
                            if (content.find("+") != string::npos)
                            {
                                record.localValues[columnIndex] = to_string(currentVal + newVal);
                            }
                            else if (content.find("-") != string::npos)
                            {
                                record.localValues[columnIndex] = to_string(currentVal - newVal);
                            }
                            else if (content.find("*") != string::npos)
                            {
                                record.localValues[columnIndex] = to_string(currentVal * newVal);
                            }
                            else if (content.find("/") != string::npos)
                            {
                                record.localValues[columnIndex] = to_string(currentVal / newVal);
                            }
                        }
                        else if (table.columns[columnIndex].type == "FLOAT")
                        {
                            float currentVal = stof(record.localValues[columnIndex]);
                            float newVal = stof(content.substr(content.find_last_of("+-") + 1));
                            float result;
                            if (content.find("+") != string::npos)
                            {
                                result = currentVal + newVal;
                            }
                            else if (content.find("-") != string::npos)
                            {
                                result = currentVal - newVal;
                            }
                            else if (content.find("*") != string::npos)
                            {
                                result = currentVal * newVal;
                            }
                            else if (content.find("/") != string::npos)
                            {
                                result = currentVal / newVal;
                            }
                            // 使用 stringstream 来格式化浮点数
                            std::ostringstream out;
                            out << std::fixed << std::setprecision(2) << result;
                            record.localValues[columnIndex] = out.str();
                        }
                    }
                    else
                    {
                        record.localValues[columnIndex] = content;
                    }
                }
            }
        }
        saveDatabase(currentDatabase->name);
    }
    void deleteRecord(const string &tableName, const string &whereClause)
    {
        if (currentDatabase == nullptr)
        {
            cout << "No database selected." << endl;
            return;
        }
        auto &db = *currentDatabase;
        if (db.tables.find(tableName) == db.tables.end())
        {
            cout << "Table does not exist." << endl;
            return;
        }
        Table &table = db.tables[tableName];
        // Parse the WHERE clause
        vector<pair<string, pair<string, string>>> conditions;
        string logicalOperator = "AND";
        if (!whereClause.empty())
        {
            istringstream iss(whereClause);
            string token, columnName, op, value;
            while (iss >> token)
            {
                if (token == "AND" || token == "OR")
                {
                    logicalOperator = token;
                }
                else
                {
                    columnName = token;
                    iss >> op;
                    getline(iss, value);
                    if (value.front() == ' ')
                        value = value.substr(1);
                    if (value.front() == '\'' && value.back() == '\'')
                    {
                        value = value.substr(1, value.length() - 2);
                    }
                    conditions.emplace_back(columnName, make_pair(op, value));
                }
            }
        }
        // Delete records that satisfy the WHERE clause
        auto it = table.records.begin();
        while (it != table.records.end())
        {
            if (evaluateCondition(tableName, *it, conditions, logicalOperator))
            {
                it = table.records.erase(it);
            }
            else
            {
                ++it;
            }
        }
        saveDatabase(currentDatabase->name);
    }
    void loadDatabase(const string &DBname)
    {
        ifstream dbFile(DBname + ".txt");
        if (!dbFile.is_open())
        {
            error("Failed to open file for loading.");
            return;
        }
        Database loadedDb(DBname);
        string line;
        Table *currentTable = nullptr;
        while (getline(dbFile, line))
        {
            if (line.empty())
            {
                continue;
            }
            if (line.find("TABLE") == 0)
            {
                string tableName = line.substr(6);
                tableName.erase(0, tableName.find_first_not_of(" \t\n"));
                tableName.erase(tableName.find_last_not_of(" \t\n") + 1);
                loadedDb.tables[tableName] = Table(tableName);
                currentTable = &loadedDb.tables[tableName];
            }
            else if (currentTable != nullptr && line.find(')') == string::npos)
            {
                istringstream iss(line);
                string columnName, columnType;
                iss >> columnName >> columnType;
                if (columnType == "INTEGER" || columnType == "TEXT" || columnType == "FLOAT")
                {
                    currentTable->addColumn(columnName, columnType);
                }
                else
                {
                    cout << "Warning: Invalid column type '" << columnType << "' in table '" << currentTable->name << "'. Skipping column." << endl;
                }
            }
            else if (currentTable != nullptr && line.find("INSERT INTO") == 0)
            {
                Record record;
                istringstream iss(line);
                string value;
                while (getline(iss, value, ','))
                {
                    value.erase(0, value.find_first_not_of(" \t\n"));
                    value.erase(value.find_last_not_of(" \t\n") + 1);
                    record.localValues.push_back(value);
                }
                currentTable->records.push_back(record);
            }
        }
        dbFile.close();
        databases[DBname] = loadedDb;
    }
    void saveDatabase(const string &DBname)
    {
        ofstream dbFile(DBname + ".txt");
        if (!dbFile.is_open())
        {
            error("Failed to open file for saving.");
            return;
        }

        auto &db = databases[DBname];
        dbFile << "CREATE DATABASE " << DBname << ";" << endl;
        // Iterate through tables
        for (const auto &tablePair : db.tables)
        {
            const Table &table = tablePair.second;
            dbFile << "CREATE TABLE " << table.name << ";" << endl;

            // Write columns
            for (size_t i = 0; i < table.columns.size(); ++i)
            {
                const Column &col = table.columns[i];
                dbFile << "    " << col.name << " " << col.type;
                if (i < table.columns.size() - 1)
                {
                    dbFile << ",";
                }
                dbFile << "\n";
            }
            dbFile << ");" << endl;

            // Write records
            for (const auto &record : table.records)
            {
                dbFile << "INSERT INTO " << table.name << " VALUES (";
                dbFile << join(record.localValues, ",") << endl;
            }
            dbFile << ");" << endl;
        }

        dbFile.close();
    }
};
int main(int argc, char *argv[])
{

    MiniDB minidb;
    string command;
    string fullcommand;
    string fileName;
    cin >> fileName;
    ifstream inputFile(fileName + ".sql");
    ofstream output("output.csv");
    if (!inputFile.is_open())
    {
        cout << "File not found" << endl;
        return 0;
    }
    streambuf *coutbuf = cout.rdbuf();
    cout.rdbuf(output.rdbuf());
    while (getline(inputFile, command))
    {
        if (command.empty())
        {
            continue;
        }
        command = command.erase(0, command.find_first_not_of(" \t\n"));
        command = command.erase(command.find_last_not_of(" \t\n") + 1);
        if (command.find("CREATE DATABASE") != string::npos)
        {
            string dbName = command.substr(16);
            dbName = dbName.substr(0, dbName.size() - 1); // remove the semicolon
            minidb.createDatabase(dbName);
        }
        else if (command.find("USE DATABASE") != string::npos)
        {
            static string lastUsedDatabase = "";
            string dbName = command.substr(13);
            dbName = dbName.substr(0, dbName.size() - 1); // remove the semicolon
            if (dbName == lastUsedDatabase) {
                continue;
            }
            lastUsedDatabase = dbName;
            minidb.useDatabase(dbName);
        }
        else if (command.find("CREATE TABLE") != string::npos)
        {
            fullcommand = command;
            while (getline(inputFile, command))
            {
                fullcommand += command;
                if (command.find(");") != string::npos)
                {
                    break;
                }
            }
            minidb.createTable(fullcommand);
        }
        else if (command.find("DROP TABLE") != string::npos)
        {
            string tableName = command.substr(11);
            tableName = tableName.substr(0, tableName.size() - 1); // remove the semicolon
            minidb.dropTable(tableName);
        }
        else if (command.find("INSERT INTO") != string::npos)
        { // print table already exists

            string tableName = command.substr(12);
            tableName = tableName.substr(0, tableName.find("VALUES") - 1);
            tableName = tableName.erase(0, tableName.find_first_not_of(" \n\t"));
            tableName = tableName.erase(tableName.find_last_not_of(" \t\n") + 1);
            string values = command.substr(command.find("VALUES") + 6);
            values = values.substr(0, values.size());
            vector<string> valueList;
            istringstream iss(values);
            string value;
            while (getline(iss, value, ','))
            {
                valueList.push_back(value);
            }

            minidb.insertIntoTable(command, tableName, valueList);
        }
        else if (command.find("SELECT") != string::npos && command.find(";") != string::npos)
        {
            command = command.substr(command.find("SELECT") + 6);

            vector<string> columns;
            string column;
            string columncommand = command.substr(0, command.find("FROM"));

            // Check if '*' is in columns, meaning select all columns
            if (columncommand.find("*") != string::npos)
            {
                columns.push_back("*");
            }
            else
            {
                // If specific columns are specified
                istringstream columnStream(columncommand);
                while (getline(columnStream, column, ','))
                {
                    column = column.erase(0, column.find_first_not_of(" \t\n"));
                    column = column.erase(column.find_last_not_of(" \t\n") + 1);
                    columns.push_back(column);
                }
            }

            // Get the table name and WHERE clause
            string tableName = "";
            string whereClause = "";
            string joinClause = "";

            if (command.find("WHERE") != string::npos)
            {
                tableName = command.substr(command.find("FROM") + 5, command.find("WHERE") - command.find("FROM") - 6);
                whereClause = command.substr(command.find("WHERE") + 6);
                whereClause = whereClause.substr(0, whereClause.size() - 1); // remove the semicolon
            }
            else
            {
                tableName = command.substr(command.find("FROM") + 5);
                tableName = tableName.substr(0, tableName.size() - 1); // remove the semicolon
            }

            // Now handle INNER JOIN

            // Perform regular SELECT operation
            minidb.select(tableName, columns, whereClause);
        }
        else if (command.find("SELECT") != string::npos && command.find(";") == string::npos)
        {
            fullcommand = command;
            // Read the rest of the command (for multi-line input)
            while (getline(inputFile, command))
            {
                fullcommand += command;
                if (command.find(";") != string::npos)
                {
                    break;
                }
            }
            // Find the SELECT part and the FROM part
            string selectcommand = fullcommand.substr(fullcommand.find("SELECT") + 6, fullcommand.find("FROM") - command.find("SELECT") - 7);

            // Extract tables and columns from the SELECT part
            string table1 = selectcommand.substr(0, selectcommand.find(","));
            string tableName1 = table1.substr(0, table1.find("."));
            string tablecolumn1 = table1.substr(table1.find(".") + 1);

            string table2 = selectcommand.substr(selectcommand.find(",") + 1);
            string tableName2 = table2.substr(0, table2.find("."));
            string tablecolumn2 = table2.substr(table2.find(".") + 1);

            // clear the spaces
            tableName1 = tableName1.erase(0, tableName1.find_first_not_of(" \t\n"));
            tableName1 = tableName1.erase(tableName1.find_last_not_of(" \t\n") + 1);
            tableName2 = tableName2.erase(0, tableName2.find_first_not_of(" \t\n"));
            tableName2 = tableName2.erase(tableName2.find_last_not_of(" \t\n") + 1);
            tablecolumn1 = tablecolumn1.erase(0, tablecolumn1.find_first_not_of(" \t\n;"));
            tablecolumn1 = tablecolumn1.erase(tablecolumn1.find_last_not_of(" \t\n;") + 1);
            tablecolumn2 = tablecolumn2.erase(0, tablecolumn2.find_first_not_of(" \t\n;"));
            tablecolumn2 = tablecolumn2.erase(tablecolumn2.find_last_not_of(" \t\n;") + 1);
            // Extract the FROM and INNER JOIN parts
            string fromcommand = fullcommand.substr(fullcommand.find("FROM") + 5, fullcommand.find("INNER JOIN") - fullcommand.find("FROM") - 5);

            // Extract join condition
            string base1 = fullcommand.substr(fullcommand.find("ON") + 3, fullcommand.find("=") - fullcommand.find("ON") - 3);
            string base3 = base1.substr(base1.find(".") + 1);
            string whereClause = "";
            string base2 = "";
            if (fullcommand.find("WHERE") == string::npos)
            {
                base2 = fullcommand.substr(fullcommand.find("=") + 2, fullcommand.find(";") - fullcommand.find("=") - 2);
            }
            else
            {
                base2 = fullcommand.substr(fullcommand.find("=") + 2, fullcommand.find("WHERE") - fullcommand.find("=") - 2);
                whereClause = fullcommand.substr(fullcommand.find("WHERE") + 6);
                whereClause = whereClause.substr(0, whereClause.size() - 1);
            }
            string base4 = base2.substr(base2.find(".") + 1);
            base3 = base3.erase(0, base3.find_first_not_of(" \t\n"));
            base3 = base3.erase(base3.find_last_not_of(" \t\n") + 1);
            base4 = base4.erase(0, base4.find_first_not_of(" \t\n"));
            base4 = base4.erase(base4.find_last_not_of(" \t\n") + 1);
            minidb.innerJoin(tableName1, tableName2, base3, base4, tablecolumn1, tablecolumn2, whereClause);
        }
        else if (command.find("UPDATE") != string::npos)
        {
            string tableName = command.substr(6, command.find("SET") - 6);
            tableName = tableName.erase(0, tableName.find_first_not_of(" \t\n"));
            tableName = tableName.erase(tableName.find_last_not_of(" \t\n") + 1);
            string setClause = command.substr(command.find("SET") + 4, command.find("WHERE") - command.find("SET") - 4);
            setClause = setClause.erase(0, setClause.find_first_not_of(" \t\n"));
            setClause = setClause.erase(setClause.find_last_not_of(" \t\n") + 1);
            string whereClause = command.substr(command.find("WHERE") + 6);
            whereClause = whereClause.substr(0, whereClause.size() - 1); // remove the semicolon
            minidb.update(tableName, setClause, whereClause);
        }
        else if (command.find("DELETE") != string::npos)
        {
            string tableName = command.substr(12, command.find("WHERE") - 12);
            tableName = tableName.erase(0, tableName.find_first_not_of(" \t\n"));
            tableName = tableName.erase(tableName.find_last_not_of(" \t\n") + 1);
            string whereClause = command.substr(command.find("WHERE") + 6, command.size() - command.find("WHERE") - 7);
            minidb.deleteRecord(tableName, whereClause);
        }
    }
    cout.rdbuf(coutbuf);
    inputFile.close();
    output.close();

    return 0;
}