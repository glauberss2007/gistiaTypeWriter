#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "cppconn/driver.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"

#include "driver/mysql_connection.h"
#include "driver/mysql_public_iface.h"

void deserealize(std::ifstream& fin) {

    std::vector<std::string> asset_being_sold, asset_being_purchased, account_owner;
    std::vector<double> dollar_value;

    if (fin.is_open()) {
        std::string line;
        while (std::getline(fin, line)) {
            std::stringstream ss(line);
            std::string s, p, o, d;

            std::getline(ss, s, ',');
            std::getline(ss, p, ',');
            std::getline(ss, d, ',');
            std::getline(ss, o, ',');

            asset_being_sold.push_back(s);
            asset_being_purchased.push_back(p);
            double d_string_to_double = std::stod(d);
            dollar_value.push_back(d_string_to_double);
            account_owner.push_back(o);
        }

// print values serealized
        int size = 25000;
        int duplication_counter = 0;
        for (int i = 0; i < size; i++) {
            std::cout << asset_being_sold[i] << ", " << asset_being_purchased[i] << ", " <<
                      dollar_value[i] << ", " << account_owner[i] << std::endl;
        }
    }
}

int main() {

    std::string instance_path = "C:\\dataset.txt";
    std::ifstream fin(instance_path, std::ios::in);

    deserealize(fin);

    sql::Driver *driver;
    sql::Connection *con;

    driver = sql::mysql::get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "admin", "admindb");
    con->setSchema("development");

    sql::Statement *stmt;
    stmt = con->createStatement();
    stmt->execute("select distinct asset_being_sold, asset_being_purchased from dataset");
    stmt->execute("SELECT account_owner,COUNT(*) FROM dataset GROUP BY account_owner HAVING count(*)>500 ORDER BY COUNT(*) DESC;");
    stmt->execute("SELECT account_owner, AVG(dollar_value) AS avg_transaction FROM dataset GROUP BY account_owner ORDER BY avg_transaction DESC");
}

