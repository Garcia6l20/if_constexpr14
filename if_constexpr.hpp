/**
 * @file if_constexpr
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

            constexpr if_constexpr(TrueT &&trueT, FalseT &&falseT = nullptr) : true_{std::move(trueT)},
                                                                     false_{std::move(falseT)} {
            }

            template<bool check = result, std::enable_if_t<check, int> = 0>
            constexpr auto operator()() {
                return true_();
            }

            template<bool check = result, std::enable_if_t<!check && !std::is_same<FalseT, std::nullptr_t>::value, int> = 0>
            constexpr auto operator()() {
                return false_();
            }

            template<bool check = result, std::enable_if_t<!check && std::is_same<FalseT, std::nullptr_t>::value, int> = 0>
            constexpr void operator()() {}
        };
    }

//    template <typename ElifT, typename...RestT>
//    struct else_if_ {
//        constexpr auto operator()(ElifT &&elif, RestT...rest) {
//
//        }
//    }
//
//    template<bool result, typename TrueT, typename ElifT, typename...RestT>
//    constexpr auto _if_(TrueT &&trueT, else_if<ElifT> &&elif, RestT &&...rest) {
//        return detail::if_<result, TrueT, RestT...>(std::forward<TrueT>(trueT), std::forward<RestT>(rest)...)();
//    }
//
//    template<bool result, typename TrueT, typename...RestT>
//    constexpr auto if_(TrueT &&trueT, RestT &&...rest) {
//        return detail::_if_<result, TrueT, RestT...>(std::forward<TrueT>(trueT), std::forward<RestT>(rest)...)();
//    }

    template<bool result, typename TrueT, typename FalseT>
    constexpr auto if_(TrueT &&trueT, FalseT &&falseT) {
        return detail::if_constexpr<result, TrueT, FalseT>(std::forward<TrueT>(trueT), std::forward<FalseT>(falseT))();
    }
    template<bool result, typename TrueT>
    constexpr auto if_(TrueT &&trueT) {
        return detail::if_constexpr<result, TrueT>(std::forward<TrueT>(trueT))();
    }
}
