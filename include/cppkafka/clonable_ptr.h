/*
 * Copyright (c) 2016, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef CPPKAFKA_CLONABLE_PTR_H
#define CPPKAFKA_CLONABLE_PTR_H

#include <memory>

namespace cppkafka {

template <typename T, typename Deleter, typename Cloner>
class ClonablePtr {
public:
    ClonablePtr(T* ptr, const Deleter& deleter, const Cloner& cloner)
    : handle_(ptr, deleter), cloner_(cloner) {

    }

    ClonablePtr(const ClonablePtr& rhs)
    : handle_(rhs.cloner_(rhs.handle_.get()), rhs.handle_.get_deleter()), cloner_(rhs.cloner_) {

    }

    ClonablePtr& operator=(const ClonablePtr& rhs) {
        handle_.reset(cloner_(rhs.handle_.get()));
        return *this;
    }

    ClonablePtr(ClonablePtr&&) = default;
    ClonablePtr& operator=(ClonablePtr&&) = default;
    ~ClonablePtr() = default;

    T* get() const {
        return handle_.get();
    }
private:
    std::unique_ptr<T, Deleter> handle_;
    Cloner cloner_;
};

} // cppkafka

#endif // CPPKAFKA_CLONABLE_PTR_H