#pragma once

// Copyright (c) 2018-present The Alive2 Authors.
// Distributed under the MIT license that can be found in the LICENSE file.

#include "ir/value.h"

namespace IR {

class Function;


class Instr : public Value {
protected:
  static VoidType voidTy;
  Instr(Type &type, std::string &&name) : Value(type, std::move(name)) {}

public:
  virtual smt::expr eqType(const Instr &i) const;
  smt::expr getTypeConstraints() const override;
  virtual smt::expr getTypeConstraints(const Function &f) const = 0;
};


class BinOp final : public Instr {
public:
  enum Op { Add, Sub, Mul, SDiv, UDiv, SRem, URem, Shl, AShr, LShr,
            And, Or, Xor };
  enum Flags { None = 0, NSW = 1, NUW = 2, NSWNUW = 3, Exact = 4 };

private:
  Value &lhs, &rhs;
  Op op;
  Flags flags;

public:
  BinOp(Type &type, std::string &&name, Value &lhs, Value &rhs, Op op,
        Flags flags = None);

  void print(std::ostream &os) const override;
  StateValue toSMT(State &s) const override;
  smt::expr getTypeConstraints(const Function &f) const override;
};


class ConversionOp final : public Instr {
public:
  enum Op { SExt, ZExt, Trunc };

private:
  Value &val;
  Op op;

public:
  ConversionOp(Type &type, std::string &&name, Value &val, Op op)
    : Instr(type, std::move(name)), val(val), op(op) {}
  void print(std::ostream &os) const override;
  StateValue toSMT(State &s) const override;
  smt::expr getTypeConstraints(const Function &f) const override;
};


class Select final : public Instr {
  Value &cond, &a, &b;
public:
  Select(Type &type, std::string &&name, Value &cond, Value &a, Value &b)
    : Instr(type, move(name)), cond(cond), a(a), b(b) {}
  void print(std::ostream &os) const override;
  StateValue toSMT(State &s) const override;
  smt::expr getTypeConstraints(const Function &f) const override;
};


class ICmp final : public Instr {
public:
  enum Cond { EQ, NE, SLE, SLT, SGE, SGT, ULE, ULT, UGE, UGT, Any };

private:
  Value &a, &b;
  smt::expr cond_var;
  Cond cond;
  bool defined;

public:
  ICmp(Type &type, std::string &&name, Cond cond, Value &a, Value &b);
  void print(std::ostream &os) const override;
  StateValue toSMT(State &s) const override;
  smt::expr getTypeConstraints(const Function &f) const override;
  void fixupTypes(const smt::Model &m) override;
  smt::expr eqType(const Instr &i) const override;
};


class Freeze final : public Instr {
  Value &val;
public:
  Freeze(Type &type, std::string &&name, Value &val)
    : Instr(type, std::move(name)), val(val) {}

  void print(std::ostream &os) const override;
  StateValue toSMT(State &s) const override;
  smt::expr getTypeConstraints(const Function &f) const override;
};


class CopyOp final : public Instr {
  Value &val;
public:
  CopyOp(Type &type, std::string &&name, Value &val)
    : Instr(type, std::move(name)), val(val) {}

  void print(std::ostream &os) const override;
  StateValue toSMT(State &s) const override;
  smt::expr getTypeConstraints(const Function &f) const override;
};


class Return final : public Instr {
  Value &val;
public:
  Return(Type &type, Value &val) : Instr(type, "return"), val(val) {}

  void print(std::ostream &os) const override;
  StateValue toSMT(State &s) const override;
  smt::expr getTypeConstraints(const Function &f) const override;
};


class Unreachable final : public Instr {
public:
  Unreachable() : Instr(Instr::voidTy, "") {}

  void print(std::ostream &os) const override;
  StateValue toSMT(State &s) const override;
  smt::expr getTypeConstraints(const Function &f) const override;
};

}
