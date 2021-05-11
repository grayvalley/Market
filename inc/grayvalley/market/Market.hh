#ifndef GVT_MARKET_HH
#define GVT_MARKET_HH

#include <map>
#include <memory>
#include <variant>
#include <iterator>
#include <algorithm>
#include <nlohmann/json.hpp>

#include <grayvalley/common/Macros.hh>
#include <grayvalley/lob/MBOOrderBook.hh>


namespace GVT {

    struct Instrument {
        std::string symbol;
        uint64_t decimals;
    };

    void from_json(const nlohmann::json& j, Instrument& inst) {
        j.at("symbol").get_to(inst.symbol);
        j.at("price-decimals").get_to(inst.decimals);
    }
}



namespace GVT {
    class InstrumentStore {
        using StoreType = std::unordered_map<std::string, GVT::Instrument>;
    protected:
        StoreType mItems;
    public:
        InstrumentStore() = default;
        InstrumentStore(const InstrumentStore& other) {
            for (const auto & item : other.mItems) {
                mItems.insert(item);
            }
        }
    public:
        void put(const std::string& key, GVT::Instrument value) {
            mItems.insert(std::make_pair(key, std::move(value)));
        }
        void put(GVT::Instrument instrument) {
            mItems.insert(std::make_pair(instrument.symbol, std::move(instrument)));
        }
        std::optional<GVT::Instrument> get(const std::string& key) const {
            auto it = mItems.find(key);
            if (it == mItems.end()){
                return std::nullopt;
            }
            return it->second;
        }
        StoreType& items() {
            return mItems;
        }
        const StoreType& items() const {
            return mItems;
        }
        StoreType::const_iterator begin() const {
            return mItems.begin();
        }
        StoreType::const_iterator end() const {
            return mItems.end();
        }
    };

    void from_json(const nlohmann::json& j, InstrumentStore& store) {
       for (auto & item : j){
           auto instrument = item.get<GVT::Instrument>();
           store.put(instrument);
       }
    }
}

namespace GVT {

    struct Exchange {
        std::string name;
        GVT::InstrumentStore instruments;
        GVT::Session market_data;
        GVT::Session order_entry;
    };

    void from_json(const nlohmann::json& j, Exchange& exchange) {
        j.at("name").get_to(exchange.name);
        j.at("instruments").get_to(exchange.instruments);
        j.at("market-data").get_to(exchange.market_data);
        j.at("order-entry").get_to(exchange.order_entry);
    }
}

namespace GVT {
    template<class OrderBookType = GVT::LOB::MBOOrderBook>
    class Market {
    public:
        GVT::Instrument instrument;
        OrderBookType OrderBook;
    public:
        explicit Market(GVT::Instrument instrument) : instrument(std::move(instrument)) {};
    };
}

namespace GVT {
    template<class OrderBookType = GVT::LOB::MBOOrderBook>
    class MarketStore {
        using ValueType = std::unique_ptr<GVT::Market<OrderBookType> >;
        using StoreType = std::unordered_map<std::string, ValueType>;
    private:
        StoreType m_store;
    public:
        explicit MarketStore() = default;
    public:

        void populate(const GVT::InstrumentStore& instruments) {
            for (auto & item : instruments) {
                auto ptr = std::make_unique<GVT::Market<OrderBookType>>(item.second);
                m_store.emplace(item.first, std::move(ptr));

            }
        }

        std::optional<Market<OrderBookType>*> get(const std::string& k) {
            auto it = m_store.find(k);
            if (it == m_store.end()){
                return std::nullopt;
            }
            return it->second.get();
        }
    };
}


#endif //GVT_MARKET_HH
