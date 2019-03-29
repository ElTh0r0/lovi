#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <QString>

class Condition {
public:
    explicit Condition(int column);
    virtual ~Condition() = default;
    virtual bool eval(const QString& value) const = 0;

    int column() const;

private:
    const int mColumn;
};

class ExactCondition : public Condition {
public:
    explicit ExactCondition(int column, const QString& expected);

    bool eval(const QString& value) const override;

private:
    const QString mExpected;
};

class ContainsCondition : public Condition {
public:
    explicit ContainsCondition(int column, const QString& expected);

    bool eval(const QString& value) const override;

private:
    const QString mExpected;
};

#endif // CONDITIONS_H