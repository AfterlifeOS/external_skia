/*
 * Copyright 2020 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "src/sksl/dsl/DSLType.h"

#include "include/core/SkSpan.h"
#include "include/core/SkTypes.h"
#include "include/private/base/SkTArray.h"
#include "src/sksl/SkSLBuiltinTypes.h"
#include "src/sksl/SkSLContext.h"
#include "src/sksl/SkSLErrorReporter.h"
#include "src/sksl/SkSLThreadContext.h"
#include "src/sksl/ir/SkSLModifiers.h"
#include "src/sksl/ir/SkSLSymbol.h"
#include "src/sksl/ir/SkSLSymbolTable.h"  // IWYU pragma: keep
#include "src/sksl/ir/SkSLType.h"
#include "src/sksl/ir/SkSLTypeReference.h"

#include <memory>
#include <string>

using namespace skia_private;

namespace SkSL::dsl {

static const SkSL::Type* find_type(const Context& context, std::string_view name, Position pos) {
    const Symbol* symbol = context.fSymbolTable->find(name);
    if (!symbol) {
        context.fErrors->error(pos, "no symbol named '" + std::string(name) + "'");
        return context.fTypes.fPoison.get();
    }
    if (!symbol->is<SkSL::Type>()) {
        context.fErrors->error(pos, "symbol '" + std::string(name) + "' is not a type");
        return context.fTypes.fPoison.get();
    }
    const SkSL::Type* type = &symbol->as<SkSL::Type>();
    return TypeReference::VerifyType(context, type, pos) ? type : context.fTypes.fPoison.get();
}

static const SkSL::Type* find_type(const Context& context,
                                   std::string_view name,
                                   Position overallPos,
                                   Modifiers* modifiers) {
    const Type* type = find_type(context, name, overallPos);
    return type->applyQualifiers(context, &modifiers->fFlags, modifiers->fPosition);
}

DSLType::DSLType(std::string_view name, Position pos)
        : fSkSLType(find_type(ThreadContext::Context(), name, pos)) {}

DSLType::DSLType(std::string_view name, Position overallPos, Modifiers* modifiers)
        : fSkSLType(find_type(ThreadContext::Context(), name, overallPos, modifiers)) {}

DSLType::DSLType(const SkSL::Type* type)
        : fSkSLType(type) {}

bool DSLType::isBoolean() const {
    return this->skslType().isBoolean();
}

bool DSLType::isNumber() const {
    return this->skslType().isNumber();
}

bool DSLType::isFloat() const {
    return this->skslType().isFloat();
}

bool DSLType::isSigned() const {
    return this->skslType().isSigned();
}

bool DSLType::isUnsigned() const {
    return this->skslType().isUnsigned();
}

bool DSLType::isInteger() const {
    return this->skslType().isInteger();
}

bool DSLType::isScalar() const {
    return this->skslType().isScalar();
}

bool DSLType::isVector() const {
    return this->skslType().isVector();
}

bool DSLType::isMatrix() const {
    return this->skslType().isMatrix();
}

bool DSLType::isArray() const {
    return this->skslType().isArray();
}

bool DSLType::isStruct() const {
    return this->skslType().isStruct();
}

bool DSLType::isInterfaceBlock() const {
    return this->skslType().isInterfaceBlock();
}

bool DSLType::isEffectChild() const {
    return this->skslType().isEffectChild();
}

DSLType Array(const DSLType& base, int count, Position pos) {
    SkSL::Context& context = ThreadContext::Context();
    count = base.skslType().convertArraySize(context, pos, pos, count);
    if (!count) {
        return DSLType(context.fTypes.fPoison.get());
    }
    return context.fSymbolTable->addArrayDimension(&base.skslType(), count);
}

DSLType UnsizedArray(const DSLType& base, Position pos) {
    SkSL::Context& context = ThreadContext::Context();
    if (!base.skslType().checkIfUsableInArray(context, pos)) {
        return DSLType(context.fTypes.fPoison.get());
    }
    return context.fSymbolTable->addArrayDimension(&base.skslType(), SkSL::Type::kUnsizedArray);
}

}  // namespace SkSL::dsl
