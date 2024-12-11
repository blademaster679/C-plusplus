#include "data_manager.hpp"
#include "removespace.hpp"
#include <numeric>
#include <string>
#include <algorithm>
#include <cctype>


bool isValidColumnType(const std::string &type);
// 函数 trim 用于去除字符串两端的空格
// 函数 join 用于将字符串数组连接成一个字符串，每个元素之间用 delimiter 分隔
std::string join(const std::vector<std::string> &elements, const std::string &delimiter)
{
    if (elements.empty())
        return "";

    return std::accumulate(std::next(elements.begin()), elements.end(), elements[0],
                           [&delimiter](const std::string &a, const std::string &b)
                           {
                               return a + delimiter + b;
                           });
}

// 函数addColumns 用于向表中添加列
void Table::addColumns(const std::string &columnName, const std::string &columnType)
{
    columns.push_back({columnName, columnType});
};
// 函数 addTable 用于向数据库中添加表
void Database::addTable(const std::string &tableName)
{
    tables[tableName] = Table(tableName);
};
// 函数 createDatabase 用于创建数据库
void MiniDB::createDatabase(const std::string &DBname)
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
// 函数 useDatabase 用于选择数据库
void MiniDB::useDatabase(const std::string &DBname)
{
    std::string dbName = DBname;
    if (databases.find(dbName) != databases.end())
    {
        currentDatabase = &databases[dbName];

        loadDatabase(dbName);
    }
}
// 函数 createTable 用于创建表
void MiniDB::createTable(const std::string &command)
{

    const std::string keyword = "CREATE TABLE";
    if (command.find(keyword) != 0)
    {
        std::cout << "Invalid command. Expected 'CREATE TABLE'." << std::endl;
        return;
    }

    size_t start = keyword.length();
    size_t end = command.find('(', start); // 找到左括号

    // 提取表名
    std::string tableName = command.substr(start, end - start);
    tableName = trim(tableName);

    if (tableName.empty())
    {
        std::cout << "Invalid command. Table name is empty." << std::endl;
        return;
    }

    // 检查表是否已经存在
    if (currentDatabase->tables.find(tableName) != currentDatabase->tables.end())
    {
        std::cout << "Table " << tableName << " already exists." << std::endl;
        return;
    }

    Table newTable(tableName);
    size_t columnStart = end + 1;

    // 找到列定义的结束位置
    size_t columnEnd = command.find(')', columnStart);
    if (columnEnd == std::string::npos)
    {
        std::cout << "Invalid command. Missing ')' at the end of column definitions." << std::endl;
        return;
    }

    // 提取列定义
    std::string columns = command.substr(columnStart, columnEnd - columnStart);
    columns = trim(columns);

    // 用逗号分隔列定义，处理每一列
    std::istringstream iss(columns);
    std::string columnDef;

    while (getline(iss, columnDef, ','))
    {
        columnDef = trim(columnDef);

        if (columnDef.empty())
            continue; // 跳过空列定义

        // 提取列名和类型
        std::istringstream colStream(columnDef);
        std::string columnName, columnType;
        colStream >> columnName >> columnType;

        // 检查列类型是否有效
        if (columnType != "INTEGER" && columnType != "TEXT" && columnType != "FLOAT")
        {
            std::cout << "Invalid column type: " << columnType << std::endl;
            return;
        }

        // 添加列到表中
        newTable.addColumns(columnName, columnType);
    }

    // 添加表到当前数据库
    currentDatabase->tables[tableName] = newTable;
    saveDatabase(currentDatabase->name);
}
// 函数 dropTable 用于删除表
void MiniDB::dropTable(const std::string &tableName)
{
    if (!currentDatabase)
    {
        std::cout << "No database selected." << std::endl;
        return;
    }

    if (currentDatabase->tables.erase(tableName) == 0)
    {
        std::cout << "Table [" << tableName << "] does not exist." << std::endl;
        return;
    }

    saveDatabase(currentDatabase->name);
}
// 函数 loadDatabase 用于加载数据库
void MiniDB::loadDatabase(const std::string &DBname)
{
    std::ifstream dbFile(DBname + ".txt");
    if (!dbFile.is_open())
    {
        error("Failed to open file for loading.");
        return;
    }

    Database loadedDb(DBname);
    std::string line;
    Table *currentTable = nullptr;
    while (getline(dbFile, line))
    {
        line = trim(line);
        if (line.empty())
            continue;

        if (line.rfind("TABLE", 0) == 0)
        {
            std::string tableName = trim(line.substr(6));
            loadedDb.tables[tableName] = Table(tableName);
            currentTable = &loadedDb.tables[tableName];
        }
        else if (currentTable && line.find(')') == std::string::npos)
        {
            std::istringstream iss(line);
            std::string columnName, columnType;
            iss >> columnName >> columnType;
            if (isValidColumnType(columnType))
                currentTable->addColumns(columnName, columnType);
            else
                std::cerr << "Warning: Invalid column type '" << columnType << "' in table '" << currentTable->name << "'. Skipping column." << std::endl;
        }
        else if (currentTable && line.rfind("INSERT INTO", 0) == 0)
        {
            std::istringstream iss(line.substr(11));
            Record record;
            std::string value;
            while (getline(iss, value, ','))
                record.localValues.push_back(trim(value));
            currentTable->records.push_back(record);
        }
    }
    dbFile.close();
    databases[DBname] = loadedDb;
}

bool isValidColumnType(const std::string &type)
{
    return type == "INTEGER" || type == "TEXT" || type == "FLOAT";
}

// 函数 saveDatabase 用于保存数据库
void MiniDB::saveDatabase(const std::string &DBname)
{
    std::ofstream dbFile(DBname + ".txt");
    if (!dbFile.is_open())
    {
        error("Failed to open file for saving.");
        return;
    }

    auto &db = databases[DBname];
    dbFile << "CREATE DATABASE " << DBname << ";" << std::endl;
    //遍历表
    for (const auto &tablePair : db.tables)
    {
        const Table &table = tablePair.second;
        dbFile << "CREATE TABLE " << table.name << ";" << std::endl;
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
        dbFile << ");" << std::endl;
        for (const auto &record : table.records)
        {
            dbFile << "INSERT INTO " << table.name << " VALUES (";
            dbFile << join(record.localValues, ",") << std::endl;
        }
        dbFile << ");" << std::endl;
    }

    dbFile.close();
}
// 函数 insertIntoTable 用于向表中插入记录
void MiniDB::insertIntoTable(const std::string &command, const std::string &tableName, const std::vector<std::string> &values)
{
    if (!currentDatabase)
    {
        std::cerr << "No database selected." << std::endl;
        return;
    }

    if (currentDatabase->tables.find(tableName) == currentDatabase->tables.end())
    {
        std::cerr << "Table " << tableName << " does not exist." << std::endl;
        return;
    }

    size_t valuesStart = command.find("VALUES") + 6;
    size_t valuesEnd = command.find(';', valuesStart);
    if (valuesStart == std::string::npos || valuesEnd == std::string::npos)
    {
        std::cerr << "Invalid command." << std::endl;
        return;
    }
    
    std::string valuepart = command.substr(valuesStart, valuesEnd - valuesStart);
    valuepart = trim(valuepart);
    if (valuepart.front() != '(' || valuepart.back() != ')')
    {
        std::cerr << "Invalid command." << std::endl;
        return;
    }
    valuepart = valuepart.substr(1, valuepart.size() - 2);

    std::vector<std::string> localValues;
    size_t start = 0, end;
    while ((end = valuepart.find(',', start)) != std::string::npos)
    {
        localValues.push_back(trim(valuepart.substr(start, end - start)));
        start = end + 1;
    }
    localValues.push_back(trim(valuepart.substr(start)));

    auto &table = currentDatabase->tables[tableName];
    if (localValues.size() != table.columns.size())
    {
        std::cerr << "Error: Number of values does not match number of columns." << std::endl;
        return;
    }

    Record newRecord;
    bool valid = true;
    for (size_t i = 0; i < table.columns.size(); i++)
    {
        const Column &column = table.columns[i];
        const std::string &value = localValues[i];

        if ((column.type == "INTEGER" && !isInteger(value)) ||
            (column.type == "FLOAT" && !isFloat(value)))
        {
            std::cerr << "Error: Invalid value for " << column.type << " column." << std::endl;
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

bool MiniDB::isInteger(const std::string &value)
{
    try
    {
        std::stoi(value);
        return true;
    }
    catch (const std::invalid_argument &)
    {
        return false;
    }
}

bool MiniDB::isFloat(const std::string &value)
{
    try
    {
        std::stof(value);
        return true;
    }
    catch (const std::invalid_argument &)
    {
        return false;
    }
}

// 函数 select 用于查询表中的记录
void MiniDB::select(const std::string &tableName, std::vector<std::string> &columns, const std::string &whereClause)
{
    if (currentDatabase == nullptr)
    {
        std::cout << "No database selected." << std::endl;
        return;
    }
    auto &db = *currentDatabase;
    if (db.tables.find(tableName) != db.tables.end())
    {
        Table &table = db.tables[tableName];
        if (columns[0] == "*")
        {
            for (size_t i = 0; i < table.columns.size(); ++i)
            {
                std::cout << table.columns[i].name;
                columns.push_back(table.columns[i].name);
                if (i < table.columns.size() - 1)
                {
                    std::cout << ",";
                }
            }
            std::cout << std::endl; 
        }
        std::vector<std::pair<std::string, std::pair<std::string, std::string>>> conditions;
        std::string logicalOperator = "AND"; 
        if (!whereClause.empty())
        {
            std::string whereClause1, whereClause2;
            std::vector<std::string> whereclauses;
            if (whereClause.find("AND") != std::string::npos)
            {
                whereClause1 = whereClause.substr(0, whereClause.find("AND") - 1);
                whereClause2 = whereClause.substr(whereClause.find("AND") + 4);
                logicalOperator = "AND";
                whereclauses.push_back(whereClause1);
                whereclauses.push_back(whereClause2);
            }
            else if (whereClause.find("OR") != std::string::npos)
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
                std::istringstream iss(where);
                std::string token, columnName, op, value;
                while (iss >> token)
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

            //检查记录是否满足WHERE子句
            if (evaluateCondition(tableName, record, conditions, logicalOperator))
            {
                for (size_t i = 0; i < columns.size(); ++i)
                {

                    auto it = find_if(table.columns.begin(), table.columns.end(), [&columns, i](const Column &c)
                                      { return c.name == columns[i]; });
                    if (it != table.columns.end())
                    {
                        //找到记录中列的索引并打印其值
                        size_t columnIndex = distance(table.columns.begin(), it);
                        if (table.columns[columnIndex].type == "FLOAT")
                        {
                            std::cout << std::fixed << std::setprecision(2) << stof(record.localValues[columnIndex]);
                        }
                        else
                        {
                            std::cout << record.localValues[columnIndex];
                        }
                        if (i < columns.size() - 1)
                        {
                            std::cout << ",";
                        }
                    }
                }
                std::cout << std::endl; 
            }
        }
    }
    std::cout << "---" << std::endl;
}
// 函数 innerJoin 用于内连接两个表
void MiniDB::innerJoin(const std::string &tableName1, const std::string &tableName2, const std::string &base1, const std::string &base2, const std::string &column1, const std::string &column2, const std::string &whereClause)
{
    if (currentDatabase == nullptr)
    {
        std::cout << "No database selected." << std::endl;
        return;
    }
    auto &db = *currentDatabase;

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
    if (it1 == table1.columns.end() || it2 == table2.columns.end())
    {
        std::cout << "Column does not exist." << std::endl;
        return;
    }
    size_t index1 = distance(table1.columns.begin(), it1);
    size_t index2 = distance(table2.columns.begin(), it2);
    size_t index3 = distance(table1.columns.begin(), it3);
    size_t index4 = distance(table2.columns.begin(), it4);
    std::string tableName;
    if (whereClause.empty())
    {
        std::cout << tableName1 << "." << column1 << "," << tableName2 << "." << column2 << std::endl;
        for (const auto &record1 : table1.records)
        {
            for (const auto &record2 : table2.records)
            {
                if (record1.localValues[index1] == record2.localValues[index2])
                {
                    std::cout << record1.localValues[index3] << "," << record2.localValues[index4] << std::endl;
                }
            }
        }
    }

    else
    {
        std::vector<std::pair<std::string, std::pair<std::string, std::string>>> conditions;
        std::string logicalOperator = "AND";
        if (!whereClause.empty())
        {

            std::string columnName, op, value;
            tableName = whereClause.substr(0, whereClause.find("."));
            columnName = whereClause.substr(whereClause.find(".") + 1, whereClause.find("=") - whereClause.find(".") - 1);
            columnName = columnName.erase(0, columnName.find_first_not_of(" \t\n;"));
            columnName = columnName.erase(columnName.find_last_not_of(" \t\n;") + 1);

            if (whereClause.find("=") != std::string::npos)
            {
                op = "=";
            }
            else if (whereClause.find(">") != std::string::npos)
            {
                op = ">";
            }
            else if (whereClause.find("<") != std::string::npos)
            {
                op = "<";
            }
            value = whereClause.substr(whereClause.find(op) + 1);
            value = trim(value);

            conditions.emplace_back(columnName, make_pair(op, value));
        }
        std::cout << tableName1 << "." << column1 << "," << tableName2 << "." << column2 << std::endl;
        for (const auto &record1 : table1.records)
        {
            for (const auto &record2 : table2.records)
            {
                if (record1.localValues[index1] == record2.localValues[index2] && evaluateCondition(tableName, record1, conditions, logicalOperator))
                {
                    std::cout << record1.localValues[index3] << "," << record2.localValues[index4] << std::endl;
                }
            }
        }
    }
    std::cout << "---" << std::endl;
}
// 函数 evaluateCondition 用于评估条件
void MiniDB::update(const std::string &tableName, const std::string &setclause, const std::string &whereClause)
{
    if (currentDatabase == nullptr)
    {
        std::cout << "No database selected." << std::endl;
        return;
    }
    auto &db = *currentDatabase;
    if (db.tables.find(tableName) == db.tables.end())
    {
        std::cout << "Table does not exist." << std::endl;
        return;
    }
    Table &table = db.tables[tableName];
    std::istringstream iss(setclause);
    std::string ope, change_columnName, content;
    iss >> change_columnName >> ope;
    content = setclause.substr(setclause.find(ope) + 1);
    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> conditions;
    std::string logicalOperator = "AND";
    if (!whereClause.empty())
    {
        std::istringstream iss(whereClause);
        std::string token, columnName, op, value;
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
        if (evaluateCondition(tableName, record, conditions, logicalOperator))
        {
            auto it = find_if(table.columns.begin(), table.columns.end(), [&change_columnName](const Column &c)
                              { return c.name == change_columnName; });
            if (it != table.columns.end())
            {
                size_t columnIndex = distance(table.columns.begin(), it);
                if (content.find("+") != std::string::npos || content.find("-") != std::string::npos || content.find("*") != std::string::npos || content.find("/") != std::string::npos)
                {
                    if (table.columns[columnIndex].type == "INTEGER")
                    {
                        int currentVal = stoi(record.localValues[columnIndex]);
                        int newVal = stoi(content.substr(content.length() - 1));
                        if (content.find("+") != std::string::npos)
                        {
                            record.localValues[columnIndex] = std::to_string(currentVal + newVal);
                        }
                        else if (content.find("-") != std::string::npos)
                        {
                            record.localValues[columnIndex] = std::to_string(currentVal - newVal);
                        }
                        else if (content.find("*") != std::string::npos)
                        {
                            record.localValues[columnIndex] = std::to_string(currentVal * newVal);
                        }
                        else if (content.find("/") != std::string::npos)
                        {
                            record.localValues[columnIndex] = std::to_string(currentVal / newVal);
                        }
                    }
                    else if (table.columns[columnIndex].type == "FLOAT")
                    {
                        float currentVal = stof(record.localValues[columnIndex]);
                        float newVal = stof(content.substr(content.find_last_of("+-") + 1));
                        float result;
                        if (content.find("+") != std::string::npos)
                        {
                            result = currentVal + newVal;
                        }
                        else if (content.find("-") != std::string::npos)
                        {
                            result = currentVal - newVal;
                        }
                        else if (content.find("*") != std::string::npos)
                        {
                            result = currentVal * newVal;
                        }
                        else if (content.find("/") != std::string::npos)
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
// 函数 deleteRecord 用于删除记录
void MiniDB::deleteRecord(const std::string &tableName, const std::string &whereClause)
{
    if (!currentDatabase)
    {
        std::cerr << "No database selected." << std::endl;
        return;
    }

    auto it = currentDatabase->tables.find(tableName);
    if (it == currentDatabase->tables.end())
    {
        std::cerr << "Table [" << tableName << "] does not exist." << std::endl;
        return;
    }

    Table &table = it->second;

    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> conditions;
    std::string logicalOperator = "AND";
    parseWhereClause(whereClause, conditions, logicalOperator);

    auto recordIt = table.records.begin();
    while (recordIt != table.records.end())
    {
        if (evaluateCondition(tableName, *recordIt, conditions, logicalOperator))
        {
            recordIt = table.records.erase(recordIt);
        }
        else
        {
            ++recordIt;
        }
    }

    saveDatabase(currentDatabase->name);
}
// 函数 parseWhereClause 用于解析 WHERE 子句
void MiniDB::parseWhereClause(const std::string &whereClause, std::vector<std::pair<std::string, std::pair<std::string, std::string>>> &conditions, std::string &logicalOperator)
{
    if (whereClause.empty())
        return;

    std::istringstream iss(whereClause);
    std::string token, columnName, op, value;
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
            value = trim(value);
            if (!value.empty() && value.front() == '\'' && value.back() == '\'')
            {
                value = value.substr(1, value.length() - 2);
            }
            conditions.emplace_back(columnName, std::make_pair(op, value));
        }
    }
}

// 函数 evaluateCondition 用于评估条件
bool MiniDB::evaluateCondition(const std::string &tableName, const Record &record, const std::vector<std::pair<std::string, std::pair<std::string, std::string>>> &conditions, const std::string &logicalOperator)
{
    auto &table = currentDatabase->tables[tableName];
    bool result = true; 

    for (size_t i = 0; i < conditions.size(); ++i)
    {
        const auto &cond = conditions[i];
        std::string columnName = cond.first;
        std::string op = cond.second.first;
        std::string value = cond.second.second;
        if (value.front() == '\'' && value.back() == '\'')
        {
            value = value.substr(1, value.length() - 2);
        }

      
        auto colIt = find_if(table.columns.begin(), table.columns.end(),
                             [&columnName](const Column &col)
                             { return col.name == columnName; });

        if (colIt == table.columns.end())
        {
            std::cerr << "Column not found: " << columnName << std::endl;
            return false;
        }

        size_t columnIndex = distance(table.columns.begin(), colIt);

        const Column &column = table.columns[columnIndex];
        bool conditionResult = false;

        if (column.type == "TEXT")
        {
            std::string tempValue = record.localValues[columnIndex];
            if (tempValue.front() == '\'' && tempValue.back() == '\'')
            {
                tempValue = tempValue.substr(1, tempValue.length() - 2); 
            }

            if (op == "=")
            {
                conditionResult = (tempValue == value);
            }
        }
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

        if (i == 0)
        {
            result = conditionResult;
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