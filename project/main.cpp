#include <iostream>
#include "data_manager.hpp"
#include "removespace.hpp"
using namespace std;

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