/*
 * IBackset.h
 *
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2019-5-13
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_IMP_IBACKSET_H_
#define INDICATOR_IMP_IBACKSET_H_

#include "../Indicator.h"

namespace hku {

class IBackset : public IndicatorImp {
    INDICATOR_IMP_SUPPORT_IND_PARAM(IBackset)
    INDICATOR_IMP_NO_PRIVATE_MEMBER_SERIALIZATION

public:
    IBackset();
    virtual ~IBackset();

    virtual bool isSerial() const override {
        return true;
    }
};

} /* namespace hku */
#endif /* INDICATOR_IMP_IBACKSET_H_ */
