#include<eosiolib/eosio.hpp>
#include<eosiolib/asset.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract]] kpi: public contract {
public:
    using contract::contract;
    struct [[eosio::table]] work{
        uint64_t id;
        name worker;
        asset token;
        uint64_t score;
        uint64_t creat_time= current_time();
        //主键
        uint64_t primary_key() const {  return id; }
        //序列化数据。
        EOSLIB_SERIALIZE( work, (id)(worker)(token)(score)(creat_time))
    };

    typedef eosio::multi_index<"work"_n, work> work_table;

    [[eosio::action]]
    void record(name workername, uint64_t score) {
        // 给workername发送10倍score的EOS
        uint64_t token_num = score * 10 * 10000;
        asset tokens = asset(token_num, symbol("EOS", 4));
        action(permission_level{_self, name("active")},
                name("eosio.token"),
                name("transfer"),
                make_tuple(_self, 
                    workername, 
                    tokens,
                    string("kpi record")) ).send();
        // 存储记录
        work_table tbl(_self, _self.value);    
        tbl.emplace(_self, [&](auto& record) {
            record.id = tbl.available_primary_key();
            record.worker = workername;
            record.token = tokens;
            record.score = score;
            record.creat_time = current_time();
        });
    }
};

#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS)                                        \
extern "C"                                                                          \
{                                                                                   \
    void apply(uint64_t receiver, uint64_t code, uint64_t action)                   \
    {                                                                               \
        auto self = receiver;                                                       \
        if ((code == self && action != name("transfer").value))                     \
        {                                                                           \
            switch (action)                                                         \
            {                                                                       \
                EOSIO_DISPATCH_HELPER(TYPE, MEMBERS)                                \
            }                                                                       \
            /* does not allow destructor of this contract to run: eosio_exit(0); */ \
        }                                                                           \
    }                                                                               \
}                                                                                   \

EOSIO_DISPATCH_CUSTOM(kpi, (record))
