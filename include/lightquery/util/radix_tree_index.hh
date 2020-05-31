//
// Created by tramboo on 2020/5/31.
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <lightquery/util/art.hh>

namespace lightquery {

    template<class valueType>
    class radix_tree_index {
    private:
        art_tree *_tree;

        // https://github.com/huanchenz/index-microbench/blob/master/ART/ART.cpp
        uint8_t *loadKey(uint8_t *key, int &key_length) {
            // Store the key of the tuple into the key vector
            // Implementation is database specific
            auto key_arr = new uint8_t[++key_length];
            key_arr[key_length - 1] = '\0';
            memcpy(key_arr, key, (size_t) (key_length - 1));
            return key_arr;
        }

        uint8_t *loadPrefix(uint8_t *key, int &key_length) {
            // Store the key of the tuple into the key vector
            // Implementation is database specific
            auto key_arr = new uint8_t[key_length];
            memcpy(key_arr, key, (size_t) key_length);
            return key_arr;
        }


    public:
        void remove_key(uint8_t *key, int key_length) {
            auto key_arr = loadKey(key, key_length);
            art_delete(_tree, key_arr, key_length);
        }

        int remove_prefix(uint8_t *key, int key_length) {
            auto key_arr = loadPrefix(key, key_length);
            return art_delete_prefix(_tree, key_arr, key_length, 0);
        }

        valueType get(uint8_t *key, int key_length) {
            auto key_arr = loadKey(key, key_length);
            return (valueType) art_search(_tree, key_arr, key_length);
        }

        void insert(uint8_t *key, valueType value, int key_length) {
            auto key_arr = loadKey(key, key_length);
            art_insert(_tree, key_arr, key_length, (void *) value);
        }

        bool contains(uint8_t *key, int key_length) {
            auto key_arr = loadKey(key, key_length);
            return art_search(_tree, key_arr, key_length) != NULL;
        }

        int disable(uint8_t *key, int key_length) {
            auto key_arr = loadPrefix(key, key_length);
            return art_disable(_tree, key_arr, key_length);
        }

        int lookup(uint8_t *key, void **ret_ptr, int key_length) {
            auto key_arr = loadKey(key, key_length);
            return art_lookup(_tree, key_arr, key_length, ret_ptr);
        }

        void iter_prefix_apply(uint8_t* key, int key_length, void (*apply)(void* data, void* target), void* data){
            auto key_arr = loadPrefix(key, key_length);
            return art_iter_active_prefix(_tree, key_arr, key_length, apply, data);
        }

        void iter_prefix_apply(void (*apply)(void* data, void* target), void* data){
            return art_iter_active(_tree, apply, data);
        }

        uint64_t get_size(){
            return _tree->size;
        }

        radix_tree_index() {
            _tree = new art_tree();
            if (init_art_tree(_tree) != 0) {
                throw ("Art tree initiliaztion failed!");
            }
        }

        ~radix_tree_index() {
            destroy_art_tree(_tree);
            delete _tree;
        }
    };
}
