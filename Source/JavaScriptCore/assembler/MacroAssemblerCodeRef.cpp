/*
 * Copyright (C) 2016-2017 Apple Inc. All rights reserved.
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

#include "config.h"
#include "MacroAssemblerCodeRef.h"

#include "Disassembler.h"
#include "JSCInlines.h"
#include "LLIntData.h"
#include <mutex>

namespace JSC {

uintptr_t g_masmScrambledPtrKey;

void MacroAssemblerCodePtr::initialize()
{
    static std::once_flag initializeOnceFlag;
    std::call_once(initializeOnceFlag, [] {
        g_masmScrambledPtrKey = makeScrambledPtrKey();
    });
}

MacroAssemblerCodePtr MacroAssemblerCodePtr::createLLIntCodePtr(OpcodeID codeId)
{
    return createFromExecutableAddress(LLInt::getCodePtr(codeId));
}

void MacroAssemblerCodePtr::dumpWithName(const char* name, PrintStream& out) const
{
    if (!m_value) {
        out.print(name, "(null)");
        return;
    }
    if (executableAddress() == dataLocation()) {
        out.print(name, "(", RawPointer(executableAddress()), ")");
        return;
    }
    out.print(name, "(executable = ", RawPointer(executableAddress()), ", dataLocation = ", RawPointer(dataLocation()), ")");
}

void MacroAssemblerCodePtr::dump(PrintStream& out) const
{
    dumpWithName("CodePtr", out);
}

MacroAssemblerCodeRef MacroAssemblerCodeRef::createLLIntCodeRef(OpcodeID codeId)
{
    return createSelfManagedCodeRef(MacroAssemblerCodePtr::createFromExecutableAddress(LLInt::getCodePtr(codeId)));
}

bool MacroAssemblerCodeRef::tryToDisassemble(PrintStream& out, const char* prefix) const
{
    return JSC::tryToDisassemble(m_codePtr, size(), prefix, out);
}

bool MacroAssemblerCodeRef::tryToDisassemble(const char* prefix) const
{
    return tryToDisassemble(WTF::dataFile(), prefix);
}

CString MacroAssemblerCodeRef::disassembly() const
{
    StringPrintStream out;
    if (!tryToDisassemble(out, ""))
        return CString();
    return out.toCString();
}

void MacroAssemblerCodeRef::dump(PrintStream& out) const
{
    m_codePtr.dumpWithName("CodeRef", out);
}

} // namespace JSC

