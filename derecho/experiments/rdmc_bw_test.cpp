#include <atomic>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <time.h>

#include "rdmc/rdmc.h"
#include "rdmc/util.h"
#include "block_size.h"
#include "aggregate_bandwidth.h"
#include "log_results.h"
#include "initialize.h"

#include "rdmc/rdmc.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main(int argc, char *argv[]) {
  // srand(time(NULL));
    uint32_t node_rank;
    uint32_t num_nodes;

    std::map<uint32_t, std::string> node_addresses;

    rdmc::query_addresses(node_addresses, node_rank);
    num_nodes = node_addresses.size();

    bool success = rdmc::initialize(node_addresses, node_rank);
    if(!success) {
        std::cout << "Failed RDMC initialization" << std::endl;
        std::cout << "Exiting" << std::endl;
    }

    // size of one message
    long long int msg_size = atoll(argv[1]);
    // set block size
    const size_t block_size = get_block_size(msg_size);
    // size of the buffer
    const size_t buffer_size = msg_size;

    // create the vector of members - node 0 is the sender
    vector<uint32_t> members(num_nodes);
    for(uint32_t i = 0; i < num_nodes; i++) {
        members[i] = i;
    }

    vector<std::unique_ptr<char[]>> buffers;
    vector<std::shared_ptr<rdma::memory_region>> mrs;

    assert(num_nodes == 2);
    // vector<int> counts(num_nodes, 0);
    volatile int count1, count2;
    int num_messages = 1000;
    // type of send algorithm
    rdmc::send_algorithm type = rdmc::BINOMIAL_SEND;

    vector<uint32_t> rotated_members(num_nodes);
    for(unsigned int i = 0; i < num_nodes; ++i) {
        for(unsigned int j = 0; j < num_nodes; ++j) {
            rotated_members[j] = (uint32_t)members[(i + j) % num_nodes];
        }
        // buffer for the message - received here by the receivers and generated
        // here by the sender
        std::unique_ptr<char[]> buffer(new char[buffer_size]);
        auto mr =
            std::make_shared<rdma::memory_region>(buffer.get(), buffer_size);
        buffers.push_back(std::move(buffer));
        mrs.push_back(mr);

        // create the group
        success = rdmc::create_group(
            i, rotated_members, block_size, type,
            [&mrs, i](size_t length) -> rdmc::receive_destination {
                return {mrs[i], 0};
            },
            [&count1, &count2, i](char *data, size_t size) {
	      // for (uint i = 0; i < size; ++i) {
	      // 	cout << data[i];
	      // }
	      // cout << endl;
	      cout << "In completion callback" << endl;
	      if (i == 0) {
		++count1;
	      }
	      else {
		++count2;
	      }
	    },
            [](std::experimental::optional<uint32_t>) {});
        if(!success) {
            std::cout << "Failed RDMC group creation" << std::endl;
            std::cout << "Exiting" << std::endl;
        }
    }

    cout << "Group creation successful" << endl;
    
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    for(int i = 0; i < num_messages; ++i) {
      cout << "Sending message: " << i << endl;
      // for (uint j = 0; j < msg_size; ++j) {
      // 	buffers[node_rank][j] = rand() % 26 + 'a';
      // }
        // send the message
        success = rdmc::send(node_rank, mrs[node_rank], 0, msg_size);
	if (node_rank == 0) {
	  while(count1 <= i) {
	  }
	}
	else {
	  while(count2 <= i) {
	  }
	}
    }
    cout << "All messages successfully sent" << endl;
    // for(unsigned int i = 0; i < num_nodes; ++i) {
    //     while(counts[i] != counts[node_rank]) {
    //     }
    // }
    while(count1 != count2);
    clock_gettime(CLOCK_REALTIME, &end_time);
    // long long int nanoseconds_elapsed =
    // (end_time.tv_sec - start_time.tv_sec) * (long long int)1e9 +
    //  (end_time.tv_nsec - start_time.tv_nsec);
    // double bw =
    // (msg_size * num_messages * num_nodes + 0.0) / nanoseconds_elapsed;
    // double avg_bw = aggregate_bandwidth(members, node_rank, bw);
    // log_results(num_nodes, 0, msg_size, avg_bw, "data_rdmc_bw");
}
