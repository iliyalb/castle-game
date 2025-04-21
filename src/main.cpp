#include "server/castle_server.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
    try
    {
        unsigned short port = 12345;
        if (argc > 1)
        {
            port = static_cast<unsigned short>(std::stoi(argv[1]));
        }

        boost::asio::io_context io_context;
        CastleServer server(io_context, port);

        std::cout << R"(
            _________                  __  .__             _________                                
            \_   ___ \_____    _______/  |_|  |   ____    /   _____/ ______________  __ ___________ 
            /    \  \/\__  \  /  ___/\   __\  | _/ __ \   \_____  \_/ __ \_  __ \  \/ // __ \_  __ \
            \     \____/ __ \_\___ \  |  | |  |_\  ___/   /        \  ___/|  | \/\   /\  ___/|  | \/
             \______  (____  /____  > |__| |____/\___  > /_______  /\___  >__|    \_/  \___  >__|   
                    \/     \/     \/                 \/          \/     \/                 \/
            )" << std::endl;

        std::cout << "Castle Game Server starting on port " << port << std::endl;
        server.start();

        // Run the server with multiple threads
        const int num_threads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads - 1; ++i)
        {
            threads.emplace_back([&io_context]()
                                 { io_context.run(); });
        }

        // Main thread also runs the io_context
        io_context.run();

        // Wait for all threads to complete
        for (auto &thread : threads)
        {
            thread.join();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
