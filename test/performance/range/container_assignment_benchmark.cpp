// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <benchmark/benchmark.h>

#include <seqan3/alphabet/aminoacid/aa27.hpp>
#include <seqan3/alphabet/nucleotide/dna4.hpp>
#include <seqan3/alphabet/nucleotide/dna5.hpp>
#include <seqan3/test/literal/bytes.hpp>
#include <seqan3/test/performance/sequence_generator.hpp>

using namespace seqan3::test::literals;

#ifndef NDEBUG
static constexpr size_t vector_size{1_MiB};
#else
static constexpr size_t vector_size{16_MiB};
#endif // NDEBUG

enum tag
{
    assignment_operator,
    std_copy,
    uninitialized_copy
};

template <tag id>
struct assignment_functor
{
    template <typename container_t>
        requires (id == tag::assignment_operator)
    static constexpr void call(container_t & to, container_t const & from) noexcept(
        noexcept(std::is_nothrow_assignable_v<container_t, container_t>))
    {
        benchmark::DoNotOptimize(to = from);
    }

    template <typename container_t>
        requires (id == tag::std_copy)
    static constexpr void call(container_t & to, container_t const & from) noexcept
    {
        std::copy(std::ranges::begin(from), std::ranges::end(from), std::ranges::begin(to));
        benchmark::DoNotOptimize(to);
    }

    template <typename container_t>
        requires (id == tag::uninitialized_copy)
    static constexpr void call(container_t & to, container_t const & from) noexcept
    {
        std::uninitialized_copy(std::ranges::begin(from), std::ranges::end(from), std::ranges::begin(to));
        benchmark::DoNotOptimize(to);
    }
};

template <typename container_t>
    requires requires (container_t v) { v.clear(); }
static constexpr void clear(container_t & container) noexcept(noexcept(std::declval<container_t>().clear()))
{
    container.clear();
}

template <typename container_t>
    requires requires (container_t v) { v.resize(1u); }
static constexpr void resize(container_t & container,
                             size_t const size) noexcept(noexcept(std::declval<container_t>().resize(1u)))
{
    container.resize(size);
}

#if SEQAN3_HAS_SEQAN2
template <typename container_t>
static constexpr void clear(container_t & container) noexcept(noexcept(seqan::clear(std::declval<container_t>())))
{
    seqan::clear(container);
}

template <typename container_t>
static constexpr void resize(container_t & container,
                             size_t const size) noexcept(noexcept(seqan::resize(std::declval<container_t>(), 1u)))
{
    seqan::resize(container, size);
}
#endif // SEQAN3_HAS_SEQAN2

template <tag id, template <typename> typename container_t, typename alphabet_t>
static void assign(benchmark::State & state)
{
    auto random_sequence = []() constexpr
    {
        if constexpr (seqan3::alphabet<alphabet_t>)
            return seqan3::test::generate_sequence<alphabet_t>(vector_size);
#if SEQAN3_HAS_SEQAN2
        else
            return seqan3::test::generate_sequence_seqan2<alphabet_t>(vector_size);
#endif // SEQAN3_HAS_SEQAN2
    }();

    container_t<alphabet_t> from{};
    resize(from, vector_size);
    std::copy(std::ranges::begin(random_sequence), std::ranges::end(random_sequence), std::ranges::begin(from));
    container_t<alphabet_t> to{};
    resize(to, vector_size);
    assignment_functor<id> fn{};

    for (auto _ : state)
    {
        fn.call(to, from);
        benchmark::ClobberMemory();
        clear(to);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_TEMPLATE(assign, tag::assignment_operator, std::vector, seqan3::dna4);
BENCHMARK_TEMPLATE(assign, tag::assignment_operator, std::vector, seqan3::dna5);
BENCHMARK_TEMPLATE(assign, tag::assignment_operator, std::vector, seqan3::aa27);

BENCHMARK_TEMPLATE(assign, tag::std_copy, std::vector, seqan3::dna4);
BENCHMARK_TEMPLATE(assign, tag::std_copy, std::vector, seqan3::dna5);
BENCHMARK_TEMPLATE(assign, tag::std_copy, std::vector, seqan3::aa27);

BENCHMARK_TEMPLATE(assign, tag::uninitialized_copy, std::vector, seqan3::dna4);
BENCHMARK_TEMPLATE(assign, tag::uninitialized_copy, std::vector, seqan3::dna5);
BENCHMARK_TEMPLATE(assign, tag::uninitialized_copy, std::vector, seqan3::aa27);

#if SEQAN3_HAS_SEQAN2
BENCHMARK_TEMPLATE(assign, tag::assignment_operator, std::vector, seqan::Dna);
BENCHMARK_TEMPLATE(assign, tag::assignment_operator, std::vector, seqan::Dna5);
BENCHMARK_TEMPLATE(assign, tag::assignment_operator, std::vector, seqan::AminoAcid);
BENCHMARK_TEMPLATE(assign, tag::assignment_operator, seqan::String, seqan::Dna);
BENCHMARK_TEMPLATE(assign, tag::assignment_operator, seqan::String, seqan::Dna5);
BENCHMARK_TEMPLATE(assign, tag::assignment_operator, seqan::String, seqan::AminoAcid);

BENCHMARK_TEMPLATE(assign, tag::std_copy, std::vector, seqan::Dna);
BENCHMARK_TEMPLATE(assign, tag::std_copy, std::vector, seqan::Dna5);
BENCHMARK_TEMPLATE(assign, tag::std_copy, std::vector, seqan::AminoAcid);
BENCHMARK_TEMPLATE(assign, tag::std_copy, seqan::String, seqan::Dna);
BENCHMARK_TEMPLATE(assign, tag::std_copy, seqan::String, seqan::Dna5);
BENCHMARK_TEMPLATE(assign, tag::std_copy, seqan::String, seqan::AminoAcid);

BENCHMARK_TEMPLATE(assign, tag::uninitialized_copy, std::vector, seqan::Dna);
BENCHMARK_TEMPLATE(assign, tag::uninitialized_copy, std::vector, seqan::Dna5);
BENCHMARK_TEMPLATE(assign, tag::uninitialized_copy, std::vector, seqan::AminoAcid);
BENCHMARK_TEMPLATE(assign, tag::uninitialized_copy, seqan::String, seqan::Dna);
BENCHMARK_TEMPLATE(assign, tag::uninitialized_copy, seqan::String, seqan::Dna5);
BENCHMARK_TEMPLATE(assign, tag::uninitialized_copy, seqan::String, seqan::AminoAcid);
#endif // SEQAN3_HAS_SEQAN2

BENCHMARK_MAIN();
