#include <chrono>

#include "../sst.h"
#include "../verbs.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::map;

class mySST : public sst::SST<mySST> {
public:
  mySST(const vector<uint32_t>& _members, uint32_t my_id, uint32_t size) : SST(this, sst::SSTParams{_members, my_id}),
							    c(size) {
        SSTInit(c);
    }
    sst::SSTFieldVector<char> c;
};

int main() {
    // input number of nodes and the local node id
    uint32_t num_nodes, my_id;
    cin >> my_id >> num_nodes;

    // input the ip addresses
    map<uint32_t, std::string> ip_addrs;
    for(size_t i = 0; i < num_nodes; ++i) {
        cin >> ip_addrs[i];
    }

    // initialize the rdma resources
    sst::verbs_initialize(ip_addrs, my_id);

    vector<uint32_t> members(num_nodes);
    for(uint i = 0; i < num_nodes; ++i) {
        members[i] = i;
    }

    mySST sst(members, my_id, 1411);
    // int b = 5 + my_id;
    // sst.a[my_id][0] = b;
    if (my_id == 1) {
      sst.put_with_completion();
    }
    sst::sync(1 - my_id);
    // while (true) {
    // int n;
    // cin >> n;
    // sst.a[my_id][0] = n;
    // sst.put_with_completion();
    // for(uint i = 0; i < num_nodes; ++i) {
    //   cout << sst.a[i][0] << endl;
    // }
    // }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
