#include <iostream>
#include <chrono>
#include <thread>

#include "derecho/experiments/aggregate_bandwidth.h"
#include "derecho/experiments/log_results.h"
#include "multicast.h"

using namespace std;
using namespace sst;

volatile bool done = false;

int main() {
    srand(time(NULL));
    constexpr uint max_msg_size = 10, window_size = 1000;
    const unsigned int num_messages = 10;
    // input number of nodes and the local node id
    uint32_t node_id, num_nodes;
    cin >> node_id >> num_nodes;

    // input the ip addresses
    map<uint32_t, string> ip_addrs;
    for(unsigned int i = 0; i < num_nodes; ++i) {
        cin >> ip_addrs[i];
    }

    // initialize the rdma resources
    verbs_initialize(ip_addrs, node_id);

    std::vector<uint32_t> members(num_nodes);
    for(uint i = 0; i < num_nodes; ++i) {
        members[i] = i;
    }

    uint num_finished = 0;
    struct timespec start_time, end_time;
    group<max_msg_size> g(
        members, node_id, window_size, [&num_finished, &num_nodes, &num_messages](
                                           uint32_t sender_rank, uint64_t index,
                                           volatile char* msg, uint32_t size) {
	  cout << "Message received: " << (const_cast<char*>(msg)) << endl;
            if(index == num_messages - 1) {
                num_finished++;
            }
            if(num_finished == num_nodes) {
                done = true;
            }
        });
    // start timer
    clock_gettime(CLOCK_REALTIME, &start_time);
    for(uint i = 0; i < num_messages; ++i) {
        volatile char* buf;
        while((buf = g.get_buffer(max_msg_size)) == NULL) {
        }
        for(uint i = 0; i < max_msg_size; ++i) {
	  buf[i] = 'a' + ((rand() + node_id) % 26);
        }
	cout << "Message sent: " << (const_cast<char*>(buf)) << endl;
        g.send();
        std::this_thread::sleep_for(1s);
    }
    // cout << "Done sending" << endl;
    while(!done) {
    }
    // end timer
    clock_gettime(CLOCK_REALTIME, &end_time);    
    // double my_time = ((end_time.tv_sec * 1e9 + end_time.tv_nsec) -
    // (start_time.tv_sec * 1e9 + start_time.tv_nsec));
    // double message_rate = (num_messages * 1e9) / my_time;

    // double sum_message_rate =
    //     aggregate_bandwidth(members, node_id, message_rate * num_nodes);
    // log_results(num_nodes, max_msg_size, sum_message_rate, "data_multicast");
    sync (1-node_id);
}