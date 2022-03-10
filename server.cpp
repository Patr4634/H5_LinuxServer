#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "functions.cpp"

using namespace std;

using chrono::high_resolution_clock;
using chrono::duration_cast;
using chrono::duration;
using chrono::milliseconds;

void FindQuery(string, string*);
void SuggestWord(string, string*);

char header_array[] = "HTTP/1.1 200 OK\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "Content-Type: text/html; charset=UTF-8\r\n\r\n";

void StartServer()
{
    // 1. instance socket
    int one = 1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cout << "Could not bind socket";
        return;
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

    // 2. setup server address, include netinet/in.h
    int port = 8001;
    struct sockaddr_in svr_addr;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr.s_addr = INADDR_ANY;
    svr_addr.sin_port = htons(port);

    // 3. bind socket and serveraddress, include <unistd.h>
    if (bind(sock, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) == -1)
    {
        close(sock);
        cout << "Could not bind port - port is probably taken";
    }

    // 4. Listen to socket
    listen(sock, 5);

    // 5. wait on client connection
    int client_fd;
    struct sockaddr_in cli_addr;
    socklen_t sin_len = sizeof(cli_addr);
    char buffer[1024] = {0};

    cout << "Waiting for connection" << endl;
    while (1)
    {
        client_fd = accept(sock, (struct sockaddr *)&cli_addr, &sin_len);
        cout << "New connection: " << client_fd << endl;

        if (client_fd == -1)
        {
            cout << "Connection error:" << endl;
        }

        // 6. read from client
        read(client_fd, buffer, 1024);
        string header(buffer);
        // cout << header;

        // 6.1 find query
        string query;
        FindQuery(header, &query);

        // 6.2 suggest word based on query
        string word;
        vector<int> timetable;

        auto time_1 = high_resolution_clock::now();
        SuggestWord(query, &word);
        auto time_2 = high_resolution_clock::now();
        auto miliseconds = duration_cast<milliseconds>(time_2 - time_1);
        timetable.push_back(miliseconds.count());

        int avg = 0;
        int sum = 0;
        for (int i = 0; i < timetable.size(); ++i)
        {
            sum += timetable[i];
        }
        avg = sum / timetable.size();
        cout << "Average time: " << to_string(avg) << endl;

        // 7. write to client
        string html = "You searched: " + query + "<br>Did you mean: " + word + "?" + "<br>It took: " + to_string(avg) + "ms\n";
        char response[sizeof(header_array) + html.size()];
        int i = 0;

        for (char c : header_array)
        {
            response[i++] = c;
        }

        for (char c : html)
        {
            response[i++] = c;
        }

        write(client_fd, response, sizeof(response));
        // cout << response << sizeof(response) << endl;
        close(client_fd);
    }
}

void FindQuery(string header, string *query)
{
    int n = header.length();
    char header_array_request[n + 1];
    strcpy(header_array_request, header.c_str());
    char keys[] = "q=";
    string respond = strpbrk(header_array_request, keys);
    respond = respond.substr(respond.find("q=") + 2);

    for (int i = 0; i < respond.length(); i++)
    {
        int whitespace = isspace(respond[i]);

        if (whitespace == 8192)
        {
            break;
        }
        else
        {
            query->push_back(respond[i]);
        }
    }
}

void SuggestWord(string query, string *word)
{
    vector<string> dictinary;
    dictinary.push_back("beans");
    dictinary.push_back("jeans");

    vector<string> candidates;
    vector<char> chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    AddLetters(query, chars, &candidates);
    RemoveLetters(query, &candidates);
    ReplaceLetters(query, chars, &candidates);
    SwapLetters(query, &candidates);
    Squared(chars, &candidates);

    bool search_done = false;
    for (int i = 0; i < candidates.size(); i++)
    {
        if (search_done)
        {
            break;
        }

        for (int j = 0; j < dictinary.size(); j++)
        {
            if (candidates[i] == dictinary[j])
            {
                word->insert(0, dictinary[j]);
                search_done = true;
            }

            if (search_done)
            {
                break;
            }
        }
    }

    search_done = false;
}