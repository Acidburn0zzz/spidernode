// Copyright Mozilla Foundation. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "v8.h"
#include "v8conversions.h"
#include "v8local.h"
#include "jsapi.h"
#include "jsfriendapi.h"
#include "js/Conversions.h"

namespace v8 {

Local<ArrayBuffer>
ArrayBuffer::New(Isolate* isolate, size_t size)
{
  JSContext* cx = JSContextFromIsolate(isolate);
  JSObject* buf = JS_NewArrayBuffer(cx, size);
  JS::Value val;
  val.setObject(*buf);
  return internal::Local<ArrayBuffer>::New(isolate, val);
}

size_t
ArrayBuffer::ByteLength() const
{
  const JS::Value* val = reinterpret_cast<const JS::Value*>(this);
  JSObject& obj = val->toObject();
  return JS_GetArrayBufferByteLength(&obj);
}

ArrayBuffer::Contents
ArrayBuffer::GetContents()
{
  const JS::Value thisVal = *reinterpret_cast<const JS::Value*>(this);
  JSObject* obj = &thisVal.toObject();
  uint8_t *data;
  bool shared;
  uint32_t length;
  js::GetArrayBufferLengthAndData(obj, &length, &shared, &data);
  Contents contents;
  contents.data_ = static_cast<void*>(data);
  contents.byte_length_ = length;
  return contents;
}
}
