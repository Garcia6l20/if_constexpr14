/**
 * @file if_constexpr.hpp
 * @autor Garcia Sylvain <garcia dot 6l20 dot gmail dot com>
 * @brief An `if constexpr` alternative for c++11
 */
#pragma once

#include <utility>

namespace ic {

    namespace detail {
        template<bool result, typename TrueT, typename FalseT = std::nullptr_t>
        struct if_constexpr {
            TrueT true_;
            FalseT false_;

            constexpr explicit if_constexpr(TrueT trueT, FalseT falseT = nullptr) noexcept
                    : true_{std::move(trueT)}, false_{std::move(falseT)} {}

            template<bool check = result, std::enable_if_t<check, int> = 0>
            constexpr auto operator()() noexcept(noexcept(true_())) {
                return true_();
            }

            template<bool check = result, std::enable_if_t<
                    !check && !std::is_same<FalseT, std::nullptr_t>::value, int> = 0>
            constexpr auto operator()() noexcept(noexcept(false_())) {
                return false_();
            }

            template<bool check = result, std::enable_if_t<
                    !check && std::is_same<FalseT, std::nullptr_t>::value, int> = 0>
            constexpr void operator()() noexcept {}
        };

        template<typename ThenT>
        struct else_ {
            ThenT then_;

            constexpr explicit else_(ThenT then) noexcept
                    : then_{std::move(then)} {}
        };

        template<class T, template<class...> class Template>
        struct is_specialization : std::false_type {
        };

        template<template<class...> class Template, class... Args>
        struct is_specialization<Template<Args...>, Template> : std::true_type {
        };

        template<bool result, typename CaseT>
        struct case_constexpr {
            static constexpr bool value = result;
            CaseT case_;

            constexpr explicit case_constexpr(CaseT &&case_) noexcept
                    : case_{std::move(case_)} {}

            constexpr auto operator()() noexcept {
                return case_();
            }
        };
    }

    template<bool result, typename TrueT, typename ElseT,
            std::enable_if_t<detail::is_specialization<ElseT, detail::else_>::value, int> = 0>
    constexpr auto if_(TrueT &&trueT, ElseT &&else_) {
        return detail::if_constexpr<result, TrueT, decltype(else_.then_)>(std::forward<TrueT>(trueT),
                                                                          std::move(else_.then_))();
    }

    template<bool result, typename TrueT>
    constexpr auto if_(TrueT &&trueT) noexcept {
        return if_<result, TrueT>(std::forward<TrueT>(trueT), [](){});
    }

    template<bool result, typename TrueT, typename ElseT,
            std::enable_if_t<!detail::is_specialization<ElseT, detail::else_>::value, int> = 0>
    constexpr auto if_(TrueT &&trueT, ElseT &&else_) noexcept {
        auto fwd = [else_ = std::forward<decltype(else_)>(else_)]() mutable {
            return else_();
        };
        return detail::if_constexpr<result, TrueT, decltype(fwd)>(std::forward<TrueT>(trueT), std::move(fwd))();
    }

    template<bool result, typename TrueT, typename ElseT>
    constexpr auto else_if_(TrueT &&trueT, ElseT &&else_) noexcept {
        return detail::if_constexpr<result, TrueT, decltype(else_.then_)>(std::forward<TrueT>(trueT),
                                                                          std::move(else_.then_));
    }

    template<bool result, typename TrueT>
    constexpr auto else_if_(TrueT &&trueT) noexcept {
        auto nop = [] {};
        return detail::if_constexpr<result, TrueT, decltype(nop)>(std::forward<TrueT>(trueT), std::move(nop));
    }

    template<typename ThenT>
    constexpr auto else_(ThenT &&then) noexcept {
        return detail::else_<ThenT>(std::forward<ThenT>(then));
    }

    template<bool result, typename CaseT>
    constexpr auto case_(CaseT &&case_) noexcept {
        return detail::case_constexpr<result, CaseT>{std::forward<CaseT>(case_)};
    }

    template<typename DefaultT>
    constexpr auto default_(DefaultT &&default_) noexcept {
        return detail::case_constexpr<true, DefaultT>{std::forward<DefaultT>(default_)};
    }

    template<typename LastT,
            std::enable_if_t<LastT::value, int> = 0>
    constexpr auto switch_(LastT &&last) noexcept {
        return last();
    }

    template<typename LastT,
            std::enable_if_t<!LastT::value, int> = 0>
    constexpr auto switch_(LastT &&last) noexcept {
    }

    template<typename FirstT, typename...CasesT,
            std::enable_if_t<FirstT::value, int> = 0>
    constexpr auto switch_(FirstT &&first, CasesT &&...) noexcept {
        return first();
    }

    template<typename FirstT, typename...CasesT,
            std::enable_if_t<!FirstT::value, int> = 0>
    constexpr auto switch_(FirstT &&first, CasesT &&...cases) noexcept {
        return switch_<CasesT...>(std::forward<CasesT>(cases)...);
    }
}
