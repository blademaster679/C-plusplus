#include "data_manager.hpp"
#include "removespace.hpp"

// Function to join elements of a vector with a delimiter
std::string join(const std::vector<std::string> &elements, const std::string &delimiter)
{
    std::ostringstream os;
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

void Table::addColumn(const std::string &columnName, const std::string &columnType){
    columns.push_back({columnName, columnType});
};

void Database::addTable(const std::string &tableName){
    tables[tableName] = Table(tableName);
};

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

void MiniDB::useDatabase(const std::string &DBname) {
        std::string dbName = DBname;
        if (databases.find(dbName) != databases.end())
        {
            currentDatabase = &databases[dbName];

            loadDatabase(dbName);
        }
    }

void MiniDB::createTable(const std::string &command){

        const std::string keyword = "CREATE TABLE";
        if (command.find(keyword) != 0)
        {
            std::cout << "Invalid command. Expected 'CREATE TABLE'." << std::endl;
            return;
        }

        size_t start = keyword.length();
        size_t end = command.find('(', start); // find the opening parenthesis
        if (end == std::string::npos)
        {
            std::cout << "Invalid command. Missing '(' after table name." << std::endl;
            return;
        }

        // extract table name
        std::string tableName = command.substr(start, end - start);
        // tableName = removeSpaces(tableName);
        tableName.erase(0, tableName.find_first_not_of(" \t\n"));
        tableName.erase(tableName.find_last_not_of(" \t\n") + 1);

        if (tableName.empty())
        {
            std::cout << "Invalid command. Table name is empty." << std::endl;
            return;
        }

        // check if table already exists
        if (currentDatabase->tables.find(tableName) != currentDatabase->tables.end())
        {
            std::cout << "Table " << tableName << " already exists." << std::endl;
            return;
        }

        Table newTable(tableName);
        size_t columnStart = end + 1;

        // find the end of column definitions
        size_t columnEnd = command.find(')', columnStart);
        if (columnEnd == std::string::npos)
        {
            std::cout << "Invalid command. Missing ')' at the end of column definitions." << std::endl;
            return;
        }

        // extract column definitions
        std::string columns = command.substr(columnStart, columnEnd - columnStart);
        // columns = removeSpaces(columns);
        // erase leading and trailing whitespaces
        columns.erase(0, columns.find_first_not_of(" \t\n"));
        columns.erase(columns.find_last_not_of(" \t\n") + 1);

        // deal with multi-line column definitions
        std::istringstream iss(columns);
        std::string columnDef;

        while (getline(iss, columnDef, ','))
        {
            // columnDef = removeSpaces(columnDef);
            // erase leading and trailing whitespaces
            columnDef.erase(0, columnDef.find_first_not_of(" \t\n"));
            columnDef.erase(columnDef.find_last_not_of(" \t\n") + 1);

            if (columnDef.empty())
                continue; // skip empty column definitions

            // extract column name and type
            std::istringstream colStream(columnDef);
            std::string columnName, columnType;
            colStream >> columnName >> columnType;

            // check if column type is valid
            if (columnType != "INTEGER" && columnType != "TEXT" && columnType != "FLOAT")
            {
                std::cout << "Invalid column type: " << columnType << std::endl;
                return;
            }

            // add column to table
            newTable.addColumn(columnName, columnType);
        }

        // add table to current database
        currentDatabase->tables[tableName] = newTable;
        saveDatabase(currentDatabase->name);
    }

void MiniDB::dropTable(const std::string &tableName){
    if (currentDatabase == nullptr)
        {
            std::cout << "No database selected." << std::endl;
            return;
        }
        auto &db = *currentDatabase;

        if (db.tables.find(tableName) == db.tables.end())
        {
            std::cout << "Table [" << tableName << "] does not exist." << std::endl;
            return;
        }
        db.tables.erase(tableName);

        saveDatabase(currentDatabase->name);
}

void MiniDB::loadDatabase(const std::string &DBname){
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
            if (line.empty())
            {
                continue;
            }
            if (line.find("TABLE") == 0)
            {
                std::string tableName = line.substr(6);
                // tableName = removeSpaces(tableName);
                tableName.erase(0, tableName.find_first_not_of(" \t\n"));
                tableName.erase(tableName.find_last_not_of(" \t\n") + 1);
                loadedDb.tables[tableName] = Table(tableName);
                currentTable = &loadedDb.tables[tableName];
            }
            else if (currentTable != nullptr && line.find(')') == std::string::npos)
            {
                std::istringstream iss(line);
                std::string columnName, columnType;
                iss >> columnName >> columnType;
                if (columnType == "INTEGER" || columnType == "TEXT" || columnType == "FLOAT")
                {
                    currentTable->addColumn(columnName, columnType);
                }
                else
                {
                    std::cout << "Warning: Invalid column type '" << columnType << "' in table '" << currentTable->name << "'. Skipping column." << std::endl;
                }
            }
            else if (currentTable != nullptr && line.find("INSERT INTO") == 0)
            {
                Record record;
                std::istringstream iss(line);
                std::string value;
                while (getline(iss, value, ','))
                {
                    // value = removeSpaces(value);
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

void MiniDB::saveDatabase(const std::string &DBname){
    std::ofstream dbFile(DBname + ".txt");
        if (!dbFile.is_open())
        {
            error("Failed to open file for saving.");
            return;
        }

        auto &db = databases[DBname];
        dbFile << "CREATE DATABASE " << DBname << ";" << std::endl;
        // Iterate through tables
        for (const auto &tablePair : db.tables)
        {
            const Table &table = tablePair.second;
            dbFile << "CREATE TABLE " << table.name << ";" << std::endl;

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
            dbFile << ");" << std::endl;

            // Write records
            for (const auto &record : table.records)
            {
                dbFile << "INSERT INTO " << table.name << " VALUES (";
                dbFile << join(record.localValues, ",") << std::endl;
            }
            dbFile << ");" << std::endl;
        }

        dbFile.close();
}

void MiniDB::insertIntoTable(const std::string &command, const std::string &tableName, const std::vector<std::string> &values){
    // check if the database is empty
        if (currentDatabase == nullptr)
        {
            std::cout << "No database selected." << std::endl;
            return;
        }

        // check if the table exists in the current database

        if (currentDatabase->tables.find(tableName) == currentDatabase->tables.end())
        {
            std::cout << "Table " << tableName << " does not exist." << std::endl;
            return;
        }
        // extract values from the command
        size_t valuesStart = command.find("VALUES") + 6;
        size_t valuesEnd = command.find(';', valuesStart);
        if (valuesStart == std::string::npos || valuesEnd == std::string::npos)
        {
            std::cout << "Invalid command." << std::endl;
            return;
        }
        std::string valuepart = command.substr(valuesStart, valuesEnd - valuesStart);
        // valuepart = removeSpaces(valuepart);
        valuepart.erase(0, valuepart.find_first_not_of(" \t\n"));
        valuepart.erase(valuepart.find_last_not_of(" \t\n") + 1);
        if (valuepart.front() != '(' || valuepart.back() != ')')
        {
            std::cout << "Invalid command." << std::endl;
            return;
        }
        valuepart = valuepart.substr(1, valuepart.size() - 2);

        std::istringstream iss(valuepart);
        std::string value;
        std::vector<std::string> localValues;

        while (getline(iss, value, ','))
        {
            // remove leading and trailing whitespace
            // value = removeSpaces(value);
            value.erase(0, value.find_first_not_of(" \t\n"));
            value.erase(value.find_last_not_of(" \t\n") + 1);

            localValues.push_back(value);
        }
        // check if the number of values matches the number of columns
        auto &table = currentDatabase->tables[tableName];

        if (localValues.size() != table.columns.size())
        {
            std::cout << "Error: Number of values does not match number of columns." << std::endl;
            return;
        }
        // insert the record
        Record newRecord;
        bool valid = true;
        for (size_t i = 0; i < table.columns.size(); i++)
        {
            const Column &column = table.columns[i];
            const std::string &value = localValues[i];
            // type checking
            if (column.type == "INTEGER")
            {
                try
                {
                    stoi(value);
                }
                catch (const std::invalid_argument &)
                {
                    std::cout << "Error: Invalid value for INTEGER column." << std::endl;
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
                catch (const std::invalid_argument &)
                {
                    std::cout << "Error: Invalid value for FLOAT column." << std::endl;
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
                std::cout << "Error: Invalid column type." << column.type << std::endl;
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

void MiniDB::select(const std::string &tableName, std::vector<std::string> &columns, const std::string &whereClause){
    if (currentDatabase == nullptr)
        {
            std::cout << "No database selected." << std::endl;
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
                    std::cout << table.columns[i].name;
                    columns.push_back(table.columns[i].name);
                    if (i < table.columns.size() - 1)
                    {
                        std::cout << ",";
                    }
                }
                std::cout << std::endl; // New line after column names
            }

            // Parse the WHERE clause into conditions and logical operators
            std::vector<std::pair<std::string, std::pair<std::string, std::string>>> conditions;
            std::string logicalOperator = "AND"; // Default to AND if no logical operator is found
            // Handle both simple and complex WHERE clauses
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
                    std::cout << std::endl; // New line after each record
                }
            }
        }
        std::cout << "---" << std::endl;
}

void MiniDB::innerJoin(const std::string &tableName1, const std::string &tableName2, const std::string &base1, const std::string &base2, const std::string &column1, const std::string &column2, const std::string &whereClause){
    // Check if the database is empty
        if (currentDatabase == nullptr)
        {
            std::cout << "No database selected." << std::endl;
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
            std::cout << "Column does not exist." << std::endl;
            return;
        }
        // Find the index of the base columns
        size_t index1 = distance(table1.columns.begin(), it1);
        size_t index2 = distance(table2.columns.begin(), it2);
        size_t index3 = distance(table1.columns.begin(), it3);
        size_t index4 = distance(table2.columns.begin(), it4);
        std::string tableName;
        // Print the header
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
            // Parse the WHERE clause
            std::vector<std::pair<std::string, std::pair<std::string, std::string>>> conditions;
            std::string logicalOperator = "AND";
            if (!whereClause.empty())
            {

                std::string columnName, op, value;
                tableName = whereClause.substr(0, whereClause.find("."));
                columnName = whereClause.substr(whereClause.find(".") + 1, whereClause.find("=") - whereClause.find(".") - 1);
                // columnName = removeSpaces(columnName);
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
                // value = removeSpaces(value);
                value = value.erase(0, value.find_first_not_of(" \t\n;"));
                value = value.erase(value.find_last_not_of(" \t\n;") + 1);

                conditions.emplace_back(columnName, make_pair(op, value));
            }
            std::cout << tableName1 << "." << column1 << "," << tableName2 << "." << column2 << std::endl;
            for (const auto &record1 : table1.records)
            {
                for (const auto &record2 : table2.records)
                {
                    // Evaluate the WHERE clause conditions for both tables
                    if (record1.localValues[index1] == record2.localValues[index2] && evaluateCondition(tableName, record1, conditions, logicalOperator))
                    {
                        std::cout << record1.localValues[index3] << "," << record2.localValues[index4] << std::endl;
                    }
                }
            }
        }
        std::cout << "---" << std::endl;
}

void MiniDB::update(const std::string &tableName, const std::string &setclause, const std::string &whereClause){
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
        // Parse the SET clause
        std::istringstream iss(setclause);
        std::string ope, change_columnName, content;
        iss >> change_columnName >> ope;
        content = setclause.substr(setclause.find(ope) + 1);
        // Parse the WHERE clause
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
            // Check if the record satisfies the WHERE clause
            if (evaluateCondition(tableName, record, conditions, logicalOperator))
            {
                auto it = find_if(table.columns.begin(), table.columns.end(), [&change_columnName](const Column &c)
                                  { return c.name == change_columnName; });
                if (it != table.columns.end())
                {
                    // Find the index of the column in the record and print its value
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

void MiniDB::deleteRecord(const std::string &tableName, const std::string &whereClause){
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
        // Parse the WHERE clause
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

bool MiniDB::evaluateCondition(const std::string &tableName, const Record &record, const std::vector<std::pair<std::string, std::pair<std::string, std::string>>> &conditions, const std::string &logicalOperator)
{
    auto &table = currentDatabase->tables[tableName];
        bool result = true; // Start with true for AND conditions

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

            // Find the column index
            auto colIt = find_if(table.columns.begin(), table.columns.end(),
                                 [&columnName](const Column &col)
                                 { return col.name == columnName; });

            if (colIt == table.columns.end())
            {
                std::cerr << "Column not found: " << columnName << std::endl;
                return false;
            }

            size_t columnIndex = distance(table.columns.begin(), colIt);

            // Evaluate condition based on column type
            const Column &column = table.columns[columnIndex];
            bool conditionResult = false;

            // Check for string type (TEXT)
            if (column.type == "TEXT")
            {
                std::string tempValue = record.localValues[columnIndex];
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