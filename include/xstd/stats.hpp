
#pragma once

#include <algorithm>
#include <cmath>
#include <iterator>
#include <ranges>
#include <type_traits>

namespace xstd
{

namespace ranges = std::ranges;
namespace views = ranges::views;

using std::forward;

using std::pow;
using std::sqrt;

namespace detail
{

template<class R1, class... Fn>
constexpr void for_each_pack_1(R1&& r1, Fn&&... fs)
{
    for (auto&& x1 : r1) {
        (fs(x1), ...);
    }
}

template<class R1, class R2, class... Fn>
constexpr void for_each_pack_2(R1&& r1, R2&& r2, Fn&&... fs)
{
    for (auto&& [x1, x2] : views::zip(r1, r2)) {
        (fs(x1, x2), ...);
    }
}

template<template<class T> class S, class R, class... Args>
constexpr auto get_stat(R&& r, Args&&... args)
{
    using T = ranges::range_value_t<R>;
    S<T> s{forward<Args>(args)...};
    for_each_pack_1(forward<R>(r), s);
    return s.value();
}

template<template<class T, class U> class S, class R, class W, class... Args>
constexpr auto get_stat(R&& r, W&& w, Args&&... args)
{
    using T = ranges::range_value_t<R>;
    using U = ranges::range_value_t<W>;
    S<T, U> s{forward<Args>(args)...};
    for_each_pack_2(forward<R>(r), forward<W>(w), s);
    return s.value();
}

}  // namespace detail

// data types
enum class stats_data_kind
{
    population,
    sample,
};

enum class stats_skewness_kind
{
    adjusted,
    unadjusted,
};

enum class stats_kurtosis_kind
{
    excess,
    nonexcess,
};

// accumulator objects

template<class T>
class mean_accumulator;

template<class T, class W = T>
class weighted_mean_accumulator
{
  protected:
    W v1_{0};
    T s1_{0};

  public:
    explicit constexpr weighted_mean_accumulator() noexcept = default;

    constexpr void operator()(const T& x, const W& w)
    {
        v1_ += w;
        s1_ += x * w;
    }

    constexpr auto value() -> T
    {
        return s1_ / v1_;
    }
};

template<class T>
class mean_accumulator : public weighted_mean_accumulator<T, T>
{
  public:
    explicit constexpr mean_accumulator() noexcept = default;

    constexpr void operator()(const T& x)
    {
        this->v1_ += T{1};
        this->s1_ += x;
    }
};

template<class T>
class geometric_mean_accumulator;

template<class T, class W = T>
class weighted_geometric_mean_accumulator
{
  protected:
    W v1_{0};
    T s1_{1};

  public:
    explicit constexpr weighted_geometric_mean_accumulator() noexcept = default;

    constexpr void operator()(const T& x, const W& w)
    {
        v1_ += w;
        s1_ *= pow(x, w);
    }

    constexpr auto value() -> T
    {
        return pow(s1_, T{1} / v1_);
    }
};

template<class T>
class geometric_mean_accumulator : public weighted_geometric_mean_accumulator<T, T>
{
  public:
    explicit constexpr geometric_mean_accumulator() noexcept = default;

    constexpr void operator()(const T& x)
    {
        this->v1_ += T{1};
        this->s1_ *= x;
    }
};

template<class T>
class harmonic_mean_accumulator;

template<class T, class W = T>
class weighted_harmonic_mean_accumulator
{
  protected:
    W v1_{0};
    T s1_{0};

  public:
    explicit constexpr weighted_harmonic_mean_accumulator() noexcept = default;

    constexpr void operator()(const T& x, const W& w)
    {
        v1_ += w;
        s1_ += w / x;
    }

    constexpr auto value() -> T
    {
        return v1_ / s1_;
    }
};

template<class T>
class harmonic_mean_accumulator : public weighted_harmonic_mean_accumulator<T, T>
{
  public:
    explicit constexpr harmonic_mean_accumulator() noexcept = default;

    constexpr void operator()(const T& x)
    {
        this->v1_ += T{1};
        this->s1_ += T{1} / x;
    }
};

template<class T>
class variance_accumulator
{
  protected:
    T ddof_{};
    T v1_{0};
    T s1_{0};
    T s2_{0};

  public:
    explicit constexpr variance_accumulator(stats_data_kind k) noexcept
    {
        if (k == stats_data_kind::population) {
            ddof_ = T{0};
        } else {
            ddof_ = T{1};
        }
    }

    explicit constexpr variance_accumulator(T ddof) noexcept
    {
        ddof_ = ddof;
    }

    constexpr void operator()(const T& x)
    {
        v1_ += T{1};
        s1_ += x;
        s2_ += x * x;
    }

    constexpr auto value() -> T
    {
        return v1_ / (v1_ - ddof_) * (s2_ / v1_ - (s1_ / v1_) * (s1_ / v1_));
    }
};

template<class T, class W = T>
class weighted_variance_accumulator
{
  protected:
    stats_data_kind data_kind_{};
    W v1_{0};
    W v2_{0};
    T s1_{0};
    T s2_{0};

  public:
    explicit constexpr weighted_variance_accumulator(stats_data_kind k) noexcept
    {
        data_kind_ = k;
    }

    constexpr void operator()(const T& x, const W& w)
    {
        v1_ += w;
        v2_ += w * w;  // overhead for population weighted variance
        s1_ += w * x;
        s2_ += w * x * x;
    }

    constexpr auto value() -> T
    {
        if (data_kind_ == stats_data_kind::population) {
            return s2_ / v1_ - (s1_ / v1_) * (s1_ / v1_);
        } else {
            return v1_ / (v1_ * v1_ - v2_) * (s2_ - s1_ * (s1_ / v1_));
        }
    }
};

template<class T>
class standard_deviation_accumulator : public variance_accumulator<T>
{
  public:
    explicit constexpr standard_deviation_accumulator(stats_data_kind k) noexcept
        : variance_accumulator<T>(k)
    {
    }

    explicit constexpr standard_deviation_accumulator(T ddof) noexcept
        : variance_accumulator<T>(ddof)
    {
    }

    constexpr auto value() -> T
    {
        auto var = variance_accumulator<T>::value();
        return sqrt(var);
    }
};

template<class T, class W = T>
class weighted_standard_deviation_accumulator : public weighted_variance_accumulator<T, W>
{
  public:
    explicit constexpr weighted_standard_deviation_accumulator(stats_data_kind k) noexcept
        : weighted_variance_accumulator<T>(k)
    {
    }

    constexpr auto value() -> T
    {
        auto var = weighted_variance_accumulator<T>::value();
        return sqrt(var);
    }
};

// accumulator objects accumulation functions

template<ranges::input_range R, class... Accumulators>
constexpr void stats_accumulate(R&& r, Accumulators&&... acc)
{
    detail::for_each_pack_1(forward<R>(r), forward<Accumulators>(acc)...);
}

template<ranges::input_range R, ranges::input_range W, class... Accumulators>
constexpr void stats_accumulate(R&& r, W&& w, Accumulators&&... acc)
{
    detail::for_each_pack_2(forward<R>(r), forward<W>(w), forward<Accumulators>(acc)...);
}

// free functions

template<ranges::input_range R>
constexpr auto mean(R&& r)
{
    return detail::get_stat<mean_accumulator>(forward<R>(r));
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto mean(R&& r, W&& w)
{
    return detail::get_stat<weighted_mean_accumulator>(forward<R>(r), forward<W>(w));
}

template<ranges::input_range R>
constexpr auto geometric_mean(R&& r)
{
    return detail::get_stat<geometric_mean_accumulator>(forward<R>(r));
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto geometric_mean(R&& r, W&& w)
{
    return detail::get_stat<weighted_geometric_mean_accumulator>(forward<R>(r), forward<W>(w));
}

template<ranges::input_range R>
constexpr auto harmonic_mean(R&& r)
{
    return detail::get_stat<harmonic_mean_accumulator>(forward<R>(r));
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto harmonic_mean(R&& r, W&& w)
{
    return detail::get_stat<weighted_harmonic_mean_accumulator>(forward<R>(r), forward<W>(w));
}

template<ranges::input_range R>
constexpr auto variance(R&& r, stats_data_kind k)
{
    return detail::get_stat<variance_accumulator>(forward<R>(r), k);
}

template<ranges::input_range R>
constexpr auto variance(R&& r, ranges::range_value_t<R> ddof)
{
    return detail::get_stat<variance_accumulator>(forward<R>(r), ddof);
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto variance(R&& r, W&& w, stats_data_kind k)
{
    return detail::get_stat<weighted_variance_accumulator>(forward<R>(r), forward<W>(w), k);
}

template<ranges::input_range R>
constexpr auto standard_deviation(R&& r, stats_data_kind k)
{
    return detail::get_stat<standard_deviation_accumulator>(forward<R>(r), k);
}

template<ranges::input_range R>
constexpr auto standard_deviation(R&& r, ranges::range_value_t<R> ddof)
{
    return detail::get_stat<standard_deviation_accumulator>(forward<R>(r), ddof);
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto standard_deviation(R&& r, W&& w, stats_data_kind k)
{
    return detail::get_stat<weighted_standard_deviation_accumulator>(forward<R>(r), forward<W>(w), k);
}

}  // namespace xstd
