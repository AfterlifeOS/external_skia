/*
 * Copyright 2020 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "include/sksl/DSLVar.h"

#include "include/core/SkTypes.h"
#include "include/private/SkSLDefines.h"
#include "include/private/SkSLStatement.h"
#include "include/private/SkSLString.h"
#include "include/private/SkSLSymbol.h"
#include "include/sksl/DSLModifiers.h"
#include "include/sksl/DSLType.h"
#include "include/sksl/SkSLOperator.h"
#include "src/sksl/SkSLThreadContext.h"
#include "src/sksl/dsl/priv/DSLWriter.h"
#include "src/sksl/ir/SkSLBinaryExpression.h"
#include "src/sksl/ir/SkSLExpression.h"
#include "src/sksl/ir/SkSLFieldAccess.h"
#include "src/sksl/ir/SkSLFunctionCall.h"
#include "src/sksl/ir/SkSLSymbolTable.h"
#include "src/sksl/ir/SkSLType.h"
#include "src/sksl/ir/SkSLVariable.h"

#include <string>

namespace SkSL {

namespace dsl {

DSLVarBase::DSLVarBase(DSLType type, std::string_view name, DSLExpression initialValue,
                       Position pos, Position namePos)
    : DSLVarBase(DSLModifiers(), std::move(type), name, std::move(initialValue), pos, namePos) {}

DSLVarBase::DSLVarBase(DSLType type, DSLExpression initialValue, Position pos, Position namePos)
    : DSLVarBase(type, "var", std::move(initialValue), pos, namePos) {}

DSLVarBase::DSLVarBase(const DSLModifiers& modifiers, DSLType type, DSLExpression initialValue,
                       Position pos, Position namePos)
    : DSLVarBase(modifiers, type, "var", std::move(initialValue), pos, namePos) {}

DSLVarBase::DSLVarBase(const DSLModifiers& modifiers, DSLType type, std::string_view name,
                       DSLExpression initialValue, Position pos, Position namePos)
    : fModifiers(std::move(modifiers))
    , fType(std::move(type))
    , fNamePosition(namePos)
    , fRawName(name)
    , fName(fType.skslType().isOpaque() ? name : DSLWriter::Name(name))
    , fInitialValue(std::move(initialValue))
    , fDeclared(DSLWriter::MarkVarsDeclared())
    , fPosition(pos) {}

DSLVarBase::~DSLVarBase() {
    if (fDeclaration && !fDeclared) {
        ThreadContext::ReportError(String::printf("variable '%.*s' was destroyed without being "
                                                  "declared",
                                                  (int)fRawName.length(),
                                                  fRawName.data()).c_str());
    }
}

void DSLVarBase::swap(DSLVarBase& other) {
    SkASSERT(this->storage() == other.storage());
    std::swap(fModifiers, other.fModifiers);
    std::swap(fType, other.fType);
    std::swap(fUniformHandle, other.fUniformHandle);
    std::swap(fDeclaration, other.fDeclaration);
    std::swap(fVar, other.fVar);
    std::swap(fNamePosition, other.fNamePosition);
    std::swap(fRawName, other.fRawName);
    std::swap(fName, other.fName);
    std::swap(fInitialValue.fExpression, other.fInitialValue.fExpression);
    std::swap(fDeclared, other.fDeclared);
    std::swap(fInitialized, other.fInitialized);
    std::swap(fPosition, other.fPosition);
}

void DSLVar::swap(DSLVar& other) {
    INHERITED::swap(other);
}

VariableStorage DSLVar::storage() const {
    return VariableStorage::kLocal;
}

DSLGlobalVar::DSLGlobalVar(const char* name)
    : INHERITED(kVoid_Type, name, DSLExpression(), Position(), Position()) {
    fName = name;
    DSLWriter::MarkDeclared(*this);
    const SkSL::Symbol* result = (*ThreadContext::SymbolTable())[fName];
    SkASSERTF(result, "could not find '%.*s' in symbol table", (int)fName.length(), fName.data());
    fVar = &result->as<SkSL::Variable>();
    fInitialized = true;
}

void DSLGlobalVar::swap(DSLGlobalVar& other) {
    INHERITED::swap(other);
}

VariableStorage DSLGlobalVar::storage() const {
    return VariableStorage::kGlobal;
}

void DSLParameter::swap(DSLParameter& other) {
    INHERITED::swap(other);
}

VariableStorage DSLParameter::storage() const {
    return VariableStorage::kParameter;
}


DSLPossibleExpression DSLVarBase::operator[](DSLExpression&& index) {
    return DSLExpression(*this, Position())[std::move(index)];
}

DSLPossibleExpression DSLVarBase::assign(DSLExpression expr) {
    return BinaryExpression::Convert(ThreadContext::Context(), Position(),
            DSLExpression(*this, Position()).release(), SkSL::Operator::Kind::EQ,
            expr.release());
}

std::unique_ptr<SkSL::Expression> DSLGlobalVar::methodCall(std::string_view methodName,
                                                           Position pos) {
    if (!this->fType.isEffectChild()) {
        ThreadContext::ReportError("type does not support method calls", pos);
        return nullptr;
    }
    return FieldAccess::Convert(ThreadContext::Context(), pos, *ThreadContext::SymbolTable(),
            DSLExpression(*this, pos).release(), methodName);
}

DSLExpression DSLGlobalVar::eval(ExpressionArray args, Position pos) {
    auto method = this->methodCall("eval", pos);
    return DSLExpression(
            method ? SkSL::FunctionCall::Convert(ThreadContext::Context(), pos, std::move(method),
                                                 std::move(args))
                   : nullptr,
            pos);
}

DSLExpression DSLGlobalVar::eval(DSLExpression x, Position pos) {
    ExpressionArray converted;
    converted.push_back(x.release());
    return this->eval(std::move(converted), pos);
}

DSLExpression DSLGlobalVar::eval(DSLExpression x, DSLExpression y, Position pos) {
    ExpressionArray converted;
    converted.push_back(x.release());
    converted.push_back(y.release());
    return this->eval(std::move(converted), pos);
}

} // namespace dsl

} // namespace SkSL
