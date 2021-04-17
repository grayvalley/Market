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
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Instrument, Name, Id, PriceDecimals)
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
        using KeyType   = GVT::Instruments::InstrumentId;
        using ValueType = std::shared_ptr<Instrument>;
        using StoreType = std::unordered_map<KeyType, ValueType>;
    protected:
        StoreType mItems;
    public:
        PREVENT_MOVE(InstrumentStore);
        PREVENT_COPY(InstrumentStore);
    public:
        InstrumentStore() = default;
    public:

        void put(const KeyType& k, const ValueType& v) {
            mItems.insert(std::make_pair(k, v));
        }

        std::optional<ValueType> get(const KeyType& k) const {
            auto it = mItems.find(k);
            if (it == mItems.end()){
                return std::nullopt;
            }
            return it->second;
        }

        StoreType& items() {
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


namespace GVT::Instruments::filter {
    static std::shared_ptr<GVT::InstrumentStore> byExchange(
            const std::shared_ptr<GVT::InstrumentStore>& store,
            const std::string& filter)
            {
        auto& src = store->items();
        auto out = std::make_shared<GVT::InstrumentStore>();
        auto& output_map = out->items();
        for (const auto& item : src){
            output_map.insert(item);
        }
        return out;
    }
}

namespace GVT {
    class Market {
    public:
        std::shared_ptr<GVT::Instrument> Instrument;
        std::shared_ptr<GVT::LOB::OrderBook> OrderBook;
    public:
        DELETE_DEFAULT_CTOR(Market);
        PREVENT_MOVE(Market);
        PREVENT_COPY(Market);
    public:
        explicit Market(std::shared_ptr<GVT::Instrument> instrument):
                Instrument(std::move(instrument)), OrderBook(new GVT::LOB::OrderBook()) {};
    };
}

namespace GVT {
    class MarketStore {
        using KeyType = GVT::Instruments::InstrumentId;
        using ValueType = std::shared_ptr<Market>;
        using StoreType = std::unordered_map<KeyType, ValueType>;
    private:
        StoreType mItems;
    public:
        const std::string Name;
        const uint64_t Id;
    public:
        DELETE_DEFAULT_CTOR(MarketStore);
        PREVENT_MOVE(MarketStore);
        PREVENT_COPY(MarketStore);
    public:
        explicit MarketStore(std::string name, uint64_t id): Name(std::move(name)), Id(id) {};
    public:

        void populate(const std::shared_ptr<GVT::InstrumentStore>& store) {
            auto& src = store->items();
            auto filtered = GVT::Instruments::filter::byExchange(store, Name);
            for (auto & item : src){
                mItems.insert(std::make_pair(item.first, std::make_shared<Market>(item.second)));
            }
        }


        std::optional<ValueType> get(const KeyType& k) {
            auto it = mItems.find(k);
            if (it == mItems.end()){
                return std::nullopt;
            }
            return it->second;
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
