#include <iostream>
#include <vector>

#include "derecho/derecho_sst.h"

using namespace derecho;
using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::vector;

int main () {
  uint32_t node_id, num_nodes;
  cin >> node_id >> num_nodes;
    // input the ip addresses
    map<uint32_t, std::string> ip_addrs;
    for(size_t i = 0; i < num_nodes; ++i) {
        cin >> ip_addrs[i];
    }

    // initialize the rdma resources
    sst::verbs_initialize(ip_addrs, node_id);

    vector<uint32_t> members(num_nodes);
    for(uint i = 0; i < num_nodes; ++i) {
        members[i] = i;
    }
    sst::SSTParams sst_params{members, node_id};
    DerechoSST derecho_sst(sst_params, 1, 2, 3);
    derecho_sst.put_with_completion();
    while (true) {
      
    }
}
