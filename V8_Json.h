/*
 * V8_Json.h
 *
 *  Created on: 2016年3月1日
 *      Author: zhangyalei
 */

#ifndef V8_JSON_H_
#define V8_JSON_H_

#include <string>
#include "include/v8.h"

namespace v8_wrap {

/// Stringify V8 value to JSON
/// return empty string for empty value
std::string json_str(v8::Isolate* isolate, v8::Handle<v8::Value> value)
{
	if (value.IsEmpty())
	{
		return std::string();
	}

	v8::HandleScope scope(isolate);

	v8::Local<v8::Object> json = isolate->GetCurrentContext()->
		Global()->Get(v8::String::NewFromUtf8(isolate, "JSON"))->ToObject();
	v8::Local<v8::Function> stringify = json->Get(v8::String::NewFromUtf8(isolate, "stringify")).As<v8::Function>();

	v8::Local<v8::Value> result = stringify->Call(json, 1, &value);
	v8::String::Utf8Value const str(result);

	return std::string(*str, str.length());
}

/// Parse JSON string into V8 value
/// return empty value for empty string
/// return Error value on parse error
v8::Handle<v8::Value> json_parse(v8::Isolate* isolate, std::string const& str)
{
	if (str.empty())
	{
		return v8::Handle<v8::Value>();
	}

	v8::EscapableHandleScope scope(isolate);

	v8::Local<v8::Object> json = isolate->GetCurrentContext()->
		Global()->Get(v8::String::NewFromUtf8(isolate, "JSON"))->ToObject();
	v8::Local<v8::Function> parse = json->Get(v8::String::NewFromUtf8(isolate, "parse")).As<v8::Function>();

	v8::Local<v8::Value> value = v8::String::NewFromUtf8(isolate, str.data(),
		v8::String::kNormalString, static_cast<int>(str.size()));

	v8::TryCatch try_catch;
	v8::Local<v8::Value> result = parse->Call(json, 1, &value);
	if (try_catch.HasCaught())
	{
		result = try_catch.Exception();
	}
	return scope.Escape(result);
}
} // namespace v8_wrap

#endif /* V8_JSON_H_ */
