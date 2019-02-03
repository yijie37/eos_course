#include<eosiolib/eosio.hpp>
#include<eosiolib/print.hpp>
#include<eosiolib/asset.hpp>
#include<eosiolib/action.hpp>
#include<eosiolib/symbol.hpp>
#include<eosiolib/singleton.hpp>
#include<eosiolib/transaction.hpp>
#include<eosiolib/crypto.h>

using namespace eosio;
using namespace std;

class trans: public contract {
    public:
        using contract::contract;

        [[eosio::action]]
        void inlinetrans(name user) {
            require_auth(_self);
            action(permission_level{_self, name("active")},
                    name("eosio.token"),
                    name("transfer"),
                    std::make_tuple(_self, user, asset(1, symbol("EOS", 4)),
                        std::string("game send EOS")) ).send();
        }

        [[eosio::action]]
        void delay(string memo) {
            eosio::transaction t{};
            t.actions.emplace_back(eosio::permission_level(_self, name("active")),
                    name("game"),
                    name("inlinetrans"),
                    std::make_tuple(name("systest")));
            t.delay_sec = 1;
            t.send(1, _self, false);
            print("delay end.");
        }

        [[eosio::action]]
        void transfer(name from, name to, asset quantity, string memo) {
            print("transfer from ", from);
        }

};

#define EOSIO_DISPATCH_CUSTOM(TYPE, MEMBERS)                                            \
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

EOSIO_DISPATCH(trans, (inlinetrans)(delay)(transfer))
