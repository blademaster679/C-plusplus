#include <iostream>
#include "data_manager.hpp"
#include "removespace.hpp"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    MiniDB minidb;
    string command;
    string fullcommand;
    string fileName = argv[1];
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
        command = trim(command);
        try
        {
            if (command.find("CREATE DATABASE") != string::npos)
            {
                string dbName = command.substr(16);
                dbName = dbName.substr(0, dbName.size() - 1); // 去除分号
                minidb.createDatabase(dbName);
            }
            else if (command.find("USE DATABASE") != string::npos)
            {
                static string lastUsedDatabase = "";
                string dbName = command.substr(13);
                dbName = dbName.substr(0, dbName.size() - 1); 
                if (dbName == lastUsedDatabase)
                {
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
                tableName = tableName.substr(0, tableName.size() - 1); 
                minidb.dropTable(tableName);
            }
            else if (command.find("INSERT INTO") != string::npos)
            { 
                string tableName = command.substr(12);
                tableName = tableName.substr(0, tableName.find("VALUES") - 1);
                tableName = trim(tableName);
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

                // 检查是否选择所有列
                if (columncommand.find("*") != string::npos)
                {
                    columns.push_back("*");
                }
                else
                {
                    istringstream columnStream(columncommand);
                    while (getline(columnStream, column, ','))
                    {
                        column = trim(column);
                        columns.push_back(column);
                    }
                }

               
                string tableName = "";
                string whereClause = "";
                string joinClause = "";

                if (command.find("WHERE") != string::npos)
                {
                    tableName = command.substr(command.find("FROM") + 5, command.find("WHERE") - command.find("FROM") - 6);
                    whereClause = command.substr(command.find("WHERE") + 6);
                    whereClause = whereClause.substr(0, whereClause.size() - 1); 
                }
                else
                {
                    tableName = command.substr(command.find("FROM") + 5);
                    tableName = tableName.substr(0, tableName.size() - 1); 
                }


               
                minidb.select(tableName, columns, whereClause);
            }
            else if (command.find("SELECT") != string::npos && command.find(";") == string::npos)
            {
                fullcommand = command;
            
                while (getline(inputFile, command))
                {
                    fullcommand += command;
                    if (command.find(";") != string::npos)
                    {
                        break;
                    }
                }
               
                string selectcommand = fullcommand.substr(fullcommand.find("SELECT") + 6, fullcommand.find("FROM") - command.find("SELECT") - 7);

                
                string table1 = selectcommand.substr(0, selectcommand.find(","));
                string tableName1 = table1.substr(0, table1.find("."));
                string tablecolumn1 = table1.substr(table1.find(".") + 1);

                string table2 = selectcommand.substr(selectcommand.find(",") + 1);
                string tableName2 = table2.substr(0, table2.find("."));
                string tablecolumn2 = table2.substr(table2.find(".") + 1);

          
                tableName1 = trim(tableName1);
                tableName2 = trim(tableName2);
                tablecolumn1 = trim(tablecolumn1);
                tablecolumn2 = trim(tablecolumn2);
               
                string fromcommand = fullcommand.substr(fullcommand.find("FROM") + 5, fullcommand.find("INNER JOIN") - fullcommand.find("FROM") - 5);

          
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
                base3 = trim(base3);
                base4 = trim(base4);
                minidb.innerJoin(tableName1, tableName2, base3, base4, tablecolumn1, tablecolumn2, whereClause);
            }
            else if (command.find("UPDATE") != string::npos)
            {
                string tableName = command.substr(6, command.find("SET") - 6);
                tableName = trim(tableName);
                string setClause = command.substr(command.find("SET") + 4, command.find("WHERE") - command.find("SET") - 4);
                setClause = trim(setClause);
                string whereClause = command.substr(command.find("WHERE") + 6);
                whereClause = whereClause.substr(0, whereClause.size() - 1); // 去除分号
                minidb.update(tableName, setClause, whereClause);
            }
            else if (command.find("DELETE") != string::npos)
            {
                string tableName = command.substr(12, command.find("WHERE") - 12);
                tableName = trim(tableName);
                string whereClause = command.substr(command.find("WHERE") + 6, command.size() - command.find("WHERE") - 7);
                minidb.deleteRecord(tableName, whereClause);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n'; // 错误处理
        }
    }
    cout.rdbuf(coutbuf);
    inputFile.close();
    output.close();

    return 0;
}