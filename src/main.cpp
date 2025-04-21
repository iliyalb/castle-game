#include <iostream>
#include <boost/asio.hpp>
#include <sqlite3.h>

int main()
{
    try {
        // Test Boost.Asio
        boost::asio::io_context io_context;
        
        // Test SQLite
        sqlite3* db;
        int rc = sqlite3_open(":memory:", &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return 1;
        }
        
        std::cout << "Successfully initialized Boost.Asio and SQLite3!" << std::endl;
        
        sqlite3_close(db);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
