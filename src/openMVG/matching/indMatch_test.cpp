// Copyright (c) 2012, 2013 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.


#include "testing/testing.h"
#include "openMVG/matching/indMatch.hpp"
#include "openMVG/matching/indMatch_utils.hpp"

using namespace openMVG;
using namespace matching;

TEST(IndMatch, IO)
{
  {
    std::set<IndexT> viewsKeys;
    PairWiseMatches matches;

    // Test save + load of empty data
    EXPECT_TRUE(Save(matches, ".", "test1", "txt", false));
    EXPECT_TRUE(Load(matches, viewsKeys, ".", "test1"));
    EXPECT_EQ(0, matches.size());

    EXPECT_TRUE(Save(matches, ".", "test2", "bin", false));
    EXPECT_TRUE(Load(matches, viewsKeys, ".", "test2"));
    EXPECT_EQ(0, matches.size());
  }
  {
    std::set<IndexT> viewsKeys;
    PairWiseMatches matches;

    // Test save + load of empty data
    EXPECT_TRUE(Save(matches, ".", "test3", "txt", true));
    EXPECT_FALSE(Load(matches, viewsKeys, ".", "test3"));
    EXPECT_EQ(0, matches.size());

    EXPECT_TRUE(Save(matches, ".", "test4", "bin", true));
    EXPECT_FALSE(Load(matches, viewsKeys, ".", "test4"));
    EXPECT_EQ(0, matches.size());
  }
  {
    std::set<IndexT> viewsKeys = {0, 1, 2};
    PairWiseMatches matches;
    // Test export with not empty data
    matches[std::make_pair(0,1)] = {{0,0},{1,1}};
    matches[std::make_pair(1,2)] = {{0,0},{1,1}, {2,2}};

    EXPECT_TRUE(Save(matches, ".", "test5", "txt", false));
    matches.clear();
    EXPECT_TRUE(Load(matches, viewsKeys, ".", "test5"));
    EXPECT_EQ(2, matches.size());
    EXPECT_EQ(1, matches.count(std::make_pair(0,1)));
    EXPECT_EQ(1, matches.count(std::make_pair(1,2)));
    EXPECT_EQ(2, matches.at(std::make_pair(0,1)).size());
    EXPECT_EQ(3, matches.at(std::make_pair(1,2)).size());
  }
  {
    std::set<IndexT> viewsKeys = {0, 1, 2};
    PairWiseMatches matches;
    // Test export with not empty data
    matches[std::make_pair(0,1)] = {{0,0},{1,1}};
    matches[std::make_pair(1,2)] = {{0,0},{1,1}, {2,2}};

    EXPECT_TRUE(Save(matches, ".", "test6", "txt", true));
    EXPECT_TRUE(Load(matches, viewsKeys, ".", "test6"));
    EXPECT_EQ(2, matches.size());
    EXPECT_EQ(1, matches.count(std::make_pair(0,1)));
    EXPECT_EQ(1, matches.count(std::make_pair(1,2)));
    EXPECT_EQ(2, matches.at(std::make_pair(0,1)).size());
    EXPECT_EQ(3, matches.at(std::make_pair(1,2)).size());
  }
  {
    std::set<IndexT> viewsKeys = {0, 1, 2};
    PairWiseMatches matches;
    matches[std::make_pair(0,1)] = {{0,0},{1,1}};
    matches[std::make_pair(1,2)] = {{0,0},{1,1}, {2,2}};

    EXPECT_TRUE(Save(matches, ".", "test7", "bin", false));
    EXPECT_TRUE(Load(matches, viewsKeys, ".", "test7"));
    EXPECT_EQ(2, matches.size());
    EXPECT_EQ(1, matches.count(std::make_pair(0,1)));
    EXPECT_EQ(1, matches.count(std::make_pair(1,2)));
    EXPECT_EQ(2, matches.at(std::make_pair(0,1)).size());
    EXPECT_EQ(3, matches.at(std::make_pair(1,2)).size());
  }
  {
    std::set<IndexT> viewsKeys = {0, 1, 2};
    PairWiseMatches matches;
    matches[std::make_pair(0,1)] = {{0,0},{1,1}};
    matches[std::make_pair(1,2)] = {{0,0},{1,1}, {2,2}};

    EXPECT_TRUE(Save(matches, ".", "test8", "bin", true));
    matches.clear();
    EXPECT_TRUE(Load(matches, viewsKeys, ".", "test8"));
    EXPECT_EQ(2, matches.size());
    EXPECT_EQ(1, matches.count(std::make_pair(0,1)));
    EXPECT_EQ(1, matches.count(std::make_pair(1,2)));
    EXPECT_EQ(2, matches.at(std::make_pair(0,1)).size());
    EXPECT_EQ(3, matches.at(std::make_pair(1,2)).size());
  }
}

TEST(IndMatch, DuplicateRemoval_NoRemoval)
{
  std::vector<IndMatch> vec_indMatch;

  vec_indMatch.push_back(IndMatch(2,3)); // 0
  vec_indMatch.push_back(IndMatch(0,1)); // 1

  // Check no removal
  EXPECT_FALSE(removeDuplicateMatches(vec_indMatch));

  // Check lexigraphical sorting
  // Due to lexigraphical sorting (0,1) must appears first
  EXPECT_EQ(IndMatch(0,1), vec_indMatch[0]);
  EXPECT_EQ(IndMatch(2,3), vec_indMatch[1]);
}

TEST(IndMatch, DuplicateRemoval_Simple)
{
  std::vector<IndMatch> vec_indMatch;

  vec_indMatch.push_back(IndMatch(0,1)); // 0
  vec_indMatch.push_back(IndMatch(0,1)); // 1: error with addition 0

  vec_indMatch.push_back(IndMatch(1,2)); // 2
  vec_indMatch.push_back(IndMatch(1,2)); // 3: error with addition 2

  EXPECT_TRUE(removeDuplicateMatches(vec_indMatch));
  // Two matches must remain (line 0 and 2)
  EXPECT_EQ(2, vec_indMatch.size());
}

TEST(IndMatch, DuplicateRemoval)
{
  std::vector<IndMatch> vec_indMatch;

  vec_indMatch.push_back(IndMatch(0,1));
  vec_indMatch.push_back(IndMatch(0,1)); // Error defined before

  // Add some other matches
  vec_indMatch.push_back(IndMatch(0,2));
  vec_indMatch.push_back(IndMatch(1,1));
  vec_indMatch.push_back(IndMatch(2,3));
  vec_indMatch.push_back(IndMatch(3,3));

  EXPECT_TRUE(removeDuplicateMatches(vec_indMatch));
  // Five matches must remain (one (0,1) must disappear)
  EXPECT_EQ(5, vec_indMatch.size());

  EXPECT_EQ(IndMatch(0,1), vec_indMatch[0]);
  EXPECT_EQ(IndMatch(0,2), vec_indMatch[1]);
  EXPECT_EQ(IndMatch(1,1), vec_indMatch[2]);
  EXPECT_EQ(IndMatch(2,3), vec_indMatch[3]);
  EXPECT_EQ(IndMatch(3,3), vec_indMatch[4]);
}


/* ************************************************************************* */
int main() { TestResult tr; return TestRegistry::runAllTests(tr);}
/* ************************************************************************* */
