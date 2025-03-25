#include "dmdatabase.h"
#include <iostream>

using namespace std;
using namespace DocketMinder;

std::vector<std::string> split(const std::string &s, char delimiter);

int main()
{
    cout << "Hello World!" << endl;

    DocketMinder::DMDatabase *dm = new DocketMinder::DMDatabase();
    // Map to store member function pointers
    std::map<std::string, std::function<void(DocketMinder::DMDatabase *, const std::vector<std::string> &)>>
        functionMap;

    // Populate the map with method names and corresponding lambdas
    functionMap["Method1"] = [](DmDatabase *dm, const std::vector<std::string> &params)
    {
        if (params.size() != 1)
            throw std::runtime_error("Method1 requires 1 parameter");
        dm->Method1(std::stoi(params[0]));
    };

    functionMap["Method2"] = [](DmDatabase *dm, const std::vector<std::string> &params)
    {
        if (params.size() != 1)
            throw std::runtime_error("Method2 requires 1 parameter");
        dm->Method2(params[0]);
    };

    functionMap["Method3"] = [](DmDatabase *dm, const std::vector<std::string> &params)
    {
        if (params.size() != 2)
            throw std::runtime_error("Method3 requires 2 parameters");
        dm->Method3(std::stoi(params[0]), params[1]);
    };

    std::ifstream scriptFile("test.script");
    std::string line;

    if (!scriptFile.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    while (std::getline(scriptFile, line))
    {
        auto parts = split(line, ',');
        if (parts.size() < 1)
        {
            std::cerr << "Invalid line format: " << line << std::endl;
            continue;
        }

        std::string methodName = parts[0];
        std::vector<std::string> params(parts.begin() + 1, parts.end());

        try
        {
            auto func = functionMap.at(methodName);
            func(dm, params);
        }
        catch (const std::out_of_range &)
        {
            std::cerr << "Unknown method: " << methodName << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error calling method " << methodName << ": " << e.what() << std::endl;
        }
    }

    delete dm;
    return 0;
}

// Helper function to split a string by a delimiter
std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}
