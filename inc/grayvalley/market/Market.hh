#ifndef GVT_MARKET_HH
#define GVT_MARKET_HH

#include <map>
#include <memory>
#include <variant>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <nlohmann/json.hpp>

#include <grayvalley/common/Macros.hh>
#include <grayvalley/lob/MBOOrderBook.hh>
#include <grayvalley/websocket/WebSocketSession.hh>


namespace GVT {
    struct Instrument {
        std::string symbol;
        int64_t decimals;
        int64_t tick_size;
    };
    void from_json(const nlohmann::json& j, Instrument& inst) {
        j.at("symbol").get_to(inst.symbol);
        j.at("price-decimals").get_to(inst.decimals);
        j.at("tick-size").get_to(inst.tick_size);
    }
}

namespace GVT {
    class InstrumentStore {
        using StoreType = std::unordered_map<std::string, GVT::Instrument>;
    protected:
        StoreType m_instruments;
    public:
        InstrumentStore() = default;
        InstrumentStore(const InstrumentStore& other) {
            for (const auto & item : other.m_instruments) {
                m_instruments.insert(item);
            }
        }
    public:
        void put(const std::string& key, GVT::Instrument value) {
            m_instruments.insert(std::make_pair(key, std::move(value)));
        }
        void put(GVT::Instrument instrument) {
            m_instruments.insert(std::make_pair(instrument.symbol, std::move(instrument)));
        }
        const GVT::Instrument& get(const std::string& key) const {
            return m_instruments.at(key);
        }
        StoreType& items() {
            return m_instruments;
        }
        const StoreType& items() const {
            return m_instruments;
        }
        StoreType::const_iterator begin() const {
            return m_instruments.begin();
        }
        StoreType::const_iterator end() const {
            return m_instruments.end();
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
                // maybe value
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

namespace GVT {
    class ISettings {
    protected:
        nlohmann::json read_json(const std::string &file_path) {
            nlohmann::json json_file;
            std::ifstream config_file(file_path);
            config_file >> json_file;
            return json_file;
        };
    };
}

namespace GVT::MarketData {
    class Settings : protected ISettings {
    private:
        std::map<std::string, GVT::Exchange> m_exchanges;
    public:
        void add(const GVT::Exchange &exchange) {
            m_exchanges[exchange.name] = exchange;
        }
        GVT::Exchange& get(const std::string& exchange){
            return m_exchanges.at(exchange);
        }
        void from(const std::string &file_path);
        void from(const nlohmann::json& json_file);
    };
}

namespace GVT::MarketData {
    void Settings::from(const std::string &file_path) {
        auto json_file = read_json(file_path);
        from(json_file);
    }
}

namespace GVT::MarketData {
    void Settings::from(const nlohmann::json& json_file) {
        auto exchanges = json_file["exchanges"];
        for (auto & item : exchanges){
            auto exchange = item.get<GVT::Exchange>();
            add(exchange);
        }
    }
}

namespace GVT::Trading {
    struct InstrumentRiskLimit {
        std::string symbol;
        int64_t     quoted_size;
        int64_t     max_long;
        int64_t     max_short;
    };
    void from_json(const nlohmann::json& j, InstrumentRiskLimit& exchange) {
        j.at("symbol").get_to(exchange.symbol);
        j.at("quoted-size").get_to(exchange.quoted_size);
        j.at("max-long").get_to(exchange.max_long);
        j.at("max-short").get_to(exchange.max_short);
    }
}

namespace GVT::Trading {
    struct ExchangeRiskLimits {
        std::string exchange;
        // why not instrument store
        std::map<std::string, GVT::Trading::InstrumentRiskLimit> instruments;
    };
    void from_json(const nlohmann::json& j, ExchangeRiskLimits& exchange) {
        j.at("exchange").get_to(exchange.exchange);
        for (auto & item : j.at("instruments")){
            auto instrument_limit = item.get<GVT::Trading::InstrumentRiskLimit>();
            exchange.instruments[instrument_limit.symbol] = instrument_limit;
        }
    }
}

namespace GVT::Trading {
    class RiskSettings : protected ISettings {
    private:
        std::map<std::string, GVT::Trading::ExchangeRiskLimits> m_risk_limits;
    public:
        void add_risk_limit(const GVT::Trading::ExchangeRiskLimits& limit){
            m_risk_limits[limit.exchange] = limit;
        }
        GVT::Trading::ExchangeRiskLimits& get_risk_limit(const std::string& exchange){
            return m_risk_limits.at(exchange);
        }
        void from(const std::string &file_path);
        void from(const nlohmann::json &json_file);
    };
}

namespace GVT::Trading {
    void RiskSettings::from(const std::string &file_path) {
        auto json_file = read_json(file_path);
        read_json(json_file);
    }
}

namespace GVT::Trading {
    void RiskSettings::from(const nlohmann::json &json_file) {
        for (auto & exchange : json_file["risk-limits"]){
            auto risk_limit = exchange.get<GVT::Trading::ExchangeRiskLimits>();
            add_risk_limit(risk_limit);
        }
    }
}

namespace GVT::Trading {
    class MarketMakerSettings : protected ISettings {
    public:
        uint64_t trader_id;
        uint64_t oms_id;
        GVT::Trading::RiskSettings risk_settings;
    public:
        void from(const std::string &file_path);
        void from(const nlohmann::json &json_file);
    };
}

namespace GVT::Trading {
    void MarketMakerSettings::from(const std::string &file_path) {
        auto json_file = read_json(file_path);
        from(json_file);
    }
}

namespace GVT::Trading {
    void MarketMakerSettings::from(const nlohmann::json &json_file) {
        json_file.at("trader-id").get_to(trader_id);
        json_file.at("oms-id").get_to(oms_id);
        risk_settings.from(json_file);
    }
}


namespace GVT::Trading {
    class OrderExecutionEngineSettings : protected ISettings {
    public:
        uint64_t id;
    public:
        void from(const std::string &file_path);
        void from(const nlohmann::json &json_file);
    };
}

namespace GVT::Trading {
    void OrderExecutionEngineSettings::from(const std::string &file_path) {
        auto json_file = read_json(file_path);
        from(json_file);
    }
}

namespace GVT::Trading {
    void OrderExecutionEngineSettings::from(const nlohmann::json &json_file) {
        json_file.at("oms-id").get_to(id);
    }
}

#endif //GVT_MARKET_HH
