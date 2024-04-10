#include "pch.h"
#include "../src/simple-heuristic.h"

TEST_CASE("greedyAlgorithm should return correct result", "[greedyAlgorithm]") {
    KnapsackInstance instance;

    SECTION("Test case 1") {
        instance.addItem(KnapsackItem(1, 10));
        instance.addItem(KnapsackItem(2, 20));
        instance.addItem(KnapsackItem(3, 30));
        instance.addItem(KnapsackItem(4, 40));

        greedyAlgorithm<KnapsackItem>(instance);

        REQUIRE(instance.getTotalWeight() == 6);
        REQUIRE(instance.getTotalCost() == 60);
        REQUIRE(instance.getItems().size() == 2);
        REQUIRE(instance.getItems()[0].weight == 2);
        REQUIRE(instance.getItems()[0].cost == 20);
        REQUIRE(instance.getItems()[1].weight == 4);
        REQUIRE(instance.getItems()[1].cost == 40);
    }

    SECTION("Test case 2") {
        instance.addItem(KnapsackItem(1, 10));
        instance.addItem(KnapsackItem(2, 20));
        instance.addItem(KnapsackItem(3, 30));
        instance.addItem(KnapsackItem(4, 40));
        instance.addItem(KnapsackItem(5, 50));

        greedyAlgorithm<KnapsackItem>(instance);

        REQUIRE(instance.getTotalWeight() == 9);
        REQUIRE(instance.getTotalCost() == 90);
        REQUIRE(instance.getItems().size() == 3);
        REQUIRE(instance.getItems()[0].weight == 2);
        REQUIRE(instance.getItems()[0].cost == 20);
        REQUIRE(instance.getItems()[1].weight == 4);
        REQUIRE(instance.getItems()[1].cost == 40);
        REQUIRE(instance.getItems()[2].weight == 5);
        REQUIRE(instance.getItems()[2].cost == 50);
    }
}