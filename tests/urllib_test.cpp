#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Urllib.hpp"

using testing::Eq;

TEST(UrlLibTest, GetsZeroCount) {
    Urllib::Decode decoder;
    decoder.setUrl("https://google.com");
    std::string decoded = decoder.decode();

    auto parameters = decoder.getParameters();

    ASSERT_THAT(0, Eq(parameters.size()));
}

TEST(UrlLibTest, GetsSingleParam) {
    Urllib::Decode decoder;
    decoder.setUrl("https://google.com?a=1");
    std::string decoded = decoder.decode();

    auto parameters = decoder.getParameters();

    ASSERT_THAT(1, Eq(parameters.size()));
    EXPECT_THAT("1", Eq(parameters.at("a")));
}

TEST(UrlLibTest, GetsMultiple) {
    Urllib::Decode decoder;
    decoder.setUrl("https://google.com?a=1&b=2&c=3&d=4");
    std::string decoded = decoder.decode();

    auto parameters = decoder.getParameters();

    EXPECT_THAT("1", Eq(parameters.at("a")));
    EXPECT_THAT("2", Eq(parameters.at("b")));
    EXPECT_THAT("3", Eq(parameters.at("c")));
    EXPECT_THAT("4", Eq(parameters.at("d")));
}
