
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

enum class statistics_data_kind
{
    population,
    sample,
};

enum class statistics_skewness_kind
{
    adjusted,
    unadjusted,
};

enum class statistics_kurtosis_kind
{
    excess,
    nonexcess,
};

// 5.2 Accumulator Objects

template<class T>
class mean_accumulator;

template<class T, class W = T>
class weighted_mean_accumulator;

template<class T>
class geometric_mean_accumulator;

template<class T, class W = T>
class weighted_geometric_mean_accumulator;

template<class T>
class harmonic_mean_accumulator;

template<class T, class W = T>
class weighted_harmonic_mean_accumulator;

template<class T>
class variance_accumulator;

template<class T, class W = T>
class weighted_variance_accumulator;

template<class T>
class standard_deviation_accumulator;

template<class T, class W = T>
class weighted_standard_deviation_accumulator;

// 5.2.1 Mean Accumulator Class Templates

template<class T>
class mean_accumulator : public weighted_mean_accumulator<T, T>
{
  public:
    constexpr mean_accumulator() noexcept = default;

    constexpr void operator()(const T& x)
    {
        this->v1_ += T{1};
        this->s1_ += x;
    }

    constexpr T value()
    {
        return weighted_mean_accumulator<T, T>::value();
    }
};

template<class T, class W>
class weighted_mean_accumulator
{
  protected:
    W v1_{0};
    T s1_{0};

  public:
    constexpr weighted_mean_accumulator() noexcept = default;

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

// 5.2.2 Geometric Mean Accumulator Class Templates

template<class T>
class geometric_mean_accumulator : public weighted_geometric_mean_accumulator<T, T>
{
  public:
    constexpr geometric_mean_accumulator() noexcept = default;

    constexpr void operator()(const T& x)
    {
        this->v1_ += T{1};
        this->s1_ *= x;
    }
    constexpr auto value() -> T
    {
        return weighted_geometric_mean_accumulator<T, T>::value();
    }
};

template<class T, class W>
class weighted_geometric_mean_accumulator
{
  protected:
    W v1_{0};
    T s1_{1};

  public:
    constexpr weighted_geometric_mean_accumulator() noexcept = default;

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

// 5.2.3 Harmonic Mean Accumulator Class Templates

template<class T>
class harmonic_mean_accumulator : public weighted_harmonic_mean_accumulator<T, T>
{
  public:
    constexpr harmonic_mean_accumulator() noexcept = default;

    constexpr void operator()(const T& x)
    {
        this->v1_ += T{1};
        this->s1_ += T{1} / x;
    }

    constexpr auto value() -> T
    {
        return weighted_harmonic_mean_accumulator<T, T>::value();
    }
};

template<class T, class W>
class weighted_harmonic_mean_accumulator
{
  protected:
    W v1_{0};
    T s1_{0};

  public:
    constexpr weighted_harmonic_mean_accumulator() noexcept = default;

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

// 5.2.4 Variance Accumulator Class Templates

template<class T>
class variance_accumulator
{
  protected:
    T ddof_{};
    T v1_{0};
    T s1_{0};
    T s2_{0};

  public:
    constexpr variance_accumulator(T ddof) noexcept
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

template<class T, class W>
class weighted_variance_accumulator
{
  protected:
    statistics_data_kind data_kind_{};
    W v1_{0};
    W v2_{0};
    T s1_{0};
    T s2_{0};

  public:
    constexpr weighted_variance_accumulator(statistics_data_kind k) noexcept
    {
        data_kind_ = k;
    }

    constexpr void operator()(const T& x, const W& w)
    {
        v1_ += w;
        v2_ += w * w;  // N.B. overhead for population weighted variance
        s1_ += w * x;
        s2_ += w * x * x;
    }

    constexpr auto value() -> T
    {
        if (data_kind_ == statistics_data_kind::population) {
            return s2_ / v1_ - (s1_ / v1_) * (s1_ / v1_);
        } else {
            return v1_ / (v1_ * v1_ - v2_) * (s2_ - s1_ * (s1_ / v1_));
        }
    }
};

// 5.2.5 Standard Deviation Accumulator Class Templates

template<class T>
class standard_deviation_accumulator : public variance_accumulator<T>
{
  public:
    constexpr standard_deviation_accumulator(T ddof) noexcept
        : variance_accumulator<T>(ddof)
    {
    }

    constexpr void operator()(const T& x)
    {
        return variance_accumulator<T>::operator()(x);
    }

    constexpr auto value() -> T
    {
        auto var = variance_accumulator<T>::value();
        return sqrt(var);
    }
};

template<class T, class W>
class weighted_standard_deviation_accumulator : public weighted_variance_accumulator<T, W>
{
  public:
    constexpr weighted_standard_deviation_accumulator(statistics_data_kind k) noexcept
        : weighted_variance_accumulator<T>(k)
    {
    }

    constexpr void operator()(const T& x, const W& w)
    {
        return weighted_variance_accumulator<T, W>::operator()(x, w);
    }

    constexpr auto value() -> T
    {
        auto var = weighted_variance_accumulator<T>::value();
        return sqrt(var);
    }
};

// 5.2.8 Accumulator Objects Accumulation Functions

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

// 5.3 Freestanding Functions

// 5.3.1 Freestanding Mean Functions

template<ranges::input_range R>
constexpr auto mean(R&& r) -> ranges::range_value_t<R>
{
    return detail::get_stat<mean_accumulator>(forward<R>(r));
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto mean(R&& r, W&& w) -> ranges::range_value_t<R>
{
    return detail::get_stat<weighted_mean_accumulator>(forward<R>(r), forward<W>(w));
}

// 5.3.2 Freestanding Geometric Mean Functions

template<ranges::input_range R>
constexpr auto geometric_mean(R&& r) -> ranges::range_value_t<R>
{
    return detail::get_stat<geometric_mean_accumulator>(forward<R>(r));
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto geometric_mean(R&& r, W&& w) -> ranges::range_value_t<R>
{
    return detail::get_stat<weighted_geometric_mean_accumulator>(forward<R>(r), forward<W>(w));
}

// 5.3.3 Freestanding Harmonic Mean Functions

template<ranges::input_range R>
constexpr auto harmonic_mean(R&& r) -> ranges::range_value_t<R>
{
    return detail::get_stat<harmonic_mean_accumulator>(forward<R>(r));
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto harmonic_mean(R&& r, W&& w) -> ranges::range_value_t<R>
{
    return detail::get_stat<weighted_harmonic_mean_accumulator>(forward<R>(r), forward<W>(w));
}

// 5.3.4 Freestanding Variance Functions

template<ranges::input_range R>
constexpr auto variance(R&& r, ranges::range_value_t<R> ddof) -> ranges::range_value_t<R>
{
    return detail::get_stat<variance_accumulator>(forward<R>(r), ddof);
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto variance(R&& r, W&& w, statistics_data_kind k) -> ranges::range_value_t<R>
{
    return detail::get_stat<weighted_variance_accumulator>(forward<R>(r), forward<W>(w), k);
}

// 5.3.5 Freestanding Standard Deviation Functions

template<ranges::input_range R>
constexpr auto standard_deviation(R&& r, ranges::range_value_t<R> ddof) -> ranges::range_value_t<R>
{
    return detail::get_stat<standard_deviation_accumulator>(forward<R>(r), ddof);
}

template<ranges::input_range R, ranges::input_range W>
constexpr auto standard_deviation(R&& r, W&& w, statistics_data_kind k) -> ranges::range_value_t<R>
{
    return detail::get_stat<weighted_standard_deviation_accumulator>(forward<R>(r), forward<W>(w), k);
}

}  // namespace xstd
