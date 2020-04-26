#include <iostream>
#include <queue>
#include <vector>

struct Request {
    Request(int arrival_time, int process_time):
        arrival_time(arrival_time),
        process_time(process_time)
    {}

    int arrival_time;
    int process_time;
};

struct Response {
    Response(bool dropped, int start_time):
        dropped(dropped),
        start_time(start_time)
    {}

    bool dropped;
    int start_time;
};

class Buffer {
public:
    Buffer(int size):
        size_(size),
        q()
    {}

    Response Process(const Request &request) {
        if(q.empty()) {
            q.push(request.arrival_time + request.process_time);
            Response processed = Response(false, request.arrival_time);
            return processed;
        } 

        for(int i = 0; i < q.size(); i++) {
            if(q.front() <= request.arrival_time) {
                q.pop();
            }
        }
        
        int finishedTime = q.back();
        if(request.arrival_time < finishedTime && q.size() == size_) {
            Response dropped = Response(true, request.arrival_time);
            return dropped;
        } else {
            int startTime = q.empty() ? request.arrival_time : finishedTime;
            q.push(startTime + request.process_time);
            Response processed = Response(false, startTime);
            return processed;
        }
    }

private:
    int size_;
    std::queue <int> q;
};

std::vector <Request> ReadRequests() {
    std::vector <Request> requests;
    int count;
    std::cin >> count;
    for (int i = 0; i < count; ++i) {
        int arrival_time, process_time;
        std::cin >> arrival_time >> process_time;
        requests.push_back(Request(arrival_time, process_time));
    }
    return requests;
}

std::vector <Response> ProcessRequests(const std::vector <Request> &requests, Buffer *buffer) {
    std::vector <Response> responses;
    for (int i = 0; i < requests.size(); ++i)
        responses.push_back(buffer->Process(requests[i]));
    return responses;
}

void PrintResponses(const std::vector <Response> &responses) {
    for (int i = 0; i < responses.size(); ++i)
        std::cout << (responses[i].dropped ? -1 : responses[i].start_time) << std::endl;
}

int main() {
    int size;
    std::cin >> size;
    std::vector <Request> requests = ReadRequests();

    Buffer buffer(size);
    std::vector <Response> responses = ProcessRequests(requests, &buffer);

    PrintResponses(responses);
    return 0;
}
