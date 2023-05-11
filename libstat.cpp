#include "libstat.h"

void statistics::aggregations::SumAggregation::PutValue(double value) {
    sum_ += value;
}

std::optional<double> statistics::aggregations::SumAggregation::Get() const {
    return sum_;
}

void statistics::aggregations::AggregateMaximum::PutValue(double value) {
    cur_max_ = std::max(value, cur_max_.value_or(value));
}

std::optional<double> statistics::aggregations::AggregateMaximum::Get() const {
    return cur_max_;
}

void statistics::aggregations::AggregatorAverage::PutValue(double value) {
    sum_.PutValue(value);
    ++count_;
}

std::optional<double> statistics::aggregations::AggregatorAverage::Get() const {
    auto val = sum_.Get();
    if (!val || count_ == 0) {
        return std::nullopt;
    }

    return *val / count_;
}

void statistics::aggregations::AggregStd::PutValue(double value) {
    sum_.PutValue(value);
    sum_sq_.PutValue(value * value);
    ++count_;
}

std::optional<double> statistics::aggregations::AggregStd::Get() const {
    auto val = sum_.Get();
    auto val2 = sum_sq_.Get();

    if (!val || !val2 || count_ < 2) {
        return std::nullopt;
    }

    return ::std::sqrt((*val2 - *val * *val / count_) / count_);
}

void statistics::aggregations::Mode::PutValue(double value) {
    const size_t new_count = ++counts_[round(value)];

    if (new_count > cur_count_) {
        cur_max_ = value;
        cur_count_ = new_count;
    }
}

std::optional<double> statistics::aggregations::Mode::Get() const {
    return cur_max_;
}
