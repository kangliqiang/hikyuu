/*
 *  Copyright (c) 2024 hikyuu.org
 *
 *  Created on: 2024-12-20
 *      Author: fasiondog
 */

#pragma once

#define EXPOERT_TA_FUNC(func) BOOST_CLASS_EXPORT(hku::Cls_##func)

#define TA_IN1_OUT1_IMP(func)                                                              \
    Cls_##func::Cls_##func() : IndicatorImp(#func, 1) {}                                   \
    Cls_##func::~Cls_##func() {}                                                           \
                                                                                           \
    void Cls_##func::_checkParam(const string &name) const {}                              \
                                                                                           \
    void Cls_##func::_calculate(const Indicator &data) {                                   \
        m_discard = data.discard();                                                        \
        size_t total = data.size();                                                        \
        if (m_discard >= total) {                                                          \
            m_discard = total;                                                             \
            return;                                                                        \
        }                                                                                  \
                                                                                           \
        auto const *src = data.data();                                                     \
        auto *dst = this->data();                                                          \
                                                                                           \
        int outBegIdx;                                                                     \
        int outNbElement;                                                                  \
        func(m_discard, total - 1, src, &outBegIdx, &outNbElement, dst + m_discard);       \
        if (outBegIdx != m_discard) {                                                      \
            memmove(dst + outBegIdx, dst + m_discard, sizeof(double) * outNbElement);      \
            double null_double = Null<double>();                                           \
            for (size_t i = m_discard; i < outBegIdx; ++i) {                               \
                _set(null_double, i);                                                      \
            }                                                                              \
            m_discard = outBegIdx;                                                         \
        }                                                                                  \
    }                                                                                      \
                                                                                           \
    void Cls_##func::_dyn_run_one_step(const Indicator &ind, size_t curPos, size_t step) { \
        HKU_IF_RETURN(ind.discard() > curPos, void());                                     \
        double buf[1];                                                                     \
        auto const *src = ind.data();                                                      \
        int outBegIdx;                                                                     \
        int outNbElement;                                                                  \
        func(curPos, curPos + 1, src, &outBegIdx, &outNbElement, buf);                     \
        if (outNbElement >= 1) {                                                           \
            _set(buf[0], curPos);                                                          \
        }                                                                                  \
    }                                                                                      \
                                                                                           \
    Indicator HKU_API func() {                                                             \
        return Indicator(make_shared<Cls_##func>());                                       \
    }

#define TA_IN1_OUT1_N_IMP(func, func_lookback, period, period_min, period_max)               \
    Cls_##func::Cls_##func() : IndicatorImp(#func, 1) {                                      \
        setParam<int>("n", period);                                                          \
    }                                                                                        \
    Cls_##func::~Cls_##func() {}                                                             \
                                                                                             \
    void Cls_##func::_checkParam(const string &name) const {                                 \
        if (name == "n") {                                                                   \
            int n = getParam<int>("n");                                                      \
            HKU_ASSERT(n >= period_min && n <= period_max);                                  \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    void Cls_##func::_calculate(const Indicator &data) {                                     \
        int n = getParam<int>("n");                                                          \
        m_discard = data.discard() + TA_CMO_Lookback(n);                                     \
        size_t total = data.size();                                                          \
        if (m_discard >= total) {                                                            \
            m_discard = total;                                                               \
            return;                                                                          \
        }                                                                                    \
                                                                                             \
        auto const *src = data.data();                                                       \
        auto *dst = this->data();                                                            \
                                                                                             \
        int outBegIdx;                                                                       \
        int outNbElement;                                                                    \
        func(data.discard(), total - 1, src, n, &outBegIdx, &outNbElement, dst + m_discard); \
        if (outBegIdx != m_discard) {                                                        \
            memmove(dst + outBegIdx, dst + m_discard, sizeof(double) * outNbElement);        \
            double null_double = Null<double>();                                             \
            for (size_t i = m_discard; i < outBegIdx; ++i) {                                 \
                _set(null_double, i);                                                        \
            }                                                                                \
            m_discard = outBegIdx;                                                           \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    void Cls_##func::_dyn_run_one_step(const Indicator &ind, size_t curPos, size_t step) {   \
        int back = func_lookback(step);                                                      \
        HKU_IF_RETURN(back<0 || back + ind.discard()> curPos, void());                       \
                                                                                             \
        std::unique_ptr<double[]> buf = std::make_unique<double[]>(curPos);                  \
        auto const *src = ind.data();                                                        \
        int outBegIdx;                                                                       \
        int outNbElement;                                                                    \
        func(ind.discard(), curPos, src, step, &outBegIdx, &outNbElement, buf.get());        \
        if (outNbElement >= 1) {                                                             \
            _set(buf.get()[outNbElement - 1], curPos);                                       \
        }                                                                                    \
    }                                                                                        \
                                                                                             \
    Indicator HKU_API func(int n) {                                                          \
        auto p = make_shared<Cls_##func>();                                                  \
        p->setParam<int>("n", n);                                                            \
        return Indicator(p);                                                                 \
    }                                                                                        \
                                                                                             \
    Indicator HKU_API func(const IndParam &n) {                                              \
        IndicatorImpPtr p = make_shared<Cls_##func>();                                       \
        p->setIndParam("n", n);                                                              \
        return Indicator(p);                                                                 \
    }
