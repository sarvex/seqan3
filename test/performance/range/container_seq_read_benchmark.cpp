// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <benchmark/benchmark.h>

#include <deque>
#include <list>
#include <vector>

#include <sdsl/int_vector.hpp>

#include <seqan3/alignment/decorator/gap_decorator.hpp>
#include <seqan3/alphabet/all.hpp>
#include <seqan3/alphabet/container/bitpacked_sequence.hpp>
#include <seqan3/test/performance/sequence_generator.hpp>
#include <seqan3/test/seqan2.hpp>
#include <seqan3/utility/container/small_vector.hpp>

template <typename t>
using sdsl_int_vec = sdsl::int_vector<sizeof(t) * 8>;

template <typename t>
using small_vec = seqan3::small_vector<t, 10'000>;

// ============================================================================
//  sequential_read
// ============================================================================

template <template <typename> typename container_t, typename alphabet_t, bool const_qualified = false>
void sequential_read(benchmark::State & state)
{
    container_t<alphabet_t> container = []()
    {
        auto container = seqan3::test::generate_sequence<alphabet_t>(10'000, 0, 0);
        return container_t<alphabet_t>(container.begin(), container.end());
    }();

    using container_reference_t =
        std::conditional_t<const_qualified, container_t<alphabet_t> const &, container_t<alphabet_t> &>;

    container_reference_t container_reference{container};

    alphabet_t letter{};
    for (auto _ : state)
        for (auto && element : container_reference)
            benchmark::DoNotOptimize(letter = element);

    state.counters["sizeof"] = sizeof(alphabet_t);
    if constexpr (seqan3::alphabet<alphabet_t>)
        state.counters["alph_size"] = seqan3::alphabet_size<alphabet_t>;
    state.counters["const"] = const_qualified;
}

BENCHMARK_TEMPLATE(sequential_read, std::vector, char);
BENCHMARK_TEMPLATE(sequential_read, std::vector, uint8_t);
BENCHMARK_TEMPLATE(sequential_read, std::vector, uint16_t);
BENCHMARK_TEMPLATE(sequential_read, std::vector, uint32_t);
BENCHMARK_TEMPLATE(sequential_read, std::vector, uint64_t);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::gap);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::dna4);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::dna5);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::gapped<seqan3::dna4>);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::dna15);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::aa27);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::qualified<seqan3::dna4, seqan3::phred42>);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::alphabet_variant<char, seqan3::dna4>);

BENCHMARK_TEMPLATE(sequential_read, std::deque, char);
BENCHMARK_TEMPLATE(sequential_read, std::deque, uint8_t);
BENCHMARK_TEMPLATE(sequential_read, std::deque, uint16_t);
BENCHMARK_TEMPLATE(sequential_read, std::deque, uint32_t);
BENCHMARK_TEMPLATE(sequential_read, std::deque, uint64_t);
BENCHMARK_TEMPLATE(sequential_read, std::deque, seqan3::gap);
BENCHMARK_TEMPLATE(sequential_read, std::deque, seqan3::dna4);
BENCHMARK_TEMPLATE(sequential_read, std::deque, seqan3::dna5);
BENCHMARK_TEMPLATE(sequential_read, std::deque, seqan3::gapped<seqan3::dna4>);
BENCHMARK_TEMPLATE(sequential_read, std::deque, seqan3::dna15);
BENCHMARK_TEMPLATE(sequential_read, std::deque, seqan3::aa27);
BENCHMARK_TEMPLATE(sequential_read, std::deque, seqan3::qualified<seqan3::dna4, seqan3::phred42>);
BENCHMARK_TEMPLATE(sequential_read, std::deque, seqan3::alphabet_variant<char, seqan3::dna4>);

BENCHMARK_TEMPLATE(sequential_read, std::list, char);
BENCHMARK_TEMPLATE(sequential_read, std::list, uint8_t);
BENCHMARK_TEMPLATE(sequential_read, std::list, uint16_t);
BENCHMARK_TEMPLATE(sequential_read, std::list, uint32_t);
BENCHMARK_TEMPLATE(sequential_read, std::list, uint64_t);
BENCHMARK_TEMPLATE(sequential_read, std::list, seqan3::gap);
BENCHMARK_TEMPLATE(sequential_read, std::list, seqan3::dna4);
BENCHMARK_TEMPLATE(sequential_read, std::list, seqan3::dna5);
BENCHMARK_TEMPLATE(sequential_read, std::list, seqan3::gapped<seqan3::dna4>);
BENCHMARK_TEMPLATE(sequential_read, std::list, seqan3::dna15);
BENCHMARK_TEMPLATE(sequential_read, std::list, seqan3::aa27);
BENCHMARK_TEMPLATE(sequential_read, std::list, seqan3::qualified<seqan3::dna4, seqan3::phred42>);
BENCHMARK_TEMPLATE(sequential_read, std::list, seqan3::alphabet_variant<char, seqan3::dna4>);

BENCHMARK_TEMPLATE(sequential_read, sdsl_int_vec, uint8_t);
BENCHMARK_TEMPLATE(sequential_read, sdsl_int_vec, uint16_t);
BENCHMARK_TEMPLATE(sequential_read, sdsl_int_vec, uint32_t);
BENCHMARK_TEMPLATE(sequential_read, sdsl_int_vec, uint64_t);

BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, char);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, uint32_t);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::gap);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::dna4);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::dna5);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::gapped<seqan3::dna4>);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::dna15);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::aa27);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::qualified<seqan3::dna4, seqan3::phred42>);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::alphabet_variant<char, seqan3::dna4>);

BENCHMARK_TEMPLATE(sequential_read, small_vec, char);
BENCHMARK_TEMPLATE(sequential_read, small_vec, uint32_t);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::gap);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::dna4);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::dna5);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::gapped<seqan3::dna4>);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::dna15);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::aa27);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::qualified<seqan3::dna4, seqan3::phred42>);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::alphabet_variant<char, seqan3::dna4>);

// ============================================================================
//  sequential_read (const)
// ============================================================================

BENCHMARK_TEMPLATE(sequential_read, std::vector, char, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, uint8_t, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, uint16_t, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, uint32_t, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, uint64_t, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::gap, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::dna4, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::dna5, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::gapped<seqan3::dna4>, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::dna15, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::aa27, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::qualified<seqan3::dna4, seqan3::phred42>, true);
BENCHMARK_TEMPLATE(sequential_read, std::vector, seqan3::alphabet_variant<char, seqan3::dna4>, true);

BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, char, true);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, uint32_t, true);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::gap, true);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::dna4, true);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::dna5, true);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::gapped<seqan3::dna4>, true);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::dna15, true);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::aa27, true);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::qualified<seqan3::dna4, seqan3::phred42>, true);
BENCHMARK_TEMPLATE(sequential_read, seqan3::bitpacked_sequence, seqan3::alphabet_variant<char, seqan3::dna4>, true);

BENCHMARK_TEMPLATE(sequential_read, small_vec, char, true);
BENCHMARK_TEMPLATE(sequential_read, small_vec, uint32_t, true);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::gap, true);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::dna4, true);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::dna5, true);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::gapped<seqan3::dna4>, true);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::dna15, true);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::aa27, true);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::qualified<seqan3::dna4, seqan3::phred42>, true);
BENCHMARK_TEMPLATE(sequential_read, small_vec, seqan3::alphabet_variant<char, seqan3::dna4>, true);

// ============================================================================
//  SeqAn2: sequential_read
// ============================================================================

#if SEQAN3_HAS_SEQAN2

#    include <seqan/sequence.h>

template <template <typename...> typename container_t,
          typename spec_t,
          typename alphabet_t,
          bool const_qualified = false>
void sequential_read2(benchmark::State & state)
{
    container_t<alphabet_t, spec_t> container{seqan3::test::generate_sequence_seqan2<alphabet_t>(10'000, 0, 0)};

    using container_reference_t =
        std::conditional_t<const_qualified, container_t<alphabet_t, spec_t> const &, container_t<alphabet_t, spec_t> &>;

    container_reference_t container_reference{container};

    alphabet_t letter{};
    for (auto _ : state)
        for (auto && element : container_reference)
            benchmark::DoNotOptimize(letter = element);

    state.counters["sizeof"] = sizeof(alphabet_t);
    state.counters["alph_size"] = seqan::ValueSize<alphabet_t>::VALUE;
    state.counters["const"] = const_qualified;
}

BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, char);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, uint8_t);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, uint16_t);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, uint32_t);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, uint64_t);

BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::Dna);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::Dna5);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::Iupac);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::AminoAcid);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::Dna5Q);

BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::Dna);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::Dna5);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::Iupac);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::AminoAcid);
// BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::Dna5Q); // broken in SeqAn2

BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Array<10'000>, seqan::Dna);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Array<10'000>, seqan::Dna5);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Array<10'000>, seqan::Iupac);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Array<10'000>, seqan::AminoAcid);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Array<10'000>, seqan::Dna5Q);

// ============================================================================
//  sequential_read (const)
// ============================================================================

BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, char, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, uint8_t, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, uint16_t, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, uint32_t, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, uint64_t, true);

BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::Dna, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::Dna5, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::Iupac, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::AminoAcid, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Alloc<>, seqan::Dna5Q, true);

BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::Dna, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::Dna5, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::Iupac, true);
BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::AminoAcid, true);
// BENCHMARK_TEMPLATE(sequential_read2, seqan::String, seqan::Packed<>, seqan::Dna5Q); // broken in SeqAn2

#endif // SEQAN3_HAS_SEQAN2

// ============================================================================
//  run
// ============================================================================

BENCHMARK_MAIN();
