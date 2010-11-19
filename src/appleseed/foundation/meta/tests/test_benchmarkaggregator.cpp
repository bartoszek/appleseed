
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010 Francois Beaune
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// appleseed.foundation headers.
#include "foundation/utility/benchmark/benchmarkaggregator.h"
#include "foundation/utility/containers/dictionary.h"
#include "foundation/utility/uid.h"
#include "foundation/utility/test.h"

// boost headers.
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost;
using namespace foundation;
using namespace std;

TEST_SUITE(Foundation_Utility_Benchmark_BenchmarkAggregator)
{
    TEST_CASE(EmptyDirectory)
    {
        BenchmarkAggregator aggregator;
        aggregator.scan_directory("data/test_benchmarkaggregator/empty directory");

        EXPECT_TRUE(aggregator.get_benchmarks().empty());
    }

    TEST_CASE(IncompleteBenchmarkFile)
    {
        BenchmarkAggregator aggregator;
        aggregator.scan_directory("data/test_benchmarkaggregator/incomplete benchmark file/");

        const Dictionary& benchmarks = aggregator.get_benchmarks();

        const BenchmarkSerie& serie =
            aggregator.get_serie(
                benchmarks.dictionaries()
                    .get("Release").dictionaries()
                    .get("Suite")
                    .get<UniqueID>("Case"));

        EXPECT_EQ(1, serie.size());
    }

    TEST_CASE(NonBenchmarkFile)
    {
        BenchmarkAggregator aggregator;
        aggregator.scan_directory("data/test_benchmarkaggregator/non benchmark file/");

        EXPECT_TRUE(aggregator.get_benchmarks().empty());
    }

    TEST_CASE(SingleBenchmarkFile)
    {
        using namespace gregorian;
        using namespace posix_time;

        BenchmarkAggregator aggregator;
        aggregator.scan_directory("data/test_benchmarkaggregator/single benchmark file/");

        const ptime date(date(2010, Jun, 22), time_duration(17, 45, 31));

        const Dictionary& benchmarks = aggregator.get_benchmarks();

        const BenchmarkSerie& serie1 =
            aggregator.get_serie(
                benchmarks.dictionaries()
                    .get("Release").dictionaries()
                    .get("Suite")
                    .get<UniqueID>("Case1"));

        ASSERT_EQ(1, serie1.size());
        EXPECT_EQ(date, serie1[0].m_date);
        EXPECT_EQ(779.34, serie1[0].m_ticks);

        const BenchmarkSerie& serie2 =
            aggregator.get_serie(
                benchmarks.dictionaries()
                    .get("Release").dictionaries()
                    .get("Suite")
                    .get<UniqueID>("Case2"));

        ASSERT_EQ(1, serie2.size());
        EXPECT_EQ(date, serie2[0].m_date);
        EXPECT_EQ(877.22, serie2[0].m_ticks);
    }

    TEST_CASE(MultipleBenchmarkFiles)
    {
        using namespace gregorian;
        using namespace posix_time;

        BenchmarkAggregator aggregator;
        aggregator.scan_directory("data/test_benchmarkaggregator/multiple benchmark files/");

        const Dictionary& benchmarks = aggregator.get_benchmarks();

        const BenchmarkSerie& serie =
            aggregator.get_serie(
                benchmarks.dictionaries()
                    .get("Release").dictionaries()
                    .get("Suite")
                    .get<UniqueID>("Case"));

        ASSERT_EQ(2, serie.size());

        EXPECT_EQ(ptime(date(2009, May, 21), time_duration(16, 44, 30)), serie[0].m_date);
        EXPECT_EQ(779.34, serie[0].m_ticks);

        EXPECT_EQ(ptime(date(2010, Jun, 22), time_duration(17, 45, 31)), serie[1].m_date);
        EXPECT_EQ(877.22, serie[1].m_ticks);
    }
}
