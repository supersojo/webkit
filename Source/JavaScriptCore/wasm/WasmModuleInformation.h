/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(WEBASSEMBLY)

#include "WasmFormat.h"

namespace JSC { namespace Wasm {

struct ModuleInformation : public ThreadSafeRefCounted<ModuleInformation> {
    ModuleInformation() = delete;
    ModuleInformation(const ModuleInformation&) = delete;
    ModuleInformation(ModuleInformation&&) = delete;

    ModuleInformation(Vector<uint8_t>&& sourceBytes);

    JS_EXPORT_PRIVATE ~ModuleInformation();
    
    size_t functionIndexSpaceSize() const { return importFunctionSignatureIndices.size() + internalFunctionSignatureIndices.size(); }
    bool isImportedFunctionFromFunctionIndexSpace(size_t functionIndex) const
    {
        ASSERT(functionIndex < functionIndexSpaceSize());
        return functionIndex < importFunctionSignatureIndices.size();
    }
    SignatureIndex signatureIndexFromFunctionIndexSpace(size_t functionIndex) const
    {
        return isImportedFunctionFromFunctionIndexSpace(functionIndex)
            ? importFunctionSignatureIndices[functionIndex]
            : internalFunctionSignatureIndices[functionIndex - importFunctionSignatureIndices.size()];
    }

    uint32_t importFunctionCount() const { return importFunctionSignatureIndices.size(); }
    uint32_t internalFunctionCount() const { return internalFunctionSignatureIndices.size(); }

    const Vector<uint8_t> source;
    const CString hash;

    Vector<Import> imports;
    Vector<SignatureIndex> importFunctionSignatureIndices;
    Vector<SignatureIndex> internalFunctionSignatureIndices;
    Vector<Ref<Signature>> usedSignatures;

    MemoryInformation memory;

    Vector<FunctionLocationInBinary> functionLocationInBinary;

    Vector<Export> exports;
    std::optional<uint32_t> startFunctionIndexSpace;
    Vector<Segment::Ptr> data;
    Vector<Element> elements;
    TableInformation tableInformation;
    Vector<Global> globals;
    unsigned firstInternalGlobal { 0 };
    Vector<CustomSection> customSections;
    RefPtr<NameSection> nameSection;
};

    
} } // namespace JSC::Wasm

#endif // ENABLE(WEBASSEMBLY)
