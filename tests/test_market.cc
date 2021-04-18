
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

#include <grayvalley/market/Market.hh>


TEST(InstrumentTests, TestJSONParsing) {

    // create an instrument
    GVT::Instrument instrument = {"Name", "Exchange", 0, 2};

    // conversion: instrument -> json
    nlohmann::json j = instrument;

    // conversion: json -> instrument
    auto instrument2 = j.get<GVT::Instrument>();

    ASSERT_EQ(instrument.Id, instrument2.Id);
    ASSERT_EQ(instrument.Name, instrument2.Name);
    ASSERT_EQ(instrument.PriceDecimals, instrument2.PriceDecimals);
}

TEST(InstrumentStoreTests, TestPutAndGet){

    // create an instrument
    auto instrument = GVT::Instrument{"Name", "Exchange", 0, 2};

    // create instrument store
    GVT::InstrumentStore store;

    // add the instrument against name and id
    store.put(GVT::Instruments::makeName(instrument.Name), instrument);
    store.put(GVT::Instruments::makeId(instrument.Id), instrument);

    // get the instrument using name
    auto ins_name = store.get(GVT::Instruments::makeName("Name"));
    auto ins_id = store.get(GVT::Instruments::makeId(0));

    auto val1 = ins_name.value();
    auto val2 = ins_id.value();

    ASSERT_EQ(instrument.Exchange, val1.Exchange);
    ASSERT_EQ(instrument.Name, val1.Name);
    ASSERT_EQ(instrument.Id, val1.Id);

    ASSERT_EQ(instrument.Exchange, val2.Exchange);
    ASSERT_EQ(instrument.Name, val2.Name);
    ASSERT_EQ(instrument.Id, val2.Id);
}

TEST(InstrumentStoreTests, TestFilter){

    // create instruments
    auto instrument1 = GVT::Instrument{"Name1", "Exchange1", 0, 2};
    auto instrument2 = GVT::Instrument{"Name2", "Exchange2", 1, 2};

    // create instrument store
    GVT::InstrumentStore store;

    // add the instrument against name and id
    store.put(GVT::Instruments::makeName(instrument1.Name), instrument1);
    store.put(GVT::Instruments::makeId(instrument1.Id), instrument1);
    store.put(GVT::Instruments::makeName(instrument2.Name), instrument2);
    store.put(GVT::Instruments::makeId(instrument2.Id), instrument2);

    auto filtered1 = GVT::Instruments::filter::byExchange(store, "Exchange1");
    auto filtered2 = GVT::Instruments::filter::byExchange(store, "Exchange2");

    ASSERT_TRUE(filtered1.get(GVT::Instruments::makeName("Name1")));
    ASSERT_FALSE(filtered1.get(GVT::Instruments::makeName("Name2")));

    ASSERT_FALSE(filtered2.get(GVT::Instruments::makeName("Name1")));
    ASSERT_TRUE(filtered2.get(GVT::Instruments::makeName("Name2")));
}


TEST(MarketTests, TestConstruction){

    // create an instrument
    auto instrument = GVT::Instrument{"Name", "Exchange", 0, 2};

    // create a market
    GVT::Market market(std::move(instrument));

    ASSERT_TRUE(market.OrderBook.is_empty());
}

TEST(MarketStoreTests, TestConstruction){

    // create an instrument
    auto instrument = GVT::Instrument{"Name", "Exchange", 0, 2};

    // create instrument store
    GVT::InstrumentStore store;

    // add the instrument against name and id
    store.put(GVT::Instruments::makeName(instrument.Name), instrument);
    store.put(GVT::Instruments::makeId(instrument.Id), instrument);

    // create market store
    GVT::MarketStore markets("Name", 0);

    // populate from instrument store
    markets.populate(store);

    // get market using name and instrument id
    auto market1 = markets.get(GVT::Instruments::makeName(instrument.Name));
    auto market2 = markets.get(GVT::Instruments::makeId(instrument.Id));

    ASSERT_EQ(instrument.Exchange, market1.value()->Instrument.Exchange);
    ASSERT_EQ(instrument.Name, market1.value()->Instrument.Name);
    ASSERT_EQ(instrument.Id, market1.value()->Instrument.Id);

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}