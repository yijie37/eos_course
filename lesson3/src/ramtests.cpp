#include<eosiolib/eosio.hpp>

using namespace eosio;
using namespace std;

/**
 * 测试操作multi_index消耗的RAM
 * 0.一个primary key，两个secondary key
 * 1.primary key 使用auto increament
 * 2.不使用 EOSLIB_SERIALIZE序列化
 * 3.去掉一个没有索引的字段
 * 4.去掉一个二级索引字段
 */
class [[eosio::contract]] ramtests: public contract {
    public:
        using contract::contract;
        
        struct [[eosio::table]] ramtest {
            uint64_t pkey;
            uint64_t seckey1;
            uint64_t seckey2;
            uint64_t nonekeyfield;

            uint64_t primary_key() const { return pkey; };
            uint64_t get_seckey1() const { return seckey1; };
            uint64_t get_seckey2() const { return seckey2; };
            
            EOSLIB_SERIALIZE(ramtest, (pkey)(seckey1)(seckey2)(nonekeyfield))
        };

        typedef eosio::multi_index<"ramtest"_n, ramtest, \
                indexed_by<"seckey1"_n, const_mem_fun<ramtest, uint64_t, &ramtest::get_seckey1>>, \
                indexed_by<"seckey2"_n, const_mem_fun<ramtest, uint64_t, &ramtest::get_seckey2>>> ramtest_index;

        struct [[eosio::table]] ramtest1 {
            uint64_t pkey;
            uint64_t aiseckey1;
            uint64_t seckey2;
            uint64_t nonekeyfield;

            uint64_t primary_key() const { return pkey; };
            uint64_t get_aiseckey1() const { return aiseckey1; };
            uint64_t get_seckey2() const { return seckey2; };
            
            EOSLIB_SERIALIZE(ramtest1, (pkey)(aiseckey1)(seckey2)(nonekeyfield))
        };

        typedef eosio::multi_index< "ramtest1"_n, ramtest1, \
                indexed_by<"seckey1"_n, const_mem_fun<ramtest1, uint64_t, &ramtest1::get_aiseckey1>>, \
                indexed_by<"seckey2"_n, const_mem_fun<ramtest1, uint64_t, &ramtest1::get_seckey2>>> ramtest_index1;

        struct [[eosio::table]] ramtest2 {
            uint64_t pkey;
            uint64_t seckey1;
            uint64_t seckey2;
            uint64_t nonekeyfield;

            uint64_t primary_key() const { return pkey; };
            uint64_t get_aiseckey1() const { return seckey1; };
            uint64_t get_seckey2() const { return seckey2; };
        };

        typedef eosio::multi_index< "ramtest2"_n, ramtest2, \
                indexed_by<"seckey1"_n, const_mem_fun<ramtest2, uint64_t, &ramtest2::get_aiseckey1>>, \
                indexed_by<"seckey2"_n, const_mem_fun<ramtest2, uint64_t, &ramtest2::get_seckey2>>> ramtest_index2;

        struct [[eosio::table]] ramtest3 {
            uint64_t pkey;
            uint64_t seckey1;
            uint64_t seckey2;

            uint64_t primary_key() const { return pkey; };
            uint64_t get_seckey1() const { return seckey1; };
            uint64_t get_seckey2() const { return seckey2; };
            
            EOSLIB_SERIALIZE(ramtest3, (pkey)(seckey1)(seckey2))
        };

        typedef eosio::multi_index< "ramtest3"_n, ramtest3, \
                indexed_by<"seckey1"_n, const_mem_fun<ramtest3, uint64_t, &ramtest3::get_seckey1>>, \
                indexed_by<"seckey2"_n, const_mem_fun<ramtest3, uint64_t, &ramtest3::get_seckey2>>> ramtest_index3;

        struct [[eosio::table]] ramtest4 {
            uint64_t pkey;
            uint64_t seckey1;
            uint64_t nonekeyfield;

            uint64_t primary_key() const { return pkey; };
            uint64_t get_seckey1() const { return seckey1; };
            
            EOSLIB_SERIALIZE(ramtest4, (pkey)(seckey1)(nonekeyfield))
        };

        typedef eosio::multi_index< "ramtest4"_n, ramtest4, \
                indexed_by<"seckey1"_n, const_mem_fun<ramtest4, uint64_t, &ramtest4::get_seckey1>>> ramtest_index4;

        [[eosio::action]]
        void add(uint64_t pk, uint64_t sk0, uint64_t sk1, uint64_t nk) {
            ramtest_index tbl0(_self, _self.value);
            tbl0.emplace(_self, [&](auto &tt){
                tt.pkey = pk;
                tt.seckey1 = sk0;
                tt.seckey2 = sk1;
                tt.nonekeyfield = nk;
            });
        }

        [[eosio::action]]
        void del(uint64_t pk) {
            ramtest_index tbl0(_self, _self.value);
            auto iter = tbl0.find(pk);
            tbl0.erase(iter);
        }

        [[eosio::action]]
        void update1(uint64_t pk, uint64_t sk0, uint64_t sk1, uint64_t nk) {
            ramtest_index tbl0(_self, _self.value);
            auto iter = tbl0.find(pk);
            tbl0.modify(iter, _self, [&](auto &tt){
                tt.pkey = pk;
                tt.seckey1 = sk0;
                tt.seckey2 = sk1;
                tt.nonekeyfield = nk;
            });
        }

        // 
        [[eosio::action]]
        void add1(uint64_t sk0, uint64_t sk1, uint64_t nk) {
            ramtest_index1 tbl1(_self, _self.value);
            tbl1.emplace(_self, [&](auto &tt){
                tt.pkey = tbl1.available_primary_key();
                tt.aiseckey1 = sk0;
                tt.seckey2 = sk1;
                tt.nonekeyfield = nk;
            });
        }

        [[eosio::action]]
        void add2(uint64_t pk, uint64_t sk0, uint64_t sk1, uint64_t nk) {
            ramtest_index2 tbl2(_self, _self.value);
            tbl2.emplace(_self, [&](auto &tt){
                tt.pkey = pk;
                tt.seckey1 = sk0;
                tt.seckey2 = sk1;
                tt.nonekeyfield = nk;
            });
        }
        
        [[eosio::action]]
        void add3(uint64_t pk, uint64_t sk0, uint64_t sk1) {
            ramtest_index3 tbl3(_self, _self.value);
            tbl3.emplace(_self, [&](auto &tt){
                tt.pkey = pk;
                tt.seckey1 = sk0;
                tt.seckey2 = sk1;
            });
        }

        [[eosio::action]]
        void add4(uint64_t pk, uint64_t sk0, uint64_t nk) {
            ramtest_index4 tbl4(_self, _self.value);
            tbl4.emplace(_self, [&](auto &tt){
                tt.pkey = pk;
                tt.seckey1 = sk0;
                tt.nonekeyfield = nk;
            });
        }
};

EOSIO_DISPATCH(ramtests, (add)(del)(update1)(add1)(add2)(add3)(add4))
//EOSIO_DISPATCH(ramtests, (add))
