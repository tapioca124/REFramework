namespace sdk {
class ResourceManager;
class Resource;
}

#pragma once

#include <string_view>
#include <optional>

#include "ManagedObject.hpp"
#include "intrusive_ptr.hpp"

class REManagedObject;

namespace sdk {
class Resource {
public:
    void add_ref();
    void release();
    REManagedObject* create_holder(sdk::RETypeDefinition* t);

private:
    friend class sdk::ResourceManager;

    static void update_pointers();
    static void (*s_add_ref_fn)(Resource*);
    static void (*s_release_fn)(Resource*);
    static inline std::optional<size_t> s_refcount_offset{};
};

class ResourceManager {
public:
    static ResourceManager* get();
    static void update_pointers();
    // Some games/versions may not have a resolvable create_userdata implementation.
    // This is intentionally resolved lazily to avoid noisy startup logs for games that don't support it.
    static void update_userdata_pointers();

public:
    sdk::Resource* create_resource(void* type_info, std::wstring_view name);
    intrusive_ptr<sdk::ManagedObject> create_userdata(void* type_info, std::wstring_view name);

    static auto get_create_resource_function() {
        update_pointers();

        return s_create_resource_fn;
    }

    static auto get_create_userdata_function() {
        update_userdata_pointers();

        return s_create_userdata_fn;
    }

private:
    friend class sdk::Resource;

    static sdk::Resource* (*s_create_resource_fn)(ResourceManager*, void*, const wchar_t*);
    static uintptr_t s_create_resource_reference;

    static intrusive_ptr<sdk::ManagedObject>* (*s_create_userdata_fn)(ResourceManager*, intrusive_ptr<sdk::ManagedObject>*, void*, const wchar_t*);
};
}
