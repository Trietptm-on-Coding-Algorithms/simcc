// Created: 2009/08/07
// Author: weizili
#pragma once

#include "simcc/inner_pre.h"
#include "simcc/gcc_list.h"

#include "json_common.h"
#include "json_value.h"
#include "json_parser.h"

namespace simcc {
namespace json {
 
// A JSONArray is an ordered sequence of values. Its external text form is a
// string wrapped in square brackets with commas separating the values. The
// internal form is an object.

class JSONObject;
class JSONTokener;
class SIMCC_EXPORT JSONArray : public Object, public JSONParser {
public:
    typedef simcc::list<ObjectPtr>                ObjectPtrList;
    typedef ObjectPtrList::iterator               iterator;
    typedef ObjectPtrList::const_iterator         const_iterator;
    typedef ObjectPtrList::reverse_iterator       reverse_iterator;
    typedef ObjectPtrList::const_reverse_iterator const_reverse_iterator;
public:
    enum { Type = kJSONArray };
    
    // Construct an empty JSONArray.
    JSONArray() : Object(kJSONArray) {}
    virtual ~JSONArray();

    //  Construct a JSONArray from a source JSON text.
    //  @param source A string that begins with
    //  <code>[</code>&nbsp;<small>(left bracket)</small>
    //   and ends with <code>]</code>&nbsp;<small>(right bracket)</small>.
    //  @return number of characters parsed. Return 0 if failed to parse.
    uint32 Parse(const string& source);

    // Construct a JSONArray from a source JSON text.
    // @param source A string that begins with
    // <code>[</code>&nbsp;<small>(left bracket)</small>
    //  and ends with <code>]</code>&nbsp;<small>(right bracket)</small>.
    // @param source_len, the length of the source string.
    //   if you use the default value(-1), we will use strlen(source) to
    //   calculate the length.
    // @return number of characters parsed. Return 0 if failed to parse.
    uint32 Parse(const char* source, const int64 source_len = -1);

    // Get the object value associated with an index.
    // @param index
    //  The index must be between 0 and length() - 1.
    // @return An object value.
    //   NULL, If there is no value for the index.
    Object*      Get(int index) const;
    JSONBoolean* GetJSONBoolean(int index) const;
    JSONDouble*  GetJSONDouble(int index) const;
    JSONInteger* GetJSONInteger(int index) const;
    JSONString*  GetJSONString(int index) const;
    JSONArray*   GetJSONArray(int index) const;
    JSONObject*  GetJSONObject(int index) const;

    bool IsNull(int index) const;

    // Gets a value
    // @param strKey, the key
    // @param default_value, the default value.
    // @return the bool value.
    //   or default_value, If there is no value for the key strKey
    bool GetBool(int index, bool default_value = false) const;
    float64 GetDouble(int index, float64 default_value = 0) const;
    int64 GetInteger(int index, int64 default_value = 0) const;
    const string& GetString(int index, const string& default_value = StringUtil::kEmpty) const;

    // Get a decimal number whether it is a JSONDouble or a JSONInteger
    float64 GetDecimal(int index, float64 default_value = 0.0)const;

    // Gets an array.
    // @param strKey, the key
    // @param array[out] the result is stored
    // @param count Size of array
    // @param default_value, the default value of the array element
    // @return true, if found the key strKey
    //   or false, If there is no value for the key strKey
    void GetBoolArray(bool* array, simcc::uint32 count, bool default_value = false)const;
    void GetByteArray(simcc::uint8* array, simcc::uint32 count, simcc::uint8 default_value = 0)const;
    void GetInt32Array(simcc::int32* array, simcc::uint32 count, simcc::int32 default_value = 0)const;
    void GetInt64Array(simcc::int64* array, simcc::uint32 count, simcc::int64 default_value = 0)const;
    void GetFloat32Array(simcc::float32* array, simcc::uint32 count, simcc::float32 default_value = 0.0f)const;
    void GetFloat64Array(simcc::float64* array, simcc::uint32 count, simcc::float64 default_value = 0.0)const;
    void GetStringArray(string* array, simcc::uint32 count, const string& default_value = StringUtil::kEmpty)const;

    // Gets an array.
    // @remark  Try to find typed value, of can not find,set default value.
    // @param strKey, the key
    // @param vec[out] the result is stored
    void GetBoolArray(std::vector<bool>& vec, bool default_value = false)const;
    void GetByteArray(std::vector<simcc::uint8>& vec, simcc::uint8 default_value = 0)const;
    void GetInt32Array(std::vector<simcc::int32>& vec, simcc::int32 default_value = 0)const;
    void GetInt64Array(std::vector<simcc::int64>& vec, simcc::int64 default_value = 0)const;
    void GetFloat32Array(std::vector<simcc::float32>& vec, simcc::float32 default_value = 0.0f)const;
    void GetFloat64Array(std::vector<simcc::float64>& vec, simcc::float64 default_value = 0.0)const;
    void GetStringArray(std::vector<string>& vec, const string& default_value = StringUtil::kEmpty)const;

    // Append a value. This increases the array's length by one.
    //
    // @param value A boolean/double/int64/string/Object value.
    // @return this.
    JSONArray* Put(const bool value);
    JSONArray* Put(const simcc::float64 value);
    JSONArray* Put(const simcc::int64 value);
    JSONArray* Put(const string& s);
    JSONArray* Put(const char* s);
    JSONArray* Put(Object* value); // Do not delete this pointer, it will be managed by this JSONArray
    JSONArray* Put(const ObjectPtr& value);

    // Remove a index and close the hole.
    // @param index The index of the element to be removed.
    // @return true if remove the element success
    bool Remove(int index);

    // Returns whether the JSON object is empty, i.e. whether its size is 0.
    bool empty() const {
        return list_.empty();
    }

    // All the elements in the list container are dropped, their destructors are called,
    // and then they are removed from the list container, leaving it with a size of 0.
    void clear() {
        list_.clear();
    }

    iterator erase(iterator it) {
        return list_.erase(it);
    }

    // Get the number of elements in the JSONArray, included nulls.
    // @return The length (or size).
    size_t size() const {
        return list_.size();
    }

    // Make a JSON text of this JSONArray. For compactness, no
    // unnecessary whitespace is added. If it is not possible to produce a
    // syntactically correct JSON text then null will be returned instead. This
    // could occur if the array contains an invalid number.
    // <p>
    // @warning This method assumes that the data structure is a cyclical.
    // @override override method from base class json::Object
    // @return a printable, displayable, transmittable
    //  representation of the array.
    using Object::ToString; // string ToString(bool readable = false, bool utf8_to_unicode = true) const;
    virtual void ToString(string& s, bool readable = false, bool utf8_to_unicode = true)const;
    virtual void ToStringBuf(simcc::DataStream& sb, size_t indent = 0, bool utf8_to_unicode = true)const;

    // @warning This method is quite not effective, so make sure why you need call this method
    // @note We mostly use this method to do some unit test
    // @return true If rhs is the same type and the value is equal to each other
    virtual bool Equals(const Object& rhs);

    // iterator
public:
    // Gets objects in the array.
    // @remark Caller is responsible for identify the concrete type of object element.
    const ObjectPtrList& GetObjects()const {
        return list_;
    }

    const_iterator begin() const {
        return list_.begin();
    }

    iterator begin() {
        return list_.begin();
    }

    const_iterator end() const {
        return list_.end();
    }

    iterator end() {
        return list_.end();
    }

    const_reverse_iterator rbegin() const {
        return list_.rbegin();
    }

    reverse_iterator rbegin() {
        return list_.rbegin();
    }

    const_reverse_iterator rend() const {
        return list_.rend();
    }

    reverse_iterator rend() {
        return list_.rend();
    }

protected:
    friend class JSONTokener;
    friend class JSONObject;
    friend class JSONParser;

    // @return number of characters parsed. Return 0 if failed to parse.
    simcc::uint32 Parse(JSONTokener* token);
    JSONArray(JSONTokener* token);

    // Construct a JSONArray from a source JSON text string.
    // @note Don't use this tow constructors, unless you can make sure
    //   the json text string is valid format. You can you JSONArray()
    //   instead, and use Parse(...) method to build a JSON object
    //   from a json text string.
    // @param source    A source json text string
    JSONArray(const string& source);
    JSONArray(const char* source);

    template<class T>
    const T& GetElement(int index, const T& default_value)const;

    template<class T, class U>
    void GetElement(T*, uint32 count, const U& default_value)const;

    template<class T, class U>
    void GetElement(std::vector<T>& vec, const U& default_value)const;

private:
    // override method from base class json::Object
    virtual bool LoadFrom(simcc::DataStream& file);
    virtual void SaveTo(simcc::DataStream& file) const;

private:
    ObjectPtrList list_; // The list where the JSONArray's properties are kept.

    template<class T>
    T* GetObject(int index) const;
private:
    const_iterator begin(size_t index) const;
    iterator begin(size_t index);
}; //end of class


typedef simcc::RefPtr<JSONArray>  JSONArrayPtr;

}
}



