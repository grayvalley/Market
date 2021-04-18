#ifndef GVT_MARKET_HH
#define GVT_MARKET_HH

#include <map>
#include <memory>
#include <variant>
#include <iterator>
#include <algorithm>
#include <nlohmann/json.hpp>

#include <grayvalley/common/Macros.hh>
#include <grayvalley/lob/OrderBook.hh>


namespace GVT {
    class Instrument {
    public:
        std::string Name;
        std::string Exchange;
        uint64_t Id;
        uint64_t PriceDecimals;
    public:

        Instrument() = default;

        Instrument(std::string name, std::string exchange, uint64_t id, uint64_t decimals):
            Name{name}, Exchange{exchange}, Id{id}, PriceDecimals{decimals} {}

        Instrument(const Instrument& other) {
            Name = other.Name;
            Exchange = other.Exchange;
            Id = other.Id;
            PriceDecimals = other.PriceDecimals;
        }

    public:
        [[nodiscard]] std::string description() const;
    };
}

namespace GVT {
    std::string Instrument::description() const {
        std::stringstream ss;
        ss << "Instrument ";
        ss << "Name: " << Name << ", ";
        ss << "Id: " << Id << std::endl;
        return ss.str();
    }
}

namespace GVT::Instruments {

    using InstrumentId = std::variant<std::string, uint64_t>;

    template<typename... T>
    constexpr auto makeName(T&&... args){
        return InstrumentId(std::in_place_index<0>, std::forward<T>(args)...);
    }

    template<typename... T>
    constexpr auto makeId(T&&... args){
        return InstrumentId(std::in_place_index<1>, std::forward<T>(args)...);
    }
}

namespace GVT {
    class InstrumentStore {
        using KeyType = GVT::Instruments::InstrumentId;
        using ValueType = GVT::Instrument;
        using StoreType = std::unordered_map<KeyType, ValueType>;
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

        void put(KeyType k, ValueType v) {
            mItems.insert(std::make_pair(k, std::move(v)));
        }

        std::optional<GVT::Instrument> get(const KeyType& k) const {

            auto it = mItems.find(k);

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
}

namespace GVT {
    class Market {
    public:
        GVT::Instrument Instrument;
        GVT::LOB::OrderBook OrderBook;
    public:
        explicit Market(GVT::Instrument instrument) : Instrument(instrument) {};
    };
}

namespace GVT {
    class MarketStore {
        using KeyType = GVT::Instruments::InstrumentId;
        using ValueType = std::unique_ptr<GVT::Market>;
        using StoreType = std::unordered_map<KeyType, ValueType>;
    private:
        StoreType mItems;
    public:
        const std::string Name;
        const uint64_t Id;
    public:
        explicit MarketStore(std::string name, uint64_t id): Name(std::move(name)), Id(id) {};
    public:

        void populate(const GVT::InstrumentStore& store) {
            for (auto & item : store) {
                if (item.second.Exchange == Name) {
                    auto ptr = std::make_unique<GVT::Market>(item.second);
                    mItems.emplace(item.first, std::move(ptr));
                }
            }
        }

        std::optional<Market*> get(const KeyType& k) {
            auto it = mItems.find(k);
            if (it == mItems.end()){
                return std::nullopt;
            }
            return it->second.get();
        }

        StoreType::const_iterator begin() const {
            return mItems.begin();
        }

        StoreType::const_iterator end() const {
            return mItems.end();
        }
    };
}


#endif //GVT_MARKET_HH
